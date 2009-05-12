#include "ms_util.h"
#include "ms_mscml_handler.h"
#include "ms_media.h"
#include "non_conf_sip_handler.h"
#include "call_leg_handler.h"
#include "ms_media_handler.h"

e_MsRetCode MS_NewCallLeg( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i, t_SIPConnObject* poLocalRxConn_i, t_SIPConnObject* poRemoteRxConn_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MsCtx* poMsCtx = (t_MsCtx*)hAppCtx_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hAppCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poLocalRxConn_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poRemoteRxConn_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
		poMsCtx->hCallLegStack = hClStack_i;

		t_SIPTag oSipTag;
		t_String oUser = TSTRING_INITIALIZER;
		t_String oHost = TSTRING_INITIALIZER;

		// Get Request URI of the message
		memset( &oSipTag, 0, sizeof (t_SIPTag));
		oSipTag.oSIPTagType = SIP_TAG_RL_REQUEST_URI;
		if( AMPS_SUCCESS == SIP_GetTagFromMsg( poMsCtx->hSipStack, hSipMsg_i, &oSipTag ) )
		{
			TS_SetN((char*)&oSipTag.u_SIPTagValue.oRURI.puchUser, oSipTag.u_SIPTagValue.oRURI.unUserLength, &oUser );

			TS_SetN((char*)&oSipTag.u_SIPTagValue.oRURI.puchHost, oSipTag.u_SIPTagValue.oRURI.unHostLength, &oHost );
		}
		else
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
		if( MS_ERR_NONE == eRetCode )
		{
			TS_Print( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Request URI: user = ", &oUser );
			TS_Print( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Request URI: host = ", &oHost );
			// Check if request is for this media-server
			if( (0 == TS_CompareCase( &oHost, &poMsCtx->oLocalUri.oHost )) || (0 == TS_CompareCase( &oHost, &poMsCtx->oLocalIp )) )
			{
				// Check if request-uri belongs to a conference
				t_String oConf = {(unsigned char*)"conf=", sizeof ("conf=")-1};
				if( 0 == TS_CompareCase( &oUser, &oConf ) )
				{
					t_CallLegInitConfig oCallLegInitConfig;

					oCallLegInitConfig.hAppCtx              = NULL;
					oCallLegInitConfig.oCallLegCb.pfRequestCompleted    = MS_ConfRequestCompleted;
					//oCallLegInitConfig.oCallLegCb.pfRequestProceeding	= MS_RequestProceeding;
					oCallLegInitConfig.oCallLegCb.pfProvisionalRespRecvd    = MS_ConfProvisionalRespRecvd;
					oCallLegInitConfig.oCallLegCb.pfFinalRespRecvd      = MS_ConfFinalRespRecvd;
					oCallLegInitConfig.oCallLegCb.pfIncomingInvite      = MS_ConfIncomingInvite;
					oCallLegInitConfig.oCallLegCb.pfIncomingNonInvite   = MS_ConfIncomingNonInvite;
					oCallLegInitConfig.oLocalRxCnx                     = *poLocalRxConn_i;

					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Local Connection in configuration of call leg = %s:%d:%d\n", oCallLegInitConfig.oLocalRxCnx.pchIPAddr, oCallLegInitConfig.oLocalRxCnx.nPort, oCallLegInitConfig.oLocalRxCnx.nTptType );

					eRetCode = cl_initialize( hClStack_i, hCallLeg_i, &oCallLegInitConfig );

					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "cl_initialize return code = %s\n", get_ms_ret_code_string( eRetCode ) );

					//eRetCode = cl_incoming_request( hClStack_i, hCallLeg_i, hSipMsg_i );
				}
				else if( 0 == TS_CompareCase( &oUser, &poMsCtx->oLocalUri.oUser ))
				// Check if request-uri belongs to this media-server
				{
					TS_Print( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Request is out of conference but for media-server, user = ", &poMsCtx->oLocalUri.oUser );

					t_CallLegInitConfig oCallLegInitConfig;

					oCallLegInitConfig.hAppCtx              = NULL;
					oCallLegInitConfig.oCallLegCb.pfRequestCompleted    = MS_NonConfRequestCompleted;
					//oCallLegInitConfig.oCallLegCb.pfRequestProceeding	= MS_NonConfRequestProceeding;
					oCallLegInitConfig.oCallLegCb.pfProvisionalRespRecvd    = MS_NonConfProvisionalRespRecvd;
					oCallLegInitConfig.oCallLegCb.pfFinalRespRecvd      = MS_NonConfFinalRespRecvd;
					oCallLegInitConfig.oCallLegCb.pfIncomingInvite      = MS_NonConfIncomingInvite;
					oCallLegInitConfig.oCallLegCb.pfIncomingNonInvite   = MS_NonConfIncomingNonInvite;
					oCallLegInitConfig.oLocalRxCnx                     = *poLocalRxConn_i;

					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Local Connection in configuration of call leg = %s:%d:%d\n", oCallLegInitConfig.oLocalRxCnx.pchIPAddr, oCallLegInitConfig.oLocalRxCnx.nPort, oCallLegInitConfig.oLocalRxCnx.nTptType );

					eRetCode = cl_initialize( hClStack_i, hCallLeg_i, &oCallLegInitConfig );

					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "cl_initialize return code = %s\n", get_ms_ret_code_string( eRetCode ) );
				}
				else
				{
					TS_Print( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Request not for media server, host = ", &poMsCtx->oLocalUri.oHost );

					eRetCode = MS_ERR_NOT_FOUND;
				}
			}
			else
			{
				TS_Print( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Request does not match media-server hostname = ", &poMsCtx->oLocalUri.oHost );

				eRetCode = MS_ERR_NOT_FOUND;
			}
			TS_Free( &oUser );
			TS_Free( &oHost );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_ConfRequestCompleted( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE  hSipMsg_i )
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
		//ms_assert_return( NULL != hAppCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
		t_MsUserCtrlCtx* poUserCtrlCtx = NULL;
		t_MsConfCtx* poMsConfCtx = NULL;

		eRetCode = cl_get_app_handle( hClStack_i, hCallLeg_i, (h_CallLegApp*)&poUserCtrlCtx );
		if( NULL != poUserCtrlCtx )
		{
			if( poUserCtrlCtx->eState == UCS_DESTROY_IN_PROGRESS )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Destroying Call Leg Ctx = %p\n", poUserCtrlCtx );

				poMsConfCtx = poUserCtrlCtx->poMsConfCtx;

				ms_assert_return( poMsConfCtx->nMagicNum == MS_CONF_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
				// if the call leg destroyed is a conf ctrl leg ctx
				if( poMsConfCtx->poConfCtrlCtx == poUserCtrlCtx )
				{
					eRetCode = MS_DestroyConfCtx( poMsCtx, poMsConfCtx );
				}
				else
				{
					eRetCode = MS_DestroyUserCtrlCtx( poMsCtx, poUserCtrlCtx );
					if( poMsConfCtx->nAvailableCtrlCtx == -1 )
					{
						FW_Free( poUserCtrlCtx );
						DListRemoveFromData( poMsConfCtx->poUserCtrlCtxList, poUserCtrlCtx, NULL );
					}
					else
					{
						poUserCtrlCtx->eState = UCS_AVAILABLE;
					}
				}
				// Do not process pending requests
				eRetCode = MS_ERR_END_OF_LIST;
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_ConfProvisionalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
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

e_MsRetCode MS_ConfFinalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MsCtx* poMsCtx = NULL;

	cl_get_stack_app_handle( hClStack_i, (h_CallLegApp*)&poMsCtx );

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		t_MsConfCtx* poMsConfCtx = NULL;

		eRetCode = MS_GetConfCtx( poMsCtx, hCallLeg_i, hSipMsg_i, &poMsConfCtx );
		if( MS_ERR_NONE == eRetCode )
		{
			t_MsUserCtrlCtx* poUserCtrlCtx = NULL;
			ms_assert_return( poMsConfCtx->nMagicNum == MS_CONF_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );

			MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Conference already exists = %p\n", poMsConfCtx );

			eRetCode = MS_GetUserCtrlCtx( poMsCtx, poMsConfCtx->poUserCtrlCtxList, hCallLeg_i, &poUserCtrlCtx );
			if( MS_ERR_NONE == eRetCode )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poUserCtrlCtx );

				eRetCode = MS_HandleMscmlMessage( poMsCtx, poMsConfCtx, poUserCtrlCtx, hSipMsg_i );
			}
			else
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Response received on non-existant call-leg\n" );
			}
		}
		else
		{
			MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Response received on non-existant conference\n" );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_ConfIncomingInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
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
		SNMP_IncrementPerformanceCtr( poMsCtx, PC_InviteAttemptConf );
		SNMP_CheckFault( poMsCtx->hSnmpComm, FC_InvitesReceived, poMsCtx->pulPerformanceCtrs[PC_InviteAttemptConf] + poMsCtx->pulPerformanceCtrs[PC_InviteAttemptNonConf], poMsCtx->pulFaultCtrs[FC_InvitesReceived] );

		t_MsConfCtx* poMsConfCtx = NULL;
		e_SIPResStatusCodes eSipStatusCode = SIP_RES_OK;
		t_String oContentType = {(unsigned char*)"sdp", (unsigned int) sizeof ("sdp")-1};
		t_String oContentBody = TSTRING_INITIALIZER;
		t_MsUserCtrlCtx* poUserCtrlCtx = NULL;
		t_SipAppData oSipData;
		t_SipAppData* poSipData = NULL;

		memset( &oSipData, 0, sizeof (t_SipAppData));

		eRetCode = MS_GetConfCtx( poMsCtx, hCallLeg_i, hSipMsg_i, &poMsConfCtx );
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert_return( poMsConfCtx->nMagicNum == MS_CONF_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
			// Conference already exists
			MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Conference already exists = %p\n", poMsConfCtx );
			if( UCS_DESTROY_IN_PROGRESS != poMsConfCtx->poConfCtrlCtx->eState )
			{
				eRetCode = MS_GetUserCtrlCtx( poMsCtx, poMsConfCtx->poUserCtrlCtxList, hCallLeg_i, &poUserCtrlCtx );
				if( MS_ERR_NONE == eRetCode )
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "reInvite received on an existing User Ctrl Ctx = %p\n", poUserCtrlCtx );

					poSipData = &poUserCtrlCtx->oSipData;
					if( UCS_DESTROY_IN_PROGRESS != poUserCtrlCtx->eState )
					{
						t_AMPSDList* poNegInfoList = NULL;
						h_Sdp hNegotiatedSdp = NULL;

						FW_DListInit( &poNegInfoList );

						eRetCode = MS_NegotiateSdp( poMsCtx, poUserCtrlCtx, hSipMsg_i, &hNegotiatedSdp, poNegInfoList );
						if( eRetCode == MS_ERR_NONE )
						{
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

							eRetCode = SdpMediaDescSetMedia( poMsCtx->hSdpParser, hNegotiatedSdp, poNegMediaInfo->nNegMediaDescIdx, poNegSdpMedia );

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
						eSipStatusCode = SIP_RES_BUSY_HERE;
					}
				}
				else
				{
					// Search for a empty user ctrl leg
					eRetCode = MS_GetAvailableUserCtrlCtx( poMsCtx, poMsConfCtx, &poUserCtrlCtx );
					if( MS_ERR_NONE == eRetCode )
					{
						poUserCtrlCtx->eState = UCS_INITIALIZED;
						poUserCtrlCtx->hCallLeg = hCallLeg_i;
						poUserCtrlCtx->poMsConfCtx = poMsConfCtx;

						// Save contact from message
						eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &poUserCtrlCtx->oSipData.oRemoteContact );

						// Save From URI as Remote URI
						eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &poUserCtrlCtx->oSipData.oRemoteUriInfo );

						cl_set_app_handle( hClStack_i, hCallLeg_i, poUserCtrlCtx );

						poSipData = &poUserCtrlCtx->oSipData;

						eRetCode = MS_NegotiateSdp( poMsCtx, poUserCtrlCtx, hSipMsg_i, &poUserCtrlCtx->hNegotiatedSdp, poUserCtrlCtx->poNegInfoList );
						if( eRetCode == MS_ERR_NONE )
						{
							eRetCode = MS_Media_ChainSetMode( poMsCtx, poMsConfCtx, poUserCtrlCtx, MM_PARKED, NULL );

							h_MediaComp hUdpRecv = NULL;

							eRetCode = MS_Media_GetUdpRecv( poMsCtx, poUserCtrlCtx->hRxChain, &hUdpRecv );

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

							eRetCode = MS_HandleMscmlMessage( poMsCtx, poMsConfCtx, poUserCtrlCtx, hSipMsg_i );

							// Encode negotiated sdp
							eRetCode = SdpEncode( poMsCtx->hSdpParser, poUserCtrlCtx->hNegotiatedSdp, &oContentBody );
						}
						else if( MS_ERR_DECODE == eRetCode )
						{
							eSipStatusCode = SIP_RES_BAD_REQUEST;

							// Save contact from message
							eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

							// Save From URI as Remote URI
							eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

							poSipData = &oSipData;
						}
						else
						{
							// SDP Negotiation failed
							eSipStatusCode = SIP_RES_UNSUPPORTED_MEDIA_TYPE;

							// Save contact from message
							eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

							// Save From URI as Remote URI
							eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

							poSipData = &oSipData;
						}
						if( eRetCode != MS_ERR_NONE )
						{
							MS_DestroyUserCtrlCtx( poMsCtx, poUserCtrlCtx );
							if( poMsConfCtx->nAvailableCtrlCtx == -1 )
							{
								FW_Free( poUserCtrlCtx );
								DListRemoveFromData( poMsCtx->poConfCtxList, poUserCtrlCtx, NULL );
							}
							else
							{
								poUserCtrlCtx->eState = UCS_AVAILABLE;
							}
						}
					}
					else
					{
						eSipStatusCode = SIP_RES_BUSY_HERE;

						// Save contact from message
						eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

						// Save From URI as Remote URI
						eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

						poSipData = &oSipData;
					}
				}
			}
			else
			{
				// Conference is being destroyed. Handle no more requests
				eSipStatusCode = SIP_RES_BUSY_HERE;

				// Save contact from message
				eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

				// Save From URI as Remote URI
				eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

				poSipData = &oSipData;
			}
		}
		else
		{
			MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Conference does not exist\n" );

			// Create a new conference
			poMsConfCtx = MS_Malloc( NULL, sizeof (t_MsConfCtx));
			if( NULL != poMsConfCtx )
			{
				// Initialize conference
				memset( poMsConfCtx, 0, sizeof (t_MsConfCtx));
				poMsConfCtx->nMagicNum = MS_CONF_CTX_MAGIC_NUM;
				AMPS_DListInit( &poMsConfCtx->poUserCtrlCtxList );
				AMPS_DListInit( &poMsConfCtx->phMixerCompList );
				poMsConfCtx->nAvailableCtrlCtx = 0;
				MS_GetConfIdFromMsg( poMsCtx, hSipMsg_i, &poMsConfCtx->oConfId );

				{
					t_SyncTimerConfig oTimerConfig;

					oTimerConfig.pvFw = poMsCtx->pvFrameworkCtx;
					oTimerConfig.nTraceId = poMsCtx->nTraceId;

					eRetCode = ST_Init( &oTimerConfig, &poMsConfCtx->hTimer );

					ms_assert( MS_ERR_NONE == eRetCode );
				}

				// Add a default mixer
				h_MediaComp hMixer = NULL;
				t_MixerConfig oMixerConfig;

				oMixerConfig.oConfig.nTraceId = poMsCtx->nTraceId;
				oMixerConfig.oConfig.pvFrameworkCtx = poMsCtx->pvFrameworkCtx;
				oMixerConfig.nBufferTimeMs = poMsCtx->nJitterBufferMs;
				oMixerConfig.nPacketizationTimeMs = 20;     //default packetization time

				eRetCode = Media_CompCreate( MCT_MIXER, &oMixerConfig, &hMixer );

				ms_assert( MS_ERR_NONE == eRetCode );

				FW_DListAppend( poMsConfCtx->phMixerCompList, hMixer );

				// Create conf control ctx
				eRetCode = MS_CreateConfCtrlCtx( poMsCtx, poMsConfCtx, &poMsConfCtx->poConfCtrlCtx );
				if( MS_ERR_NONE == eRetCode )
				{
					poUserCtrlCtx = poMsConfCtx->poConfCtrlCtx;
					poMsConfCtx->poConfCtrlCtx->hCallLeg = hCallLeg_i;
					poSipData = &poMsConfCtx->poConfCtrlCtx->oSipData;

					// Save contact from message
					eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &poMsConfCtx->poConfCtrlCtx->oSipData.oRemoteContact );

					// Save From URI as Remote URI
					eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &poMsConfCtx->poConfCtrlCtx->oSipData.oRemoteUriInfo );

					cl_set_app_handle( hClStack_i, hCallLeg_i, poMsConfCtx->poConfCtrlCtx );

					eRetCode = MS_HandleMscmlMessage( poMsCtx, poMsConfCtx, poMsConfCtx->poConfCtrlCtx, hSipMsg_i );
				}
				else
				{
					TS_Free( &poMsConfCtx->oConfId );
					AMPS_DListFree( &poMsConfCtx->poUserCtrlCtxList, NULL );
					AMPS_DListFree( &poMsConfCtx->phMixerCompList, NULL );
					FW_Free( poMsConfCtx );

					eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
					eSipStatusCode = SIP_RES_BUSY_HERE;
				}
			}
			else
			{
				eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
				eSipStatusCode = SIP_RES_BUSY_HERE;

				// Save contact from message
				eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

				// Save From URI as Remote URI
				eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

				poSipData = &oSipData;
			}
			if( MS_ERR_NONE == eRetCode )
			{
				FW_DListAppend( poMsCtx->poConfCtxList, poMsConfCtx );
			}
		}
		eRetCode = MS_SendResponse( poMsCtx, hCallLeg_i, poSipData, hSipMsg_i, eSipStatusCode, &oContentType, &oContentBody );
		if( NULL == poUserCtrlCtx )
		{
			TS_Free( &poSipData->oRemoteContact.oUser );
			TS_Free( &poSipData->oRemoteContact.oHost );

			TS_Free( &poSipData->oRemoteUriInfo.oUri.oUser );
			TS_Free( &poSipData->oRemoteUriInfo.oUri.oHost );
			TS_Free( &poSipData->oRemoteUriInfo.oTag );

			TS_Free( &poSipData->oLocalTag );
		}
		TS_Free( &oContentBody );
		if( MS_ERR_NONE == eRetCode )
		{
			SNMP_IncrementPerformanceCtr( poMsCtx, PC_InviteSuccessConf );
		}
		else
		{
			SNMP_IncrementPerformanceCtr( poMsCtx, PC_InviteFailedConf );
		}
		SNMP_IncrementPerformanceCtr( poMsCtx, PC_InviteAnsweredConf );
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_ConfIncomingNonInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MsCtx* poMsCtx = NULL;

	cl_get_stack_app_handle( hClStack_i, (h_CallLegApp*)&poMsCtx );

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		//ms_assert_return( NULL != hAppCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_SIPTag oSipTag;
		e_SIPResStatusCodes eSipStatusCode = SIP_RES_OK;
		t_MsUserCtrlCtx* poUserCtrlCtx = NULL;

		t_SipAppData oSipData;
		t_SipAppData* poSipData = NULL;     // points to Data in user-ctrl ctx or for this request

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

			TS_Print( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Request received = ", (t_String*)(&oSipTag.u_SIPTagValue.oSIPStringRLMethod));
			// Check if INFO received
			if( 0 == TS_CompareCase((t_String*)(&oSipTag.u_SIPTagValue.oSIPStringRLMethod), &oInfoRlMethod ) )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "INFO received\n" );

				SNMP_IncrementPerformanceCtr( poMsCtx, PC_InfoAttemptConf );
				SNMP_CheckFault( poMsCtx->hSnmpComm, FC_InfoReceived, poMsCtx->pulPerformanceCtrs[PC_InfoAttemptConf] + poMsCtx->pulPerformanceCtrs[PC_InfoAttemptNonConf], poMsCtx->pulFaultCtrs[FC_InfoReceived] );

				t_MsConfCtx* poMsConfCtx = NULL;

				eRetCode = MS_GetConfCtx( poMsCtx, hCallLeg_i, hSipMsg_i, &poMsConfCtx );
				if( MS_ERR_NONE == eRetCode )
				{
					ms_assert_return( poMsConfCtx->nMagicNum == MS_CONF_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );

					// Conference already exists
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Conference already exists = %p\n", poMsConfCtx );
					if( hCallLeg_i ==  poMsConfCtx->poConfCtrlCtx->hCallLeg )
					{
						MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Message received on Conf Ctrl Leg (ignored)\n" );

						// Save contact from message
						eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

						// Save From URI as Remote URI
						eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

						poSipData = &oSipData;
					}
					else
					{
						eRetCode = MS_GetUserCtrlCtx( poMsCtx, poMsConfCtx->poUserCtrlCtxList, hCallLeg_i, &poUserCtrlCtx );
						if( MS_ERR_NONE == eRetCode )
						{
							MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poUserCtrlCtx );

							poSipData = &poUserCtrlCtx->oSipData;

							eRetCode = MS_HandleMscmlMessage( poMsCtx, poMsConfCtx, poUserCtrlCtx, hSipMsg_i );
						}
						else
						{
							// Request received on non-existant call-leg
							eSipStatusCode = SIP_RES_BAD_REQUEST;

							// Save contact from message
							eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

							// Save From URI as Remote URI
							eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

							poSipData = &oSipData;
						}
					}
				}
				else
				{
					// Request received on non-existant conference
					eSipStatusCode = SIP_RES_BAD_REQUEST;

					// Save contact from message
					eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

					// Save From URI as Remote URI
					eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

					poSipData = &oSipData;
				}
				if( MS_ERR_NONE == eRetCode )
				{
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_InfoSuccessConf );
				}
				else
				{
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_InfoFailedConf );
				}
				SNMP_IncrementPerformanceCtr( poMsCtx, PC_InfoAnsweredConf );
			}
			else if( 0 == TS_CompareCase((t_String*)(&oSipTag.u_SIPTagValue.oSIPStringRLMethod), &oByeRlMethod ) )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "BYE received\n" );

				SNMP_IncrementPerformanceCtr( poMsCtx, PC_ByeAttemptConf );

				t_MsConfCtx* poMsConfCtx = NULL;

				eRetCode = MS_GetConfCtx( poMsCtx, hCallLeg_i, hSipMsg_i, &poMsConfCtx );
				if( MS_ERR_NONE == eRetCode )
				{
					ms_assert_return( poMsConfCtx->nMagicNum == MS_CONF_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
					// Conference already exists
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Conference already exists = %p\n", poMsConfCtx );
					// Message received on Conf Ctrl Leg
					if( hCallLeg_i ==  poMsConfCtx->poConfCtrlCtx->hCallLeg )
					{
						MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Message received on Conf Ctrl Leg\n" );

						poSipData = &poMsConfCtx->poConfCtrlCtx->oSipData;

						t_AMPSSList* poUserCtrlCtxNode = NULL;

						for( poUserCtrlCtxNode = poMsConfCtx->poUserCtrlCtxList->poAMPSSListHead; poUserCtrlCtxNode != NULL; poUserCtrlCtxNode = poUserCtrlCtxNode->poAMPSSListNext )
						{
							poUserCtrlCtx = poUserCtrlCtxNode->pvData;

							MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Send BYE on User Ctrl Ctx = %p\n", poUserCtrlCtx );

							t_String oMscmlContentType = TSTRING_INITIALIZER;
							t_String oContentBody = TSTRING_INITIALIZER;

							eRetCode = MS_SendByeRequest( poMsCtx, poUserCtrlCtx, &oMscmlContentType, &oContentBody );

							poUserCtrlCtx->eState = UCS_DESTROY_IN_PROGRESS;
						}
						//FW_DListFree( &poMsConfCtx->poUserCtrlCtxList, NULL );
						//FW_DListFree( &poMsConfCtx->phMixerCompList, MediaComp_LListRemoveLinkDataCallback );

//                      poUserCtrlCtx = poMsConfCtx->poConfCtrlCtx;

						poMsConfCtx->poConfCtrlCtx->eState = UCS_DESTROY_IN_PROGRESS;
					}
					else
					{
						eRetCode = MS_GetUserCtrlCtx( poMsCtx, poMsConfCtx->poUserCtrlCtxList, hCallLeg_i, &poUserCtrlCtx );
						if( MS_ERR_NONE == eRetCode )
						{
							MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poUserCtrlCtx );

							poSipData = &poUserCtrlCtx->oSipData;

							poUserCtrlCtx->eState = UCS_DESTROY_IN_PROGRESS;
						}
						else
						{
							MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Request received on non-existant call-leg\n" );

							eSipStatusCode = SIP_RES_BAD_REQUEST;

							// Save contact from message
							eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

							// Save From URI as Remote URI
							eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

							poSipData = &oSipData;
						}
					}
				}
				else
				{
					// Request received on non-existant conference
					eSipStatusCode = SIP_RES_BAD_REQUEST;

					// Save contact from message
					eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

					// Save From URI as Remote URI
					eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

					poSipData = &oSipData;
				}
				if( MS_ERR_NONE == eRetCode )
				{
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_ByeSuccessConf );
				}
				else
				{
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_ByeFailedConf );
				}
				SNMP_IncrementPerformanceCtr( poMsCtx, PC_ByeAnsweredConf );
			}
			else if( 0 == TS_CompareCase((t_String*)(&oSipTag.u_SIPTagValue.oSIPStringRLMethod), &oCancelRlMethod ) )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "CANCEL received\n" );

				SNMP_IncrementPerformanceCtr( poMsCtx, PC_CancelAttemptConf );

				t_MsConfCtx* poMsConfCtx = NULL;

				eRetCode = MS_GetConfCtx( poMsCtx, hCallLeg_i, hSipMsg_i, &poMsConfCtx );
				if( MS_ERR_NONE == eRetCode )
				{
					ms_assert_return( poMsConfCtx->nMagicNum == MS_CONF_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
					// Conference already exists
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Conference already exists = %p\n", poMsConfCtx );
					// Message received on Conf Ctrl Leg
					if( hCallLeg_i ==  poMsConfCtx->poConfCtrlCtx->hCallLeg )
					{
						MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Message received on Conf Ctrl Leg\n" );
						if( poMsConfCtx->poConfCtrlCtx->eState == UCS_INITIALIZED )
						{
							MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Cancel Ignored\n" );

							poSipData = &poMsConfCtx->poConfCtrlCtx->oSipData;
							eSipStatusCode = SIP_RES_OK;
						}
					}
					else
					{
						eRetCode = MS_GetUserCtrlCtx( poMsCtx, poMsConfCtx->poUserCtrlCtxList, hCallLeg_i, &poUserCtrlCtx );
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

							eSipStatusCode = SIP_RES_BAD_REQUEST;

							// Save contact from message
							eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

							// Save From URI as Remote URI
							eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

							poSipData = &oSipData;
						}
					}
				}
				else
				{
					// Request received on non-existant conference
					eSipStatusCode = SIP_RES_BAD_REQUEST;

					// Save contact from message
					eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

					// Save From URI as Remote URI
					eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

					poSipData = &oSipData;
				}
				if( MS_ERR_NONE == eRetCode )
				{
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_CancelSuccessConf );
				}
				else
				{
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_CancelFailedConf );
				}
				SNMP_IncrementPerformanceCtr( poMsCtx, PC_CancelAnsweredConf );
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
				eSipStatusCode = SIP_RES_METHOD_NOT_ALLOWED;

				// Save contact from message
				eRetCode = SU_GetContactUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteContact );

				// Save From URI as Remote URI
				eRetCode = SU_GetFromUri( poMsCtx->nTraceId, poMsCtx->hSipStack, hSipMsg_i, &oSipData.oRemoteUriInfo );

				poSipData = &oSipData;
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

