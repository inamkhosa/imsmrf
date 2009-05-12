#include "g729dec_priv.h"

e_MsRetCode G729DecInit( h_CompConfig hCompConfig_i, h_MediaComp* phG729Dec_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_G729DecConfig* poG729DecConfig = (t_G729DecConfig*)hCompConfig_i;

	MS_TRACE( poG729DecConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poG729DecConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poG729DecConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phG729Dec_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_G729Dec* poG729Dec = NULL;

		// Allocate G729Dec structure
		poG729Dec = FW_Malloc( sizeof (t_G729Dec));
		if( NULL != poG729Dec )
		{
			poG729Dec->oComp.eType = MCT_G729DEC;
			poG729Dec->oComp.eGroup = MCG_FILTER;
			// Populate G729Dec structure
			poG729Dec->oComp.nTraceId = poG729DecConfig->oConfig.nTraceId;
			poG729Dec->oComp.pvFrameworkCtx = poG729DecConfig->oConfig.pvFrameworkCtx;
			memset( &poG729Dec->oAppData, 0, sizeof (t_MediaCompAppData));

			poG729Dec->nMagicNum = MCT_G729DEC_MAGIC_NUM;
			poG729Dec->unStartOfMedia = 1;

			// decoder initialization
			int i;

			for( i = 0; i < M; i++ )
			{
				poG729Dec->synth_buf[i] = 0;
			}
			poG729Dec->synth = poG729Dec->synth_buf + M;
			poG729Dec->voicing = 60;

			Init_Decod_ld8k();
			Init_Post_Filter();
			Init_Post_Process();

			{
				*phG729Dec_o = (h_MediaComp) poG729Dec;
			}
		}
		else
		{
			MS_TRACE( poG729DecConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_G729Dec\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poG729DecConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode G729DecSetAppData( h_MediaComp hG729Dec_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_G729Dec* poG729Dec = hG729Dec_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poG729Dec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_G729DEC_MAGIC_NUM == poG729Dec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poG729Dec->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode G729DecDestroy( h_MediaComp hG729Dec_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_G729Dec* poG729Dec = hG729Dec_i;
	int nTraceId = poG729Dec->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poG729Dec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_G729DEC_MAGIC_NUM == poG729Dec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
//      gsm_destroy( poG729Dec->poEncoder );

		memset( poG729Dec, 0, sizeof (t_G729Dec));
		FW_Free( poG729Dec );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode G729DecSetStatus( h_MediaComp hG729Dec_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_G729Dec* poG729Dec = hG729Dec_i;

	MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poG729Dec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_G729DEC_MAGIC_NUM == poG729Dec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

