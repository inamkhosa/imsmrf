#include "rtpsend_priv.h"

e_MsRetCode RtpSendInit( h_CompConfig hCompConfig_i, h_MediaComp* phRtpSend_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpSendConfig* poRtpSendConfig = (t_RtpSendConfig*)hCompConfig_i;

	MS_TRACE( poRtpSendConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpSendConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poRtpSendConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phRtpSend_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_RtpSend* poRtpSend = NULL;

		// Allocate RtpSend structure
		poRtpSend = FW_Malloc( sizeof (t_RtpSend));
		if( NULL != poRtpSend )
		{
			poRtpSend->oComp.eType = MCT_RTPSEND;
			poRtpSend->oComp.eGroup = MCG_FILTER;
			// Populate RtpSend structure
			poRtpSend->oComp.nTraceId = poRtpSendConfig->oConfig.nTraceId;
			poRtpSend->oComp.pvFrameworkCtx = poRtpSendConfig->oConfig.pvFrameworkCtx;
			memset( &poRtpSend->oAppData, 0, sizeof (t_MediaCompAppData));

			poRtpSend->nMagicNum = MCT_RTPSEND_MAGIC_NUM;

			poRtpSend->usSeqNo = random32( 1 );
			poRtpSend->unSsrc = random32( 2 );
			poRtpSend->unFirstPktTimestamp = random32( 3 );
			poRtpSend->nSamplingMultiplier = poRtpSendConfig->nSamplingRate / 1000;

			*phRtpSend_o = (h_MediaComp) poRtpSend;
		}
		else
		{
			MS_TRACE( poRtpSendConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_RtpSend\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poRtpSendConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpSendSetAppData( h_MediaComp hRtpSend_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpSend* poRtpSend = hRtpSend_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RTPSEND_MAGIC_NUM == poRtpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poRtpSend->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpSendDestroy( h_MediaComp hRtpSend_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpSend* poRtpSend = hRtpSend_i;
	int nTraceId = poRtpSend->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RTPSEND_MAGIC_NUM == poRtpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		memset( poRtpSend, 0, sizeof (t_RtpSend));
		FW_Free( poRtpSend );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpSendSetStatus( h_MediaComp hRtpSend_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpSend* poRtpSend = hRtpSend_i;

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RTPSEND_MAGIC_NUM == poRtpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpSendSetParam( h_MediaComp hRtpSend_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpSend* poRtpSend = hRtpSend_i;

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RTPSEND_MAGIC_NUM == poRtpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_RTPSEND_SAMPLING_RATE:
			{
				poRtpSend->nSamplingMultiplier = ((int)pvConfigData_i)/ 1000;
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

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpSendGetParam( h_MediaComp hRtpSend_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpSend* poRtpSend = hRtpSend_i;

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RTPSEND_MAGIC_NUM == poRtpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_RTPSEND_NUM_PKTS_SENT:
			{
				unsigned int* punPktsSent = (unsigned int*)pvConfigData_i;

				*punPktsSent = poRtpSend->unPktsSent;
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

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

