#include "null_priv.h"

e_MsRetCode NullInit( h_CompConfig hCompConfig_i, h_MediaComp* phNull_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_NullConfig* poNullConfig = (t_NullConfig*)hCompConfig_i;

	MS_TRACE( poNullConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poNullConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poNullConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phNull_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_Null* poNull = NULL;

		// Allocate Null structure
		poNull = FW_Malloc( sizeof (t_Null));
		if( NULL != poNull )
		{
			poNull->oComp.eType = MCT_NULL;
			poNull->oComp.eGroup = MCG_SINK;
			// Populate Null structure
			poNull->oComp.nTraceId = poNullConfig->oConfig.nTraceId;
			poNull->oComp.pvFrameworkCtx = poNullConfig->oConfig.pvFrameworkCtx;
			memset( &poNull->oAppData, 0, sizeof (t_MediaCompAppData));

			poNull->nMagicNum = MCT_NULL_MAGIC_NUM;

			*phNull_o = (h_MediaComp) poNull;
		}
		else
		{
			MS_TRACE( poNullConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_Null\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poNullConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode NullSetAppData( h_MediaComp hNull_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Null* poNull = hNull_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poNull, poNull->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poNull, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_NULL_MAGIC_NUM == poNull->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poNull->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poNull, poNull->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode NullDestroy( h_MediaComp hNull_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Null* poNull = hNull_i;
	int nTraceId = poNull->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poNull, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_NULL_MAGIC_NUM == poNull->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		memset( poNull, 0, sizeof (t_Null));
		FW_Free( poNull );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode NullSetStatus( h_MediaComp hNull_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Null* poNull = hNull_i;

	MEDIA_TRACE( poNull, poNull->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poNull, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_NULL_MAGIC_NUM == poNull->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poNull, poNull->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poNull, poNull->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

