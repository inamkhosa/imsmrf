#include "fw_api.h"
#include "media/udpsend.h"
#include "media/fileread.h"

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

	h_MediaComp hFileRead = NULL;
	t_FileReadConfig oFileReadConfig;
	t_MediaCompAppData oFileReadAppData;
	int nFileReadTraceId;
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
	// Initialize FileRead
	nFileReadTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nFileReadTraceId );

	oFileReadConfig.oConfig.nTraceId = nFileReadTraceId;
	oFileReadConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	oFileReadConfig.pcFileName = argv[1];
	oFileReadConfig.unReadSize =  atoi( argv[2] );
	oFileReadConfig.unReadTimerMs = atoi( argv[3] );
	oFileReadConfig.unNumReadsPerTimeout = atoi( argv[4] );

	eRetCode = FileReadInit( &oFileReadConfig, &hFileRead );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileReadInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
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
	// Configure FileRead
	oFileReadAppData.hAppDataCtx = hUdpSend;
	oFileReadAppData.pfAppDataCb = UdpSendAppDataCb;
	oFileReadAppData.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = FileReadSetAppData( hFileRead, &oFileReadAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileReadSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
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
	// Set Status FileRead
	eRetCode = FileReadSetStatus( hFileRead, MCS_START );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileReadSetStatus: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	FW_Main( pvAMPSContext );

	printf( "Main: Leaving.\n" );

	FileReadDestroy( hFileRead );
	UdpSendDestroy( hUdpSend );

	FW_Cleanup( pvAMPSContext );

	return (AMPS_SUCCESS);
}

e_MsRetCode Test_AppSignalCb( h_MediaComp hUdpSend_i, h_MediaCompAppCtx hUdpSendAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	return (eRetCode);
}

