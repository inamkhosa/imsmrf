#ifndef _MS_UTIL_H_
#define _MS_UTIL_H_

#include "ms_main.h"
#include "sdp_api.h"
#include "sipstack/SIP_API.h"
#include "call_leg_handler.h"
#include "ms_media.h"
#include "ms_team.h"
#include "ms_sm_handler.h"
#include "ms_media_handler.h"

#include "util/sip_util.h"

e_MsRetCode MS_GetConfCtx( t_MsCtx* poMsCtx_i, h_CallLeg hCallLeg_i, SIP_MSG_HANDLE hSipMsg_i, t_MsConfCtx** ppoMsConfCtx_o );

e_MsRetCode MS_GetConfIdFromMsg( t_MsCtx* poMsCtx_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poConfId_i );

e_MsRetCode MS_GetMscmlFromMsg( t_MsCtx* poMsCtx_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poMscml_i );

e_MsRetCode MS_GetSdpFromMsg( t_MsCtx* poMsCtx_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poMscml_i );

e_MsRetCode MS_GetAvailableUserCtrlCtx( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx** ppoMsUserCtrlCtx_o );

e_MsRetCode MS_NegotiateSdp( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, SIP_MSG_HANDLE hSipMsg_i, h_Sdp* phNegotiatedSdp_o, t_AMPSDList* poNegInfoList_o );

e_MsRetCode MS_SendResponse( t_MsCtx* poMsCtx_i, h_CallLeg hCallLeg_i, t_SipAppData* poSipData_i, SIP_MSG_HANDLE hSipMsg_i, e_SIPResStatusCodes eSipStatusCode_i, t_String* poContentType_i, t_String* poContentBody_i );

e_MsRetCode MS_SendInfoRequest( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i, t_String* poContentType_i, t_String* poContentBody_i );

e_MsRetCode MS_SendByeRequest( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i, t_String* poContentType_i, t_String* poContentBody_i );

e_MsRetCode MS_GetUserCtrlCtx( t_MsCtx* poMsCtx_i, t_AMPSDList* poUserCtxList_i, h_CallLeg hCallLeg_i, t_MsUserCtrlCtx** ppoUserCtrlCtx_o );

e_MsRetCode MS_DestroyUserCtrlCtx( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i );

e_MsRetCode MS_DestroyConfCtrlCtx( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i );

e_MsRetCode MS_DestroyConfCtx( t_MsCtx* poMsCtx_i, t_MsConfCtx* poConfCtx_i );

// e_MsRetCode MS_GetCodecName( t_MsCtx* poMsCtx_i, h_Sdp hSdp_i, int nDescIndex_i, t_String* poCodecName_i );

e_MsRetCode MS_GetRemotePort( t_MsCtx* poMsCtx_i, h_Sdp hRemoteSdp_i, h_Sdp hNegotiatedSdp_i, int* pnRemotePort_o );

e_MsRetCode MS_GetUserCtxFromId( t_MsCtx* poMsCtx_i, t_MsConfCtx* poConfCtrlCtx_i, t_String* poTeamMateId_i, t_MsUserCtrlCtx** ppoUserCtrlCtx_o );

//int NegMediaInfo_RemoveLinkDataCb( void** ppvNegMediaInfo_io );

e_MsRetCode MS_GetTelEventPayloadType( t_MsCtx* poMsCtx_i, t_AMPSDList* poNegInfoList_i, h_Sdp hNegotiatedSdp_i, unsigned char* pucTelephonyPayloadType_o );

e_MsRetCode MS_CreateUserCtrlCtx( t_MsCtx* poMsCtx_i, AMPS_BOOL bReserveMedia_i, t_MsUserCtrlCtx** ppoMsUserCtrlCtx_o );

e_MsRetCode MS_CreateConfCtrlCtx( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx** ppoMsConfCtrlCtx_o );

#endif /* _MS_UTIL_H_ */

