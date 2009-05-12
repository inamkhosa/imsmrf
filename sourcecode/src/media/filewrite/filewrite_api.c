#include "filewrite_priv.h"

e_MsRetCode FileWriteInit( h_CompConfig hCompConfig_i, h_MediaComp* phFileWrite_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileWriteConfig* poFileWriteConfig = (t_FileWriteConfig*)hCompConfig_i;

	MS_TRACE( poFileWriteConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileWriteConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poFileWriteConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phFileWrite_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_FileWrite* poFileWrite = NULL;

		// Allocate FileWrite structure
		poFileWrite = FW_Malloc( sizeof (t_FileWrite));
		if( NULL != poFileWrite )
		{
			int nRetVal = AMPS_SUCCESS;

			poFileWrite->oComp.eType = MCT_FILEWRITE;
			poFileWrite->oComp.eGroup = MCG_SINK;
			// Populate FileWrite structure
			poFileWrite->oComp.nTraceId = poFileWriteConfig->oConfig.nTraceId;
			poFileWrite->oComp.pvFrameworkCtx = poFileWriteConfig->oConfig.pvFrameworkCtx;

			poFileWrite->nMagicNum = MCT_FILEWRITE_MAGIC_NUM;
			if( NULL != poFileWriteConfig->pcFileName )
			{
				poFileWrite->pcFileName = strdup( poFileWriteConfig->pcFileName );
				ms_assert_return( NULL != poFileWrite->pcFileName, eRetCode = MS_ERR_INSUFFICIENT_MEMORY );
			}
			poFileWrite->unMode = poFileWriteConfig->unMode;
			poFileWrite->nByteWritten = 0;

			memset( &poFileWrite->oAppData, 0, sizeof (t_MediaCompAppData));
			if( NULL != poFileWrite->pcFileName )
			{
				// Open file
				nRetVal = FW_CreateFileObject( poFileWrite->oComp.pvFrameworkCtx, poFileWrite->pcFileName, &poFileWrite->hFile, poFileWrite->unMode );
				if( AMPS_SUCCESS == nRetVal )
				{
					MS_TRACE( poFileWriteConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "File opened successfully\n" );
				}
				else
				{
					MS_TRACE( poFileWriteConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_CreateFileObject returned with ret-code = %d\n", nRetVal );

					FW_Free( poFileWrite );
					poFileWrite = NULL;
					eRetCode = MS_ERR_FRAMEWORK;
				}
			}
			if( MS_ERR_NONE == eRetCode )
			{
				*phFileWrite_o = (h_MediaComp) poFileWrite;
			}
		}
		else
		{
			MS_TRACE( poFileWriteConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_FileWrite\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phFileWrite_o );
		}
	}

	MS_TRACE( poFileWriteConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode FileWriteSetAppData( h_MediaComp hFileWrite_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileWrite* poFileWrite = hFileWrite_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		//ms_assert_return( NULL != poAppData->pfAppDataCb	, eRetCode = MS_ERR_PRE_COND_FAILED);
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_FILEWRITE_MAGIC_NUM == poFileWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poFileWrite->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode FileWriteDestroy( h_MediaComp hFileWrite_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileWrite* poFileWrite = hFileWrite_i;
	int nTraceId = poFileWrite->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_FILEWRITE_MAGIC_NUM == poFileWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nRetVal = 0;
		if( NULL != poFileWrite->hFile )
		{
			nRetVal = FW_DestroyFileObject( poFileWrite->oComp.pvFrameworkCtx, poFileWrite->hFile );
			if( AMPS_SUCCESS != nRetVal )
			{
				MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_DestroyFileObject returned with ret-code = %d\n", nRetVal );
			}
		}
		if( NULL != poFileWrite->pcFileName )
		{
			FW_Free( poFileWrite->pcFileName );
		}
		FW_Free( poFileWrite );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode FileWriteSetStatus( h_MediaComp hFileWrite_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileWrite* poFileWrite = hFileWrite_i;

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_FILEWRITE_MAGIC_NUM == poFileWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eStatus )
		{
			case MCS_START:
			{
				break;
			}

			case MCS_STOP:
			{
				break;
			}

			default:
			{
				MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode FileWriteGetParam( h_MediaComp hFileWrite_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileWrite* poFileWrite = hFileWrite_i;

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_FILEWRITE_MAGIC_NUM == poFileWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_FILEWRITE_BYTES_WRITTEN:
			{
				int* pnByteWritten = (int*)pvConfigData_i;

				*pnByteWritten = poFileWrite->nByteWritten;
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

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode FileWriteSetParam( h_MediaComp hFileWrite_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_FileWrite* poFileWrite = hFileWrite_i;

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_FILEWRITE_MAGIC_NUM == poFileWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_FILEWRITE_FILENAME:
			{
				char* pcFileName = (char*)pvConfigData_i;
				AMPS_FILE_HANDLE hFile;
				int nRetVal = 0;
				// Open file
				nRetVal = FW_CreateFileObject( poFileWrite->oComp.pvFrameworkCtx, pcFileName, &hFile, poFileWrite->unMode );
				if( AMPS_SUCCESS == nRetVal )
				{
					MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "File opened successfully\n" );
					// Close previous file
					if( NULL != poFileWrite->hFile )
					{
						nRetVal = FW_DestroyFileObject( poFileWrite->oComp.pvFrameworkCtx, poFileWrite->hFile );
						if( AMPS_SUCCESS != nRetVal )
						{
							MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_DestroyFileObject returned with ret-code = %d\n", nRetVal );
						}
					}
					poFileWrite->hFile = hFile;
					poFileWrite->nByteWritten = 0;
					if( NULL != poFileWrite->pcFileName )
					{
						FW_Free( poFileWrite->pcFileName );
					}
					poFileWrite->pcFileName = strdup( pcFileName );
				}
				else
				{
					MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_CreateFileObject returned with ret-code = %d\n", nRetVal );
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

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

