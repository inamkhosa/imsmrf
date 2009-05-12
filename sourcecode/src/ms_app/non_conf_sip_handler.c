#include "ms_util.h"
#include "ms_mscml_handler.h"
#include "ms_media.h"
#include "call_leg_handler.h"
#include "call_leg_handler.h"
#include "ms_media_handler.h"

e_MsRetCode MS_NonConfRequestCompleted( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE  hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MsCtx* poMsCtx = NULL;

	cl_get_stack_app_handle( hClStack_i, (h_CallLegApp*)&poMsCtx );

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		//ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hAppCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
		t_MsUserCtrlCtx* poUserCtrlCtx = NULL;

		eRetCode = cl_get_app_handle( hClStack_i, hCallLeg_i, (h_CallLegApp*)&poUserCtrlCtx );
		if( NULL != poUserCtrlCtx )
		{
			if( poUserCtrlCtx->eState == UCS_DESTROY_IN_PROGRESS )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Destroying Call Leg Ctx = %p\n", poUserCtrlCtx );

				eRetCode = MS_DestroyUserCtrlCtx( poMsCtx, poUserCtrlCtx );
				FW_Free( poUserCtrlCtx );
				DListRemoveFromData( poMsCtx->poUserCtrlCtxList, poUserCtrlCtx, NULL );

				// Do not process pending requests
				eRetCode = MS_ERR_END_OF_LIST;
			}
		}
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_NonConfProvisionalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MsCtx* poMsCtx = NULL;

	cl_get_stack_app_handle( hClStack_i, (h_CallLegApp*)&poMsCtx );

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

//  //PRE_CONDITION
//  {
//      ms_assert_return( NULL != hClStack_i                , eRetCode = MS_ERR_PRE_COND_FAILED);
//      ms_assert_return( NULL != hCallLeg_i                , eRetCode = MS_ERR_PRE_COND_FAILED);
//      ms_assert_return( NULL != hSipMsg_i		        , eRetCode = MS_ERR_PRE_COND_FAILED);
//  }
	/* - */

	{
		SIP_HANDLE hSipStack;

		eRetCode = cl_get_sip_handle( hClStack_i, &hSipStack );
		if( MS_ERR_NONE == eRetCode )
		{
			t_SIPTag oSipTag;
			unsigned int unStatusCode = 0;

			memset( &oSipTag, 0, sizeof (t_SIPTag));
			oSipTag.oSIPTagType = SIP_TAG_SL_STATUS_CODE;
			// Check SIP status code
			if( 0 == SIP_GetTagFromMsg( hSipStack, hSipMsg_i, &oSipTag ) )
			{
				unStatusCode = oSipTag.u_SIPTagValue.unStatusCode;

				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "SIP Status code = %d\n", unStatusCode );
			}
		}
	}

	//post_condition:
	{
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_NonConfFinalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MsCtx* poMsCtx = NULL;

	cl_get_stack_app_handle( hClStack_i, (h_CallLegApp*)&poMsCtx );

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		t_MsUserCtrlCtx* poUserCtrlCtx = NULL;

		eRetCode = MS_GetUserCtrlCtx( poMsCtx, poMsCtx->poUserCtrlCtxList, hCallLeg_i, &poUserCtrlCtx );
		if( MS_ERR_NONE == eRetCode )
		{
			MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poUserCtrlCtx );

			eRetCode = MS_HandleMscmlMessage( poMsCtx, NULL, poUserCtrlCtx, hSipMsg_i );
		}
		else
		{
			// Request received on non-existant call-leg
		}
	}
	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_NonConfIncomingInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MsCtx* poMsCtx = NULL;

	cl_get_stack_app_handle( hClStack_i, (h_CallLegApp*)&poMsCtx );

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		//ms_assert_return( NULL != hAppCtx_i		, eRetCode = MS_ERR_PRE_COND_FAILED);
	}
	/* - */

	{
		SNMP_IncrementPerformanceCtr( poMsCtx, PC_InviteAttemptNonConf );
		SNMP_CheckFault( poMsCtx->hSnmpComm, FC_InvitesReceived, poMsCtx->pulPerformanceCtrs[PC_InviteAttemptConf] + poMsCtx->pulPerformanceCtrs[PC_InviteAttemptNonConf], poMsCtx->pulFaultCtrs[FC_InvitesReceived] );

		e_SIPResStatusCodes eSipStatusCode = SIP_RES_OK;
		t_String oContentType = {(unsigned char*)"sdp", (unsigned int) sizeof ("sdp")-1};
		t_String oContentBody = TSTRING_INITIALIZER;
		t_MsUserCtrlCtx* poUserCtrlCtx = NULL;

		t_SipAppData oSipData;
		t_SipAppData* poSipData = NULL;

		memset( &oSipData, 0, sizeof (t_SipAppData));

		eRetCode = MS_GetUserCtrlCtx( poMsCtx, poMsCtx->poUserCtrlCtxList, hCallLeg_i, &poUserCtrlCtx );
		if( MS_ERR_NONE == eRetCode )
		{
			MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "reInvite received on an existing User Ctrl Ctx = %p\n", poUserCtrlCtx );
			if( UCS_DESTROY_IN_PROGRESS != poUserCtrlCtx->eState )
			{
				t_AMPSDList* poNegInfoList = NULL;
				h_Sdp hNegotiatedSdp = NULL;

				FW_DListInit( &poNegInfoList );

				eRetCode = MS_NegotiateSdp( poMsCtx, poUserCtrlCtx, hSipMsg_i, &hNegotiatedSdp, poNegInfoList );
				if( eRetCode == MS_ERR_NONE )
				{
					eRetCode = MS_Media_ChainSetMode( poMsCtx, NULL, poUserCtrlCtx, MM_PARKED, NULL );

					h_MediaComp hUdpRecv = NULL;

					eRetCode = MS_Media_GetUdpRecv( poMsCtx, poUserCtrlCtx->hRxChain, &hUdpRecv );

					ms_assert( MS_ERR_NONE == eRetCode );

					int nLocalPort = -1;
					eRetCode =  Media_CompGetParam( hUdpRecv, MCC_UDPRECV_LOCAL_PORT, (void*)&nLocalPort );
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Local port for media reception = %d\n", nLocalPort );

					// Update Negotiated SDP with local port in media line
					t_SdpNegMediaInfo* poNegMediaInfo = NULL;
					poNegMediaInfo = poNegInfoList->poAMPSSListHead->pvData;
					t_SDPMedia* poNegSdpMedia = NULL;

					eRetCode = SdpMediaDescGetMedia( poMsCtx->hSdpParser, hNegotiatedSdp, poNegMediaInfo->nNegMediaDescIdx, &poNegSdpMedia );

					ms_assert( MS_ERR_NONE == eRetCode );

					poNegSdpMedia->nMediaPrimaryPort = nLocalPort;

					eRetCode = SdpMediaDescSetMedia( poMsCtx->hSdpParser, poUserCtrlCtx->hNegotiatedSdp, poNegMediaInfo->nNegMediaDescIdx, poNegSdpMedia );

					// Encode negotiated sdp
					eRetCode = SdpEncode( poMsCtx->hSdpParser, hNegotiatedSdp, &oContentBody );
					if( MS_ERR_NONE == eRetCode )
					{
						FW_DListFree( &poUserCtrlCtx->poNegInfoList, SdpOamNegMediaInfoRemoveLinkCb );

						poUserCtrlCtx->poNegInfoList = poNegInfoList;

						DeallocateSdp( &poUserCtrlCtx->hNegotiatedSdp );

						poUserCtrlCtx->hNegotiatedSdp = hNegotiatedSdp;
					}
				}
				else if( MS_ERR_DECODE == eRetCode )
				{
					eSipStatusCode = SIP_RES_BAD_REQUEST;
				}
				else
				{
					// SDP Negotiation failed
					eSipStatusCode = SIP_RES_UNSUPPORTED_MEDIA_TYPE;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
				eSipStatusCode = SIP_RES_BUSY_HERE;
			}
		}
		else
		{
			eRetCode = MS_CreateUserCtrlCtx( poMsCtx, AMPS_TRUE, &poUserCtrlCtx );
			if( MS_ERR_NONE != eRetCode )
			{
				eSipStatusCode = SIP_RES_SERVER_INTERNAL_ERROR;
				eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
			}
			else
			{
				poUserCtrlCtx->eState = UCS_INITIALIZED;
				poUserCtrlCtx->hCallLeg = hCallLeg_i;
				poUserCtrlCtx->bDtmfClamp =  AMPS_FALSE;

				poSipData = &poUserCtrlCtx->oSipData;

				// Save contact from message
				eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &poUserCtrlCtx->oSipData.oRemoteContact );

				// Save From URI as Remote URI
				eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &poUserCtrlCtx->oSipData.oRemoteUriInfo );

				cl_set_app_handle( hClStack_i, hCallLeg_i, poUserCtrlCtx );

				// Negotiate SDP

				eRetCode = MS_NegotiateSdp( poMsCtx, poUserCtrlCtx, hSipMsg_i, &poUserCtrlCtx->hNegotiatedSdp, poUserCtrlCtx->poNegInfoList );
				if( eRetCode == MS_ERR_NONE )
				{
					eRetCode = MS_Media_ChainSetMode( poMsCtx, NULL, poUserCtrlCtx, MM_PARKED, NULL );

					h_MediaComp hUdpRecv = NULL;

					eRetCode = MS_Media_GetUdpRecv( poMsCtx, poUserCtrlCtx->hRxChain, &hUdpRecv );

					ms_assert( MS_ERR_NONE == eRetCode );

					int nLocalPort = -1;
					eRetCode =  Media_CompGetParam( hUdpRecv, MCC_UDPRECV_LOCAL_PORT, (void*)&nLocalPort );
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Local port for media reception = %d\n", nLocalPort );

					// Update Negotiated SDP with local port in media line
					t_SdpNegMediaInfo* poNegMediaInfo = NULL;
					poNegMediaInfo = poUserCtrlCtx->poNegInfoList->poAMPSSListHead->pvData;
					t_SDPMedia* poNegSdpMedia = NULL;

					eRetCode = SdpMediaDescGetMedia( poMsCtx->hSdpParser, poUserCtrlCtx->hNegotiatedSdp, poNegMediaInfo->nNegMediaDescIdx, &poNegSdpMedia );

					ms_assert( MS_ERR_NONE == eRetCode );

					poNegSdpMedia->nMediaPrimaryPort = nLocalPort;

					eRetCode = SdpMediaDescSetMedia( poMsCtx->hSdpParser, poUserCtrlCtx->hNegotiatedSdp, poNegMediaInfo->nNegMediaDescIdx, poNegSdpMedia );

					eRetCode = MS_HandleMscmlMessage( poMsCtx, NULL, poUserCtrlCtx, hSipMsg_i );

					// Encode negotiated sdp
					SdpEncode( poMsCtx->hSdpParser, poUserCtrlCtx->hNegotiatedSdp, &oContentBody );
				}
				else if( MS_ERR_DECODE == eRetCode )
				{
					eSipStatusCode = SIP_RES_BAD_REQUEST;
				}
				else
				{
					// SDP Negotiation failed
					eSipStatusCode = SIP_RES_UNSUPPORTED_MEDIA_TYPE;
				}
				if( eRetCode == MS_ERR_NONE )
				{
					FW_DListAppend( poMsCtx->poUserCtrlCtxList, poUserCtrlCtx );
				}
				else
				{
					MS_DestroyUserCtrlCtx( poMsCtx, poUserCtrlCtx );
					FW_Free( poUserCtrlCtx );
					poUserCtrlCtx = NULL;
				}
			}
		}
		if( MS_ERR_NONE != eRetCode || NULL == poUserCtrlCtx )
		{
			// Save contact from message
			eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

			// Save From URI as Remote URI
			eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

			poSipData = &oSipData;
		}
		else
		{
			poSipData = &poUserCtrlCtx->oSipData;
		}
		if( MS_ERR_NONE == eRetCode )
		{
			SNMP_IncrementPerformanceCtr( poMsCtx, PC_InviteSuccessNonConf );
		}
		else
		{
			SNMP_IncrementPerformanceCtr( poMsCtx, PC_InviteFailedNonConf );
		}
		SNMP_IncrementPerformanceCtr( poMsCtx, PC_InviteAnsweredNonConf );

		eRetCode = MS_SendResponse( poMsCtx, poUserCtrlCtx->hCallLeg, poSipData, hSipMsg_i, eSipStatusCode, &oContentType, &oContentBody );

		//TS_Free(&oContentType);
		TS_Free( &oContentBody );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_NonConfIncomingNonInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MsCtx* poMsCtx = NULL;

	cl_get_stack_app_handle( hClStack_i, (h_CallLegApp*)&poMsCtx );

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hAppCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_SIPTag oSipTag;
		e_SIPResStatusCodes eSipStatusCode = SIP_RES_OK;
		t_MsUserCtrlCtx* poUserCtrlCtx = NULL;
		t_SipAppData oSipData;
		t_SipAppData* poSipData = NULL;

		memset( &oSipData, 0, sizeof (t_SipAppData));

		memset( &oSipTag, 0, sizeof (t_SIPTag));
		oSipTag.oSIPTagType = SIP_TAG_RL_METHOD;
		if( 0 == SIP_GetTagFromMsg( poMsCtx->hSipStack, hSipMsg_i, &oSipTag ) )
		{
			t_String oInfoRlMethod = {(unsigned char*)"INFO", (unsigned int) sizeof ("INFO")-1};
			t_String oByeRlMethod = {(unsigned char*)"BYE", (unsigned int) sizeof ("BYE")-1};
			t_String oCancelRlMethod = {(unsigned char*)"CANCEL", (unsigned int) sizeof ("CANCEL")-1};

			t_String oContentType = TSTRING_INITIALIZER;
			t_String oContentBody = TSTRING_INITIALIZER;
			// Check if INFO received
			if( 0 == TS_CompareCase((t_String*)(&oSipTag.u_SIPTagValue.oSIPStringRLMethod), &oInfoRlMethod ) )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "INFO received\n" );

				SNMP_IncrementPerformanceCtr( poMsCtx, PC_InfoAttemptNonConf );
				SNMP_CheckFault( poMsCtx->hSnmpComm, FC_InfoReceived, poMsCtx->pulPerformanceCtrs[PC_InfoAttemptConf] + poMsCtx->pulPerformanceCtrs[PC_InfoAttemptNonConf], poMsCtx->pulFaultCtrs[FC_InfoReceived] );

				eRetCode = MS_GetUserCtrlCtx( poMsCtx, poMsCtx->poUserCtrlCtxList, hCallLeg_i, &poUserCtrlCtx );
				if( MS_ERR_NONE == eRetCode )
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poUserCtrlCtx );

					eRetCode = MS_HandleMscmlMessage( poMsCtx, NULL, poUserCtrlCtx, hSipMsg_i );
				}
				else
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Request received on non-existant call-leg\n" );

					eRetCode = MS_ERR_INVALID_INPUT;
					eSipStatusCode = SIP_RES_BAD_REQUEST;
				}
				if( MS_ERR_NONE == eRetCode )
				{
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_InfoSuccessNonConf );
				}
				else
				{
					eSipStatusCode = SIP_RES_BAD_REQUEST;
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_InfoFailedNonConf );
				}
				SNMP_IncrementPerformanceCtr( poMsCtx, PC_InfoAnsweredNonConf );
			}
			else if( 0 == TS_CompareCase((t_String*)(&oSipTag.u_SIPTagValue.oSIPStringRLMethod), &oByeRlMethod ) )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "BYE received\n" );

				SNMP_IncrementPerformanceCtr( poMsCtx, PC_ByeAttemptNonConf );

				eRetCode = MS_GetUserCtrlCtx( poMsCtx, poMsCtx->poUserCtrlCtxList, hCallLeg_i, &poUserCtrlCtx );
				if( MS_ERR_NONE == eRetCode )
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poUserCtrlCtx );

					poUserCtrlCtx->eState = UCS_DESTROY_IN_PROGRESS;
				}
				else
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Request received on non-existant call-leg\n" );

					eRetCode = MS_ERR_INVALID_INPUT;
					eSipStatusCode = SIP_RES_BAD_REQUEST;
				}
				if( MS_ERR_NONE == eRetCode )
				{
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_ByeSuccessNonConf );
				}
				else
				{
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_ByeFailedNonConf );
				}
				SNMP_IncrementPerformanceCtr( poMsCtx, PC_ByeAnsweredNonConf );
			}
			else if( 0 == TS_CompareCase((t_String*)(&oSipTag.u_SIPTagValue.oSIPStringRLMethod), &oCancelRlMethod ) )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "CANCEL received\n" );

				SNMP_IncrementPerformanceCtr( poMsCtx, PC_CancelAttemptNonConf );

				eRetCode = MS_GetUserCtrlCtx( poMsCtx, poMsCtx->poUserCtrlCtxList, hCallLeg_i, &poUserCtrlCtx );
				if( MS_ERR_NONE == eRetCode )
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poUserCtrlCtx );
					if( poUserCtrlCtx->eState == UCS_INITIALIZED )
					{
						MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Cancel Ignored\n" );

						poSipData = &poUserCtrlCtx->oSipData;
						eSipStatusCode = SIP_RES_OK;
					}
				}
				else
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Request received on non-existant call-leg\n" );

					eRetCode = MS_ERR_INVALID_INPUT;
					eSipStatusCode = SIP_RES_BAD_REQUEST;
				}
				if( MS_ERR_NONE == eRetCode )
				{
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_CancelSuccessNonConf );
				}
				else
				{
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_CancelFailedNonConf );
				}
				SNMP_IncrementPerformanceCtr( poMsCtx, PC_CancelAnsweredNonConf );
			}

			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
				eSipStatusCode = SIP_RES_METHOD_NOT_ALLOWED;
			}
			if( MS_ERR_NONE != eRetCode || NULL == poUserCtrlCtx )
			{
				// Save contact from message
				eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

				// Save From URI as Remote URI
				eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

				poSipData = &oSipData;
			}
			else
			{
				poSipData = &poUserCtrlCtx->oSipData;
			}
			eRetCode = MS_SendResponse( poMsCtx, hCallLeg_i, poSipData, hSipMsg_i, eSipStatusCode, &oContentType, &oContentBody );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

