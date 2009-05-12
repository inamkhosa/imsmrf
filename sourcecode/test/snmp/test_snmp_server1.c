/******************************************************************************
** Written by Razi-ud-Din Chaudhry
**
** Copyright (C) 2008 AdvancedIMS
**
** All rights reserved.
**
*******************************************************************************
** FILE NAME        : test_snmp_server1.c
** DESCRIPTION      : The file contains test for starting snmp communication.
** PROJECT NAME     : Media Server
** TYPE             : ANSI C
** CONTRIBUTOR(S)   : Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

#include "snmp/snmp_comm.h"

typedef enum
{
	APP_INTERNAL_EVENT_MAX = 1
} e_RTP_InternalEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX = 1
} e_RTP_ExternalEvent;

/******************************************************************************
** Function Name:
**		main
**
** Parameters:
**		int nArgc_i
**			type: input
**			detail: This number indicates the no of command line
**				    arguments passed to this program.
**
**		char*  ppcArgv_i[]
**			type: input
**			detail: This pointer array contains the command line
**				    arguments passed to this program.
**
**
** Return:
**		An integer specifying the success or failure of this program.
**
**
** Additional Details:
**		This function calls functions defined in the snmp communication
**		    api and checks the response obtained.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

int main( int nArgc_i, char* ppcArgv_i[] )
{
	char pcLocalIp[AMPS_MAX_IP_STRING_LENGTH];
	int nLocalPort;
	char pcRemoteIp[AMPS_MAX_IP_STRING_LENGTH];
	int nRemotePort;
	if( nArgc_i != 5 )
	{
		printf( "Usage: %s Local_Ip Local_Port Remote_Ip Remote_Port\n", ppcArgv_i[0] );
		return (-1);
	}
	else
	{
		strcpy( pcLocalIp, ppcArgv_i[1] );
		nLocalPort = atoi( ppcArgv_i[2] );
		printf( "Local Ip: %s ___ Local Port: %d\n", pcLocalIp, nLocalPort );
		strcpy( pcRemoteIp, ppcArgv_i[3] );
		nRemotePort = atoi( ppcArgv_i[4] );
		printf( "Remote Ip: %s ___ Remote Port: %d\n", pcRemoteIp, nRemotePort );
	}
	void* pvAMPSContext = NULL;
	AMPS_SCHED_HANDLE hScheduler;
	int nTraceId;
	e_MsRetCode eRet_Code = MS_ERR_NONE;
	t_SnmpCommConfig oSnmpCommConfig;
	t_MsCtx oMsCtx;
	h_SnmpComm hSnmpComm;
	AMPS_NET_HANDLE hUdpNetHandle;

	printf( "Welcome to SNMP Communication\n" );
	printf( "*****************************\n" );

	pvAMPSContext = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );
	if( NULL == pvAMPSContext )
	{
		printf( "FW_Init: AMPS_Init is failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	else
	{
		printf( "pvAMPSContext = %p\n", pvAMPSContext );
	}
	FW_SetTraceMode( pvAMPSContext, AMPS_TRACE_MODE_BOTH );
	FW_SetTraceLevel( pvAMPSContext, 31 );
	FW_EnableLogging( pvAMPSContext, "logfile.txt" );

	FW_EvtSysSetEvtHandleValidationCallback( pvAMPSContext, NULL );
	hScheduler = FW_SchedInit( pvAMPSContext, APP_INTERNAL_EVENT_MAX, APP_EXTERNAL_EVENT_MAX, NULL, NULL, NULL, NULL );
	if( NULL == hScheduler )
	{
		printf( "FW_SchedInit failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	else
	{
		printf( "FW_SchedInit successful.\n" );
	}
	nTraceId = FW_GetTraceID( pvAMPSContext );
	FW_SetTraceForTraceID( pvAMPSContext, nTraceId );

	oSnmpCommConfig.nTraceId = nTraceId;
	strncpy( oSnmpCommConfig.pcLocalIp, pcLocalIp, sizeof (pcLocalIp) - 1 );
	oSnmpCommConfig.nLocalPort = nLocalPort;
	strncpy( oSnmpCommConfig.pcRemoteIp, pcRemoteIp, sizeof (pcRemoteIp) - 1 );
	oSnmpCommConfig.nRemotePort = nRemotePort;
	memset( &oMsCtx, 0, sizeof (t_MsCtx*));
	oMsCtx.poDoc = xmlParseFile( "./config.xml" );
	memset( &oMsCtx.oLocalUri, 0, sizeof (t_Uri));
	memset( &oMsCtx.pulPerformanceCtrs, 0, sizeof (oMsCtx.pulPerformanceCtrs));
	memset( &oMsCtx.pulFaultCtrs, 0, sizeof (oMsCtx.pulFaultCtrs));
	oSnmpCommConfig.poMsCtx = &oMsCtx;
	oSnmpCommConfig.pvFrameworkCtx = pvAMPSContext;

	// Initialize snmp communication
	eRet_Code = SNMP_Init( &oSnmpCommConfig, &hSnmpComm );
	if( MS_ERR_NONE == eRet_Code )
	{
		printf( "Snmp communication initialized successfully\n" );
	}
	else
	{
		printf( "Error while initializing snmp communication\n" );
	}
	FW_Main( pvAMPSContext );

	xmlSaveFile( "./config.xml", oSnmpCommConfig.poMsCtx->poDoc );
	// Destroy snmp communication
	eRet_Code = SNMP_Destroy( hSnmpComm );
	if( MS_ERR_NONE == eRet_Code )
	{
		printf( "Snmp communication destroyed successfully\n" );
	}
	else
	{
		printf( "Error while destroying snmp communication\n" );
	}
	FW_SchedCleanup( pvAMPSContext, hScheduler );
	FW_Cleanup( pvAMPSContext );

	return (0);
}

