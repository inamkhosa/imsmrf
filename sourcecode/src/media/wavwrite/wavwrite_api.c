#include "wavwrite_priv.h"

e_MsRetCode WavWriteInit( h_CompConfig hCompConfig_i, h_MediaComp* phWavWrite_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavWriteConfig* poWavWriteConfig = (t_WavWriteConfig*)hCompConfig_i;

	MS_TRACE( poWavWriteConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavWriteConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poWavWriteConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phWavWrite_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_WavWrite* poWavWrite = NULL;

		// Allocate WavWrite structure
		poWavWrite = FW_Malloc( sizeof (t_WavWrite));
		if( NULL != poWavWrite )
		{
			poWavWrite->oComp.eType = MCT_WAVWRITE;
			poWavWrite->oComp.eGroup = MCG_SINK;
			// Populate WavWrite structure
			poWavWrite->oComp.nTraceId = poWavWriteConfig->oConfig.nTraceId;
			poWavWrite->oComp.pvFrameworkCtx = poWavWriteConfig->oConfig.pvFrameworkCtx;

			poWavWrite->nMagicNum = MCT_WAVWRITE_MAGIC_NUM;
			if( NULL != poWavWriteConfig->pcFileName )
			{
				poWavWrite->pcFileName = strdup( poWavWriteConfig->pcFileName );
				ms_assert_return( NULL != poWavWrite->pcFileName, eRetCode = MS_ERR_INSUFFICIENT_MEMORY );
			}
			poWavWrite->unMode = poWavWriteConfig->unMode;
			poWavWrite->nByteWritten = 0;

			memset( &poWavWrite->oAppData, 0, sizeof (t_MediaCompAppData));
			if( NULL != poWavWrite->pcFileName )
			{
				poWavWrite->oSfInfo.samplerate = 8000;
				poWavWrite->oSfInfo.channels = 1;
				poWavWrite->oSfInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16 | SF_ENDIAN_LITTLE;
				poWavWrite->oSfInfo.sections = 1;
				poWavWrite->oSfInfo.seekable = 1;

				ms_assert( TRUE == sf_format_check( &poWavWrite->oSfInfo ));

				//TODO: Create directory, if it does not exist

				poWavWrite->poFile  = sf_open( poWavWrite->pcFileName, SFM_WRITE, &poWavWrite->oSfInfo );
				if( NULL != poWavWrite->poFile )
				{
					MS_TRACE( poWavWriteConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "File opened successfully\n" );
				}
				else
				{
					MS_TRACE( poWavWriteConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "sf_open returned with error  = %s\n", sf_perror( NULL ) );

					FW_Free( poWavWrite );
					eRetCode = MS_ERR_FRAMEWORK;
				}
			}
			if( MS_ERR_NONE == eRetCode )
			{
				*phWavWrite_o = (h_MediaComp) poWavWrite;
			}
		}
		else
		{
			MS_TRACE( poWavWriteConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_WavWrite\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phWavWrite_o );
		}
	}

	MS_TRACE( poWavWriteConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode WavWriteSetAppData( h_MediaComp hWavWrite_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavWrite* poWavWrite = hWavWrite_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		//ms_assert_return( NULL != poAppData->pfAppDataCb	, eRetCode = MS_ERR_PRE_COND_FAILED);
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_WAVWRITE_MAGIC_NUM == poWavWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poWavWrite->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode WavWriteDestroy( h_MediaComp hWavWrite_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavWrite* poWavWrite = hWavWrite_i;
	int nTraceId = poWavWrite->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_WAVWRITE_MAGIC_NUM == poWavWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( NULL != poWavWrite->poFile )
		{
			sf_close( poWavWrite->poFile );
		}
		if( NULL != poWavWrite->pcFileName )
		{
			FW_Free( poWavWrite->pcFileName );
		}
		FW_Free( poWavWrite );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode WavWriteSetStatus( h_MediaComp hWavWrite_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavWrite* poWavWrite = hWavWrite_i;

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_WAVWRITE_MAGIC_NUM == poWavWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode WavWriteGetParam( h_MediaComp hWavWrite_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavWrite* poWavWrite = hWavWrite_i;

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_WAVWRITE_MAGIC_NUM == poWavWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_WAVWRITE_BYTES_WRITTEN:
			{
				int* pnByteWritten = (int*)pvConfigData_i;

				*pnByteWritten = poWavWrite->nByteWritten;
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

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode WavWriteSetParam( h_MediaComp hWavWrite_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_WavWrite* poWavWrite = hWavWrite_i;

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_WAVWRITE_MAGIC_NUM == poWavWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_WAVWRITE_FILENAME:
			{
				char* pcFileName = (char*)pvConfigData_i;
				SNDFILE* poFile;

				poWavWrite->oSfInfo.samplerate = 8000;
				poWavWrite->oSfInfo.channels = 1;
				poWavWrite->oSfInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16 | SF_ENDIAN_LITTLE;
				poWavWrite->oSfInfo.sections = 1;
				poWavWrite->oSfInfo.seekable = 1;

				//TODO: Create directory, if it does not exist

				// Open file
				poFile  = sf_open( pcFileName, SFM_WRITE, &poWavWrite->oSfInfo );
				if( NULL != poFile )
				{
					MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "File opened successfully\n" );
					// Close previous file
					if( NULL != poWavWrite->poFile )
					{
						sf_close( poWavWrite->poFile );
					}
					poWavWrite->poFile = poFile;
					poWavWrite->nByteWritten = 0;
					if( NULL != poWavWrite->pcFileName )
					{
						FW_Free( poWavWrite->pcFileName );
					}
					poWavWrite->pcFileName = strdup( pcFileName );
				}
				else
				{
					MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "sf_open returned with error  = %s\n", sf_perror( NULL ) );

					FW_Free( poWavWrite );
					eRetCode = MS_ERR_FRAMEWORK;
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

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

