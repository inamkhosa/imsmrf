/*! @file sip_handler.c
*
* @ingroup CallLeg
*
* @brief The file contains implementation of functions for sip handler in call leg API.
*
* These functions either are called by or call the functions of sip stack.
*
*/

#include "sip_handler.h"

/**************
FUNCTION NAME		: cl_handle_sip_request
DESCRIPTION		: Function called by sip stack when a request is received
FUNCTION PARAMETERS
hSipCtx_i	: Context of SIP Stack
hSipCnx_i	: Pointer to SIP Connection
hSipReq_i	: Received SIP message
RETURN TYPE
void        :
**************/
/*! @fn void cl_handle_sip_request( SIP_APP_HANDLE hSipCtx_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipReq_i, t_SIPConnObject oLocalRxConn_i, t_SIPConnObject oRemoteRxConn_i )
*
* @brief This function is called by sip stack when a request is received.
*
* @param[in] hSipCtx_i is the context handle of sip stack.
*
* @param[in] hSipCnx_i is the handle of sip connection.
*
* @param[in] hSipReq_i is the handle of received sip request.
*
* @param[in] oLocalRxConn_i is the local connection object.
*
* @param[in] oRemoteRxConn_i is the remote connectioh object.
*
*/

void cl_handle_sip_request( SIP_APP_HANDLE hSipCtx_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipReq_i, t_SIPConnObject oLocalRxConn_i, t_SIPConnObject oRemoteRxConn_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLegStack* poClStack = (t_CallLegStack*) hSipCtx_i;
	h_CallLeg hCallLeg = NULL;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
	}

	{
		int nRetVal = 0;

		MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Sip Connection handle = %p\n", hSipCnx_i );

		nRetVal = SIP_GetAppConnHandle( poClStack->hSipStack, hSipCnx_i, &hCallLeg );
		if( 0 == nRetVal )

		{
			if( NULL != hCallLeg )

			{
				MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Call Leg exists for Sip Connection handle = %p\n", hCallLeg );
				eRetCode = cl_incoming_request((h_CallLegStack)poClStack, hCallLeg, hSipReq_i );
			}
			else
			{
				t_SIPTag oSipTag;
				oSipTag.oSIPTagType = SIP_TAG_RL_METHOD;

				MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Call Leg does not exists for Sip Connection handle = %p\n", hCallLeg );
				// Check SIP status code
				if( 0 == SIP_GetTagFromMsg( poClStack->hSipStack, hSipReq_i, &oSipTag ) )
				{
					char* pcMethod = (char*)oSipTag.u_SIPTagValue.oSIPStringRLMethod.poString;
					unsigned int unLength = oSipTag.u_SIPTagValue.oSIPStringRLMethod.unLength;

					unLength = (unLength < sizeof ("INVITE") ) ? unLength : sizeof ("INVITE");
					if( 0 == strncasecmp( pcMethod, "INVITE", unLength ) )
					{
						t_CallLegCreateConfig oCallLegCreateConfig;

						oCallLegCreateConfig.hSipCnx = hSipCnx_i;

						eRetCode = cl_create( poClStack, &hCallLeg, &oCallLegCreateConfig );

						MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "cl_create return code value=%s\n", get_ms_ret_code_string( eRetCode ));
						if( MS_ERR_NONE == eRetCode )
						{
							MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling pfnNewCallLegCb = %p\n", poClStack->pfnNewCallLegCb );

							ms_assert( NULL != poClStack->pfnNewCallLegCb );

							eRetCode = (poClStack->pfnNewCallLegCb)( poClStack, hCallLeg, poClStack->hAppCtx, hSipReq_i, &oLocalRxConn_i, &oRemoteRxConn_i );
							if( MS_ERR_NONE == eRetCode )
							{
								eRetCode = cl_incoming_request( poClStack, hCallLeg, hSipReq_i );
							}
							else
							{
								eRetCode = cl_send_response( poClStack, hSipCnx_i, hSipReq_i, SIP_RES_NOT_FOUND, &oLocalRxConn_i );
							}
						}
					}
				}
			}
		}
		SIP_DeAllocateMsg( poClStack->hSipStack, hSipReq_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving. Return code = %s\n", get_ms_ret_code_string( eRetCode ) );
}

/**************
FUNCTION NAME		: cl_handle_sip_response
DESCRIPTION		: Function called by sip stack when a response is received
FUNCTION PARAMETERS
hSipCtx_i	: Context of SIP Stack
hSipCnx_i	: Pointer to SIP Connection
hSipResp_i	: Received SIP message
RETURN TYPE
void        :
**************/
/*! @fn void cl_handle_sip_response( SIP_APP_HANDLE hSipCtx_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipResp_i, t_SIPConnObject oSipLocalRxConn_i, t_SIPConnObject oSipRemoteRxConn_i )
*
* @brief This function is called by sip stack when a response is received.
*
* @param[in] hSipCtx_i is the context handle of sip stack.
*
* @param[in] hSipCnx_i is the handle of sip connection.
*
* @param[in] hSipResp_i is the handle of received sip response.
*
* @param[in] oSipLocalRxConn_i is the local connection object.
*
* @param[in] oSipRemoteRxConn_i is the remote connectioh object.
*
*/

void cl_handle_sip_response( SIP_APP_HANDLE hSipCtx_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipResp_i, t_SIPConnObject oSipLocalRxConn_i, t_SIPConnObject oSipRemoteRxConn_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLegStack* poClStack = (t_CallLegStack*) hSipCtx_i;
	h_CallLeg hCallLeg = NULL;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
	}

	{
		int nRetVal = 0;
		nRetVal = SIP_GetAppConnHandle( poClStack->hSipStack, hSipCnx_i, &hCallLeg );
		if( 0 == nRetVal )
		{
			if( NULL != hCallLeg )
			{
				eRetCode = cl_incoming_response((h_CallLegStack)poClStack, hCallLeg, hSipResp_i );
			}
		}
		SIP_DeAllocateMsg( poClStack->hSipStack, hSipResp_i );
	}

	//POST_CONDITION
	{
	}

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving. Return code = %s\n", get_ms_ret_code_string( eRetCode ) );
}

/**************
FUNCTION NAME		: cl_handle_sip_error
DESCRIPTION		: Function called by sip stack when a response is received
FUNCTION PARAMETERS
hSipCtx_i	: Context of SIP Stack
hSipCnx_i	: Pointer to SIP Connection
hSipMsg_i	: Received SIP message
eSipErrCode_i	: ErrorCode
RETURN TYPE
void        :
**************/
/*! @fn void cl_handle_sip_error( SIP_APP_HANDLE hSipCtx_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipMsg_i, e_SIPErrCode eSipErrCode_i )
*
* @brief This function is called by sip stack when an error message is received.
*
* @param[in] hSipCtx_i is the context handle of sip stack.
*
* @param[in] hSipCnx_i is the handle of sip connection.
*
* @param[in] hSipMsg_i is the handle of received sip message.
*
* @param[in] eSipErrCode_i is the sip error code.
*
*/

void cl_handle_sip_error( SIP_APP_HANDLE hSipCtx_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipMsg_i, e_SIPErrCode eSipErrCode_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_CallLegStack* poClStack = (t_CallLegStack*) hSipCtx_i;

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering: error code = %d\n", eSipErrCode_i );

	//PRE_CONDITION
	{
	}

	{
	}

	//POST_CONDITION
	{
	}

	MS_TRACE( poClStack->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving. Return code = %s\n", get_ms_ret_code_string( eRetCode ) );
}

/*! @fn e_MsRetCode cl_send_response( t_CallLegStack* poClStack_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipMsg_i, e_SIPResStatusCodes eSipStatusCode_i, t_SIPConnObject* poLocalRxConn_i )
*
* @brief This function is calls the functions of sip stack for sending a response.
*
* @param[in] poClStack_i is the pointer to the call leg stack.
*
* @param[in] hSipCnx_i is the handle of sip connection.
*
* @param[in] hSipMsg_i is the handle of sip message to be sent.
*
* @param[in] eSipStatusCode_i is the sip status code for the response.
*
* @param[in] poLocalRxConn_i is the local connection object.
*
*/

e_MsRetCode cl_send_response( t_CallLegStack* poClStack_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipMsg_i, e_SIPResStatusCodes eSipStatusCode_i, t_SIPConnObject* poLocalRxConn_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poClStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poLocalRxConn_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SIP_MSG_HANDLE hSipResp = NULL;
		SIP_CONN_HANDLE hSipCnx;        // dummy connection
		t_SipAppData oSipData;

		memset( &oSipData, 0, sizeof (t_SipAppData));

		hSipResp = SIP_CreateResponse( poClStack_i->hSipStack, hSipCnx, hSipMsg_i, eSipStatusCode_i );
		if( NULL != hSipResp )
		{
			int nRetVal = 0;
			t_SIPConnObject oSipRemoteConnObj;

			memset( &oSipRemoteConnObj, 0, sizeof (t_SIPConnObject));

			SIP_GetSIPTxConnObjectForResponse( poClStack_i->hSipStack, hSipResp, &oSipRemoteConnObj );

			nRetVal = SIP_SendResponse( poClStack_i->hSipStack, hSipCnx_i, &hSipResp, *poLocalRxConn_i, &oSipRemoteConnObj );
			if( 0 != nRetVal )
			{
				MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "SIP_SendResponse returned with error-code = %d\n", nRetVal );

				eRetCode = MS_ERR_SIP_STACK;
			}
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

	MS_TRACE( poClStack_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

