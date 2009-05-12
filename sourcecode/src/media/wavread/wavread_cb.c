#include "wavread_priv.h"

int WavReadTimeOutNotifyCallBack( AMPS_HANDLE hAMPS_HANDLE_i, void* pvData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavRead* poWavRead = pvData_i;

	MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavRead, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_WAVREAD_MAGIC_NUM == poWavRead->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( NULL != poWavRead->hReadTimerId )
		{
			unsigned int unNumReads = 0;
			int nRetVal = 0;
			t_DataPkt* poDataPkt = NULL;

			poWavRead->hReadTimerId = NULL;

			while( unNumReads < poWavRead->unNumReadsPerTimeout )
			{
				unNumReads++;

				nRetVal = 0;
				poDataPkt = NULL;

				eRetCode = AllocGenPkt((h_DataPkt*)&poDataPkt );
				if( MS_ERR_NONE == eRetCode )
				{
					MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Generic Packet allocation successful = %p\n", poDataPkt );
					// allocate data buffer
					poDataPkt->oGenPkt.pucData = FW_Malloc( poWavRead->unReadSize );
					if( NULL != poDataPkt->oGenPkt.pucData )
					{
						MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Data buffer allocation successful = %p\n", poDataPkt->oGenPkt.pucData );

						nRetVal = sf_read_raw( poWavRead->poFile, poDataPkt->oGenPkt.pucData, poWavRead->unReadSize );
						if( 0 < nRetVal )
						{
							poDataPkt->oGenPkt.nLength = nRetVal;
							poDataPkt->nPktGenTime = poWavRead->nLastPktGenTimeMs;

							poWavRead->nLastPktGenTimeMs += poWavRead->unReadTimerMs / poWavRead->unNumReadsPerTimeout;

							MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "%d bytes read from file\n", poDataPkt->oGenPkt.nLength );

							MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Pkt Generation Time = %d\n", poDataPkt->nPktGenTime );

							MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Current Time = %d\n", FW_GetCurrentTimeInMilliSec( poWavRead->oComp.pvFrameworkCtx ) );

							// Call data callback function of the next layer
							ms_assert_return( NULL != poWavRead->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
							eRetCode = (poWavRead->oAppData.pfAppDataCb)((h_MediaComp)poWavRead, (h_MediaCompAppCtx)poWavRead->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
						}
						if( 0 == nRetVal || poWavRead->unReadSize > nRetVal )
						{
							// Generate EOF signal

							// Allocate signal packet
							t_SigPkt* poSigPkt = FW_Malloc( sizeof (t_SigPkt));
							// Check if allocation is successful
							if( NULL != poSigPkt )
							{
								MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Signal packet allocated\n" );

								// Initialize signal packet
								poSigPkt->eType = SPT_EOF_Detected;
								poSigPkt->pfnSignalPktInit =  WavReadSigPktInit;
								poSigPkt->pfnSignalPktDelete = WavReadSigPktDelete;

								(poSigPkt->pfnSignalPktInit)( poSigPkt );

								// Populate values
								poSigPkt->oWavReadPkt.lFileOffset = 0;

								MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Singal callback function\n" );

								// Call signal callback function
								ms_assert_return( NULL != poWavRead->oAppData.pfAppSignalCb, eRetCode = MS_ERR_NOT_INITIALIZED );
								eRetCode = (poWavRead->oAppData.pfAppSignalCb)((h_MediaComp)poWavRead, (h_MediaCompAppCtx)poWavRead->oAppData.hAppSignalCtx, (h_DataPkt)poSigPkt );
							}
							else
							{
								eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
							}
							eRetCode = WavReadSetStatus((h_MediaComp)poWavRead, MCS_STOP );

							eRetCode = MS_ERR_END_OF_LIST;
							// Dont read any more
							break;
						}
						else if( 0 > nRetVal )
						{
							MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "sf_read_raw returned with error = %d\n", nRetVal );

							(poDataPkt->pfnDataPktDelete)( poDataPkt );
							FW_Free( poDataPkt );

							eRetCode = MS_ERR_FRAMEWORK;
						}
					}
					else
					{
						MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Data buffer allocation unsuccessful\n" );

						(poDataPkt->pfnDataPktDelete)( poDataPkt );
						FW_Free( poDataPkt );

						eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
						break;
					}
				}
				else
				{
					MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Generic Packet allocation unsuccessful\n" );
					break;
				}
			}
			if( MS_ERR_NONE == eRetCode )
			{
				MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Restarting timer.\n" );

				poWavRead->hReadTimerId = FW_StartTimer( hAMPS_HANDLE_i, poWavRead->unReadTimerMs, WavReadTimeOutNotifyCallBack, pvData_i );
			}
		}
		else
		{
			MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_WARNING, "Component stopped\n" );
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return ((int)eRetCode);
}

e_MsRetCode WavReadAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_WavRead* poWavRead = hMediaCompAppCtx_i;

	MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

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
	MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

