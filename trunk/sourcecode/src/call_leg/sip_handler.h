/*! @file sip_handler.h
*
* @ingroup CallLeg
*
* @brief The file contains defines for sip handler in call leg API.
*
*/

#ifndef _SIP_HANDLER_H_
#define _SIP_HANDLER_H_

#include "fw_api.h"

#include "ms_debug.h"

#include "sdp_api.h"
#include "call_leg.h"

e_MsRetCode cl_send_response( t_CallLegStack* poClStack_i, SIP_CONN_HANDLE hSipCnx_i, SIP_MSG_HANDLE hSipMsg_i, e_SIPResStatusCodes eSipStatusCode_i, t_SIPConnObject* poLocalRxConn_i );

#endif /* _SIP_HANDLER_H_ */

