#include "ms_util.h"

e_MsRetCode MS_GetConfCtx( t_MsCtx* poMsCtx_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i, t_MsConfCtx** ppoMsConfCtx_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != ppoMsConfCtx_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_AMPSSList* poConfCtxNode = NULL;
		t_String oConfId = TSTRING_INITIALIZER;
		int nConfCtxFound = AMPS_FALSE;
		t_MsConfCtx* poMsConfCtx = NULL;

		eRetCode = MS_GetConfIdFromMsg( poMsCtx_i, hSipMsg_i, &oConfId );
		if( MS_ERR_NONE == eRetCode )
		{
			TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Conference Id in message = ", &oConfId );

			for( poConfCtxNode = poMsCtx_i->poConfCtxList->poAMPSSListHead; poConfCtxNode != NULL; poConfCtxNode = poConfCtxNode->poAMPSSListNext )
			{
				poMsConfCtx = poConfCtxNode->pvData;
				if( 0 == TS_CompareCase( &poMsConfCtx->oConfId, &oConfId ) )
				{
					nConfCtxFound = AMPS_TRUE;
					*ppoMsConfCtx_o = poConfCtxNode->pvData;

					ms_assert_return( (*ppoMsConfCtx_o)->nMagicNum == MS_CONF_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
					break;
				}
			}
			TS_Free( &oConfId );
		}
		if( nConfCtxFound == AMPS_FALSE )
		{
			eRetCode = MS_ERR_END_OF_LIST;
		}
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( NULL != *ppoMsConfCtx_o );
		}
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_SetLocalSdp( t_MsCtx* poMsCtx_i, h_Sdp hLocalSdp_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hLocalSdp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( NULL != poMsCtx_i->hLocalSdp )
		{
			DeallocateSdp( poMsCtx_i->hLocalSdp );
		}
		poMsCtx_i->hLocalSdp = hLocalSdp_i;
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_SetSipStack( t_MsCtx* poMsCtx_i, SIP_HANDLE hSipStack_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poMsCtx_i->hSipStack = hSipStack_i;
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_GetConfIdFromMsg( t_MsCtx* poMsCtx_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poConfId_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poConfId_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsCtx_i->hSipStack, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_SIPTag oSipTag;

		memset( &oSipTag, 0, sizeof (t_SIPTag));
		oSipTag.oSIPTagType = SIP_TAG_RL_REQUEST_URI;
		if( AMPS_SUCCESS == SIP_GetTagFromMsg( poMsCtx_i->hSipStack, hSipMsg_i, &oSipTag ) )
		{
			ms_assert( NULL != oSipTag.u_SIPTagValue.oRURI.puchUser );
			ms_assert( 0 < oSipTag.u_SIPTagValue.oRURI.unUserLength );
			ms_assert( 0 == memcmp( oSipTag.u_SIPTagValue.oRURI.puchUser, "conf=", 5 ));

			// Get Conf Id for user part

			TS_SetN((char*)&oSipTag.u_SIPTagValue.oRURI.puchUser[5], oSipTag.u_SIPTagValue.oRURI.unUserLength-5, poConfId_i );
		}
		else
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_GetMscmlFromMsg( t_MsCtx* poMsCtx_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poMscml_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMscml_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsCtx_i->hSipStack, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_SIPTag oSipTag;

		// Check if Content type is "application/mediaservercontrol+xml"
		memset( &oSipTag, 0, sizeof (t_SIPTag));
		oSipTag.oSIPTagType = SIP_TAG_CONTENT_TYPE;
		if( AMPS_SUCCESS == SIP_GetTagFromMsg( poMsCtx_i->hSipStack, hSipMsg_i, &oSipTag ) )
		{
			t_String oMsgContentType = TSTRING_INITIALIZER;
			t_String oMscmlContentType = {(unsigned char*)"mediaservercontrol+xml", (unsigned int) sizeof ("mediaservercontrol+xml")-1};

			TS_SetN((char*)oSipTag.u_SIPTagValue.oContentType.oSubtype.poString, oSipTag.u_SIPTagValue.oContentType.oSubtype.unLength, &oMsgContentType );

			TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Content type of message = ", &oMsgContentType );
			if( 0 == TS_CompareCase( &oMsgContentType, &oMscmlContentType ) )
			{
				t_SIPMsgPayload oSipMsgPayload;
				memset( &oSipMsgPayload, 0, sizeof (t_SIPMsgPayload));
				if( AMPS_SUCCESS == SIP_GetSIPMsgPayload( poMsCtx_i->hSipStack, hSipMsg_i, &oSipMsgPayload ) )
				{
					TS_SetN((char*)oSipMsgPayload.oSIPStringForContent.poString, oSipMsgPayload.oSIPStringForContent.unLength, poMscml_i );
				}
			}
			else
			{
				eRetCode = MS_ERR_MISSING_FIELD;
			}
			TS_Free( &oMsgContentType );
		}
		else
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != poMscml_i->pucData );
			ms_assert( 0 < poMscml_i->unLength );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_GetAvailableUserCtrlCtx( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx** ppoMsUserCtrlCtx_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsConfCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsConfCtx_i->nMagicNum == MS_CONF_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
		ms_assert_return( NULL != ppoMsUserCtrlCtx_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_AMPSSList* poUserCtrlCtxNode = NULL;
		AMPS_BOOL bUserCtrlCtxFound = AMPS_FALSE;
		t_MsUserCtrlCtx* poUserCtrlCtx = NULL;
		if( poMsConfCtx_i->nAvailableCtrlCtx == -1 )    //TODO: #define
		{
			eRetCode = MS_CreateUserCtrlCtx( poMsCtx_i, AMPS_FALSE, ppoMsUserCtrlCtx_o );

			FW_DListAppend( poMsConfCtx_i->poUserCtrlCtxList, *ppoMsUserCtrlCtx_o );
		}
		else
		{
			for( poUserCtrlCtxNode = poMsConfCtx_i->poUserCtrlCtxList->poAMPSSListHead; poUserCtrlCtxNode != NULL; poUserCtrlCtxNode = poUserCtrlCtxNode->poAMPSSListNext )
			{
				poUserCtrlCtx = poUserCtrlCtxNode->pvData;
				if( UCS_AVAILABLE == poUserCtrlCtx->eState )
				{
					bUserCtrlCtxFound = AMPS_TRUE;
					*ppoMsUserCtrlCtx_o = poUserCtrlCtx;
					break;
				}
			}
			if( bUserCtrlCtxFound == AMPS_FALSE )
			{
				eRetCode = MS_ERR_END_OF_LIST;
			}
		}
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( NULL != *ppoMsUserCtrlCtx_o );
		}
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_NegotiateSdp( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, SIP_MSG_HANDLE hSipMsg_i, h_Sdp* phNegotiatedSdp_o, t_AMPSDList* poNegInfoList_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phNegotiatedSdp_o, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsUserCtrlCtx_i->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
	}

	{
		t_String oSdp = TSTRING_INITIALIZER;

		eRetCode = MS_GetSdpFromMsg( poMsCtx_i, hSipMsg_i, &oSdp );
		if( MS_ERR_NONE == eRetCode )
		{
			TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Remote SDP = ", &oSdp );

			t_SdpOamConfig oSdpOamConfig;

			t_SdpConfig oSdpConfig;
			//h_Sdp hRemoteSdp = NULL;

			// Initialize hRemoteSdp
			oSdpConfig.nTraceId = poMsCtx_i->nTraceId;

			eRetCode = AllocateSdp( &oSdpConfig, &poMsUserCtrlCtx_i->hRemoteSdp );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = SdpDecode( poMsCtx_i->hSdpParser, poMsUserCtrlCtx_i->hRemoteSdp, oSdp.pucData, oSdp.unLength );
				if( MS_ERR_NONE == eRetCode )
				{
					MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "SDP parsed successfully\n" );

					// Initialize Negotiated sdp

					eRetCode = AllocateSdp( &oSdpConfig, phNegotiatedSdp_o );
					if( MS_ERR_NONE == eRetCode )
					{
						memset( &oSdpOamConfig, 0, sizeof (t_SdpOamConfig));
						oSdpOamConfig.nTraceId =  poMsCtx_i->nTraceId;
						oSdpOamConfig.hSdpParser = poMsCtx_i->hSdpParser;

						// Add default attributes
						{
							t_SDPVersion oVersion;

							oVersion.nVersion = 0;
							eRetCode = SdpSetProtoVersion( poMsCtx_i->hSdpParser, *phNegotiatedSdp_o, &oVersion );

							ms_assert( MS_ERR_NONE == eRetCode );

							t_SDPOrigin oOrigin;
							t_String oSess = TSTRING_INITIALIZER;

							TS_AllocBuffer( &oSess, 32 );
							memset( &oOrigin, 0, sizeof (t_SDPOrigin));

							TS_CopyDuplicate( &oOrigin.oUserName, &poMsCtx_i->oLocalUri.oUser );

							TS_Free( &oSess );

							TS_AllocBuffer( &oSess, 32 );

							TS_GenerateRandomString( &oSess );
							//oOrigin.nSessionId = TS_StringToInt(&oSess, 10);

							oOrigin.nSessionId = 0;

							TS_GenerateRandomString( &oSess );
//							oOrigin.nSessionVersion = TS_StringToInt(&oSess, 10);

							oOrigin.nSessionVersion = 0;

							TS_Free( &oSess );

							TS_Set( "IN", &oOrigin.oSDPConnectionData.oNetType );
							TS_Set( "IP4", &oOrigin.oSDPConnectionData.oAddrType );
							TS_CopyDuplicate( &oOrigin.oSDPConnectionData.oConnectionAddress, &poMsCtx_i->oLocalIp );

							eRetCode = SdpSetOrigin( poMsCtx_i->hSdpParser, *phNegotiatedSdp_o, &oOrigin );

							ms_assert( MS_ERR_NONE == eRetCode );

							SdpFreeOrigin( &oOrigin );

							// Set Session Name
							t_SDPSessionName oSessName;

							memset( &oSessName, 0, sizeof (t_SDPSessionName));

							TS_CopyDuplicate( &oSessName.oName, &poMsCtx_i->oLocalUri.oUser );

							eRetCode = SdpSetSessionName( poMsCtx_i->hSdpParser, *phNegotiatedSdp_o, &oSessName );

							SdpFreeSessionName( &oSessName );

							ms_assert( MS_ERR_NONE == eRetCode );

							// Set Time
							t_SDPTiming oTime;

							oTime.nStartTime = 0;
							oTime.nStopTime = 0;

							eRetCode = SdpAddTime( poMsCtx_i->hSdpParser, *phNegotiatedSdp_o, -1, &oTime );

							ms_assert( MS_ERR_NONE == eRetCode );

							// Set Connection
							t_SDPConnection oConn;

							memset( &oConn, 0, sizeof (t_SDPConnection));

							TS_Set( "IN", &oConn.oNetType );
							TS_Set( "IP4", &oConn.oAddrType );
							TS_CopyDuplicate( &oConn.oConnectionAddress, &poMsCtx_i->oLocalIp );

							eRetCode = SdpSetConnection( poMsCtx_i->hSdpParser, *phNegotiatedSdp_o, &oConn );

							SdpFreeConnection( &oConn );

							ms_assert( MS_ERR_NONE == eRetCode );
						}

						// Add default media attributes in Remote SDP for codecs in media line
						SdpOamAddDefaultMediaDescAttr( poMsUserCtrlCtx_i->hRemoteSdp, &oSdpOamConfig );

						eRetCode = SdpOamGenerateAnswer( poMsCtx_i->hLocalSdp,  poMsUserCtrlCtx_i->hRemoteSdp, &oSdpOamConfig, phNegotiatedSdp_o,   poNegInfoList_o );
						if( MS_ERR_NONE != eRetCode )
						{
							DeallocateSdp( phNegotiatedSdp_o );
						}
					}
					else
					{
					}
					//DeallocateSdp( &hRemoteSdp );
				}
				else
				{
					MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "SDP not parsed successfully\n" );
				}
			}
			else
			{
			}
			TS_Free( &oSdp );
		}
		else
		{
			MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "SDP not present in message\n" );
		}
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( NULL != *phNegotiatedSdp_o );
		}
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_SendResponse( t_MsCtx* poMsCtx_i, h_CallLeg hCallLeg_i, t_SipAppData* poSipData_i, SIP_MSG_HANDLE hSipMsg_i, e_SIPResStatusCodes eSipStatusCode_i, t_String* poContentType_i, t_String* poContentBody_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poSipData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poContentType_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poContentBody_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SIP_MSG_HANDLE hSipResp = NULL;
		SIP_CONN_HANDLE hSipCnx;        // dummy connection

		hSipResp = SIP_CreateResponse( poMsCtx_i->hSipStack, hSipCnx, hSipMsg_i, eSipStatusCode_i );
		if( NULL != hSipResp )
		{
			// Generate TO Tag if not already present
			if( NULL == poSipData_i->oLocalTag.pucData )
			{
				TS_AllocBuffer( &poSipData_i->oLocalTag, 32 );

				TS_GenerateRandomString( &poSipData_i->oLocalTag );

				TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Generating Local Tag = ", &poSipData_i->oLocalTag );
			}
			{
				int nRetVal = 0;

				t_SIPString oTagName = {(unsigned char*)"tag", sizeof ("tag")-1};
				t_SIPString oParamTagValue = {NULL, 0};

				oParamTagValue.poString = poSipData_i->oLocalTag.pucData;
				oParamTagValue.unLength = poSipData_i->oLocalTag.unLength;

				//Set TO tag
				nRetVal = SIP_SetParamToMsgTag( poMsCtx_i->hSipStack, hSipResp, SIP_TAG_TO_PARAM, oTagName, oParamTagValue );
				if( nRetVal != AMPS_SUCCESS )
				{
					eRetCode = MS_ERR_SIP_STACK;
				}
			}

			// Set Local URI as contact
			eRetCode = SU_SetContactUri( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipResp, &poMsCtx_i->oLocalUri.oUser, &poMsCtx_i->oLocalIp, poMsCtx_i->oLocalUri.unPort );

			ms_assert( MS_ERR_NONE == eRetCode );

			eRetCode = SU_AddContentInMsg( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipResp, poContentType_i, poContentBody_i );

			eRetCode = cl_send_final_response( poMsCtx_i->hCallLegStack, hCallLeg_i, hSipResp );
		}
		else
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_SendInfoRequest( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i, t_String* poContentType_i, t_String* poContentBody_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poContentType_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poContentBody_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SIP_MSG_HANDLE hSipReq = NULL;

		hSipReq = SIP_AllocateMsg( poMsCtx_i->hSipStack, SIP_MSG_TYPE_INFO );
		if( NULL != hSipReq ) // && NULL != hSdp )
		{
			// Set Remote URI as Request URI
			eRetCode = SU_SetRequestUri( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipReq, &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oUser, &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oHost, poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.unPort );

			ms_assert( MS_ERR_NONE == eRetCode );
			if( NULL == poUserCtrlCtx_i->oSipData.oLocalTag.pucData )
			{
				TS_AllocBuffer( &poUserCtrlCtx_i->oSipData.oLocalTag, 32 );

				TS_GenerateRandomString( &poUserCtrlCtx_i->oSipData.oLocalTag );

				TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Generating Local Tag = ", &poUserCtrlCtx_i->oSipData.oLocalTag );
			}
			TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Local Tag = ", &poUserCtrlCtx_i->oSipData.oLocalTag );

			// Set Media Server Uri as local uri
			eRetCode = SU_SetFromUri( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipReq, &poMsCtx_i->oLocalUri.oUser, &poMsCtx_i->oLocalUri.oHost, poMsCtx_i->oLocalUri.unPort, &poUserCtrlCtx_i->oSipData.oLocalTag );

			ms_assert( MS_ERR_NONE == eRetCode );

			// Set Remote URI as TO URI
			eRetCode = SU_SetToUri( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipReq, &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oUser, &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oHost, poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.unPort, &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oTag );

			ms_assert( MS_ERR_NONE == eRetCode );

			// Set Local URI as contact
			eRetCode = SU_SetContactUri( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipReq, &poMsCtx_i->oLocalIp, &poMsCtx_i->oLocalUri.oHost, poMsCtx_i->oLocalUri.unPort );

			ms_assert( MS_ERR_NONE == eRetCode );

			eRetCode = SU_AddContentInMsg( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipReq, poContentType_i, poContentBody_i );
			if( MS_ERR_NONE == eRetCode )
			{
				// send INFO message
				eRetCode = cl_send_non_invite_request( poMsCtx_i->hCallLegStack, poUserCtrlCtx_i->hCallLeg, hSipReq );
			}
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_SendByeRequest( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i, t_String* poContentType_i, t_String* poContentBody_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poContentType_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poContentBody_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SIP_MSG_HANDLE hSipReq = NULL;

		hSipReq = SIP_AllocateMsg( poMsCtx_i->hSipStack, SIP_MSG_TYPE_BYE );
		if( NULL != hSipReq ) // && NULL != hSdp )
		{
			// Set Remote URI as Request URI
			eRetCode = SU_SetRequestUri( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipReq, &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oUser, &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oHost, poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.unPort );

			ms_assert( MS_ERR_NONE == eRetCode );

			TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Local Tag = ", &poUserCtrlCtx_i->oSipData.oLocalTag );

			// Set Media Server Uri as local uri
			eRetCode = SU_SetFromUri( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipReq, &poMsCtx_i->oLocalUri.oUser, &poMsCtx_i->oLocalUri.oHost, poMsCtx_i->oLocalUri.unPort, &poUserCtrlCtx_i->oSipData.oLocalTag );

			ms_assert( MS_ERR_NONE == eRetCode );

			// Set Remote URI as TO URI
			eRetCode = SU_SetToUri( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipReq, &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oUser, &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oHost, poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.unPort, &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oTag );

			ms_assert( MS_ERR_NONE == eRetCode );

			// Set Local URI as contact
			eRetCode = SU_SetContactUri( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipReq, &poMsCtx_i->oLocalIp, &poMsCtx_i->oLocalUri.oHost, poMsCtx_i->oLocalUri.unPort );

			ms_assert( MS_ERR_NONE == eRetCode );

			eRetCode = SU_AddContentInMsg( poMsCtx_i->nTraceId, poMsCtx_i->hSipStack, hSipReq, poContentType_i, poContentBody_i );
			if( MS_ERR_NONE == eRetCode )
			{
				// send INFO message
				eRetCode = cl_send_non_invite_request( poMsCtx_i->hCallLegStack, poUserCtrlCtx_i->hCallLeg, hSipReq );
			}
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_GetUserCtrlCtx( t_MsCtx* poMsCtx_i, t_AMPSDList* poUserCtxList_i, h_CallLeg hCallLeg_i, t_MsUserCtrlCtx** ppoUserCtrlCtx_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUserCtxList_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != ppoUserCtrlCtx_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_AMPSSList* poUserCtrlCtxNode = NULL;
		t_MsUserCtrlCtx* poUserCtrlCtx = NULL;
		int nUserCtlCtxFound = AMPS_FALSE;

		for( poUserCtrlCtxNode = poUserCtxList_i->poAMPSSListHead; poUserCtrlCtxNode != NULL; poUserCtrlCtxNode = poUserCtrlCtxNode->poAMPSSListNext )
		{
			poUserCtrlCtx = poUserCtrlCtxNode->pvData;
			if( poUserCtrlCtx->hCallLeg == hCallLeg_i )
			{
				*ppoUserCtrlCtx_o = poUserCtrlCtx;
				nUserCtlCtxFound = AMPS_TRUE;
				break;
			}
		}
		if( nUserCtlCtxFound != AMPS_TRUE )
		{
			eRetCode = MS_ERR_END_OF_LIST;
		}
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( NULL != *ppoUserCtrlCtx_o );
		}
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_DestroyUserCtrlCtx( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_close( poMsCtx_i->hCallLegStack, poUserCtrlCtx_i->hCallLeg );
		eRetCode = cl_destroy( poMsCtx_i->hCallLegStack, &poUserCtrlCtx_i->hCallLeg );
		if( NULL != poUserCtrlCtx_i->hNegotiatedSdp )
		{
			eRetCode = DeallocateSdp( &poUserCtrlCtx_i->hNegotiatedSdp );
		}
		if( NULL != poUserCtrlCtx_i->hRemoteSdp )
		{
			eRetCode = DeallocateSdp( &poUserCtrlCtx_i->hRemoteSdp );
		}
		if( NULL != poUserCtrlCtx_i->hRxChain )
		{
			unsigned int unValue;
			eRetCode = MS_GetNumOfRTPPackets( poMsCtx_i, poUserCtrlCtx_i->hRxChain, MCT_RTPRECV, MCC_RTPRECV_NUM_PKTS_RECV, &unValue );
			if( MS_ERR_NONE == eRetCode )
			{
				SNMP_AddInPerformanceCtr( poMsCtx_i, PC_IncomingRTPPackets, unValue );
			}
			eRetCode = MS_Media_RxChainDestroy( poMsCtx_i, poUserCtrlCtx_i->hRxChain );
			MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "MS_Media_RxChainDestroy poUserCtrlCtx_i->hRxChain %p\n", poUserCtrlCtx_i->hRxChain );
		}
		if( NULL != poUserCtrlCtx_i->hTxChain )
		{
			unsigned int unValue;
			eRetCode = MS_GetNumOfRTPPackets( poMsCtx_i, poUserCtrlCtx_i->hTxChain, MCT_RTPSEND, MCC_RTPSEND_NUM_PKTS_SENT, &unValue );
			if( MS_ERR_NONE == eRetCode )
			{
				SNMP_AddInPerformanceCtr( poMsCtx_i, PC_OutgoingRTPPackets, unValue );
			}
			eRetCode = MS_Media_TxChainDestroy( poMsCtx_i, poUserCtrlCtx_i->hTxChain );
			MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "MS_Media_TxChainDestroy poUserCtrlCtx_i->hTxChain %p\n", poUserCtrlCtx_i->hTxChain );
		}
		if( NULL != poUserCtrlCtx_i->pvMscmlOpCtx )
		{
			MOP_DestroySM( poUserCtrlCtx_i->pvMscmlOpCtx );
		}
		TS_Free( &poUserCtrlCtx_i->oTeamMemberId );

		t_AMPSSList* poListNode = NULL;
		t_MsUserCtrlCtx* poTeamMateUserCtrlCtx = NULL;

		// Remove this node from team mate list
		for( poListNode = poUserCtrlCtx_i->poTeamMateList->poAMPSSListHead; poListNode != NULL; poListNode = poListNode->poAMPSSListNext )
		{
			poTeamMateUserCtrlCtx = poListNode->pvData;

			eRetCode = MS_Media_ChainSetMode( poMsCtx_i, poTeamMateUserCtrlCtx->poMsConfCtx, poTeamMateUserCtrlCtx, MM_MUTE, NULL );

			DListRemoveFromData( poTeamMateUserCtrlCtx->poTeamMateList, poUserCtrlCtx_i, NULL );
		}
		eRetCode = MS_Team_Destroy( poMsCtx_i, &poUserCtrlCtx_i->poTeamMateList );
		if( NULL != poUserCtrlCtx_i->poQuarantineBuffer )
		{
			FW_DListFree( &poUserCtrlCtx_i->poQuarantineBuffer, Signal_LListRemoveLinkDataCallback );
		}
		if( NULL != poUserCtrlCtx_i->poNegInfoList )
		{
			FW_DListFree( &poUserCtrlCtx_i->poNegInfoList, SdpOamNegMediaInfoRemoveLinkCb );
		}
		TS_Free( &poUserCtrlCtx_i->oSipData.oRemoteContact.oUser );
		TS_Free( &poUserCtrlCtx_i->oSipData.oRemoteContact.oHost );

		TS_Free( &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oUser );
		TS_Free( &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oHost );
		TS_Free( &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oTag );

		TS_Free( &poUserCtrlCtx_i->oSipData.oLocalTag );

		//MS_Free( NULL, poUserCtrlCtx_i );
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_DestroyConfCtx( t_MsCtx* poMsCtx_i, t_MsConfCtx* poConfCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poConfCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_DestroyConfCtrlCtx( poMsCtx_i, poConfCtx_i->poConfCtrlCtx );
		FW_Free( poConfCtx_i->poConfCtrlCtx );

		t_AMPSSList* poUserCtrlCtxNode = NULL;

		// Delete all user ctrl legs
		for( poUserCtrlCtxNode = poConfCtx_i->poUserCtrlCtxList->poAMPSSListHead; poUserCtrlCtxNode != NULL; poUserCtrlCtxNode = poUserCtrlCtxNode->poAMPSSListNext )
		{
			eRetCode = MS_DestroyUserCtrlCtx( poMsCtx_i, (t_MsUserCtrlCtx*)poUserCtrlCtxNode->pvData );
			FW_Free( (t_MsUserCtrlCtx*)poUserCtrlCtxNode->pvData );
		}
		TS_Free( &poConfCtx_i->oConfId );

		DListRemoveFromData( poMsCtx_i->poConfCtxList, poConfCtx_i, NULL );
		if( NULL != poConfCtx_i->hTimer )
		{
			eRetCode = ST_Destroy( poConfCtx_i->hTimer );
		}
		FW_Free( poConfCtx_i );
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_DestroyConfCtrlCtx( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_close( poMsCtx_i->hCallLegStack, poUserCtrlCtx_i->hCallLeg );
		eRetCode = cl_destroy( poMsCtx_i->hCallLegStack, &poUserCtrlCtx_i->hCallLeg );
		if( NULL != poUserCtrlCtx_i->hNegotiatedSdp )
		{
			eRetCode = DeallocateSdp( &poUserCtrlCtx_i->hNegotiatedSdp );
		}
		if( NULL != poUserCtrlCtx_i->hRxChain )
		{
			eRetCode = MS_Media_RxChainDestroy( poMsCtx_i, poUserCtrlCtx_i->hRxChain );
			MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "MS_Media_RxChainDestroy poUserCtrlCtx_i->hRxChain %p\n", poUserCtrlCtx_i->hRxChain );
		}
		if( NULL != poUserCtrlCtx_i->hTxChain )
		{
			eRetCode = MS_Media_TxChainDestroy( poMsCtx_i, poUserCtrlCtx_i->hTxChain );
			MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "MS_Media_TxChainDestroy poUserCtrlCtx_i->hTxChain %p\n", poUserCtrlCtx_i->hRxChain );
		}
		if( NULL != poUserCtrlCtx_i->pvMscmlOpCtx )
		{
			MOP_DestroySM( poUserCtrlCtx_i->pvMscmlOpCtx );
		}
		TS_Free( &poUserCtrlCtx_i->oTeamMemberId );

		eRetCode = MS_Team_Destroy( poMsCtx_i, &poUserCtrlCtx_i->poTeamMateList );
		if( NULL != poUserCtrlCtx_i->poQuarantineBuffer )
		{
			FW_DListFree( &poUserCtrlCtx_i->poQuarantineBuffer, Signal_LListRemoveLinkDataCallback );
		}
		if( NULL != poUserCtrlCtx_i->poNegInfoList )
		{
			FW_DListFree( &poUserCtrlCtx_i->poNegInfoList, SdpOamNegMediaInfoRemoveLinkCb );
		}
		TS_Free( &poUserCtrlCtx_i->oSipData.oRemoteContact.oUser );
		TS_Free( &poUserCtrlCtx_i->oSipData.oRemoteContact.oHost );

		TS_Free( &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oUser );
		TS_Free( &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oUri.oHost );
		TS_Free( &poUserCtrlCtx_i->oSipData.oRemoteUriInfo.oTag );

		TS_Free( &poUserCtrlCtx_i->oSipData.oLocalTag );

		//MS_Free( NULL, poUserCtrlCtx_i );
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_GetSdpFromMsg( t_MsCtx* poMsCtx_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poSdp_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poSdp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsCtx_i->hSipStack, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_SIPTag oSipTag;

		memset( &oSipTag, 0, sizeof (t_SIPTag));
		oSipTag.oSIPTagType = SIP_TAG_CONTENT_TYPE;
		if( AMPS_SUCCESS == SIP_GetTagFromMsg( poMsCtx_i->hSipStack, hSipMsg_i, &oSipTag ) )
		{
			t_String oMsgContentType = TSTRING_INITIALIZER;
			t_String oMscmlContentType = {(unsigned char*)"sdp", (unsigned int) sizeof ("sdp")-1};

			TS_SetN((char*)oSipTag.u_SIPTagValue.oContentType.oSubtype.poString, oSipTag.u_SIPTagValue.oContentType.oSubtype.unLength, &oMsgContentType );

			TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Content type of message = ", &oMsgContentType );
			if( 0 == TS_CompareCase( &oMsgContentType, &oMscmlContentType ) )
			{
				t_SIPMsgPayload oSipMsgPayload;
				memset( &oSipMsgPayload, 0, sizeof (t_SIPMsgPayload));
				if( AMPS_SUCCESS == SIP_GetSIPMsgPayload( poMsCtx_i->hSipStack, hSipMsg_i, &oSipMsgPayload ) )
				{
					TS_SetN((char*)oSipMsgPayload.oSIPStringForContent.poString, oSipMsgPayload.oSIPStringForContent.unLength, poSdp_i );
				}
				else
				{
					eRetCode = MS_ERR_MISSING_FIELD;
				}
			}
			else
			{
				eRetCode = MS_ERR_MISSING_FIELD;
			}
			TS_Free( &oMsgContentType );
		}
		else
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != poSdp_i->pucData );
			ms_assert( 0 < poSdp_i->unLength );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

// Get Remote port from media line of the remote media descriptor that matches with negotiated SDP
e_MsRetCode MS_GetRemotePort( t_MsCtx* poMsCtx_i, h_Sdp hRemoteSdp_i, h_Sdp hNegotiatedSdp_i, int* pnRemotePort_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hRemoteSdp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hNegotiatedSdp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pnRemotePort_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		// Get first (negotiated) media line from negotiated SDP
		t_SDPMedia* poNegSdpMedia = NULL;

		eRetCode = SdpMediaDescGetMedia( poMsCtx_i->hSdpParser, hNegotiatedSdp_i, 1, &poNegSdpMedia );
		if( MS_ERR_NONE == eRetCode )
		{
			// Find remote media line corresponding to negotiated media
			int nDescIndex = 1;
			t_SDPMedia* poRemoteSdpMedia = NULL;

			while( MS_ERR_NONE == SdpMediaDescGetMedia( poMsCtx_i->hSdpParser, hRemoteSdp_i, nDescIndex, &poRemoteSdpMedia ))
			{
				nDescIndex++;
				// Match codec of negotiated and remote SDP
				if( 0 == TS_CompareCase( (t_String*)poRemoteSdpMedia->poMediaFormatList->poAMPSSListHead->pvData, (t_String*)poNegSdpMedia->poMediaFormatList->poAMPSSListHead->pvData ))
				{
					break;
				}
				else
				{
					poRemoteSdpMedia = NULL;
				}
			}
			if( NULL != poRemoteSdpMedia )
			{
				*pnRemotePort_o = poRemoteSdpMedia->nMediaPrimaryPort;
			}
			else
			{
				eRetCode = MS_ERR_END_OF_LIST;
			}
		}
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( 0 != *pnRemotePort_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_GetUserCtxFromId( t_MsCtx* poMsCtx_i, t_MsConfCtx* poConfCtrlCtx_i, t_String* poTeamMateId_i, t_MsUserCtrlCtx** ppoUserCtrlCtx_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poConfCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poTeamMateId_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != ppoUserCtrlCtx_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_AMPSSList* poConfCtrlCtxNode = NULL;
		t_MsUserCtrlCtx* poUserCtrlCtx = NULL;

		for( poConfCtrlCtxNode = poConfCtrlCtx_i->poUserCtrlCtxList->poAMPSSListHead; poConfCtrlCtxNode != NULL; poConfCtrlCtxNode = poConfCtrlCtxNode->poAMPSSListNext )
		{
			poUserCtrlCtx = (t_MsUserCtrlCtx*)poConfCtrlCtxNode->pvData;

			ms_assert( poUserCtrlCtx != NULL );
			ms_assert( poUserCtrlCtx->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM );
			if( 0 == TS_Compare( &poUserCtrlCtx->oTeamMemberId, poTeamMateId_i ) )
			{
				break;
			}
		}
		if( NULL == poConfCtrlCtxNode )
		{
			eRetCode = MS_ERR_END_OF_LIST;
		}
		else
		{
			*ppoUserCtrlCtx_o = poUserCtrlCtx;
		}
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( *ppoUserCtrlCtx_o != NULL );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

// int NegMediaInfo_RemoveLinkDataCb( void** ppvNegMediaInfo_io )
// {
//  t_SdpNegMediaInfo* poNegMediaInfo = NULL;
//
//  poNegMediaInfo = *ppvNegMediaInfo_io;
//
//  FW_Free( poNegMediaInfo );
//
//  *ppvNegMediaInfo_io = NULL;
// }

e_MsRetCode MS_CreateUserCtrlCtx( t_MsCtx* poMsCtx_i, AMPS_BOOL bReserveMedia_i, t_MsUserCtrlCtx** ppoMsUserCtrlCtx_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != ppoMsUserCtrlCtx_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MsUserCtrlCtx* poUserCtrlCtx = NULL;
		poUserCtrlCtx = MS_Malloc( NULL, sizeof (t_MsUserCtrlCtx));
		if( NULL == poUserCtrlCtx )
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
		else
		{
			memset( poUserCtrlCtx, 0, sizeof (t_MsUserCtrlCtx));

			poUserCtrlCtx->nMagicNum = MS_USER_CTRL_CTX_MAGIC_NUM;
			poUserCtrlCtx->poMsConfCtx = NULL;
			poUserCtrlCtx->eState = UCS_AVAILABLE;
			poUserCtrlCtx->eCurrentMode = MM_FULL;
			MS_Team_Init( poMsCtx_i, &poUserCtrlCtx->poTeamMateList );
			FW_DListInit( &poUserCtrlCtx->poQuarantineBuffer );
			FW_DListInit( &poUserCtrlCtx->poNegInfoList );

			poUserCtrlCtx->bDtmfClamp =  AMPS_TRUE;
			poUserCtrlCtx->hCallLeg = NULL;

			{
				t_MediaCompConfig oChainConfig;

				oChainConfig.nTraceId = poMsCtx_i->nTraceId;
				oChainConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

				eRetCode = Media_ChainCreate((h_MediaChain)&oChainConfig, &poUserCtrlCtx->hRxChain );
				eRetCode = Media_ChainSetAppCb( poUserCtrlCtx->hRxChain, MS_Media_RxChainCb, (h_MediaCompAppCtx)poMsCtx_i, (h_MediaCompAppCtx)poUserCtrlCtx );

				eRetCode = Media_ChainCreate((h_MediaChain)&oChainConfig, &poUserCtrlCtx->hTxChain );
				eRetCode = Media_ChainSetAppCb( poUserCtrlCtx->hTxChain, MS_Media_TxChainCb, (h_MediaCompAppCtx)poMsCtx_i, (h_MediaCompAppCtx)poUserCtrlCtx );
			}
			if( AMPS_TRUE == bReserveMedia_i )
			{
				eRetCode = MS_Media_RxChainInit( poMsCtx_i, poUserCtrlCtx->hRxChain );
				if( MS_ERR_NONE == eRetCode )
				{
					eRetCode = MS_Media_TxChainInit( poMsCtx_i, poUserCtrlCtx->hTxChain );
				}
			}
			if( MS_ERR_NONE == eRetCode )
			{
				*ppoMsUserCtrlCtx_o = poUserCtrlCtx;
			}
			else
			{
				MS_DestroyUserCtrlCtx( poMsCtx_i, poUserCtrlCtx );
				FW_Free( poUserCtrlCtx );
			}
		}
		MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", *ppoMsUserCtrlCtx_o );
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( NULL != *ppoMsUserCtrlCtx_o );
		}
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_CreateConfCtrlCtx( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx** ppoMsConfCtrlCtx_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsConfCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MS_CONF_CTX_MAGIC_NUM == poMsConfCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != ppoMsConfCtrlCtx_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MsUserCtrlCtx* poConfCtrlCtx = NULL;
		poConfCtrlCtx = MS_Malloc( NULL, sizeof (t_MsUserCtrlCtx));
		if( NULL == poConfCtrlCtx )
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
		else
		{
			memset( poConfCtrlCtx, 0, sizeof (t_MsUserCtrlCtx));

			poConfCtrlCtx->nMagicNum = MS_CONF_CTRL_CTX_MAGIC_NUM;
			poConfCtrlCtx->poMsConfCtx = poMsConfCtx_i;
			poConfCtrlCtx->eState = UCS_INITIALIZED;
			poConfCtrlCtx->eCurrentMode = MM_FULL;
			MS_Team_Init( poMsCtx_i, &poConfCtrlCtx->poTeamMateList );
			FW_DListInit( &poConfCtrlCtx->poQuarantineBuffer );
			FW_DListInit( &poConfCtrlCtx->poNegInfoList );
			poConfCtrlCtx->bDtmfClamp =  AMPS_TRUE;

			*ppoMsConfCtrlCtx_o = poConfCtrlCtx;
		}
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( NULL != *ppoMsConfCtrlCtx_o );
		}
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_GetTelEventPayloadType( t_MsCtx* poMsCtx_i, t_AMPSDList* poNegInfoList_i, h_Sdp hNegSdp_i, unsigned char* pucTelephonyPayloadType_o )
{
	e_MsRetCode eRetCode = MS_ERR_END_OF_LIST;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poNegInfoList_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hNegSdp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pucTelephonyPayloadType_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_AMPSSList* poNegMediaInfoEntry = NULL;
		t_SdpNegMediaInfo* poNegMediaInfo = NULL;

		for( poNegMediaInfoEntry = poNegInfoList_i->poAMPSSListHead; poNegMediaInfoEntry != NULL; poNegMediaInfoEntry = poNegMediaInfoEntry->poAMPSSListNext )
		{
			t_SDPAttribute* poAttrib = NULL;
			poNegMediaInfo = poNegMediaInfoEntry->pvData;

			eRetCode = SdpMediaDescGetAttribute( poMsCtx_i->hSdpParser, hNegSdp_i, poNegMediaInfo->nNegMediaDescIdx, poNegMediaInfo->nNegMediaDescAttrIdx, &poAttrib );

			ms_assert( MS_ERR_NONE == eRetCode );
			if( AMPS_TRUE == poAttrib->nAttribValuePresent )
			{
				t_String oEvent = {"telephone-event", sizeof ("telephone-event")-1};
				if( -1 != TS_CompareSubString( &poAttrib->attribute.oAttributeValue, &oEvent ) )
				{
					*pucTelephonyPayloadType_o = (unsigned char) TS_StringToInt( &poAttrib->attribute.oAttributeValue, 10 );

					break;
				}
			}
		}
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( 0 != *pucTelephonyPayloadType_o );
		}
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

