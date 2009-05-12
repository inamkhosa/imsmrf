#include "fw_api.h"
#include "media/udpsend.h"
#include "media/wavread.h"

typedef enum
{
	APP_INTERNAL_EVENT_MAX
} e_RTP_InteranlEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX
} e_RTP_ExternalEvent;

e_MsRetCode Test_AppSignalCb( h_MediaComp hUdpSend_i, h_MediaCompAppCtx hUdpSendAppCtx_i, h_SigPkt h_SigPkt_i );

int main( int argc, char* argv[] )
{
	void* pvAMPSContext = NULL;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	h_MediaComp hUdpSend = NULL;
	t_UdpSendConfig oUdpSendConfig;
	t_MediaCompAppData oUdpSendAppData;
	int nUdpSendTraceId;

	h_MediaComp hWavRead = NULL;
	t_WavReadConfig oWavReadConfig;
	t_MediaCompAppData oWavReadAppData;
	int nWavReadTraceId;
	if( argc != 5 )
	{
		printf( "usage: %s filename read_size delay reads_per_timeout\n", argv[0] );
		exit( -1 );
	}
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
	// Initialize WavRead
	nWavReadTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nWavReadTraceId );

	oWavReadConfig.oConfig.nTraceId = nWavReadTraceId;
	oWavReadConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	oWavReadConfig.pcFileName = argv[1];
	oWavReadConfig.unReadSize =  atoi( argv[2] );
	oWavReadConfig.unReadTimerMs = atoi( argv[3] );
	oWavReadConfig.unNumReadsPerTimeout = atoi( argv[4] );

	eRetCode = WavReadInit( &oWavReadConfig, &hWavRead );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "WavReadInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Initialize UdpSend

	nUdpSendTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nUdpSendTraceId );

	oUdpSendConfig.oConfig.nTraceId = nUdpSendTraceId;
	oUdpSendConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	strcpy( oUdpSendConfig.pcLocalIp, "127.0.0.1" );
	oUdpSendConfig.nLocalPort = 7000;
	strcpy( oUdpSendConfig.pcRemoteIp, "127.0.0.1" );
	oUdpSendConfig.nRemotePort = 9000;
	oUdpSendConfig.hUdpNetHandle = NULL;

	eRetCode = UdpSendInit( &oUdpSendConfig, &hUdpSend );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpSendInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Configure WavRead
	oWavReadAppData.hAppDataCtx = hUdpSend;
	oWavReadAppData.pfAppDataCb = UdpSendAppDataCb;
	oWavReadAppData.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = WavReadSetAppData( hWavRead, &oWavReadAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "WavReadSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Configure UdpSend
	oUdpSendAppData.hAppDataCtx = NULL;
	oUdpSendAppData.pfAppDataCb = NULL;
	oUdpSendAppData.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = UdpSendSetAppData( hUdpSend, &oUdpSendAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpSendSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	eRetCode = UdpSendSetStatus( hUdpSend, MCS_START );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpSendSetStatus: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Set Status WavRead
	eRetCode = WavReadSetStatus( hWavRead, MCS_START );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "WavReadSetStatus: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	FW_Main( pvAMPSContext );

	printf( "Main: Leaving.\n" );

	WavReadDestroy( hWavRead );
	UdpSendDestroy( hUdpSend );

	FW_Cleanup( pvAMPSContext );

	return (AMPS_SUCCESS);
}

e_MsRetCode Test_AppSignalCb( h_MediaComp hUdpSend_i, h_MediaCompAppCtx hUdpSendAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	return (eRetCode);
}

