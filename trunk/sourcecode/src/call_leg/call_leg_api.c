/*! @file call_leg_api.c
*
* @ingroup CallLeg
*
* @brief The file contains implementation of functions for call_leg API.
*
*/

#include "call_leg.h"
#include "call_leg_sm.h"

/*! @fn e_MsRetCode cl_initstack( t_CallLegStackConfig* poCallLegStackConfig_i, h_CallLegStack* phClStack_o )
*
* @brief This function inializes the call leg stack.
*
* @param[in] poCallLegStackConfig_i is the configuration for call leg stack.
*
* @param[out] phClStack_o is the pointer to the handle of initialized call leg stack.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_initstack( t_CallLegStackConfig* poCallLegStackConfig_i, h_CallLegStack* phClStack_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLegStack* poClStack = NULL;

	MS_TRACE( poCallLegStackConfig_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != poCallLegStackConfig_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phClStack_o, eRetCode = MS_ERR_PRE_COND_FAILED );
		//ms_assert_return( NULL != poCallLegStackConfig_i->hSipStack, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLegStackConfig_i->pfnNewCallLegCb, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		poClStack = malloc( sizeof (t_CallLegStack));
		if( NULL != poClStack )
		{
			poClStack->hSipStack        = poCallLegStackConfig_i->hSipStack;
			poClStack->nTraceId         = poCallLegStackConfig_i->nTraceId;
			poClStack->pfnNewCallLegCb  = poCallLegStackConfig_i->pfnNewCallLegCb;
			poClStack->hAppCtx  = poCallLegStackConfig_i->hAppCtx;
			poClStack->pvFrameworkCtx = poCallLegStackConfig_i->pvFrameworkCtx;

			*phClStack_o = (h_CallLegStack) poClStack;
		}
		else
		{
			MS_TRACE( poCallLegStackConfig_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_CallLegStack\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCallLegStackConfig_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_create( h_CallLegStack hClStack_i, h_CallLeg* phCallLeg_o, t_CallLegCreateConfig* poCallLegCreateConfig_i )
*
* @brief This function creates a call leg.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[out] phCallLeg_o is the pointer to the handle of created call leg.
*
* @param[in] poCallLegCreateConfig_i is the configuration required for creating a call leg.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_create( h_CallLegStack hClStack_i, h_CallLeg* phCallLeg_o, t_CallLegCreateConfig* poCallLegCreateConfig_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLeg* poCallLeg = NULL;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLegCreateConfig_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phCallLeg_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		poCallLeg = (t_CallLeg*)AMPS_Malloc( sizeof (t_CallLeg));
		if( NULL == poCallLeg )
		{
			MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_CallLeg\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
		else
		{
			//initialize
			poCallLeg->bInitialized = AMPS_FALSE;
			if( SIP_IsConnHandleValid( poClStack->hSipStack, poCallLegCreateConfig_i->hSipCnx ) )
			{
				AMPS_DListInit( &poCallLeg->poPendingEventList );
				poCallLeg->hSipCnx  = poCallLegCreateConfig_i->hSipCnx;
				SIP_SetAppConnHandle( poClStack->hSipStack, poCallLeg->hSipCnx, (SIP_APP_CONN_HANDLE)poCallLeg );
			}
			else
			{
				MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Creating sip connection\n" );

				// create SIP connection
				poCallLeg->hSipCnx = cl_create_sip_handle( poClStack, poCallLeg );
				if( !SIP_IsConnHandleValid( poClStack->hSipStack, poCallLeg->hSipCnx ) )
				{
					MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_ERROR, "SIP_CONN_HANDLE Createion failed\n" );
					eRetCode = MS_ERR_SIP_STACK;

					// free the allocated memory for call leg
					free( poCallLeg );
					poCallLeg = NULL;
				}
			}
			*phCallLeg_o =  poCallLeg;
		}
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert_return( *phCallLeg_o != NULL, eRetCode = MS_ERR_POST_COND_FAILED );
			ms_assert_return( SIP_IsConnHandleValid( poClStack->hSipStack, poCallLeg->hSipCnx ), eRetCode = MS_ERR_POST_COND_FAILED );
		}
	}
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_initialize( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, t_CallLegInitConfig* poCallLegInitConfig_i )
*
* @brief This function initializes a call leg.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hCallLeg_i is the handle of call leg.
*
* @param[in] poCallLegInitConfig_i is the configuration required for initializing a call leg.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_initialize( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, t_CallLegInitConfig* poCallLegInitConfig_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLeg* poCallLeg = (t_CallLeg*)hCallLeg_i;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( AMPS_FALSE == poCallLeg->bInitialized, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLegInitConfig_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLegInitConfig_i->oCallLegCb.pfRequestCompleted, eRetCode = MS_ERR_PRE_COND_FAILED );
		//ms_assert_return( NULL != poCallLegInitConfig_i->oCallLegCb.pfRequestProceeding , eRetCode = MS_ERR_PRE_COND_FAILED);
		ms_assert_return( NULL != poCallLegInitConfig_i->oCallLegCb.pfProvisionalRespRecvd, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLegInitConfig_i->oCallLegCb.pfFinalRespRecvd, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLegInitConfig_i->oCallLegCb.pfIncomingInvite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLegInitConfig_i->oCallLegCb.pfIncomingNonInvite, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		poCallLeg->eState   = CLS_Start;
		poCallLeg->oCallLegCb   = poCallLegInitConfig_i->oCallLegCb;
		poCallLeg->hAppCtx  = poCallLegInitConfig_i->hAppCtx;
		poCallLeg->bInitialized = AMPS_TRUE;
		poCallLeg->oLocalRxCnx = poCallLegInitConfig_i->oLocalRxCnx;
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_send_invite_request( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
*
* @brief This function sends an invite request.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hCallLeg_i is the handle of call leg.
*
* @param[in] hSipMsg_i is the handle of sip invite message to be sent.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_send_invite_request( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLeg* poCallLeg = (t_CallLeg*)hCallLeg_i;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( AMPS_TRUE == poCallLeg->bInitialized, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = (ppf_CallLegSm[poCallLeg->eState][CLE_Send_invite_request])( poClStack, poCallLeg, (h_CallLegEventData)hSipMsg_i );
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_send_non_invite_request( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
*
* @brief This function sends a non-invite request.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hCallLeg_i is the handle of call leg.
*
* @param[in] hSipMsg_i is the handle of sip non-invite request to be sent.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_send_non_invite_request( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLeg* poCallLeg = (t_CallLeg*)hCallLeg_i;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( AMPS_TRUE == poCallLeg->bInitialized, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = (ppf_CallLegSm[poCallLeg->eState][CLE_Send_non_invite_request])( poClStack, poCallLeg, (h_CallLegEventData)hSipMsg_i );
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_send_ack( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
*
* @brief This function sends acknowledgement.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hCallLeg_i is the handle of call leg.
*
* @param[in] hSipMsg_i is the handle of sip acknowledgement message to be sent.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_send_ack( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLeg* poCallLeg = (t_CallLeg*)hCallLeg_i;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( AMPS_TRUE == poCallLeg->bInitialized, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = (ppf_CallLegSm[poCallLeg->eState][CLE_Send_ack])( poClStack, poCallLeg, (h_CallLegEventData)hSipMsg_i );
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_send_provisional_response( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
*
* @brief This function sends a provisional response.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hCallLeg_i is the handle of call leg.
*
* @param[in] hSipMsg_i is the handle of sip provisional response to be sent.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_send_provisional_response( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLeg* poCallLeg = (t_CallLeg*)hCallLeg_i;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( AMPS_TRUE == poCallLeg->bInitialized, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = (ppf_CallLegSm[poCallLeg->eState][CLE_Send_provisional_response])( poClStack, poCallLeg, (h_CallLegEventData)hSipMsg_i );
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_send_final_response( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
*
* @brief This function sends a final response.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hCallLeg_i is the handle of call leg.
*
* @param[in] hSipMsg_i is the handle of sip final response to be sent.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_send_final_response( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLeg* poCallLeg = (t_CallLeg*)hCallLeg_i;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( AMPS_TRUE == poCallLeg->bInitialized, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
		eRetCode = (ppf_CallLegSm[poCallLeg->eState][CLE_Send_final_response])( poClStack, poCallLeg, (h_CallLegEventData)hSipMsg_i );
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_close( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i )
*
* @brief This function closes the call leg.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hCallLeg_i is the handle of call leg.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_close( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLeg* poCallLeg = (t_CallLeg*)hCallLeg_i;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( AMPS_TRUE == poCallLeg->bInitialized, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		poCallLeg->bInitialized                         = AMPS_FALSE;
		poCallLeg->oCallLegCb.pfRequestCompleted        = NULL;
		//poCallLeg->oCallLegCb.pfRequestProceeding     = NULL;
		poCallLeg->oCallLegCb.pfProvisionalRespRecvd    = NULL;
		poCallLeg->oCallLegCb.pfFinalRespRecvd          = NULL;
		poCallLeg->oCallLegCb.pfIncomingInvite          = NULL;
		poCallLeg->oCallLegCb.pfIncomingNonInvite       = NULL;
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_destroy( h_CallLegStack hClStack_i, h_CallLeg* phCallLeg_io )
*
* @brief This function destroys a call leg.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in,out] phCallLeg_io is the pointer to the handle of call leg.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_destroy( h_CallLegStack hClStack_i, h_CallLeg* phCallLeg_io )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLeg* poCallLeg = NULL;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		t_CallLeg* poCallLegPreCond = NULL;

		ms_assert_return( NULL != phCallLeg_io, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != *phCallLeg_io, eRetCode = MS_ERR_PRE_COND_FAILED );
		poCallLegPreCond = (t_CallLeg*)(*phCallLeg_io);
		ms_assert_return( AMPS_FALSE == poCallLegPreCond->bInitialized, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		poCallLeg = (t_CallLeg*)(*phCallLeg_io);

		SIP_DestroyConnHandle( poClStack->hSipStack, poCallLeg->hSipCnx );

		t_AMPSSList* poNode = NULL;

		for( poNode = poCallLeg->poPendingEventList->poAMPSSListHead; poNode != NULL; poNode = poNode->poAMPSSListNext )
		{
			t_CallLegEventData* poEvData = poNode->pvData;

			SIP_DeAllocateMsg( poClStack->hSipStack, poEvData->hEvData );

			FW_Free( poEvData );
		}
		FW_DListFree( &poCallLeg->poPendingEventList, NULL );

		FW_Free( poCallLeg );
		*phCallLeg_io = NULL;
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_incoming_request( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
*
* @brief This function handles an incoming request.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hCallLeg_i is the handle of call leg.
*
* @param[in] hSipMsg_i is the handle of incoming request.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_incoming_request( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLeg* poCallLeg = (t_CallLeg*)hCallLeg_i;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( AMPS_TRUE == poCallLeg->bInitialized, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = (ppf_CallLegSm[poCallLeg->eState][CLE_Incoming_request])( poClStack, poCallLeg, (h_CallLegEventData)hSipMsg_i );
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_incoming_response( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
*
* @brief This function handles an incoming response.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hCallLeg_i is the handle of call leg.
*
* @param[in] hSipMsg_i is the handle of incoming response.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_incoming_response( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLeg* poCallLeg = (t_CallLeg*)hCallLeg_i;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( AMPS_TRUE == poCallLeg->bInitialized, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = (ppf_CallLegSm[poCallLeg->eState][CLE_Incoming_response])( poClStack, poCallLeg, (h_CallLegEventData)hSipMsg_i );
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_closestack( h_CallLegStack* phClStack_io )
*
* @brief This function closes a call leg stack.
*
* @param[in,out] phClStack_io is the pointer to the handle of call leg stack to be closed.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_closestack( h_CallLegStack* phClStack_io )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLegStack* poClStack = (t_CallLegStack*)(*phClStack_io);

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != phClStack_io, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != *phClStack_io, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		free( poClStack );
		*phClStack_io = NULL;
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_get_sip_handle( h_CallLegStack hClStack_i, SIP_HANDLE* phSipStack_o )
*
* @brief This function gets the sip stack handle for a call leg stack.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[out] phSipStack_o is the pointer to the obtained handle of sip stack.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_get_sip_handle( h_CallLegStack hClStack_i, SIP_HANDLE* phSipStack_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phSipStack_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		*phSipStack_o = poClStack->hSipStack;
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_set_sip_handle( h_CallLegStack hClStack_i, SIP_HANDLE hSipStack_i )
*
* @brief This function sets the sip stack handle for a call leg stack.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hSipStack_i is the handle of sip stack.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_set_sip_handle( h_CallLegStack hClStack_i, SIP_HANDLE hSipStack_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		poClStack->hSipStack = hSipStack_i;
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_set_app_handle( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i )
*
* @brief This function sets the application handle for a call leg stack.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hCallLeg_i is the handle of call leg.
*
* @param[in] hAppCtx_i is the handle of application context.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_set_app_handle( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;
	t_CallLeg* poCallLeg = (t_CallLeg*)hCallLeg_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		poCallLeg->hAppCtx = hAppCtx_i;
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_get_app_handle( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp* phAppCtx_o )
*
* @brief This function gets the application handle for a call leg.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[in] hCallLeg_i is the handle of call leg.
*
* @param[out] phAppCtx_o is the pointer to the obtained handle of application context.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_get_app_handle( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp* phAppCtx_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;
	t_CallLeg* poCallLeg = (t_CallLeg*)hCallLeg_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phAppCtx_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		*phAppCtx_o = poCallLeg->hAppCtx;
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode cl_get_stack_app_handle( h_CallLegStack hClStack_i, h_CallLegApp* phAppCtx_o )
*
* @brief This function gets the application handle for a call leg stack.
*
* @param[in] hClStack_i is the handle of call leg stack.
*
* @param[out] phAppCtx_o is the pointer to the obtained handle of application context.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode cl_get_stack_app_handle( h_CallLegStack hClStack_i, h_CallLegApp* phAppCtx_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLegStack* poClStack = (t_CallLegStack*)hClStack_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//pre-condition:
	{
		ms_assert_return( NULL != hClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phAppCtx_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		*phAppCtx_o = poClStack->hAppCtx;
	}

	POST_CONDITION
	/* - */

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

