/******************************************************************************
** Copyright (c) 2008-2009
**
*******************************************************************************
** FILE NAME      : SIP_API.h
** DESCRIPTION    : Consists of all the APIs related to SIP Stack.
** PROJECT NAME   : SIP Stack
** TYPE           : ANSI C
** CONTRIBUTOR(S) : Zaka Kiani
*******************************************************************************
** $Id: SIP_API.h,v 1.69 2009/02/12 15:12:30 arif Exp $
** $Source: /work/cvsroot/sipstack/SIP_API.h,v $
*******************************************************************************
*******************************************************************************
*/

#ifndef __SIP_API_H__
#define __SIP_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "SIP_Defines.h"

typedef void(*SIP_MsgHandlerCallback)(SIP_APP_HANDLE r_hSIP_APP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPConnObject r_oSIPLocalRxConn, t_SIPConnObject r_oSIPRemoteRxConn);
typedef void(*SIP_MsgErrHandlerCallback)(SIP_HANDLE r_hSIP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, e_SIPErrCode r_oSIPErrCode);
typedef void*(*SIP_AppMallocCallback)(SIP_APP_MM_HANDLE r_hSIP_APP_MM_HANDLE, unsigned int r_unSize);
typedef void(*SIP_AppFreeCallback)(SIP_APP_MM_HANDLE r_hSIP_APP_MM_HANDLE, void* r_pvBuffer);

typedef void(*SIP_SendReqHook)(SIP_HANDLE r_hSIP_HANDLE, void* r_pvSendReqHookInstallerData, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPConnObject r_oSIPRxConnObject, t_SIPConnObject* r_poSIPTxConnObject);
typedef void(*SIP_SendResHook)(SIP_HANDLE r_hSIP_HANDLE, void* r_pvSendResHookInstallerData, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPConnObject r_oSIPRxConnObject, t_SIPConnObject* r_poSIPTxConnObject);

void SIP_SetPublicIPAddressAndPort(SIP_HANDLE r_hSIP_HANDLE, int r_nPort, char* r_chIPAddress);
void SIP_GetTunnelingInterfaceHandle(SIP_HANDLE r_hSIP_HANDLE, AMPS_NET_HANDLE* r_phAMPSNetHandle);
int SIP_IntToString(unsigned char* r_puchBuffer, int r_nValue, int r_nBase);

int SIP_GetSIPTxConnObjectForResponse(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPConnObject* r_poSIPConnObject);

//Internals
SIP_HANDLE SIP_Init(AMPS_HANDLE r_hAMPSHandle, SIP_APP_HANDLE r_hSIP_APP_HANDLE, t_SIPStackConfiguration* r_poSIPStackConfiguration, e_SIPStackMode r_oSIPStackMode);
void SIP_Cleanup(AMPS_HANDLE r_hAMPSHandle, SIP_HANDLE r_hSIP_HANDLE);

int SIP_OpenConnection(SIP_HANDLE r_hSIP_HANDLE, t_SIPConnObject r_oSIPConnObject);
int SIP_OpenSecConnection(SIP_HANDLE r_hSIP_HANDLE, t_SIPConnObject r_oSIPConnObject, char* r_pchTunnelingKey);

int SIP_SetDomain(SIP_HANDLE r_hSIP_HANDLE, char* r_pchDomainName, int r_nPort);
//SIP Utility APIs
void SIP_InstallSendReqHook(SIP_HANDLE r_hSIP_HANDLE, SIP_SendReqHook r_pfSIP_SendReqHook, void* r_pvSendReqHookInstallerData);
void SIP_InstallSendResHook(SIP_HANDLE r_hSIP_HANDLE, SIP_SendResHook r_pfSIP_SendResHook, void* r_pvSendResHookInstallerData);

void SIP_SetAppConnHandle(SIP_HANDLE r_hSIP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_APP_CONN_HANDLE r_hSIP_APP_CONN_HANDLE);
int SIP_GetAppConnHandle(SIP_HANDLE r_hSIP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_APP_CONN_HANDLE* r_phSIP_APP_CONN_HANDLE);
int SIP_GetConnHandleByCallId(SIP_HANDLE r_hSIP_HANDLE, char* r_pchCallId, int r_nCallIdLength, SIP_CONN_HANDLE* r_hSIP_CONN_HANDLE);
void SIP_DestroyConnHandle(SIP_HANDLE r_hSIP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE);

int SIP_CloneMsgHandle(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSrcSIP_MSG_HANDLE, SIP_MSG_HANDLE* r_phSIP_MSG_HANDLE);
int SIP_SetBranchInMsgHandle(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, char* r_pchBranch, int r_nBranchLength);
void SIP_SetTimerCallbackInMessage(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, SIP_MsgErrHandlerCallback r_pfSIP_MsgErrHandlerCallback);
void SIP_SetAppHandleInMessage(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, SIP_APP_HANDLE r_hSIP_APP_HANDLE);

int SIP_GenerateRandString(SIP_HANDLE r_hSIP_HANDLE, char* r_pchRandString, int r_nRandStringLength);
void SIP_GetRemoteRxConnObject(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPConnObject* r_poSIPConnObject);
void SIP_GetLocalRxConnObject(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPConnObject* r_poSIPConnObject);

//SIP APIs
void SIP_RegMsgHandlerCallback(SIP_HANDLE r_hSIP_HANDLE, e_SIPMsgType r_oSIPMsgType, SIP_MsgHandlerCallback r_pfSIP_MsgHandlerCallback);
void SIP_RegErrorCallback(SIP_HANDLE r_hSIP_HANDLE, SIP_APP_HANDLE r_hSIP_APP_HANDLE, e_SIPErrCode r_oSIPErrCode, SIP_MsgErrHandlerCallback r_pfSIP_MsgErrHandlerCallback);
void SIP_RegMemoryCallback(SIP_HANDLE r_hSIP_HANDLE, SIP_APP_MM_HANDLE r_hSIP_APP_MM_HANDLE, SIP_AppMallocCallback r_pfSIP_AppMallocCallback, SIP_AppFreeCallback r_pfSIP_AppFreeCallback);
int SIP_CreateConnectionHandle(SIP_HANDLE r_hSIP_HANDLE, unsigned char* r_puchCallID, SIP_CONN_HANDLE* r_phSIPConnHandle);
SIP_MSG_HANDLE SIP_AllocateMsg(SIP_HANDLE r_hSIP_HANDLE, e_SIPMsgType r_eSIPMsgType);
void SIP_DeAllocateMsg(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE);

int SIP_SendRequestWithTimerCallback(SIP_HANDLE r_hSIP_HANDLE, SIP_APP_HANDLE r_hSIP_APP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_MSG_HANDLE* r_phSIP_MSG_HANDLE, t_SIPConnObject r_oSIPRxConnObject, t_SIPConnObject* r_poSIPTxConnObject, SIP_MsgErrHandlerCallback r_pfSIP_MsgErrHandlerCallback);
int SIP_SendRequest(SIP_HANDLE r_hSIP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_MSG_HANDLE* r_phSIP_MSG_HANDLE, t_SIPConnObject r_oSIPLocalConnObject, t_SIPConnObject* r_poSIPTransport);
int SIP_SendResponseWithTimerCallback(SIP_HANDLE r_hSIP_HANDLE, SIP_APP_HANDLE r_hSIP_APP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_MSG_HANDLE* r_phSIP_MSG_HANDLE, t_SIPConnObject r_oSIPRxConnObject, t_SIPConnObject* r_poSIPTxConnObject, SIP_MsgErrHandlerCallback r_pfSIP_MsgErrHandlerCallback);
int SIP_SendResponse(SIP_HANDLE r_hSIP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_MSG_HANDLE* r_phSIP_MSG_HANDLE, t_SIPConnObject r_oSIPLocalConnObject, t_SIPConnObject* r_poSIPTransport);

SIP_MSG_HANDLE SIP_CreateResponse(SIP_HANDLE r_hSIP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, e_SIPResStatusCodes r_oSIPResStatusCodes);

void SIP_SetAppHandle(SIP_HANDLE r_hSIP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE, SIP_APP_HANDLE r_hSIP_APP_HANDLE);
SIP_APP_HANDLE SIP_GetAppHandle(SIP_HANDLE r_hSIP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE);

int SIP_SetTagToMsg(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPTag* r_poSIPTag);
int SIP_GetTagFromMsg(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPTag* r_poSIPTag);
int SIP_RemoveTagFromMsg(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPTag* r_poSIPTag);

int SIP_SetListTagToMsg(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPTag* r_poSIPTag, int r_nNodeLocation);
int SIP_GetListTagFromMsg(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPTag* r_poSIPTag, int r_nNodeLocation);
int SIP_AddListTagToMsg(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPTag* r_poSIPTag, int r_nNodeLocation);
int SIP_RemoveListTagFromMsg(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, e_SIPTagType r_oSIPTagType, int r_nNodeLocation);

int SIP_SetParamToMsgTag(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, e_SIPTagType r_oSIPTagType, t_SIPString r_oParamNameString, t_SIPString r_oParamValueString);
int SIP_GetParamFromMsgTag(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, e_SIPTagType r_oSIPTagType, t_SIPString* r_poParamNameString, t_SIPString* r_poParamValueString);

SIP_TAG_VALUE SIP_FindTag(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, e_SIPTagType r_oSIPTagType, unsigned char* r_puchKey);
unsigned int SIP_MsgListCount(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, e_SIPTagType r_oSIPTagType);


int SIP_SetOpaqueContent(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPString* r_poContentString);
/////////////////////////////////
int SIP_GetMyDomain(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPDomainInfo* r_poSIPDomainInfo);
AMPS_BOOL SIP_IsMyDomain(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE);
AMPS_BOOL SIP_IsMyDomainInURI(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, void* r_poSIPURL);
int SIP_CopyURI(SIP_HANDLE r_hSIP_HANDLE, t_SIPURL* r_poSIPDestinationURL, t_SIPURL* r_poSIPSourceURL);
void SIP_FreeURI(SIP_HANDLE r_hSIP_HANDLE, t_SIPURL* r_poSIPURL);
int SIP_SetURI(SIP_HANDLE r_hSIP_HANDLE,  t_SIPURL* r_poSIPURL, t_SIPString* r_poSchemeString, t_SIPString* r_poUserString, t_SIPString* r_poHostString, unsigned int r_unPort);
int SIP_URIToString(SIP_HANDLE r_hSIP_HANDLE, t_SIPURL* r_poSIPURL, t_SIPString* r_poURLString);
int SIP_StringToURI(SIP_HANDLE r_hSIP_HANDLE, t_SIPString r_oURLString, t_SIPURL* r_poSIPURL);

int SIP_SetExpiresParam(SIP_HANDLE r_hSIP_HANDLE, t_SIPContact* r_poSIPContact, int r_nExpires);

AMPS_BOOL SIP_IsConnHandleValid(SIP_HANDLE r_hSIP_HANDLE, SIP_CONN_HANDLE r_hSIP_CONN_HANDLE);
AMPS_BOOL SIP_IsMsgHandleValid(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE);

///////// Set/Get SIP Message Payload
int SIP_SetSIPMsgPayload(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, SIP_MSG_PAYLOAD_HANDLE r_hSIP_MSG_PAYLOAD_HANDLE);
int SIP_GetSIPMsgPayload(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, SIP_MSG_PAYLOAD_HANDLE r_hSIP_MSG_PAYLOAD_HANDLE);

AMPS_BOOL SIP_IsIPNumeric(unsigned char* r_puchStr, int r_unLength);
int SIP_ResolveURI(SIP_HANDLE r_hSIP_HANDLE, int r_nTraceId, void* r_pvSIPRouteSetEntry);
int SIP_CopyString(t_SIPString* r_poDstString, t_SIPString* r_poSrcString);
AMPS_BOOL SIP_CompareString(t_SIPString* r_poSIPString1, t_SIPString* r_poSIPString2);

//Authentication related APIs
e_SIPAuthStatusdCode SIP_VerifyAuthorizationHeader(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPAuthorization* r_poSIPAuthorization, unsigned char* r_puchNonce);
int SIP_GenerateAuthenticationHeader(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE, t_SIPAuthenticate* r_poSIPAuthanticate, unsigned char* r_puchNonce, unsigned char r_uchStale, unsigned char* r_puchDomain, unsigned char* r_puchAlgorithm, unsigned char* r_puchOpaque);

e_SIPMsgType SIP_GetMsgSubType(SIP_HANDLE r_hSIP_HANDLE, SIP_MSG_HANDLE r_hSIP_MSG_HANDLE);

#ifdef __cplusplus
}
#endif

#endif /*__SIP_API_H__ */

