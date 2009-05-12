#include "fileread_priv.h"

e_MsRetCode FileReadInit( h_CompConfig hCompConfig_i, h_MediaComp* phFileRead_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileReadConfig* poFileReadConfig = (t_FileReadConfig*)hCompConfig_i;

	MS_TRACE( poFileReadConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileReadConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poFileReadConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phFileRead_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_FileRead* poFileRead = NULL;

		// Allocate FileRead structure
		poFileRead = FW_Malloc( sizeof (t_FileRead));
		if( NULL != poFileRead )
		{
			poFileRead->oComp.eType = MCT_FILEREAD;
			poFileRead->oComp.eGroup = MCG_SOURCE;
			// Populate FileRead structure
			poFileRead->oComp.nTraceId = poFileReadConfig->oConfig.nTraceId;
			poFileRead->oComp.pvFrameworkCtx = poFileReadConfig->oConfig.pvFrameworkCtx;
			memset( &poFileRead->oAppData, 0, sizeof (t_MediaCompAppData));

			poFileRead->nMagicNum = MCT_FILEREAD_MAGIC_NUM;
			if( NULL != poFileReadConfig->pcFileName )
			{
				poFileRead->pcFileName = strdup( poFileReadConfig->pcFileName );
				ms_assert_return( NULL != poFileRead->pcFileName, eRetCode = MS_ERR_INSUFFICIENT_MEMORY );
			}
			poFileRead->unReadSize = poFileReadConfig->unReadSize;
			poFileRead->unNumReadsPerTimeout = poFileReadConfig->unNumReadsPerTimeout;
//          poFileRead->unMode = poFileReadConfig->unMode;
			poFileRead->unReadTimerMs = poFileReadConfig->unReadTimerMs;
			// Open file
			if( NULL != poFileRead->pcFileName )
			{
				poFileRead->poFile  = fopen( poFileRead->pcFileName, "rb" );
				if( NULL != poFileRead->poFile )
				{
					MS_TRACE( poFileReadConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "File opened successfully\n" );
				}
				else
				{
					MS_TRACE( poFileReadConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "fopen returned with error  = %d\n", errno );

					switch( errno )
					{
						case EACCES: // Permission denied
						{
							break;
						}
					}

					FW_Free( poFileRead );
					eRetCode = MS_ERR_FRAMEWORK;
				}
			}
			if( MS_ERR_NONE == eRetCode )
			{
				*phFileRead_o = (h_MediaComp) poFileRead;
			}
		}
		else
		{
			MS_TRACE( poFileReadConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_FileRead\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poFileReadConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode FileReadSetAppData( h_MediaComp hFileRead_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileRead* poFileRead = hFileRead_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileRead, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_FILEREAD_MAGIC_NUM == poFileRead->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poFileRead->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode FileReadDestroy( h_MediaComp hFileRead_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileRead* poFileRead = hFileRead_i;
	int nTraceId = poFileRead->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileRead, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_FILEREAD_MAGIC_NUM == poFileRead->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( NULL != poFileRead->poFile )
		{
			fclose( poFileRead->poFile );
		}
		if( NULL != poFileRead->pcFileName )
		{
			FW_Free( poFileRead->pcFileName );
		}
		FW_Free( poFileRead );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode FileReadSetStatus( h_MediaComp hFileRead_i, e_MediaCompStatus eStatus_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileRead* poFileRead = hFileRead_i;

	MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileRead, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_FILEREAD_MAGIC_NUM == poFileRead->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eStatus_i )
		{
			case MCS_START:
			{
				poFileRead->hReadTimerId = FW_StartTimer( poFileRead->oComp.pvFrameworkCtx, poFileRead->unReadTimerMs, FileReadTimeOutNotifyCallBack, poFileRead );
				if( NULL != poFileRead->hReadTimerId )
				{
					MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Timer started successfully\n" );
				}
				else
				{
					MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_StartTimer returned with error\n" );

					eRetCode = MS_ERR_FRAMEWORK;
				}
				break;
			}

			case MCS_STOP:
			{
				int nRetVal = AMPS_SUCCESS;
				if( NULL != poFileRead->hReadTimerId )
				{
					nRetVal = FW_DeleteTimer( poFileRead->oComp.pvFrameworkCtx, poFileRead->hReadTimerId  );
					if( AMPS_SUCCESS != nRetVal )
					{
						MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_DeleteTimer returned with ret-code = %d\n", nRetVal );

						eRetCode = MS_ERR_FRAMEWORK;
					}
					else
					{
						poFileRead->hReadTimerId = NULL;
					}
				}
				else
				{
					MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_WARNING, "Component already stopped\n", nRetVal );
				}
				break;
			}

			default:
			{
				MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus_i );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode FileReadSetParam( h_MediaComp hFileRead_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileRead* poFileRead = hFileRead_i;

	MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileRead, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_FILEREAD_MAGIC_NUM == poFileRead->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		FILE* poFile = NULL;

		switch( eConfig_i )
		{
			case MCC_FILEREAD_FILENAME:
			{
				char* pcFileName = (char*)pvConfigData_i;

				MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Filename = %s\n", pcFileName );

				// Open file
				poFile = fopen( pcFileName, "rb" );
				if( NULL != poFile )
				{
					if( NULL != poFileRead->pcFileName )
					{
						// Close previous file
						fclose( poFileRead->poFile );

						FW_Free( poFileRead->pcFileName );
					}
					// Update local data
					poFileRead->pcFileName = strdup( pcFileName );
					ms_assert_return( NULL != poFileRead->pcFileName, eRetCode = MS_ERR_INSUFFICIENT_MEMORY );

					poFileRead->poFile = poFile;
				}
				else
				{
					eRetCode = MS_ERR_FRAMEWORK;
				}
				break;
			}

			case MCC_FILEREAD_FILEOFFSET:
			{
				MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "offset = %d\n", (long)pvConfigData_i );
				if( NULL != poFileRead->poFile )
				{
					fseek( poFileRead->poFile, (long)pvConfigData_i, SEEK_SET );
				}
				else
				{
					eRetCode = MS_ERR_INVALID_COMP_STATUS;
				}
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

	MEDIA_TRACE( poFileRead, poFileRead->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

