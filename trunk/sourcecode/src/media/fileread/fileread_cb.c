#include "fileread_priv.h"

int FileReadTimeOutNotifyCallBack( AMPS_HANDLE hAMPS_HANDLE_i, void* pvData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileRead* poFileRead = pvData_i;

	MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileRead, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_FILEREAD_MAGIC_NUM == poFileRead->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		unsigned int unNumReads = 0;
		int nRetVal = 0;
		t_DataPkt* poDataPkt = NULL;

		while( unNumReads < poFileRead->unNumReadsPerTimeout )
		{
			unNumReads++;

			nRetVal = 0;
			poDataPkt = NULL;

			eRetCode = AllocGenPkt((h_DataPkt*)&poDataPkt );
			if( MS_ERR_NONE == eRetCode )
			{
				MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Generic Packet allocation successful = %p\n", poDataPkt );
				// allocate data buffer
				poDataPkt->oGenPkt.pucData = FW_Malloc( poFileRead->unReadSize );
				if( NULL != poDataPkt->oGenPkt.pucData )
				{
					MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Data buffer allocation successful = %p\n", poDataPkt->oGenPkt.pucData );

					nRetVal = fread( poDataPkt->oGenPkt.pucData, 1, poFileRead->unReadSize, poFileRead->poFile );
					if( 0 < nRetVal )
					{
						poDataPkt->oGenPkt.nLength = nRetVal;
						poDataPkt->nPktGenTime = FW_GetCurrentTimeInMilliSec( poFileRead->oComp.pvFrameworkCtx );

						MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "%d bytes read from file\n", poDataPkt->oGenPkt.nLength );

						// Call data callback function of the next layer
						ms_assert_return( NULL != poFileRead->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
						eRetCode = (poFileRead->oAppData.pfAppDataCb)((h_MediaComp)poFileRead, (h_MediaCompAppCtx)poFileRead->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
					}
					else if( 0 ==  nRetVal )
					{
						// Delete Data packet
						(poDataPkt->pfnDataPktDelete)( poDataPkt );
						FW_Free( poDataPkt );

						// Generate EOF signal

						// Allocate signal packet
						t_SigPkt* poSigPkt = FW_Malloc( sizeof (t_SigPkt));
						// Check if allocation is successful
						if( NULL != poSigPkt )
						{
							MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Signal packet allocated\n" );

							// Initialize signal packet
							poSigPkt->eType = SPT_EOF_Detected;
							poSigPkt->pfnSignalPktInit =  FileReadSigPktInit;
							poSigPkt->pfnSignalPktDelete = FileReadSigPktDelete;

							(poSigPkt->pfnSignalPktInit)( poSigPkt );

							// Populate values
							poSigPkt->oFileReadPkt.lFileOffset = ftell( poFileRead->poFile );

							MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Singal callback function\n" );

							// Call signal callback function
							ms_assert_return( NULL != poFileRead->oAppData.pfAppSignalCb, eRetCode = MS_ERR_NOT_INITIALIZED );
							eRetCode = (poFileRead->oAppData.pfAppSignalCb)((h_MediaComp)poFileRead, (h_MediaCompAppCtx)poFileRead->oAppData.hAppSignalCtx, (h_DataPkt)poSigPkt );
						}
						else
						{
							eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
						}
						eRetCode = FileReadSetStatus((h_MediaComp)poFileRead, MCS_STOP );

						eRetCode = MS_ERR_END_OF_LIST;
						// Dont read any more
						break;
					}
					else
					{
						MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "fread returned with error = %d\n", nRetVal );

						(poDataPkt->pfnDataPktDelete)( poDataPkt );
						FW_Free( poDataPkt );

						eRetCode = MS_ERR_FRAMEWORK;
					}
				}
				else
				{
					MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Data buffer allocation unsuccessful\n" );

					(poDataPkt->pfnDataPktDelete)( poDataPkt );
					FW_Free( poDataPkt );

					eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
					break;
				}
			}
			else
			{
				MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Generic Packet allocation unsuccessful\n" );
				break;
			}
		}
		if( MS_ERR_NONE == eRetCode )
		{
			poFileRead->hReadTimerId = FW_StartTimer( hAMPS_HANDLE_i, poFileRead->unReadTimerMs, FileReadTimeOutNotifyCallBack, pvData_i );
		}
		else
		{
			poFileRead->hReadTimerId = NULL;
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return ((int)eRetCode);
}

e_MsRetCode FileReadAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_FileRead* poFileRead = hMediaCompAppCtx_i;

	MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
	}

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}
	MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

