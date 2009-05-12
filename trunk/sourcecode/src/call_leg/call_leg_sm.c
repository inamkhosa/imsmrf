/*! @file call_leg_sm.c
*
* @ingroup CallLeg
*
* @brief The file contains implementation of functions for call leg state machine.
*
*/

#include "call_leg_sm.h"

f_CallLegSm ppf_CallLegSm[CLS_Max][CLE_Max] = {
	{
		Start__Send_invite_request,
		Start__Send_non_invite_request,
		Start__Send_ack,
		Start__Send_provisional_response,
		Start__Send_final_response,
		Start__Incoming_request,
		Start__Incoming_response
	},
	{
		Waiting_for_invite_response__Send_invite_request,
		Waiting_for_invite_response__Send_non_invite_request,
		Waiting_for_invite_response__Send_ack,
		Waiting_for_invite_response__Send_provisional_response,
		Waiting_for_invite_response__Send_final_response,
		Waiting_for_invite_response__Incoming_request,
		Waiting_for_invite_response__Incoming_response
	},
	{
		Waiting_for_app_ack__Send_invite_request,
		Waiting_for_app_ack__Send_non_invite_request,
		Waiting_for_app_ack__Send_ack,
		Waiting_for_app_ack__Send_provisional_response,
		Waiting_for_app_ack__Send_final_response,
		Waiting_for_app_ack__Incoming_request,
		Waiting_for_app_ack__Incoming_response
	},
	{
		Waiting_for_non_invite_response__Send_invite_request,
		Waiting_for_non_invite_response__Send_non_invite_request,
		Waiting_for_non_invite_response__Send_ack,
		Waiting_for_non_invite_response__Send_provisional_response,
		Waiting_for_non_invite_response__Send_final_response,
		Waiting_for_non_invite_response__Incoming_request,
		Waiting_for_non_invite_response__Incoming_response
	},
	{
		Waiting_for_invite_app_response__Send_invite_request,
		Waiting_for_invite_app_response__Send_non_invite_request,
		Waiting_for_invite_app_response__Send_ack,
		Waiting_for_invite_app_response__Send_provisional_response,
		Waiting_for_invite_app_response__Send_final_response,
		Waiting_for_invite_app_response__Incoming_request,
		Waiting_for_invite_app_response__Incoming_response
	},
	{
		Waiting_for_non_invite_app_response__Send_invite_request,
		Waiting_for_non_invite_app_response__Send_non_invite_request,
		Waiting_for_non_invite_app_response__Send_ack,
		Waiting_for_non_invite_app_response__Send_provisional_response,
		Waiting_for_non_invite_app_response__Send_final_response,
		Waiting_for_non_invite_app_response__Incoming_request,
		Waiting_for_non_invite_app_response__Incoming_response
	},
	{
		Waiting_for_ack__Send_invite_request,
		Waiting_for_ack__Send_non_invite_request,
		Waiting_for_ack__Send_ack,
		Waiting_for_ack__Send_provisional_response,
		Waiting_for_ack__Send_final_response,
		Waiting_for_ack__Incoming_request,
		Waiting_for_ack__Incoming_response
	}
};

/*! @fn e_MsRetCode Start__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function starts sending an invite request.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Start__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_send_request( poClStack_i, poCallLeg_i, (SIP_MSG_HANDLE)hEventData_i, CLS_Waiting_for_invite_response );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Start__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function starts sending a non-invite request.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Start__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_send_request( poClStack_i, poCallLeg_i, (SIP_MSG_HANDLE)hEventData_i, CLS_Waiting_for_non_invite_response );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Start__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function starts sending acknowledgement.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Start__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Start__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function starts sending a provisional response.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Start__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Start__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function starts sending a final response.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Start__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Start__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function starts receiving a sip request.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Start__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SIP_MSG_HANDLE hSipMsg = (SIP_MSG_HANDLE) hEventData_i;
		t_SIPTag oSipTag;
		oSipTag.oSIPTagType = SIP_TAG_RL_METHOD;
		// Check SIP status code
		if( 0 == SIP_GetTagFromMsg( poClStack_i->hSipStack, hSipMsg, &oSipTag ) )
		{
			char* pcMethod = (char*)oSipTag.u_SIPTagValue.oSIPStringRLMethod.poString;
			unsigned int unLength = oSipTag.u_SIPTagValue.oSIPStringRLMethod.unLength;

			unLength = (unLength < sizeof ("INVITE") ) ? unLength : sizeof ("INVITE");
			if( 0 == strncasecmp( pcMethod, "INVITE", unLength ) )
			{
				MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "INVITE request received\n" );

				ms_assert_return( NULL != poCallLeg_i->oCallLegCb.pfIncomingInvite, eRetCode = MS_ERR_INTERNAL );

				poCallLeg_i->eState = CLS_Waiting_for_invite_app_response;

				//request is an INVITE request
				eRetCode = (poCallLeg_i->oCallLegCb.pfIncomingInvite)( poClStack_i, (h_CallLeg)poCallLeg_i, poCallLeg_i->hAppCtx, hSipMsg );
			}
			else
			{
				MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Non-INVITE request received\n" );

				ms_assert_return( NULL != poCallLeg_i->oCallLegCb.pfIncomingNonInvite, eRetCode = MS_ERR_INTERNAL );

				poCallLeg_i->eState = CLS_Waiting_for_non_invite_app_response;

				//request is a Non-INVITE reqeust
				eRetCode = (poCallLeg_i->oCallLegCb.pfIncomingNonInvite)( poClStack_i, (h_CallLeg)poCallLeg_i, poCallLeg_i->hAppCtx, hSipMsg );
			}
		}
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Start__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function starts receiving a sip response.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Start__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores an invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_invite_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores a non-invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_non_invite_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores acknowledgement event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores provisional response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores final response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores incoming request event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SIP_MSG_HANDLE hSipMsg = (SIP_MSG_HANDLE) hEventData_i;
		t_SIPTag oSipTag;
		oSipTag.oSIPTagType = SIP_TAG_RL_METHOD;
		// Check SIP status code
		if( 0 == SIP_GetTagFromMsg( poClStack_i->hSipStack, hSipMsg, &oSipTag ) )
		{
			char* pcMethod = (char*)oSipTag.u_SIPTagValue.oSIPStringRLMethod.poString;
			unsigned int unLength = oSipTag.u_SIPTagValue.oSIPStringRLMethod.unLength;

			unLength = (unLength < sizeof ("CANCEL") ) ? unLength : sizeof ("CANCEL");
			if( 0 == strncasecmp( pcMethod, "CANCEL", unLength ) )
			{
				MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Non-INVITE(CANCEL)request received\n" );

				ms_assert_return( NULL != poCallLeg_i->oCallLegCb.pfIncomingNonInvite, eRetCode = MS_ERR_INTERNAL );

				//request is a Non-INVITE reqeust
				eRetCode = (poCallLeg_i->oCallLegCb.pfIncomingNonInvite)( poClStack_i, (h_CallLeg)poCallLeg_i, poCallLeg_i->hAppCtx, hSipMsg );
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores incoming response event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SIP_MSG_HANDLE hSipMsg = (SIP_MSG_HANDLE) hEventData_i;
		t_SIPTag oSipTag;
		unsigned int unStatusCode = 0;

		oSipTag.oSIPTagType = SIP_TAG_SL_STATUS_CODE;
		// Check SIP status code
		if( 0 == SIP_GetTagFromMsg( poClStack_i->hSipStack, hSipMsg, &oSipTag ) )
		{
			unStatusCode = oSipTag.u_SIPTagValue.unStatusCode;

			MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Status code = %d\n", unStatusCode );
			if( 100 <= unStatusCode && unStatusCode < 200 )
			{
				//ms_assert_return( NULL != poCallLeg_i->oCallLegCb.pfRequestProceeding, eRetCode = MS_ERR_INTERNAL);
				ms_assert_return( NULL != poCallLeg_i->oCallLegCb.pfProvisionalRespRecvd, eRetCode = MS_ERR_INTERNAL );

				//eRetCode = (poCallLeg_i->oCallLegCb.pfRequestProceeding)(poClStack_i,(h_CallLeg)poCallLeg_i, poCallLeg_i->hAppCtx, hSipMsg);
				eRetCode = (poCallLeg_i->oCallLegCb.pfProvisionalRespRecvd)( poClStack_i, (h_CallLeg)poCallLeg_i, poCallLeg_i->hAppCtx, hSipMsg );
			}
			else if( 200 <= unStatusCode && unStatusCode < 700 )
			{
				ms_assert_return( NULL != poCallLeg_i->oCallLegCb.pfFinalRespRecvd, eRetCode = MS_ERR_INTERNAL );

				// change state
				poCallLeg_i->eState = CLS_Waiting_for_app_ack;

				eRetCode = (poCallLeg_i->oCallLegCb.pfFinalRespRecvd)( poClStack_i, (h_CallLeg)poCallLeg_i, poCallLeg_i->hAppCtx, hSipMsg );
			}
		}
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_app_ack__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_app_ack__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_invite_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_app_ack__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores non-invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_app_ack__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_non_invite_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_app_ack__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores acknowledgement event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_app_ack__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_send_request( poClStack_i, poCallLeg_i, (SIP_MSG_HANDLE)hEventData_i, CLS_Start );
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = cl_process_pending_request( poClStack_i, poCallLeg_i );
		}
	}
	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_app_ack__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores provisional response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_app_ack__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_app_ack__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores final response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_app_ack__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_app_ack__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores incoming request event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_app_ack__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Incoming_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_app_ack__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores incoming response event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_app_ack__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_invite_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores non-invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_non_invite_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores acknowledgement event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores provisional response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores final response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores incoming request event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Incoming_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores incoming response event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SIP_MSG_HANDLE hSipMsg = (SIP_MSG_HANDLE) hEventData_i;

		ms_assert_return( NULL != poCallLeg_i->oCallLegCb.pfRequestCompleted, eRetCode = MS_ERR_INTERNAL );

		// change state
		poCallLeg_i->eState = CLS_Start;

		eRetCode = (poCallLeg_i->oCallLegCb.pfRequestCompleted)( poClStack_i, (h_CallLeg)poCallLeg_i, poCallLeg_i->hAppCtx, hSipMsg );
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = cl_process_pending_request( poClStack_i, poCallLeg_i );
		}
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_app_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_app_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_invite_request, hEventData_i );
	}

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	POST_CONDITION

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_app_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores non-invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_app_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_non_invite_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_app_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores acknowledgement event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_app_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_app_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores provisional response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_app_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nSipRetVal = 0;
		SIP_MSG_HANDLE hSipMsg = (SIP_MSG_HANDLE) hEventData_i;
		t_SIPConnObject oSipRemoteConnObj;

		memset( &oSipRemoteConnObj, 0, sizeof (t_SIPConnObject));

		MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Sip Connection handle = %p\n", poCallLeg_i->hSipCnx );

		MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Local Connection = %s:%d:%d\n", poCallLeg_i->oLocalRxCnx.pchIPAddr, poCallLeg_i->oLocalRxCnx.nPort, poCallLeg_i->oLocalRxCnx.nTptType );

		MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Remote Connection = %s:%d:%d\n", oSipRemoteConnObj.pchIPAddr, oSipRemoteConnObj.nPort, oSipRemoteConnObj.nTptType );

		nSipRetVal = SIP_SendResponse( poClStack_i->hSipStack, poCallLeg_i->hSipCnx, &hSipMsg, poCallLeg_i->oLocalRxCnx, &oSipRemoteConnObj );
		if( 0 != nSipRetVal )
		{
			MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "SIP_SendResponse returned with error-code = %d\n", nSipRetVal );

			eRetCode = MS_ERR_SIP_STACK;
		}
		else
		{
			// SIP message succussfully sent
			// remain in the same state
		}
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_app_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores final response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_app_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nSipRetVal = 0;
		SIP_MSG_HANDLE hSipMsg = (SIP_MSG_HANDLE) hEventData_i;
		t_SIPConnObject oSipRemoteConnObj;

		memset( &oSipRemoteConnObj, 0, sizeof (t_SIPConnObject));

		MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Sip Connection handle = %p\n", poCallLeg_i->hSipCnx );

		SIP_GetSIPTxConnObjectForResponse( poClStack_i->hSipStack, hSipMsg, &oSipRemoteConnObj );

		MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Local Connection = %s:%d:%d\n", poCallLeg_i->oLocalRxCnx.pchIPAddr, poCallLeg_i->oLocalRxCnx.nPort, poCallLeg_i->oLocalRxCnx.nTptType );

		MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Remote Connection = %s:%d:%d\n", oSipRemoteConnObj.pchIPAddr, oSipRemoteConnObj.nPort, oSipRemoteConnObj.nTptType );

		nSipRetVal = SIP_SendResponse( poClStack_i->hSipStack, poCallLeg_i->hSipCnx, &hSipMsg, poCallLeg_i->oLocalRxCnx, &oSipRemoteConnObj );
		if( 0 != nSipRetVal )
		{
			MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "SIP_SendResponse returned with error-code = %d\n", nSipRetVal );

			eRetCode = MS_ERR_SIP_STACK;
		}
		else
		{
			// SIP message succussfully sent

			poCallLeg_i->eState = CLS_Waiting_for_ack;
		}
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_app_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores incoming request event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_app_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Incoming_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_invite_app_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the invite response and stores incoming response event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_invite_app_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_app_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_app_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_invite_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_app_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores non-invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_app_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_non_invite_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_app_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores acknowledgement event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_app_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_app_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores provisional response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_app_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_app_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores final response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_app_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nSipRetVal = 0;
		SIP_MSG_HANDLE hSipMsg = (SIP_MSG_HANDLE) hEventData_i;
		t_SIPConnObject oSipRemoteConnObj;

		memset( &oSipRemoteConnObj, 0, sizeof (t_SIPConnObject));

		MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Sip Connection handle = %p\n", poCallLeg_i->hSipCnx );

		SIP_GetSIPTxConnObjectForResponse( poClStack_i->hSipStack, hSipMsg, &oSipRemoteConnObj );

		MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Local Connection = %s:%d:%d\n", poCallLeg_i->oLocalRxCnx.pchIPAddr, poCallLeg_i->oLocalRxCnx.nPort, poCallLeg_i->oLocalRxCnx.nTptType );

		MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Remote Connection = %s:%d:%d\n", oSipRemoteConnObj.pchIPAddr, oSipRemoteConnObj.nPort, oSipRemoteConnObj.nTptType );

		nSipRetVal = SIP_SendResponse( poClStack_i->hSipStack, poCallLeg_i->hSipCnx, &hSipMsg, poCallLeg_i->oLocalRxCnx, &oSipRemoteConnObj );
		if( 0 != nSipRetVal )
		{
			MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "SIP_SendResponse returned with error-code = %d\n", nSipRetVal );

			eRetCode = MS_ERR_SIP_STACK;
		}
		else
		{
			// SIP message succussfully sent

			ms_assert_return( NULL != poCallLeg_i->oCallLegCb.pfRequestCompleted, eRetCode = MS_ERR_INTERNAL );

			poCallLeg_i->eState = CLS_Start;

			eRetCode = (poCallLeg_i->oCallLegCb.pfRequestCompleted)( poClStack_i, (h_CallLeg)poCallLeg_i, poCallLeg_i->hAppCtx, hSipMsg );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = cl_process_pending_request( poClStack_i, poCallLeg_i );
			}
		}
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_app_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores incoming request event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_app_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Incoming_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_non_invite_app_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the non-invite response and stores incoming response event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_non_invite_app_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_ack__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_ack__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_invite_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_ack__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores non-invite request event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_ack__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = cl_queue_request( poClStack_i, poCallLeg_i, CLE_Send_non_invite_request, hEventData_i );
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_ack__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores acknowledgement event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_ack__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_ack__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores provisional response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_ack__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_ack__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores final response event in the queue for sending.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_ack__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_ack__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores incoming request event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_ack__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SIP_MSG_HANDLE hSipMsg = (SIP_MSG_HANDLE) hEventData_i;
		t_SIPTag oSipTag;
		oSipTag.oSIPTagType = SIP_TAG_RL_METHOD;
		// Check SIP status code
		if( 0 == SIP_GetTagFromMsg( poClStack_i->hSipStack, hSipMsg, &oSipTag ) )
		{
			char* pcMethod = (char*)oSipTag.u_SIPTagValue.oSIPStringRLMethod.poString;
			unsigned int unLength = oSipTag.u_SIPTagValue.oSIPStringRLMethod.unLength;

			unLength = (unLength < sizeof ("ACK") ) ? unLength : sizeof ("ACK");
			if( 0 == strncasecmp( pcMethod, "ACK", unLength ) )
			{
				MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "ACK Received.\n" );

				ms_assert_return( NULL != poCallLeg_i->oCallLegCb.pfRequestCompleted, eRetCode = MS_ERR_INTERNAL );

				poCallLeg_i->eState = CLS_Start;

				//request is an ACK request
				eRetCode = (poCallLeg_i->oCallLegCb.pfRequestCompleted)( poClStack_i, (h_CallLeg)poCallLeg_i, poCallLeg_i->hAppCtx, hSipMsg );
				if( MS_ERR_NONE == eRetCode )
				{
					eRetCode = cl_process_pending_request( poClStack_i, poCallLeg_i );
				}
			}
			else
			{
				//request is a Non-INVITE reqeust
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Waiting_for_ack__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
*
* @brief This function waits for the acknowledgement and stores incoming response event in the queue.
*
* @param[in] poClStack_i is the handle of call leg stack.
*
* @param[in] poCallLeg_i is the handle of call leg.
*
* @param[in] hEventData_i is the handle of data for the call leg event.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Waiting_for_ack__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCallLeg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hEventData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	POST_CONDITION

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

