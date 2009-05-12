#include "call_leg_test.h"

int nTraceId_g = 0;

e_MsRetCode Test_NewCallLeg( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

//	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	/* pre-condition */
//  {
//      ms_assert_return( NULL != hClStack_i                , eRetCode = MS_ERR_PRE_COND_FAILED);
//      ms_assert_return( NULL != hCallLeg_i                , eRetCode = MS_ERR_PRE_COND_FAILED);
//      ms_assert_return( NULL != hSipMsg_i		        , eRetCode = MS_ERR_PRE_COND_FAILED);
//
//  }
	/* - */
	{
		t_CallLegInitConfig oCallLegInitConfig;

		oCallLegInitConfig.hAppCtx              = NULL;
		oCallLegInitConfig.oCallLegCb.pfRequestCompleted    = Test_RequestCompleted;
		//oCallLegInitConfig.oCallLegCb.pfRequestProceeding	= Test_RequestProceeding;
		oCallLegInitConfig.oCallLegCb.pfProvisionalRespRecvd    = Test_ProvisionalRespRecvd;
		oCallLegInitConfig.oCallLegCb.pfFinalRespRecvd          = Test_FinalRespRecvd;
		oCallLegInitConfig.oCallLegCb.pfIncomingInvite      = Test_IncomingInvite;
		oCallLegInitConfig.oCallLegCb.pfIncomingNonInvite   = Test_IncomingNonInvite;

		eRetCode = cl_initialize( hClStack_i, hCallLeg_i, &oCallLegInitConfig );

		MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, " cl_initialize return code = %d\n", eRetCode );

		eRetCode = cl_incoming_request( hClStack_i, hCallLeg_i, hSipMsg_i );
	}

	////post_condition:
	{
	}
	/* - */

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %d\n", eRetCode );

	return (eRetCode);
}

e_MsRetCode Test_RequestCompleted( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE  hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	/* pre-condition */
//  {
//      ms_assert_return( NULL != hClStack_i                , eRetCode = MS_ERR_PRE_COND_FAILED);
//      ms_assert_return( NULL != hCallLeg_i                , eRetCode = MS_ERR_PRE_COND_FAILED);
//      ms_assert_return( NULL != hSipMsg_i		        , eRetCode = MS_ERR_PRE_COND_FAILED);
//
//  }
	/* - */
	{
		SIP_HANDLE hSipStack;

		eRetCode = cl_get_sip_handle( hClStack_i, &hSipStack );
		if( MS_ERR_NONE == eRetCode )
		{
			t_SIPTag oSipTag;
			unsigned int unStatusCode = 0;
			oSipTag.oSIPTagType = SIP_TAG_SL_STATUS_CODE;
			// Check SIP status code
			if( 0 == SIP_GetTagFromMsg( hSipStack, hSipMsg_i, &oSipTag ) )
			{
				unStatusCode = oSipTag.u_SIPTagValue.unStatusCode;

				MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "SIP Status code = %d\n", unStatusCode );
			}
		}
	}

	//post_condition:
	{
	}
	/* - */

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %d\n", eRetCode );

	return (eRetCode);
}

//e_MsRetCode Test_RequestProceeding		(h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i)
e_MsRetCode Test_ProvisionalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

//	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

//  /* pre-condition */
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
			oSipTag.oSIPTagType = SIP_TAG_SL_STATUS_CODE;
			// Check SIP status code
			if( 0 == SIP_GetTagFromMsg( hSipStack, hSipMsg_i, &oSipTag ) )
			{
				unStatusCode = oSipTag.u_SIPTagValue.unStatusCode;

				MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "SIP Status code = %d\n", unStatusCode );
			}
		}
	}

	//post_condition:
	{
	}
	/* - */

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %d\n", eRetCode );

	return (eRetCode);
}

e_MsRetCode Test_FinalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %d\n", eRetCode );

	return (eRetCode);
}

e_MsRetCode Test_IncomingInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

//	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	/* pre-condition */
//  {
//      ms_assert_return( NULL != hClStack_i            , eRetCode = MS_ERR_PRE_COND_FAILED);
//      ms_assert_return( NULL != hCallLeg_i            , eRetCode = MS_ERR_PRE_COND_FAILED);
//      ms_assert_return( NULL != hSipMsg_i			, eRetCode = MS_ERR_PRE_COND_FAILED);
//
//  }
	/* - */

	{
		SIP_HANDLE hSipStack;
		SIP_CONN_HANDLE hSipCnx;        // dummy connection
		SIP_MSG_HANDLE hSipResp;

		eRetCode = cl_get_sip_handle( hClStack_i, &hSipStack );
		if( MS_ERR_NONE == eRetCode )
		{
			//cl_assert( NULL != hSipStack );
			hSipResp = SIP_CreateResponse( hSipStack, hSipCnx, hSipMsg_i, SIP_RES_RINGING );
			if( NULL != hSipResp )
			{
				eRetCode = cl_send_provisional_response( hClStack_i, hCallLeg_i, hSipResp );
			}
			else
			{
				eRetCode = MS_ERR_SIP_STACK;
			}
			hSipResp = SIP_CreateResponse( hSipStack, hSipCnx, hSipMsg_i, SIP_RES_OK );
			if( NULL != hSipResp )
			{
				eRetCode = cl_send_final_response( hClStack_i, hCallLeg_i, hSipResp );
			}
			else
			{
				eRetCode = MS_ERR_SIP_STACK;
			}
		}
	}

	//post_condition:
	{
	}
	/* - */

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %d\n", eRetCode );

	return (eRetCode);
}

e_MsRetCode Test_IncomingNonInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

//	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	/* pre-condition */
//  {
//      ms_assert_return( NULL != hClStack_i                , eRetCode = MS_ERR_PRE_COND_FAILED);
//      ms_assert_return( NULL != hCallLeg_i                , eRetCode = MS_ERR_PRE_COND_FAILED);
//      ms_assert_return( NULL != hSipMsg_i		        , eRetCode = MS_ERR_PRE_COND_FAILED);
//
//  }
	/* - */

	{
		SIP_HANDLE hSipStack;
		SIP_CONN_HANDLE hSipCnx;        // dummy connection
		SIP_MSG_HANDLE hSipResp;

		eRetCode = cl_get_sip_handle( hClStack_i, &hSipStack );
		if( MS_ERR_NONE == eRetCode )
		{
			hSipResp = SIP_CreateResponse( hSipStack, hSipCnx, hSipMsg_i, SIP_RES_OK );
			if( NULL != hSipResp )
			{
				eRetCode = cl_send_final_response( hClStack_i, hCallLeg_i, hSipResp );
			}
			else
			{
				eRetCode = MS_ERR_SIP_STACK;
			}
		}
	}

	//post_condition:
	{
	}
	/* - */

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %d\n", eRetCode );

	return (eRetCode);
}

int Test_TimeOutNotifyCallBack( AMPS_HANDLE hFramework, void* pvData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	h_CallLegStack hClStack = (h_CallLegStack)pvData_i;

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//AMPS_StartTimer(hFramework, 500, Test_TimeOutNotifyCallBack, pvData_i);
	{
		SIP_HANDLE hSipStack;

		eRetCode = cl_get_sip_handle( hClStack, &hSipStack );
		if( MS_ERR_NONE == eRetCode )
		{
			SIP_MSG_HANDLE hSipMsg;
			//SIP_SDP_MSG_HANDLE	hSdp;

			hSipMsg = SIP_AllocateMsg( hSipStack, SIP_MSG_TYPE_INVITE );
			//hSdp = SIP_Allocate
			// SDPMsg(hSipStack);
			if( NULL != hSipMsg ) // && NULL != hSdp )
			{
				//SIP_AppendSDPMsg(hSipStack, hSipMsg, hSdp);

				//set_request_line_method(hSipStack, hSipMsg, "INVITE");
				set_request_uri( hSipStack, hSipMsg, "waqqas", "192.168.0.1", 0 );
				set_from_uri( hSipStack, hSipMsg, "waqqas", "192.168.0.225", 5060 );
				set_to_uri( hSipStack, hSipMsg, "waqqas", "192.168.0.1", 5060 );
				set_contact_uri( hSipStack, hSipMsg, "waqqas", "192.168.0.225", 5060 );

				{
					h_CallLeg hCallLeg = NULL;
					t_CallLegCreateConfig oCallLegCreateConfig;

					memset( &oCallLegCreateConfig.hSipCnx, 0, sizeof (SIP_CONN_HANDLE));

					eRetCode = cl_create( hClStack, &hCallLeg, &oCallLegCreateConfig );

					MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "cl_create return code value = %d\n", eRetCode );
					if( MS_ERR_NONE == eRetCode )
					{
						t_CallLegInitConfig oCallLegInitConfig;

						oCallLegInitConfig.hAppCtx = NULL;
						oCallLegInitConfig.oCallLegCb.pfRequestCompleted = Test_RequestCompleted;
						//oCallLegInitConfig.oCallLegCb.pfRequestProceeding = Test_RequestProceeding;
						oCallLegInitConfig.oCallLegCb.pfProvisionalRespRecvd    = Test_ProvisionalRespRecvd;
						oCallLegInitConfig.oCallLegCb.pfIncomingInvite = Test_IncomingInvite;
						oCallLegInitConfig.oCallLegCb.pfIncomingNonInvite = Test_IncomingNonInvite;

						eRetCode = cl_initialize( hClStack, hCallLeg, &oCallLegInitConfig );

						MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "cl_initialize return code value = %d\n", eRetCode );
						if( MS_ERR_NONE == eRetCode )
						{
							eRetCode = cl_send_invite_request( hClStack, hCallLeg, hSipMsg );

							MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "cl_send_invite_request return code value = %d\n", eRetCode );
						}
					}
				}
			}
			else
			{
				MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_ERROR, "SIP Message allocation failed.\n" );
			}
		}
	}

	MS_TRACE( nTraceId_g, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
	return (AMPS_SUCCESS);
}

/*
int set_request_line_method(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcMethod_i)
{
int nRetVal = 0;
t_SIPTag oSipTag;

oSipTag.oSIPTagType = SIP_TAG_RL_METHOD;
oSipTag.u_SIPTagValue.oSIPStringRLMethod.poString = (unsigned char*)strdup( pcMethod_i );
oSipTag.u_SIPTagValue.oSIPStringRLMethod.unLength = strlen(pcMethod_i);

nRetVal = SIP_SetTagToMsg(hSipStack_i, hSipMsg_i,& oSipTag);

return nRetVal;
}

int set_request_uri(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcUser_i, char* pcHost_i, unsigned int unPort_i)
{
int nRetVal = 0;
t_SIPTag oSipTag;
t_SIPString oTempString={0,0};
t_SIPString oSipUserString;
t_SIPString oSipHostString;

oSipTag.oSIPTagType = SIP_TAG_RL_REQUEST_URI;

oSipUserString.poString = (unsigned char*) pcUser_i;
oSipUserString.unLength = strlen(pcUser_i);

oSipHostString.poString = (unsigned char*) pcHost_i;
oSipHostString.unLength = strlen(pcHost_i);

nRetVal = SIP_SetURI(hSipStack_i, &oSipTag.u_SIPTagValue.oRURI, &oTempString, &oSipUserString, &oSipHostString, unPort_i);

nRetVal = SIP_SetTagToMsg(hSipStack_i, hSipMsg_i,& oSipTag);

return nRetVal;
}

int set_from_uri(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcUser_i, char* pcHost_i, unsigned int unPort_i)
{
int nRetVal = 0;
t_SIPTag oSipTag;
t_SIPString oTempString={0,0};
t_SIPString oSipUserString;
t_SIPString oSipHostString;

oSipTag.oSIPTagType = SIP_TAG_FROM_URI;

oSipUserString.poString = (unsigned char*)  pcUser_i;
oSipUserString.unLength = strlen(pcUser_i);

oSipHostString.poString = (unsigned char*)  pcHost_i;
oSipHostString.unLength = strlen(pcHost_i);

nRetVal = SIP_SetURI(hSipStack_i, &oSipTag.u_SIPTagValue.oFromURI, &oTempString, &oSipUserString, &oSipHostString, unPort_i);

nRetVal = SIP_SetTagToMsg(hSipStack_i, hSipMsg_i,& oSipTag);

return nRetVal;
}
int set_to_uri(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcUser_i, char* pcHost_i, unsigned int unPort_i)
{
int nRetVal = 0;
t_SIPTag oSipTag;
t_SIPString oTempString={0,0};
t_SIPString oSipUserString;
t_SIPString oSipHostString;

oSipTag.oSIPTagType = SIP_TAG_TO_URI;

oSipUserString.poString = (unsigned char*) pcUser_i;
oSipUserString.unLength = strlen(pcUser_i);

oSipHostString.poString = (unsigned char*) pcHost_i;
oSipHostString.unLength = strlen(pcHost_i);

nRetVal = SIP_SetURI(hSipStack_i, &oSipTag.u_SIPTagValue.oToURI, &oTempString, &oSipUserString, &oSipHostString, unPort_i);

nRetVal = SIP_SetTagToMsg(hSipStack_i, hSipMsg_i,& oSipTag);

return nRetVal;
}

int set_contact_uri(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcUser_i, char* pcHost_i, unsigned int unPort_i)
{
int nRetVal = 0;
t_SIPTag oSipTag;
t_SIPString oTempString={0,0};
t_SIPString oSipUserString;
t_SIPString oSipHostString;

oSipTag.oSIPTagType = SIP_TAG_CONTACT_URI;

oSipUserString.poString = (unsigned char*) pcUser_i;
oSipUserString.unLength = strlen(pcUser_i);

oSipHostString.poString = (unsigned char*) pcHost_i;
oSipHostString.unLength = strlen(pcHost_i);

nRetVal = SIP_SetURI(hSipStack_i, &oSipTag.u_SIPTagValue.oContactURI, &oTempString, &oSipUserString, &oSipHostString, unPort_i);

nRetVal = SIP_SetTagToMsg(hSipStack_i, hSipMsg_i,& oSipTag);

return nRetVal;
}
*/

// int set_sdp_media_line(SIP_HANDLE hSipStack_i, SIP_SDP_MSG_HANDLE hSdpMsg_i, char* pchMediaType_i, char* pchPort_i, char* pchProto_i, char* pchEnc_i, char* pchAttr_i)
// {
//  SIP_SetSDPTagToMsg(hSipStack_i, hSdpMsg_i, SIP_TAG_SDP_MEDIA_TYPE, (SIP_SDP_TAG_VALUE)pchMediaType_i);
//  SIP_SetSDPTagToMsg(hSipStack_i, hSdpMsg_i, SIP_TAG_SDP_MEDIA_PORT, (SIP_SDP_TAG_VALUE)pchPort_i);
//  SIP_SetSDPTagToMsg(hSipStack_i, hSdpMsg_i, SIP_TAG_SDP_MEDIA_PROTO, (SIP_SDP_TAG_VALUE)pchProto_i);
//  SIP_SetSDPTagToMsg(hSipStack_i, hSdpMsg_i, SIP_TAG_SDP_MEDIA_ENC, (SIP_SDP_TAG_VALUE)pchEnc_i);
//  SIP_SetSDPTagToMsg(hSipStack_i, hSdpMsg_i, SIP_TAG_SDP_MEDIA_ATTR, (SIP_SDP_TAG_VALUE)pchAttr_i);
//
//  return 0;
// }
//
// int set_sdp_origin_line(SIP_HANDLE hSipStack_i, SIP_SDP_MSG_HANDLE hSdpMsg_i, char* pcUsername_i, char* pcSessId_i, char* pcVersion_i, char* pcNetwtype_i, char* pchAttr_i)
// {
//  SIP_SetSDPTagToMsg(hSipStack_i, hSdpMsg_i, SIP_TAG_SDP_ORIGIN_USERNAME, (SIP_SDP_TAG_VALUE)pchMediaType_i);
//  SIP_SetSDPTagToMsg(hSipStack_i, hSdpMsg_i, SIP_TAG_SDP_ORIGIN_SESSIONID, (SIP_SDP_TAG_VALUE)pchPort_i);
//  SIP_SetSDPTagToMsg(hSipStack_i, hSdpMsg_i, SIP_TAG_SDP_ORIGIN_VERSION, (SIP_SDP_TAG_VALUE)pchProto_i);
//  SIP_SetSDPTagToMsg(hSipStack_i, hSdpMsg_i, SIP_TAG_SDP_ORIGIN_NETWORKTYPE, (SIP_SDP_TAG_VALUE)pchEnc_i);
//  SIP_SetSDPTagToMsg(hSipStack_i, hSdpMsg_i, SIP_TAG_SDP_ORIGIN_ADDRESSTYPE, (SIP_SDP_TAG_VALUE)pchAttr_i);
//  SIP_SetSDPTagToMsg(hSipStack_i, hSdpMsg_i, SIP_TAG_SDP_ORIGIN_ADDRESS, (SIP_SDP_TAG_VALUE)pchAttr_i);
//
//  return 0;
// }

