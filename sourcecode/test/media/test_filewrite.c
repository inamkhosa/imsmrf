#include "fw_api.h"
#include "media/udprecv.h"
#include "media/filewrite.h"

typedef enum
{
	APP_INTERNAL_EVENT_MAX
} e_RTP_InteranlEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX
} e_RTP_ExternalEvent;

e_MsRetCode Test_FileWriteAppSignalCb( h_MediaComp hFileWrite_i, h_MediaCompAppCtx hFileWriteAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode Test_UdpAppSignalCb( h_MediaComp hUdpRecv_i, h_MediaCompAppCtx hUdpRecvAppCtx_i, h_SigPkt h_SigPkt_i );

int main( int argc, char* argv[] )
{
	void* pvAMPSContext = NULL;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	h_MediaComp hUdpRecv = NULL;
	t_UdpRecvConfig oUdpRecvConfig;
	t_MediaCompAppData oUdpAppData;
	int nUdpRecvTraceId;

	h_MediaComp hFileWrite = NULL;
	t_FileWriteConfig oFileWriteConfig;
	t_MediaCompAppData oFileWriteAppData;
	int nFileWriteTraceId;
	if( argc != 3 )
	{
		printf( "usage: %s filename mode\n", argv[0] );
		exit( -1 );
	}
	pvAMPSContext = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );
	if( NULL == pvAMPSContext )
	{
		printf( "FW_Init: AMPS_Init is failed.\n" );
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
	// configure udp

	nUdpRecvTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nUdpRecvTraceId );

	oUdpRecvConfig.oConfig.nTraceId = nUdpRecvTraceId;
	oUdpRecvConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	strcpy( oUdpRecvConfig.pcLocalIp, "127.0.0.1" );
	oUdpRecvConfig.nLocalPort = 8000;
	strcpy( oUdpRecvConfig.pcRemoteIp, "127.0.0.1" );
	oUdpRecvConfig.nRemotePort = 8000;
	oUdpRecvConfig.hUdpNetHandle = NULL;

	eRetCode = UdpRecvInit( &oUdpRecvConfig, &hUdpRecv );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpRecvInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// configure FileWrite
	nFileWriteTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nFileWriteTraceId );

	oFileWriteConfig.oConfig.nTraceId = nFileWriteTraceId;
	oFileWriteConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	oFileWriteConfig.pcFileName = argv[1];
	oFileWriteConfig.unMode = atoi( argv[2] );

	eRetCode = FileWriteInit( &oFileWriteConfig, &hFileWrite );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileWriteInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// set upd application context data
	oUdpAppData.hAppDataCtx = hFileWrite;
	oUdpAppData.pfAppDataCb = FileWriteAppDataCb;
	oUdpAppData.pfAppSignalCb = Test_UdpAppSignalCb;

	eRetCode = UdpRecvSetAppData( hUdpRecv, &oUdpAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpRecvSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// set rtp application context data
	oFileWriteAppData.hAppDataCtx = NULL;
	oFileWriteAppData.pfAppDataCb = NULL;
	oFileWriteAppData.pfAppSignalCb = Test_FileWriteAppSignalCb;

	eRetCode = FileWriteSetAppData( hFileWrite, &oFileWriteAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileWriteSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// start everything
	FW_Main( pvAMPSContext );

	printf( "Main: Leaving.\n" );

	UdpRecvDestroy( hUdpRecv );
	FileWriteDestroy( hFileWrite );

	FW_Cleanup( pvAMPSContext );

	return (AMPS_SUCCESS);
}

e_MsRetCode Test_UdpAppSignalCb( h_MediaComp hUdpRecv_i, h_MediaCompAppCtx hUdpRecvAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	return (eRetCode);
}

e_MsRetCode Test_FileWriteAppSignalCb( h_MediaComp hFileWrite_i, h_MediaCompAppCtx hFileWriteAppCtx_i, h_SigPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = (t_SigPkt*)hSigPkt_i;

	printf( "Test_RtpAppSignalCb: called\n" );
	printf( "type = %d\n", poSigPkt->eType );

	switch( poSigPkt->eType )
	{
		case SPT_DTMP_Packet:
		{
			printf( "SPT_DTMP_Packet\n" );
			printf( "DTMF digit = %d, volume = %d\n", poSigPkt->oDtmfPkt.ucDtmfKey, poSigPkt->oDtmfPkt.ucVolume );
			break;
		}

		default:
			printf( "Unknown signal packet\n" );
	}

	(poSigPkt->pfnSignalPktDelete)( poSigPkt );
	FW_Free( poSigPkt );

	return (eRetCode);
}

