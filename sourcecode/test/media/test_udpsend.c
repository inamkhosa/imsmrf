#include "fw_api.h"
#include "media/udpsend.h"
#include "media/udprecv.h"

typedef enum
{
	APP_INTERNAL_EVENT_MAX
} e_RTP_InteranlEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX
} e_RTP_ExternalEvent;

e_MsRetCode Test_AppDataCb( h_MediaComp hUdpSend_i, h_MediaCompAppCtx hUdpSendAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode Test_AppSignalCb( h_MediaComp hUdpSend_i, h_MediaCompAppCtx hUdpSendAppCtx_i, h_SigPkt h_SigPkt_i );

int main( void )
{
	void* pvAMPSContext = NULL;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	h_MediaComp hUdpSend = NULL;
	t_UdpSendConfig oUdpSendConfig;
	t_MediaCompAppData oUdpSendAppData;
	int nUdpSendTraceId;

	h_MediaComp hUdpRecv = NULL;
	t_UdpRecvConfig oUdpRecvConfig;
	t_MediaCompAppData oUdpRecvAppData;
	int nUdpRecvTraceId;

	pvAMPSContext = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );
	if( NULL == pvAMPSContext )
	{
		printf( "FW_Init: FW_Init failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	FW_SetTraceMode( pvAMPSContext, AMPS_TRACE_MODE_BOTH );
	FW_SetTraceLevel( pvAMPSContext, 31 );
	FW_EnableLogging( pvAMPSContext, "logfile.txt" );

	FW_EvtSysSetEvtHandleValidationCallback( pvAMPSContext, NULL );
	if( NULL == FW_SchedInit( pvAMPSContext, APP_INTERNAL_EVENT_MAX, APP_EXTERNAL_EVENT_MAX, NULL, NULL, NULL, NULL ))
	{
		printf( "FW_SchedInit failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	// Initialize udp recv

	oUdpRecvConfig.oConfig.nTraceId = nUdpRecvTraceId;
	oUdpRecvConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	strcpy( oUdpRecvConfig.pcLocalIp, "127.0.0.1" );
	oUdpRecvConfig.nLocalPort = 8000;
	strcpy( oUdpRecvConfig.pcRemoteIp, "0.0.0.0" );
	oUdpRecvConfig.nRemotePort = 0;
	oUdpRecvConfig.hUdpNetHandle = NULL;

	eRetCode = UdpRecvInit( &oUdpRecvConfig, &hUdpRecv );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpRecvInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Get net handle of udp-recv
	UdpRecvGetParam( hUdpRecv, MCC_UDPRECV_NET_HANDLE, (void*)&oUdpSendConfig.hUdpNetHandle );

	// Initialize upd send
	nUdpSendTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nUdpSendTraceId );

	oUdpSendConfig.oConfig.nTraceId = nUdpSendTraceId;
	oUdpSendConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	strcpy( oUdpSendConfig.pcLocalIp, "0.0.0.0" );
	oUdpSendConfig.nLocalPort = 0;
	strcpy( oUdpSendConfig.pcRemoteIp, "127.0.0.1" );
	oUdpSendConfig.nRemotePort = 9000;
//	oUdpSendConfig.hUdpNetHandle = NULL;

	eRetCode = UdpSendInit( &oUdpSendConfig, &hUdpSend );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpSendInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Configure udp recv
	oUdpRecvAppData.hAppDataCtx = hUdpSend;
	oUdpRecvAppData.pfAppDataCb = UdpSendAppDataCb;
	oUdpRecvAppData.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = UdpRecvSetAppData( hUdpRecv, &oUdpRecvAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpRecvSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Configure udp send
	oUdpSendAppData.hAppDataCtx = NULL;
	oUdpSendAppData.pfAppDataCb = NULL;
	oUdpSendAppData.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = UdpSendSetAppData( hUdpSend, &oUdpSendAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpSendSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	FW_Main( pvAMPSContext );

	printf( "Main: Leaving.\n" );

	FW_Cleanup( pvAMPSContext );

	return (AMPS_SUCCESS);
}

e_MsRetCode Test_AppDataCb( h_MediaComp hUdpSend_i, h_MediaCompAppCtx hUdpSendAppCtx_i, h_DataPkt hDataPkt_i )
{
	t_DataPkt* poDataPkt = (t_DataPkt*)hDataPkt_i;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	printf( "Test_AppDataCb: called\n" );
	printf( "Pkt received from %s:%d\n", poDataPkt->oUdpPkt.pcRemoteIpAddress, poDataPkt->oUdpPkt.nRemotePort );
	printf( "Pkt length = %d\n%s\n", poDataPkt->oUdpPkt.nLength, poDataPkt->oUdpPkt.pucData );
	(poDataPkt->pfnDataPktDelete)( poDataPkt );
	FW_Free( poDataPkt );

	return (eRetCode);
}

e_MsRetCode Test_AppSignalCb( h_MediaComp hUdpSend_i, h_MediaCompAppCtx hUdpSendAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	return (eRetCode);
}

