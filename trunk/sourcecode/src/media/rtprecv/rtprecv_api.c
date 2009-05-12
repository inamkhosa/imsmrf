#include "rtprecv_priv.h"

e_MsRetCode RtpRecvInit( h_CompConfig hCompConfig_i, h_MediaComp* phRtpRecv_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpRecvConfig* poRtpRecvConfig = (t_RtpRecvConfig*)hCompConfig_i;

	MS_TRACE( poRtpRecvConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpRecvConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poRtpRecvConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phRtpRecv_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_RtpRecv* poRtpRecv = NULL;

		// Allocate RtpRecv structure
		poRtpRecv = FW_Malloc( sizeof (t_RtpRecv));
		if( NULL != poRtpRecv )
		{
			poRtpRecv->oComp.eType = MCT_RTPRECV;
			poRtpRecv->oComp.eGroup = MCG_FILTER;
			// Populate RtpRecv structure
			poRtpRecv->oComp.nTraceId = poRtpRecvConfig->oConfig.nTraceId;
			poRtpRecv->oComp.pvFrameworkCtx = poRtpRecvConfig->oConfig.pvFrameworkCtx;
			memset( &poRtpRecv->oAppData, 0, sizeof (t_MediaCompAppData));
			poRtpRecv->ucLastEvent = ucInvalidEvent_c;

			poRtpRecv->nMagicNum = MCT_RTPRECV_MAGIC_NUM;
			poRtpRecv->ucPayloadType = poRtpRecvConfig->ucPayloadType;
			poRtpRecv->ucLastPayloadType = poRtpRecvConfig->ucPayloadType;
			poRtpRecv->ucDtmfPayloadType = poRtpRecvConfig->ucDtmfPayloadType;
			poRtpRecv->nSamplingDivisor = poRtpRecvConfig->nSamplingRate / 1000;
			poRtpRecv->unMaxTimeStampJump = poRtpRecvConfig->unMaxTimeStampJump;

			*phRtpRecv_o = (h_MediaComp) poRtpRecv;
		}
		else
		{
			MS_TRACE( poRtpRecvConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_RtpRecv\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poRtpRecvConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpRecvSetAppData( h_MediaComp hRtpRecv_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpRecv* poRtpRecv = hRtpRecv_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poRtpRecv,  poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RTPRECV_MAGIC_NUM == poRtpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poRtpRecv->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poRtpRecv,  poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpRecvDestroy( h_MediaComp hRtpRecv_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpRecv* poRtpRecv = hRtpRecv_i;
	int nTraceId =  poRtpRecv->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RTPRECV_MAGIC_NUM == poRtpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		memset( poRtpRecv, 0, sizeof (t_RtpRecv));
		FW_Free( poRtpRecv );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpRecvSetStatus( h_MediaComp hRtpRecv_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpRecv* poRtpRecv = hRtpRecv_i;

	MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RTPRECV_MAGIC_NUM == poRtpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpRecvSetParam( h_MediaComp hRtpRecv_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpRecv* poRtpRecv = hRtpRecv_i;

	MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RTPRECV_MAGIC_NUM == poRtpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_RTPRECV_PAYLOAD_TYPE:
			{
				poRtpRecv->ucPayloadType = (unsigned char)pvConfigData_i;
				poRtpRecv->ucLastPayloadType = poRtpRecv->ucPayloadType;
				break;
			}

			case MCC_RTPRECV_TELEPHONY_EVENT_PAYLOAD_TYPE:
			{
				poRtpRecv->ucDtmfPayloadType = (unsigned char)pvConfigData_i;
				break;
			}

			case MCC_RTPRECV_SAMPLING_RATE:
			{
				poRtpRecv->nSamplingDivisor = ((int)pvConfigData_i)/ 1000;
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

	MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpRecvGetParam( h_MediaComp hRtpRecv_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_RtpRecv* poRtpRecv = hRtpRecv_i;

	MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RTPRECV_MAGIC_NUM == poRtpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_RTPRECV_NUM_PKTS_RECV:
			{
				unsigned int* punPktsRecv = (unsigned int*)pvConfigData_i;

				*punPktsRecv = poRtpRecv->unPktsRecv;
				break;
			}

			case MCC_RTPRECV_PAYLOAD_TYPE:
			{
				unsigned char* pucPayloadType = (unsigned char*)pvConfigData_i;
				*pucPayloadType = poRtpRecv->ucPayloadType;
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

	MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

