#include "pcmudec_priv.h"

e_MsRetCode PcmuDecInit( h_CompConfig hCompConfig_i, h_MediaComp* phPcmuDec_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_PcmuDecConfig* poPcmuDecConfig = (t_PcmuDecConfig*)hCompConfig_i;

	MS_TRACE( poPcmuDecConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poPcmuDecConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poPcmuDecConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phPcmuDec_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_PcmuDec* poPcmuDec = NULL;

		// Allocate PcmuDec structure
		poPcmuDec = FW_Malloc( sizeof (t_PcmuDec));
		if( NULL != poPcmuDec )
		{
			poPcmuDec->oComp.eType = MCT_PCMUDEC;
			poPcmuDec->oComp.eGroup = MCG_FILTER;
			// Populate PcmuDec structure
			poPcmuDec->oComp.nTraceId = poPcmuDecConfig->oConfig.nTraceId;
			poPcmuDec->oComp.pvFrameworkCtx = poPcmuDecConfig->oConfig.pvFrameworkCtx;
			memset( &poPcmuDec->oAppData, 0, sizeof (t_MediaCompAppData));

			poPcmuDec->nMagicNum = MCT_PCMUDEC_MAGIC_NUM;

			*phPcmuDec_o = (h_MediaComp) poPcmuDec;
		}
		else
		{
			MS_TRACE( poPcmuDecConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_PcmuDec\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poPcmuDecConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode PcmuDecSetAppData( h_MediaComp hPcmuDec_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_PcmuDec* poPcmuDec = hPcmuDec_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poPcmuDec, poPcmuDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poPcmuDec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_PCMUDEC_MAGIC_NUM == poPcmuDec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poPcmuDec->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poPcmuDec, poPcmuDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode PcmuDecDestroy( h_MediaComp hPcmuDec_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_PcmuDec* poPcmuDec = hPcmuDec_i;
	int nTraceId = poPcmuDec->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poPcmuDec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_PCMUDEC_MAGIC_NUM == poPcmuDec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		memset( poPcmuDec, 0, sizeof (t_PcmuDec));
		FW_Free( poPcmuDec );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode PcmuDecSetStatus( h_MediaComp hPcmuDec_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_PcmuDec* poPcmuDec = hPcmuDec_i;

	MEDIA_TRACE( poPcmuDec, poPcmuDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poPcmuDec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_PCMUDEC_MAGIC_NUM == poPcmuDec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poPcmuDec, poPcmuDec->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poPcmuDec, poPcmuDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

