#include "g729enc_priv.h"

e_MsRetCode G729EncInit( h_CompConfig hCompConfig_i, h_MediaComp* phG729Enc_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_G729EncConfig* poG729EncConfig = (t_G729EncConfig*)hCompConfig_i;

	MS_TRACE( poG729EncConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poG729EncConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poG729EncConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phG729Enc_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_G729Enc* poG729Enc = NULL;

		// Allocate G729Enc structure
		poG729Enc = FW_Malloc( sizeof (t_G729Enc));
		if( NULL != poG729Enc )
		{
			poG729Enc->oComp.eType = MCT_G729ENC;
			poG729Enc->oComp.eGroup = MCG_FILTER;
			// Populate G729Enc structure
			poG729Enc->oComp.nTraceId = poG729EncConfig->oConfig.nTraceId;
			poG729Enc->oComp.pvFrameworkCtx = poG729EncConfig->oConfig.pvFrameworkCtx;
			memset( &poG729Enc->oAppData, 0, sizeof (t_MediaCompAppData));

			poG729Enc->nMagicNum = MCT_G729ENC_MAGIC_NUM;
			poG729Enc->unStartOfMedia = 1;

			// codec initialization

			Init_Pre_Process();
			Init_Coder_ld8k();
			int i;

			for( i = 0; i < PRM_SIZE; i++ )
			{
				poG729Enc->pn16_AnalysisParam[i] = (Word16)0;
			}
			{
				*phG729Enc_o = (h_MediaComp) poG729Enc;
			}
		}
		else
		{
			MS_TRACE( poG729EncConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_G729Enc\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poG729EncConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode G729EncSetAppData( h_MediaComp hG729Enc_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_G729Enc* poG729Enc = hG729Enc_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poG729Enc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_G729ENC_MAGIC_NUM == poG729Enc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poG729Enc->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode G729EncDestroy( h_MediaComp hG729Enc_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_G729Enc* poG729Enc = hG729Enc_i;
	int nTraceId = poG729Enc->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poG729Enc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_G729ENC_MAGIC_NUM == poG729Enc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
//      gsm_destroy( poG729Enc->poEncoder );

		memset( poG729Enc, 0, sizeof (t_G729Enc));
		FW_Free( poG729Enc );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode G729EncSetStatus( h_MediaComp hG729Enc_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_G729Enc* poG729Enc = hG729Enc_i;

	MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poG729Enc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_G729ENC_MAGIC_NUM == poG729Enc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

