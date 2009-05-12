/*! @file call_leg_api.h
*
* @ingroup CallLeg
*
* @brief The file contains defines for call_leg API.
*
*/

/*! @addtogroup CallLeg
*
* @{
*
*/

#ifndef _CALL_LEG_API_H_
#define _CALL_LEG_API_H_

#include "call_leg/call_leg_define.h"

/*! Prototype for function which is called when a request is completed by the call leg
*
* @param[in] hClStack_i is the handle for call leg stack. It is initialized with @ref t_CallLegStack structure through the function @ref cl_initstack
*
* @param[in] hCallLeg_i is the handle for call leg. It is initialized with @ref t_CallLeg structure through the function @ref cl_initialize
*
* @param[in] hAppCtx_i is the handle for call leg application context. It is provided in @ref t_CallLegInitConfig structure during the call to the function @ref cl_initialize
*
* @param[in] hSipMsg_i is the handle for the sip message received
*
*/
typedef e_MsRetCode (*f_RequestCompleted)( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE  hSipMsg_i );
//typedef e_MsRetCode	(*f_RequestProceeding)		(h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i);
/*! Prototype for function which is called when a provisional response is received to the call leg
*
* @param[in] hClStack_i is the handle for call leg stack. It is initialized with @ref t_CallLegStack structure through the function @ref cl_initstack
*
* @param[in] hCallLeg_i is the handle for call leg. It is initialized with @ref t_CallLeg structure through the function @ref cl_initialize
*
* @param[in] hAppCtx_i is the handle for call leg application context. It is provided in @ref t_CallLegInitConfig structure during the call to the function @ref cl_initialize
*
* @param[in] hSipMsg_i is the handle for the sip message received
*
*/
typedef e_MsRetCode (*f_ProvisionalRespRecvd)( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );
/*! Prototype for function which is called when a final response is received to the call leg
*
* @param[in] hClStack_i is the handle for call leg stack. It is initialized with @ref t_CallLegStack structure through the function @ref cl_initstack
*
* @param[in] hCallLeg_i is the handle for call leg. It is initialized with @ref t_CallLeg structure through the function @ref cl_initialize
*
* @param[in] hAppCtx_i is the handle for call leg application context. It is provided in @ref t_CallLegInitConfig structure during the call to the function @ref cl_initialize
*
* @param[in] hSipMsg_i is the handle for the sip message received
*
*/
typedef e_MsRetCode (*f_FinalRespRecvd)( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );
/*! Prototype for function which is called when an invite request is received to the call leg
*
* @param[in] hClStack_i is the handle for call leg stack. It is initialized with @ref t_CallLegStack structure through the function @ref cl_initstack
*
* @param[in] hCallLeg_i is the handle for call leg. It is initialized with @ref t_CallLeg structure through the function @ref cl_initialize
*
* @param[in] hAppCtx_i is the handle for call leg application context. It is provided in @ref t_CallLegInitConfig structure during the call to the function @ref cl_initialize
*
* @param[in] hSipMsg_i is the handle for the sip message received
*
*/
typedef e_MsRetCode (*f_IncomingInvite)( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );
/*! Prototype for function which is called when a non-invite request is received to the call leg
*
* @param[in] hClStack_i is the handle for call leg stack. It is initialized with @ref t_CallLegStack structure through the function @ref cl_initstack
*
* @param[in] hCallLeg_i is the handle for call leg. It is initialized with @ref t_CallLeg structure through the function @ref cl_initialize
*
* @param[in] hAppCtx_i is the handle for call leg application context. It is provided in @ref t_CallLegInitConfig structure during the call to the function @ref cl_initialize
*
* @param[in] hSipMsg_i is the handle for the sip message received
*
*/
typedef e_MsRetCode (*f_IncomingNonInvite)( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

/*! Prototype for function which is called when a new call leg is created
*
* @param[in] hClStack_i is the handle for call leg stack. It is initialized with @ref t_CallLegStack structure through the function @ref cl_initstack
*
* @param[in] hCallLeg_i is the handle for call leg. It is initialized with @ref t_CallLeg structure through the function @ref cl_initialize
*
* @param[in] hAppCtx_i is the handle for call leg application context. It is provided in @ref t_CallLegInitConfig structure during the call to the function @ref cl_initialize
*
* @param[in] hSipMsg_i is the handle for the sip message received
*
* @param[in] poLocalCnx_i is the handle of the local connection
*
* @param[in] poRemoteCnx_i is the handle of the remote connection
*
*/
typedef e_MsRetCode (*f_NewCallLeg)( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i, t_SIPConnObject* poLocalCnx_i, t_SIPConnObject* poRemoteCnx_i );

// typedef enum
// {
//  CLS_READY,
//  CLS_IN_TRASACTION,
// }e_ClState;

/*! @struct t_CallLegCb
*
* The details about callback functions for call leg
*
*/

typedef struct _t_CallLegCb
{
	f_RequestCompleted pfRequestCompleted;  /*!< Pointer to the function when a request is completed by the call leg */
	//f_RequestProceeding	pfRequestProceeding;
	f_ProvisionalRespRecvd pfProvisionalRespRecvd;  /*!< Pointer to the function when a provisional response is received to tha call leg */
	f_FinalRespRecvd       pfFinalRespRecvd;    /*!< Pointer to the function when a final response is received to the call leg */
	f_IncomingInvite       pfIncomingInvite;    /*!< Pointer to the function when an invite request is received to the call leg */
	f_IncomingNonInvite    pfIncomingNonInvite; /*!< Pointer to the function when a non-invite request is received to the call leg */
} //! Struct variable.
/*! Call leg callback functions structure */ t_CallLegCb;

/*! @struct t_CallLegStackConfig
*
* The details about stack configuration for call leg
*
*/

typedef struct _t_CallLegStackConfig
{
	int          nTraceId;  /*!< Trace id for traces related to call leg stack */
	SIP_HANDLE   hSipStack; /*!< Handle of sip stack */
	h_CallLegApp hAppCtx;   /*!< Handle of application context */
	f_NewCallLeg pfnNewCallLegCb;   /*!< Pointer to the function when a new call leg is created */
	void*        pvFrameworkCtx;    /*!< Framework context */
} //! Struct variable.
/*! Call leg stack configuration structure */ t_CallLegStackConfig;

/*! @struct t_CallLegCreateConfig
*
* The details about configuration for creating a call leg
*
*/

typedef struct _t_CallLegCreateConfig
{
	SIP_CONN_HANDLE hSipCnx;    /*!< Handle of sip connection. Optional: NULL : create a new one, else use this one */
} //! Struct variable.
/*! Configuration structure for call leg creation */ t_CallLegCreateConfig;

/*! @struct t_CallLegInitConfig
*
* The details about initializating a call leg
*
*/

typedef struct _t_CallLegInitConfig
{
	h_CallLegApp    hAppCtx;    /*!< Handle of application context */
	t_CallLegCb     oCallLegCb; /*!< Objext containing call back functions initialization for the call leg */
	t_SIPConnObject oLocalRxCnx;    /*!< Handle of sip connection with local object */
} //! Struct variable.
/*! Configuration structure for call leg initialization */ t_CallLegInitConfig;

e_MsRetCode     cl_initstack( t_CallLegStackConfig* poCallLegStackConfig_i, h_CallLegStack* phClStack_o );

e_MsRetCode cl_create( h_CallLegStack hClStack_i, h_CallLeg* phCallLeg_o, t_CallLegCreateConfig* poCallLegCreateConfig_i );

e_MsRetCode cl_destroy( h_CallLegStack hClStack_i, h_CallLeg* phCallLeg_io );

e_MsRetCode cl_initialize( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, t_CallLegInitConfig* poCallLegInitConfig_i );

e_MsRetCode cl_close( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i );

e_MsRetCode cl_send_invite_request( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode cl_send_non_invite_request( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode cl_send_ack( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode cl_send_provisional_response( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode cl_send_final_response( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode cl_incoming_request( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode cl_incoming_response( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode cl_closestack( h_CallLegStack* phClStack_io );

e_MsRetCode cl_set_sip_handle( h_CallLegStack hClStack_i, SIP_HANDLE hSipStack_i );

e_MsRetCode cl_get_sip_handle( h_CallLegStack hClStack_i, SIP_HANDLE* phSipStack_o );

e_MsRetCode cl_set_sip_handle( h_CallLegStack hClStack_i, SIP_HANDLE phSipStack_i );

e_MsRetCode cl_get_app_handle( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp* phAppCtx_o );

e_MsRetCode cl_set_app_handle( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i );

e_MsRetCode cl_get_stack_app_handle( h_CallLegStack hClStack_i, h_CallLegApp* phAppCtx_o );

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
void cl_handle_sip_request( SIP_APP_HANDLE hSipCtx_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipReq_i, t_SIPConnObject oSipLocalRxConn_i, t_SIPConnObject oSipRemoteRxConn_i );

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
void cl_handle_sip_response( SIP_APP_HANDLE hSipCtx_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipResp_i, t_SIPConnObject oSipLocalRxConn_i, t_SIPConnObject oSipRemoteRxConn_i );

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
void cl_handle_sip_error( SIP_APP_HANDLE hSipCtx_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipMsg_i, e_SIPErrCode eSipErrCode_i );

void cl_HandleDNSLookupResponse( AMPS_HANDLE r_hAMPSHandle, void* r_pvEventData, void* r_pvEventHandle, void* r_pvEventPayload );

// e_MsRetCode cl_get_current_state( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, e_ClState* eState_o );

#endif /* _CALL_LEG_API_H_ */

/*! @} */ // End of CallLeg Module

