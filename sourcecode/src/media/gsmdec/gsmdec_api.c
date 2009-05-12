#include "gsmdec_priv.h"

e_MsRetCode GsmDecInit( h_CompConfig hCompConfig_i, h_MediaComp* phGsmDec_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_GsmDecConfig* poGsmDecConfig = (t_GsmDecConfig*)hCompConfig_i;

	MS_TRACE( poGsmDecConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poGsmDecConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poGsmDecConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phGsmDec_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_GsmDec* poGsmDec = NULL;

		// Allocate GsmDec structure
		poGsmDec = FW_Malloc( sizeof (t_GsmDec));
		if( NULL != poGsmDec )
		{
			poGsmDec->oComp.eType = MCT_GSMDEC;
			poGsmDec->oComp.eGroup = MCG_FILTER;
			// Populate GsmDec structure
			poGsmDec->oComp.nTraceId = poGsmDecConfig->oConfig.nTraceId;
			poGsmDec->oComp.pvFrameworkCtx = poGsmDecConfig->oConfig.pvFrameworkCtx;
			memset( &poGsmDec->oAppData, 0, sizeof (t_MediaCompAppData));

			poGsmDec->nMagicNum = MCT_GSMDEC_MAGIC_NUM;

			poGsmDec->poDecoder = gsm_create();
			if( NULL == poGsmDec->poDecoder )
			{
				int opt = 0;
				gsm_option( poGsmDec->poDecoder, GSM_OPT_FAST, &opt );
				gsm_option( poGsmDec->poDecoder, GSM_OPT_VERBOSE, &opt );

				eRetCode =  MS_ERR_INSUFFICIENT_MEMORY;
				FW_Free( poGsmDec );
			}
			else
			{
				*phGsmDec_o = (h_MediaComp) poGsmDec;
			}
		}
		else
		{
			MS_TRACE( poGsmDecConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_GsmDec\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poGsmDecConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode GsmDecSetAppData( h_MediaComp hGsmDec_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_GsmDec* poGsmDec = hGsmDec_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poGsmDec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_GSMDEC_MAGIC_NUM == poGsmDec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poGsmDec->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode GsmDecDestroy( h_MediaComp hGsmDec_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_GsmDec* poGsmDec = hGsmDec_i;
	int nTraceId = poGsmDec->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poGsmDec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_GSMDEC_MAGIC_NUM == poGsmDec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		gsm_destroy( poGsmDec->poDecoder );

		memset( poGsmDec, 0, sizeof (t_GsmDec));
		FW_Free( poGsmDec );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode GsmDecSetStatus( h_MediaComp hGsmDec_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_GsmDec* poGsmDec = hGsmDec_i;

	MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poGsmDec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_GSMDEC_MAGIC_NUM == poGsmDec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode GsmDecSetParam( h_MediaComp hGsmDec_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_GsmDec* poGsmDec = hGsmDec_i;

	MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poGsmDec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_GSMDEC_MAGIC_NUM == poGsmDec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			default:
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

