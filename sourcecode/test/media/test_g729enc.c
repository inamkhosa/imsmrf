#include "fw_api.h"
#include "media/udpsend.h"
#include "media/wavread.h"
#include "media/g729enc.h"
#include "media/rtpsend.h"

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

	h_MediaComp hRtpSend = NULL;
	t_RtpSendConfig oRtpSendConfig;
	t_MediaCompAppData oRtpSendAppData;
	int nRtpSendTraceId;

	h_MediaComp hUdpSend = NULL;
	t_UdpSendConfig oUdpSendConfig;
	t_MediaCompAppData oUdpSendAppData;
	int nUdpSendTraceId;

	h_MediaComp hWavRead = NULL;
	t_WavReadConfig oWavReadConfig;
	t_MediaCompAppData oWavReadAppData;
	int nWavReadTraceId;

	h_MediaComp hG729Enc = NULL;
	t_G729EncConfig oG729EncConfig;
	t_MediaCompAppData oG729EncAppData;
	int nG729EncTraceId;
	if( argc != 5 )
	{
		printf( "usage: %s filename read_size delay reads_per_timeout\n", argv[0] );
		exit( -1 );
	}
	pvAMPSContext = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5500 );
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
	// Initialize RtpSend
	nRtpSendTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nRtpSendTraceId );

	oRtpSendConfig.oConfig.nTraceId = nRtpSendTraceId;
	oRtpSendConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	oRtpSendConfig.nSamplingRate = 8000;

	eRetCode = RtpSendInit( &oRtpSendConfig, &hRtpSend );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "RtpSendInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Initialize WavRead

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
	// Initialize G729Enc
	nG729EncTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nG729EncTraceId );

	oG729EncConfig.oConfig.nTraceId = nG729EncTraceId;
	oG729EncConfig.oConfig.pvFrameworkCtx = pvAMPSContext;

	eRetCode = G729EncInit( &oG729EncConfig, &hG729Enc );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "G729EncInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Initialize UpdSend
	nUdpSendTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nUdpSendTraceId );

	oUdpSendConfig.oConfig.nTraceId = nUdpSendTraceId;
	oUdpSendConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	strcpy( oUdpSendConfig.pcLocalIp, "127.0.0.1" );
	oUdpSendConfig.nLocalPort = 7000;
	strcpy( oUdpSendConfig.pcRemoteIp, "127.0.0.1" );
	oUdpSendConfig.nRemotePort = 9000;
	oUdpSendConfig.hUdpNetHandle = NULL;
	oUdpSendConfig.bManipulateHandle = AMPS_TRUE;

	eRetCode = UdpSendInit( &oUdpSendConfig, &hUdpSend );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpSendInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Configure WavRead
	oWavReadAppData.hAppDataCtx = hG729Enc;
	oWavReadAppData.pfAppDataCb = G729EncAppDataCb;
	oWavReadAppData.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = WavReadSetAppData( hWavRead, &oWavReadAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "WavReadSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	printf( "UdpSendAppDataCb = %p\n", UdpSendAppDataCb );

	// Configure G729Enc
	oG729EncAppData.hAppDataCtx = hRtpSend;
	oG729EncAppData.pfAppDataCb = RtpSendAppDataCb;
	oG729EncAppData.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = G729EncSetAppData( hG729Enc, &oG729EncAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "G729EncSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Configure RtpSend
	oRtpSendAppData.hAppDataCtx = hUdpSend;
	oRtpSendAppData.pfAppDataCb = UdpSendAppDataCb;
	oRtpSendAppData.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = RtpSendSetAppData( hRtpSend, &oRtpSendAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "RtpSendSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
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
/*	eRetCode = UdpSendSetStatus( hUdpSend, MCS_START );
if( eRetCode != MS_ERR_NONE )
{
printf( "UdpSendSetStatus: error = %s\n", get_ms_ret_code_string( eRetCode ) );
exit( -1 );
}*/
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
	G729EncDestroy( hG729Enc );
	RtpSendDestroy( hRtpSend );
	UdpSendDestroy( hUdpSend );

	FW_Cleanup( pvAMPSContext );

	return (AMPS_SUCCESS);
}

e_MsRetCode Test_AppSignalCb( h_MediaComp hUdpSend_i, h_MediaCompAppCtx hUdpSendAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	return (eRetCode);
}

