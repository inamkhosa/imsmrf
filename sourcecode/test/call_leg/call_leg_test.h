#ifndef _CALL_LEG_TEST_H_
#define _CALL_LEG_TEST_H_

#include "call_leg/call_leg_api.h"

extern int nTraceId_g;

e_MsRetCode Test_NewCallLeg( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode Test_RequestCompleted( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE  hSipMsg_i );

//e_MsRetCode Test_RequestProceeding		(h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i);
e_MsRetCode Test_ProvisionalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode Test_FinalRespRecvd( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode Test_IncomingInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode Test_IncomingNonInvite( h_CallLegStack hClStack_i, h_CallLeg hCallLeg_i, h_CallLegApp hAppCtx_i, SIP_MSG_HANDLE hSipMsg_i );

int Test_TimeOutNotifyCallBack( AMPS_HANDLE hAMPS_HANDLE_i, void* pvData_i );

// int set_request_line_method(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcMethod_i);
// int set_request_uri(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcUser_i, char* pcHost_i, unsigned int unPort_i);
// int set_from_uri(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcUser_i, char* pcHost_i, unsigned int unPort_i);
// int set_to_uri(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcUser_i, char* pcHost_i, unsigned int unPort_i);
// int set_contact_uri(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcUser_i, char* pcHost_i, unsigned int unPort_i);

// int set_sdp_media_line(SIP_HANDLE hSipStack_i, SIP_SDP_MSG_HANDLE hSdpMsg_i, char* pchMediaType_i, char* pchPort_i, char* pchProto_i, char* pchEnc_i, char* pchAttr_i);

#endif /* _CALL_LEG_TEST_H_ */

