#ifndef _SIP_UTIL_H_
#define _SIP_UTIL_H_

#include "config.h"
#include "tstring.h"
#include "sipstack/SIP_API.h"
#include "ms_ret_code.h"
#include "ms_debug.h"

typedef struct _t_Uri
{
	t_String     oUser;
	t_String     oHost;
	unsigned int unPort;
} t_Uri;

typedef struct _t_UriInfo
{
	t_Uri    oUri;
	t_String oTag;
} t_UriInfo;

typedef struct _t_SipAppData
{
	t_Uri     oRemoteContact;
	t_UriInfo oRemoteUriInfo;
	t_String  oLocalTag;
} t_SipAppData;

e_MsRetCode SU_AddContentInMsg( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poContentType_i, t_String* poContentBody_i );

e_MsRetCode SU_SetRequestLineMethod( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poMethod_i );

e_MsRetCode SU_SetRequestUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poUser_i, t_String* poHost_i, unsigned int unPort_i );

e_MsRetCode SU_SetFromUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poUser_i, t_String* poHost_i, unsigned int unPort_i, t_String* poTag_i );

e_MsRetCode SU_SetToUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poUser_i, t_String* poHost_i, unsigned int unPort_i, t_String* poTag_i );

e_MsRetCode SU_SetContactUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poUser_i, t_String* poHost_i, unsigned int unPort_i );

e_MsRetCode SU_GetContactUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_Uri* poRemoteContact_i );

e_MsRetCode SU_GetFromUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_UriInfo* poUriInfo_i );

void SU_CopyUriToRouteSet( t_SIPRouteSetEntry* poSIPRouteSetEntry_i, t_SIPURL* poSIPUri_i );

#endif /* _SIP_UTIL_H_ */

