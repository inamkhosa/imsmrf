#include "pcmuenc_priv.h"

e_MsRetCode PcmuEncInit( h_CompConfig hCompConfig_i, h_MediaComp* phPcmuEnc_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_PcmuEncConfig* poPcmuEncConfig = (t_PcmuEncConfig*)hCompConfig_i;

	MS_TRACE( poPcmuEncConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poPcmuEncConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poPcmuEncConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phPcmuEnc_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_PcmuEnc* poPcmuEnc = NULL;

		// Allocate PcmuEnc structure
		poPcmuEnc = FW_Malloc( sizeof (t_PcmuEnc));
		if( NULL != poPcmuEnc )
		{
			poPcmuEnc->oComp.eType = MCT_PCMUENC;
			poPcmuEnc->oComp.eGroup = MCG_FILTER;
			// Populate PcmuEnc structure
			poPcmuEnc->oComp.nTraceId = poPcmuEncConfig->oConfig.nTraceId;
			poPcmuEnc->oComp.pvFrameworkCtx = poPcmuEncConfig->oConfig.pvFrameworkCtx;
			memset( &poPcmuEnc->oAppData, 0, sizeof (t_MediaCompAppData));

			poPcmuEnc->nMagicNum = MCT_PCMUENC_MAGIC_NUM;
			poPcmuEnc->unStartOfMedia = 1;

			*phPcmuEnc_o = (h_MediaComp) poPcmuEnc;
		}
		else
		{
			MS_TRACE( poPcmuEncConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_PcmuEnc\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poPcmuEncConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode PcmuEncSetAppData( h_MediaComp hPcmuEnc_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_PcmuEnc* poPcmuEnc = hPcmuEnc_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poPcmuEnc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_PCMUENC_MAGIC_NUM == poPcmuEnc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poPcmuEnc->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode PcmuEncDestroy( h_MediaComp hPcmuEnc_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_PcmuEnc* poPcmuEnc = hPcmuEnc_i;
	int nTraceId = poPcmuEnc->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poPcmuEnc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_PCMUENC_MAGIC_NUM == poPcmuEnc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		memset( poPcmuEnc, 0, sizeof (t_PcmuEnc) );
		FW_Free( poPcmuEnc );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode PcmuEncSetStatus( h_MediaComp hPcmuEnc_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_PcmuEnc* poPcmuEnc = hPcmuEnc_i;

	MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poPcmuEnc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_PCMUENC_MAGIC_NUM == poPcmuEnc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

