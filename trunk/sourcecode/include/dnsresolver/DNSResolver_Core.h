/******************************************************************************
** Copyright (c) 2006-2007
*******************************************************************************
** FILE NAME        : DNSResolver_Core.h
** DESCRIPTION      : The file contains dns resolver routines
** PROJECT NAME     : Softswitch
** TYPE             : ANSI C
** CONTRIBUTOR(S)   : arif
*******************************************************************************
** $Id: DNSResolver_Core.h,v 1.8 2008/08/19 07:25:04 zaka Exp $
** $Source: /work/cvsroot/dnsresolver/DNSResolver_Core.h,v $
*******************************************************************************
*******************************************************************************
*/

#ifndef __HEADER_DNSRESOLVER_CORE_H__
#define __HEADER_DNSRESOLVER_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _DNSResolverCtxt                         t_DNSResolverCtxt;
typedef struct _CMDNSResolver           t_CMDNSResolver;

struct _CMDNSResolver
{
	AMPS_BOOL bTraceEnable;
};

struct _DNSResolverCtxt
{
	AMPS_HANDLE          hAMPSHandle;
	int                  nTraceId;
	AMPS_IOAGENTS_HANDLE hIOAgentsHandle;
	t_CMDNSResolver      oCMDNSResolver;
};

typedef struct _SSEventDNSRequest           t_SSEventDNSRequest;
typedef struct _SSEventDNSResponse          t_SSEventDNSResponse;

struct _SSEventDNSRequest
{
	t_SIPHandle        oSIPHandle;
	void*              pvAppData;
	t_SIPRouteSetEntry oSIPRouteSetEntry;
	//SIP_ProcessDNSResponse	pfSIP_ProcessDNSResponse;
	char pchRxIPAddr[AMPS_MAX_IP_STRING_LENGTH];
};

struct _SSEventDNSResponse
{
	t_SIPHandle        oSIPHandle;
	void*              pvSIPMessage;
	int                nQueryResult;
	t_SIPRouteSetEntry oSIPRouteSetEntry;
	//SIP_ProcessDNSResponse  pfSIP_ProcessDNSResponse;
};

void* DNSResolver_ModuleInit( AMPS_HANDLE hAMPSHandle, void* r_pvDNSConfiguration );

void DNSResolver_ModuleCleanup( AMPS_HANDLE hAMPSHandle, void* r_pvDNSResolverContext );

#ifdef __cplusplus
}
#endif

#endif /*__HEADER_DNSRESOLVER_CORE_H__*/

