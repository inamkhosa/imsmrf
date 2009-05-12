#ifndef _CALL_LEG_HANDLER_H_
#define _CALL_LEG_HANDLER_H_

#include "ms_mm.h"
#include "ms.h"

e_MsRetCode MS_NewCallLeg( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i, t_SIPConnObject* poLocalRxConn_i, t_SIPConnObject* poRemoteRxConn_i );

e_MsRetCode MS_ConfRequestCompleted( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE  hSipMsg_i );

e_MsRetCode MS_ConfProvisionalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode MS_ConfFinalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode MS_ConfIncomingInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode MS_ConfIncomingNonInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

#endif /* _CALL_LEG_HANDLER_H_ */

