#include "gsmenc_priv.h"

e_MsRetCode GsmEncInit( h_CompConfig hCompConfig_i, h_MediaComp* phGsmEnc_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_GsmEncConfig* poGsmEncConfig = (t_GsmEncConfig*)hCompConfig_i;

	MS_TRACE( poGsmEncConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poGsmEncConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poGsmEncConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phGsmEnc_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_GsmEnc* poGsmEnc = NULL;

		// Allocate GsmEnc structure
		poGsmEnc = FW_Malloc( sizeof (t_GsmEnc));
		if( NULL != poGsmEnc )
		{
			poGsmEnc->oComp.eType = MCT_GSMENC;
			poGsmEnc->oComp.eGroup = MCG_FILTER;
			// Populate GsmEnc structure
			poGsmEnc->oComp.nTraceId = poGsmEncConfig->oConfig.nTraceId;
			poGsmEnc->oComp.pvFrameworkCtx = poGsmEncConfig->oConfig.pvFrameworkCtx;
			memset( &poGsmEnc->oAppData, 0, sizeof (t_MediaCompAppData));

			poGsmEnc->nMagicNum = MCT_GSMENC_MAGIC_NUM;
			poGsmEnc->unStartOfMedia = 1;

			poGsmEnc->poEncoder = gsm_create();
			if( NULL == poGsmEnc->poEncoder )
			{
				int opt = 0;
				gsm_option( poGsmEnc->poEncoder, GSM_OPT_FAST, &opt );
				gsm_option( poGsmEnc->poEncoder, GSM_OPT_VERBOSE, &opt );
				gsm_option( poGsmEnc->poEncoder, GSM_OPT_LTP_CUT, &opt );

				eRetCode =  MS_ERR_INSUFFICIENT_MEMORY;
				FW_Free( poGsmEnc );
			}
			else
			{
				*phGsmEnc_o = (h_MediaComp) poGsmEnc;
			}
		}
		else
		{
			MS_TRACE( poGsmEncConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_GsmEnc\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poGsmEncConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode GsmEncSetAppData( h_MediaComp hGsmEnc_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_GsmEnc* poGsmEnc = hGsmEnc_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poGsmEnc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_GSMENC_MAGIC_NUM == poGsmEnc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poGsmEnc->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode GsmEncDestroy( h_MediaComp hGsmEnc_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_GsmEnc* poGsmEnc = hGsmEnc_i;
	int nTraceId = poGsmEnc->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poGsmEnc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_GSMENC_MAGIC_NUM == poGsmEnc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		gsm_destroy( poGsmEnc->poEncoder );

		memset( poGsmEnc, 0, sizeof (t_GsmEnc));
		FW_Free( poGsmEnc );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode GsmEncSetStatus( h_MediaComp hGsmEnc_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_GsmEnc* poGsmEnc = hGsmEnc_i;

	MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poGsmEnc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_GSMENC_MAGIC_NUM == poGsmEnc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

