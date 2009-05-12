#include "fw_api.h"
#include "media/media.h"

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

int main( int argc, char* argv[] )
{
	void* pvAMPSContext = NULL;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	h_MediaComp hUdpSend = NULL;
	t_UdpSendConfig oUdpSendConfig;

	h_MediaComp hFileRead = NULL;
	t_FileReadConfig oFileReadConfig;

	h_MediaChain hChain = NULL;
	t_MediaCompConfig oChainConfig;
	if( argc != 4 )
	{
		printf( "usage: %s filename read_size delay\n", argv[0] );
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
	// Initialize FileRead
	oFileReadConfig.oConfig.nTraceId = FW_GetTraceID( pvAMPSContext );
	FW_SetTraceForTraceID( pvAMPSContext, oFileReadConfig.oConfig.nTraceId );

	oFileReadConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	oFileReadConfig.pcFileName = argv[1];
	oFileReadConfig.unReadSize =  atoi( argv[2] );
	oFileReadConfig.unReadTimerMs = atoi( argv[3] );

	eRetCode = Media_CompCreate( MCT_FILEREAD, (h_CompConfig)&oFileReadConfig, &hFileRead );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "Media_CompCreate(MCT_FILEREAD): error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Initialize UdpSend

	oUdpSendConfig.oConfig.nTraceId = FW_GetTraceID( pvAMPSContext );
	FW_SetTraceForTraceID( pvAMPSContext, oUdpSendConfig.oConfig.nTraceId );

	oUdpSendConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	strcpy( oUdpSendConfig.pcLocalIp, "127.0.0.1" );
	oUdpSendConfig.nLocalPort = 7000;
	strcpy( oUdpSendConfig.pcRemoteIp, "127.0.0.1" );
	oUdpSendConfig.nRemotePort = 9000;
	oUdpSendConfig.hUdpNetHandle = NULL;

	eRetCode = Media_CompCreate( MCT_UDPSEND, (h_CompConfig)&oUdpSendConfig, &hUdpSend );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "Media_CompCreate(MCT_UDPSEND): error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Create chain
	oChainConfig.nTraceId = FW_GetTraceID( pvAMPSContext );
	FW_SetTraceForTraceID( pvAMPSContext, oChainConfig.nTraceId );

	oChainConfig.pvFrameworkCtx = pvAMPSContext;

	eRetCode = Media_ChainCreate( &oChainConfig, &hChain );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "Media_ChainCreate: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	Media_ChainAddInChain( hChain, hFileRead );
	Media_ChainAddInChain( hChain, hUdpSend );

	// Link components
	eRetCode = Media_ChainSetLink( hChain, hFileRead, hUdpSend );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "Media_ChainAddLink: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	//Set Status to START
	printf( "Media_ChainSetStatus(MCS_START): calling on hChain = %p\n", hChain );
	eRetCode = Media_ChainSetStatus( hChain, MCS_START );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "Media_ChainSetStatus(MCS_START): error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	FW_Main( pvAMPSContext );

	printf( "Media_ChainSetStatus(MCS_STOP): calling on hChain = %p\n", hChain );
	eRetCode =  Media_ChainSetStatus( hChain, MCS_STOP );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "Media_ChainSetStatus(MCS_STOP): error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	Media_CompDestroy( hFileRead );
	Media_CompDestroy( hUdpSend );

	eRetCode =  Media_ChainDestroy( hChain );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "Media_ChainDestroy: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	FW_Cleanup( pvAMPSContext );

	return (AMPS_SUCCESS);
}

