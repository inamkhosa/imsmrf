/*! @file call_leg.c
*
* @ingroup CallLeg
*
* @brief The file contains implementation of additional functions for call_leg API.
*
*/

#include "call_leg.h"

/*! @fn SIP_CONN_HANDLE cl_create_sip_handle( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i )
*
* @brief This function creates a sip handle.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @return Created sip handle
*
*/

SIP_CONN_HANDLE cl_create_sip_handle( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i )
{
	int nRetVal = 0;
	SIP_CONN_HANDLE hSipCnx;
	t_String oCallId = {(unsigned char*)"                                ", 32};  // A buffer of size 32

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		TS_GenerateRandomString( &oCallId );
		nRetVal = SIP_CreateConnectionHandle( poClStack_i->hSipStack, oCallId.pucData, &hSipCnx );
		if( 0 == nRetVal )
		{
			SIP_SetAppConnHandle( poClStack_i->hSipStack, hSipCnx, (SIP_APP_CONN_HANDLE)poCallLeg_i );
		}
	}

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving\n" );

	return (hSipCnx);
}

/*! @fn void cl_HandleDNSLookupResponse( AMPS_HANDLE r_hAMPSHandle, void* r_pvEventData, void* r_pvEventHandle, void* r_pvEventPayload )
*
* @brief This function handles the response of DNS lookup.
*
* @param[in] r_hAMPSHandle is the handle of AMPS.
*
* @param[in] r_pvEventData is the data for the event.
*
* @param[in] r_pvEventHandle is the handle of the event.
*
* @param[in] r_pvEventPayload is the payload of the event.
*
*/

void cl_HandleDNSLookupResponse( AMPS_HANDLE r_hAMPSHandle, void* r_pvEventData, void* r_pvEventHandle, void* r_pvEventPayload )
{
	t_SSEventDNSResponse* poSSEventDNSResponse = r_pvEventPayload;
	t_CallLegDnsData* poCLDnsData = poSSEventDNSResponse->pvSIPMessage;
	t_CallLegStack* poCLStack = poCLDnsData->poCLStack;
	t_CallLeg* poCallLeg = poCLDnsData->poCallLeg;
	t_SIPConnObject oSipRemoteConnObj;
	int nSipRetVal;

	MS_TRACE( poCLStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	if( AMPS_ERROR_FAILURE != poSSEventDNSResponse->nQueryResult )
	{
		MS_TRACE( poCLStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Sip Connection handle = %p\n", poCallLeg->hSipCnx );

		memset( &oSipRemoteConnObj, 0, sizeof (t_SIPConnObject));

		oSipRemoteConnObj = poSSEventDNSResponse->oSIPRouteSetEntry.oSIPAddress.oSIPConnObject[0];

		nSipRetVal = SIP_SendRequest( poCLStack->hSipStack, poCallLeg->hSipCnx, &poCLDnsData->hSipMsg, poCallLeg->oLocalRxCnx, &oSipRemoteConnObj );
		if( 0 != nSipRetVal )
		{
			MS_TRACE( poCLStack->nTraceId, AMPS_TRACE_LEVEL_ERROR, "SIP_SendRequest returned with error-code = %d\n", nSipRetVal );
		}
		else
		{
			// SIP message succussfully sent
			poCallLeg->eState = poCLDnsData->eStatetoSet;
			if( poCLDnsData->bRequestCompleted )
			{
				poCallLeg->oCallLegCb.pfRequestCompleted( poCLStack, poCallLeg, poCallLeg->hAppCtx, poCLDnsData->hSipMsg );
			}
		}
	}
	else
	{
		MS_TRACE( poCLStack->nTraceId, AMPS_TRACE_LEVEL_ERROR, "DNS Query Failed.\n" );
	}
	FW_Free( poCLDnsData );

	MS_TRACE( poCLStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving\n" );
}

/*! @fn e_MsRetCode cl_queue_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, e_CallLegEvent eEvent_i, h_CallLegEventData hEventData_i )
*
* @brief This function stores the incoming requests in a queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] eEvent_i is the call leg event.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_queue_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, e_CallLegEvent eEvent_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( CLE_Max > eEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_CallLegEventData* poEvData = NULL;

		poEvData = AMPS_Malloc( sizeof (t_CallLegEventData));
		if( NULL != poEvData )
		{
			poEvData->eEvent = eEvent_i;
			poEvData->hEvData = hEventData_i;

			AMPS_DListAppend( poCallLeg_i->poPendingEventList, poEvData );
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_process_pending_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i )
*
* @brief This function processes the pending requests in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_process_pending_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		if( poCallLeg_i->eState == CLS_Start && poCallLeg_i->poPendingEventList->uchCount > 0 )
		{
			t_AMPSSList* poListNode = NULL;
			t_CallLegEventData* poEvData = NULL;

			poListNode = poCallLeg_i->poPendingEventList->poAMPSSListHead;

			ms_assert( NULL != poListNode->pvData );

			poEvData = poListNode->pvData;

			eRetCode = (ppf_CallLegSm[poCallLeg_i->eState][poEvData->eEvent])( poClStack_i, poCallLeg_i, (h_CallLegEventData)poEvData->hEvData );

			AMPS_DListRemove( &poCallLeg_i->poPendingEventList, poListNode, NULL );
		}
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_send_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, SIP_MSG_HANDLE hSipMsg_i, e_CallLegState  eNextState_i )
*
* @brief This function sends a sip request.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hSipMsg_i is the handle of sip request to be sent.
*
* @param[in] eNextState_i is the state for the call leg after sending the request.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_send_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, SIP_MSG_HANDLE hSipMsg_i, e_CallLegState  eNextState_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_SSEventDNSRequest oSSEventDNSRequest;
		t_SIPRouteSetEntry oSIPRouteSetEntry;
		t_SIPTag oSIPTag;
		t_CallLegDnsData* poCLDnsData = NULL;
		int nRouteHdrCount = 0;
		t_SIPConnObject oSipRemoteConnObj;
		int nSipRetVal;

		memset( &oSIPRouteSetEntry, 0, sizeof (t_SIPRouteSetEntry));
		nRouteHdrCount = SIP_MsgListCount( poClStack_i->hSipStack, hSipMsg_i, SIP_TAG_ROUTE );
		if( 0 == nRouteHdrCount ) // no route header present
		{
			memset( &oSIPTag, 0, sizeof (t_SIPTag));
			oSIPTag.oSIPTagType = SIP_TAG_RL_REQUEST_URI;
			if( AMPS_SUCCESS != SIP_GetTagFromMsg( poClStack_i->hSipStack, hSipMsg_i, &oSIPTag ))
			{
				MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "SIP_GetTagFromMsg Failed.\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			else
			{
				SU_CopyUriToRouteSet( &oSIPRouteSetEntry, &oSIPTag.u_SIPTagValue.oRURI );
			}
		}
		else // route header present
		{
			memset( &oSIPTag, 0, sizeof (t_SIPTag));
			oSIPTag.oSIPTagType = SIP_TAG_ROUTE;
			MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "NextHop Found to be in ROUTE Header \n" );
			if( AMPS_SUCCESS != SIP_GetListTagFromMsg( poClStack_i->hSipStack, hSipMsg_i, &oSIPTag, 0 ))
			{
				MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "SIP_GetListTagFromMsg failed.\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			else
			{
				SU_CopyUriToRouteSet( &oSIPRouteSetEntry, &oSIPTag.u_SIPTagValue.oSIPRoute.oAddr );
			}
		}
		if( MS_ERR_NONE == eRetCode )
		{
			if( AMPS_TRUE == SIP_IsIPNumeric( oSIPRouteSetEntry.puchAddress, strlen((char*)oSIPRouteSetEntry.puchAddress )))
			{
				if( AMPS_SUCCESS != SIP_ResolveURI( poClStack_i->hSipStack, poClStack_i->nTraceId, &oSIPRouteSetEntry ))
				{
					MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_DEBUG, "SIP_ResolveURI Failed.\n" );
					return (AMPS_ERROR_FAILURE);
				}
				else
				{
					memset( &oSipRemoteConnObj, 0, sizeof (t_SIPConnObject));
					oSipRemoteConnObj = oSIPRouteSetEntry.oSIPAddress.oSIPConnObject[0];
					//poCallLeg_i->poLocalCnxnTransport->nTptType = oSipTransport.nTptType;

					nSipRetVal = SIP_SendRequest( poClStack_i->hSipStack, poCallLeg_i->hSipCnx, &hSipMsg_i,  poCallLeg_i->oLocalRxCnx, &oSipRemoteConnObj );
					if( 0 != nSipRetVal )
					{
						MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "SIP_SendRequest returned with error-code = %d\n", nSipRetVal );
					}
					else
					{
						// SIP message succussfully sent
						poCallLeg_i->eState = eNextState_i;
					}
				}
			}
			else
			{
				poCLDnsData = AMPS_Malloc( sizeof (t_CallLegDnsData));
				if( NULL == poCLDnsData )
				{
					eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
				}
				else
				{
					poCLDnsData->poCLStack = poClStack_i;
					poCLDnsData->poCallLeg = poCallLeg_i;
					poCLDnsData->hSipMsg = hSipMsg_i;
					poCLDnsData->eStatetoSet = eNextState_i;
					poCLDnsData->bRequestCompleted = 0;

					memset( &oSSEventDNSRequest, 0, sizeof (t_SSEventDNSRequest));
					memset( &oSIPRouteSetEntry, 0, sizeof (t_SIPRouteSetEntry));
					oSSEventDNSRequest.pvAppData = poCLDnsData;
					oSSEventDNSRequest.oSIPRouteSetEntry = oSIPRouteSetEntry;
					if( AMPS_SUCCESS != AMPS_EvtSysSendIntEvt( poClStack_i->pvFrameworkCtx, SS_EVENT_3263_LOOKUP_REQ, &oSSEventDNSRequest, sizeof (t_SSEventDNSRequest)))
					{
						MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "AMPS_EvtSysSendIntEvt Failed.\n" );
						eRetCode = MS_ERR_FRAMEWORK;
					}
				}
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

