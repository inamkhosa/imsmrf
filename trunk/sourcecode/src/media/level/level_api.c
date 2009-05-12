#include "level_priv.h"

e_MsRetCode LevelInit( h_CompConfig hCompConfig_i, h_MediaComp* phLevel_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_LevelConfig* poLevelConfig = (t_LevelConfig*)hCompConfig_i;

	MS_TRACE( poLevelConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poLevelConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poLevelConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phLevel_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_Level* poLevel = NULL;

		// Allocate Level structure
		poLevel = FW_Malloc( sizeof (t_Level));
		if( NULL != poLevel )
		{
			poLevel->oComp.eType = MCT_LEVEL;
			poLevel->oComp.eGroup = MCG_FILTER;
			// Populate Level structure
			poLevel->oComp.nTraceId = poLevelConfig->oConfig.nTraceId;
			poLevel->oComp.pvFrameworkCtx = poLevelConfig->oConfig.pvFrameworkCtx;
			memset( &poLevel->oAppData, 0, sizeof (t_MediaCompAppData));

			poLevel->nMagicNum = MCT_LEVEL_MAGIC_NUM;
			// Convert Db in Square
			poLevel->gdSileceThresholdSq = pow((double)10, ((double)poLevelConfig->nSilenceThresholdDb)/10 );

			MS_TRACE( poLevelConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Silence threshold = %d(db),%f(sq)\n", poLevelConfig->nSilenceThresholdDb, poLevel->gdSileceThresholdSq );

			poLevel->bSoundPresent = AMPS_FALSE;

			*phLevel_o = (h_MediaComp) poLevel;
		}
		else
		{
			MS_TRACE( poLevelConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_Level\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poLevelConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode LevelSetAppData( h_MediaComp hLevel_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Level* poLevel = hLevel_i;
	t_MediaCompAppData* poAppData_i = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poLevel, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData_i->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData_i->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_LEVEL_MAGIC_NUM == poLevel->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poLevel->oAppData = *poAppData_i;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode LevelDestroy( h_MediaComp hLevel_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Level* poLevel = hLevel_i;
	int nTraceId = poLevel->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poLevel, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_LEVEL_MAGIC_NUM == poLevel->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		memset( poLevel, 0, sizeof (t_Level) );
		FW_Free( poLevel );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode LevelSetStatus( h_MediaComp hLevel_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Level* poLevel = hLevel_i;

	MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poLevel, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_LEVEL_MAGIC_NUM == poLevel->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode LevelSetParam( h_MediaComp hLevel_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Level* poLevel = hLevel_i;

	MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poLevel, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_LEVEL_MAGIC_NUM == poLevel->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_LEVEL_SILENCE_THRESHOLD_DB:
			{
				int nSilenceThresholdDb = (int)pvConfigData_i;
				poLevel->gdSileceThresholdSq = pow((double)10, ((double)nSilenceThresholdDb)/10 );
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

	MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

