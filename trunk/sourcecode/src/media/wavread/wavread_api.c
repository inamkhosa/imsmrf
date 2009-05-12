#include "wavread_priv.h"

e_MsRetCode WavReadInit( h_CompConfig hCompConfig_i, h_MediaComp* phWavRead_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavReadConfig* poWavReadConfig = (t_WavReadConfig*)hCompConfig_i;

	MS_TRACE( poWavReadConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavReadConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poWavReadConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phWavRead_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_WavRead* poWavRead = NULL;

		// Allocate WavRead structure
		poWavRead = FW_Malloc( sizeof (t_WavRead));
		if( NULL != poWavRead )
		{
			poWavRead->oComp.eType = MCT_WAVREAD;
			poWavRead->oComp.eGroup = MCG_SOURCE;
			// Populate WavRead structure
			poWavRead->oComp.nTraceId = poWavReadConfig->oConfig.nTraceId;
			poWavRead->oComp.pvFrameworkCtx = poWavReadConfig->oConfig.pvFrameworkCtx;
			memset( &poWavRead->oAppData, 0, sizeof (t_MediaCompAppData));

			poWavRead->nMagicNum = MCT_WAVREAD_MAGIC_NUM;
			if( NULL != poWavReadConfig->pcFileName )
			{
				poWavRead->pcFileName = strdup( poWavReadConfig->pcFileName );
				ms_assert_return( NULL != poWavRead->pcFileName, eRetCode = MS_ERR_INSUFFICIENT_MEMORY );
			}
			poWavRead->unReadSize = poWavReadConfig->unReadSize;
			poWavRead->unNumReadsPerTimeout = poWavReadConfig->unNumReadsPerTimeout;
//          poWavRead->unMode = poWavReadConfig->unMode;
			poWavRead->unReadTimerMs = poWavReadConfig->unReadTimerMs;
			// Open file
			if( NULL != poWavRead->pcFileName )
			{
				poWavRead->poFile  = sf_open( poWavRead->pcFileName, SFM_READ, &poWavRead->oSfInfo );
				if( NULL != poWavRead->poFile )
				{
					MS_TRACE( poWavReadConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "File opened successfully\n" );

					ms_assert( poWavRead->oSfInfo.samplerate == 8000 );
					ms_assert( poWavRead->oSfInfo.channels == 1 );
//					ms_assert(((poWavRead->oSfInfo.format) & (SF_FORMAT_SUBMASK)) == (SF_FORMAT_PCM_16));
					ms_assert( poWavRead->oSfInfo.sections == 1 );
				}
				else
				{
					MS_TRACE( poWavReadConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "sf_open returned with error  = %s\n", sf_perror( NULL ) );

					FW_Free( poWavRead );
					eRetCode = MS_ERR_FRAMEWORK;
				}
			}
			if( MS_ERR_NONE == eRetCode )
			{
				*phWavRead_o = (h_MediaComp) poWavRead;
			}
		}
		else
		{
			MS_TRACE( poWavReadConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_WavRead\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poWavReadConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode WavReadSetAppData( h_MediaComp hWavRead_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavRead* poWavRead = hWavRead_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavRead, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_WAVREAD_MAGIC_NUM == poWavRead->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poWavRead->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode WavReadDestroy( h_MediaComp hWavRead_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavRead* poWavRead = hWavRead_i;
	int nTraceId = poWavRead->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavRead, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_WAVREAD_MAGIC_NUM == poWavRead->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( NULL != poWavRead->poFile )
		{
			sf_close( poWavRead->poFile );
		}
		if( NULL != poWavRead->pcFileName )
		{
			FW_Free( poWavRead->pcFileName );
		}
		FW_Free( poWavRead );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode WavReadSetStatus( h_MediaComp hWavRead_i, e_MediaCompStatus eStatus_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavRead* poWavRead = hWavRead_i;

	MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavRead, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_WAVREAD_MAGIC_NUM == poWavRead->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eStatus_i )
		{
			case MCS_START:
			{
				poWavRead->nLastPktGenTimeMs = FW_GetCurrentTimeInMilliSec( poWavRead->oComp.pvFrameworkCtx );

				// Read the first pkt as soon as start is called
				WavReadTimeOutNotifyCallBack( poWavRead->oComp.pvFrameworkCtx, poWavRead );

				poWavRead->hReadTimerId = FW_StartTimer( poWavRead->oComp.pvFrameworkCtx, poWavRead->unReadTimerMs, WavReadTimeOutNotifyCallBack, poWavRead );
				if( NULL != poWavRead->hReadTimerId )
				{
					MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Timer started successfully\n" );
				}
				else
				{
					MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_StartTimer returned with error\n" );

					eRetCode = MS_ERR_FRAMEWORK;
				}
				break;
			}

			case MCS_STOP:
			{
				int nRetVal = AMPS_SUCCESS;

				poWavRead->nLastPktGenTimeMs = 0;
				if( NULL != poWavRead->hReadTimerId )
				{
					nRetVal = FW_DeleteTimer( poWavRead->oComp.pvFrameworkCtx, poWavRead->hReadTimerId  );
					if( AMPS_SUCCESS != nRetVal )
					{
						MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_DeleteTimer returned with ret-code = %d\n", nRetVal );

						eRetCode = MS_ERR_FRAMEWORK;
					}
					else
					{
						poWavRead->hReadTimerId = NULL;
					}
				}
				else
				{
					MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_WARNING, "Component already stopped\n" );
				}
				break;
			}

			default:
			{
				MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus_i );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode WavReadSetParam( h_MediaComp hWavRead_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavRead* poWavRead = hWavRead_i;

	MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavRead, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_WAVREAD_MAGIC_NUM == poWavRead->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SNDFILE* poFile = NULL;

		switch( eConfig_i )
		{
			case MCC_WAVREAD_FILENAME:
			{
				char* pcFileName = (char*)pvConfigData_i;
				SF_INFO oSfInfo;

				MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Filename = %s\n", pcFileName );

				ms_assert( NULL != pcFileName );
				// Open file
				poFile = sf_open( pcFileName, SFM_READ, &oSfInfo );
				if( NULL != poFile )
				{
					if( NULL != poWavRead->pcFileName )
					{
						// Close previous file
						sf_close( poWavRead->poFile );

						FW_Free( poWavRead->pcFileName );
					}
					ms_assert( oSfInfo.samplerate == 8000 );
					ms_assert( oSfInfo.channels == 1 );
//					ms_assert(((poWavRead->oSfInfo.format) & (SF_FORMAT_SUBMASK)) == (SF_FORMAT_PCM_16));
					ms_assert( oSfInfo.sections == 1 );

					// Update local data
					poWavRead->pcFileName = strdup( pcFileName );
					ms_assert_return( NULL != poWavRead->pcFileName, eRetCode = MS_ERR_INSUFFICIENT_MEMORY );
					memcpy( &poWavRead->oSfInfo, &oSfInfo, sizeof (SF_INFO));

					poWavRead->poFile = poFile;
				}
				else
				{
					eRetCode = MS_ERR_FRAMEWORK;
				}
				break;
			}

			case MCC_WAVREAD_FILEOFFSET:
			{
				MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "offset = %d\n", (long)pvConfigData_i );
				if( NULL != poWavRead->poFile )
				{
					sf_seek( poWavRead->poFile, (long)pvConfigData_i, SEEK_SET );
				}
				else
				{
					eRetCode = MS_ERR_INVALID_COMP_STATUS;
				}
				break;
			}

			case MCC_WAVREAD_TIMER_MS:
			{
				poWavRead->unReadTimerMs = (unsigned int)pvConfigData_i;
				break;
			}

			case MCC_WAVREAD_READ_PER_TIMEOUT:
			{
				poWavRead->unNumReadsPerTimeout = (unsigned int)pvConfigData_i;
				break;
			}

			case MCC_WAVREAD_READ_SIZE:
			{
				poWavRead->unReadSize = (unsigned int)pvConfigData_i;
				break;
			}

			default:
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poWavRead, poWavRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

