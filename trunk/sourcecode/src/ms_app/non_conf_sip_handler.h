#ifndef _NON_CONF_SIP_HANDLER_H_
#define _NON_CONF_SIP_HANDLER_H_

#include "call_leg/call_leg_api.h"

e_MsRetCode MS_NonConfRequestCompleted( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE  hSipMsg_i );

e_MsRetCode MS_NonConfProvisionalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode MS_NonConfFinalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode MS_NonConfIncomingInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode MS_NonConfIncomingNonInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

#endif /* _NON_CONF_SIP_HANDLER_H_ */

