#include "fw_api.h"
#include "media/udprecv.h"
#include "media/rtprecv.h"
#include "media/g729dec.h"
#include "media/wavwrite.h"

typedef enum
{
	APP_INTERNAL_EVENT_MAX
} e_RTP_InteranlEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX
} e_RTP_ExternalEvent;

e_MsRetCode Test_MediaCompAppDataCb( h_MediaComp hRtpRecv_i, h_MediaCompAppCtx hRtpRecvAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode Test_G729DecAppSignalCb( h_MediaComp hRtpRecv_i, h_MediaCompAppCtx hRtpRecvAppCtx_i, h_SigPkt hSigPkt_i );

e_MsRetCode Test_RtpAppSignalCb( h_MediaComp hRtpRecv_i, h_MediaCompAppCtx hRtpRecvAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode Test_UdpAppSignalCb( h_MediaComp hUdpRecv_i, h_MediaCompAppCtx hUdpRecvAppCtx_i, h_SigPkt h_SigPkt_i );

void* pvAMPSContext = NULL;

int main( int argc, char* argv[] )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	h_MediaComp hUdpRecv = NULL;
	t_UdpRecvConfig oUdpRecvConfig;
	t_MediaCompAppData oUdpAppData;
	int nUdpRecvTraceId;

	h_MediaComp hRtpRecv = NULL;
	t_RtpRecvConfig oRtpRecvConfig;
	t_MediaCompAppData oRtpAppData;
	int nRtpRecvTraceId;

	h_MediaComp hG729Dec = NULL;
	t_G729DecConfig oG729DecConfig;
	t_MediaCompAppData oG729DecAppData;
	int nG729DecTraceId;

	h_MediaComp hWavWrite = NULL;
	t_WavWriteConfig oWavWriteConfig;
	t_MediaCompAppData oWavWriteAppData;
	int nWavWriteTraceId;
	if( argc != 2 )
	{
		printf( "usage: %s filename\n", argv[0] );
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
	// Initialize UdpRecv
	nUdpRecvTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nUdpRecvTraceId );

	oUdpRecvConfig.oConfig.nTraceId = nUdpRecvTraceId;
	oUdpRecvConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	strcpy( oUdpRecvConfig.pcLocalIp, "127.0.0.1" );
	oUdpRecvConfig.nLocalPort = 9000;
	strcpy( oUdpRecvConfig.pcRemoteIp, "127.0.0.1" );
	oUdpRecvConfig.nRemotePort = 9000;
	oUdpRecvConfig.hUdpNetHandle = NULL;
	oUdpRecvConfig.bManipulateHandle = AMPS_TRUE;

	eRetCode = UdpRecvInit( &oUdpRecvConfig, &hUdpRecv );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpRecvInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Initialize RtpRecv
	nRtpRecvTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nRtpRecvTraceId );

	oRtpRecvConfig.oConfig.nTraceId = nRtpRecvTraceId;
	oRtpRecvConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	oRtpRecvConfig.ucPayloadType = 18;
	oRtpRecvConfig.ucDtmfPayloadType = 101;
	oRtpRecvConfig.nSamplingRate =  8000;
	oRtpRecvConfig.unMaxTimeStampJump = 50000;

	eRetCode = RtpRecvInit( &oRtpRecvConfig, &hRtpRecv );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "RtpRecvInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Initialize G729Dec
	nG729DecTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nG729DecTraceId );

	oG729DecConfig.oConfig.nTraceId = nG729DecTraceId;
	oG729DecConfig.oConfig.pvFrameworkCtx = pvAMPSContext;

	eRetCode = G729DecInit( &oG729DecConfig, &hG729Dec );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "G729DecInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	nWavWriteTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nWavWriteTraceId );

	oWavWriteConfig.oConfig.nTraceId = nWavWriteTraceId;
	oWavWriteConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	oWavWriteConfig.pcFileName = argv[1];
	oWavWriteConfig.unMode = AMPS_FILE_MODE_WRITE_CREATE;

	eRetCode = WavWriteInit( &oWavWriteConfig, &hWavWrite );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "WavWriteInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// set udp application context data
	oUdpAppData.hAppDataCtx = hRtpRecv;
	oUdpAppData.pfAppDataCb = RtpRecvAppDataCb;
	oUdpAppData.pfAppSignalCb = Test_UdpAppSignalCb;

	oUdpAppData.hAppDataCtx = hG729Dec;
	oUdpAppData.pfAppDataCb = G729DecAppDataCb;
	oUdpAppData.pfAppSignalCb = Test_RtpAppSignalCb;

	eRetCode = UdpRecvSetAppData( hUdpRecv, &oUdpAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpRecvSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// set rtp application context data
	oRtpAppData.hAppDataCtx = hG729Dec;
	oRtpAppData.pfAppDataCb = G729DecAppDataCb;
	oRtpAppData.pfAppSignalCb = Test_RtpAppSignalCb;

	eRetCode = RtpRecvSetAppData( hRtpRecv, &oRtpAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "RtpRecvSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// set pcmu dec application context data
	oG729DecAppData.hAppDataCtx = hWavWrite;
	oG729DecAppData.pfAppDataCb = WavWriteAppDataCb;
	oG729DecAppData.pfAppSignalCb = Test_G729DecAppSignalCb;

	eRetCode = G729DecSetAppData( hG729Dec, &oG729DecAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "G729DecSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// set rtp application context data
	oWavWriteAppData.hAppDataCtx = NULL;
	oWavWriteAppData.pfAppDataCb = NULL;
	oWavWriteAppData.pfAppSignalCb = Test_G729DecAppSignalCb;

	eRetCode = WavWriteSetAppData( hWavWrite, &oWavWriteAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "WavWriteSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	UdpRecvSetStatus( hUdpRecv, MCS_START );
	RtpRecvSetStatus( hRtpRecv, MCS_START );
	G729DecSetStatus( hG729Dec, MCS_START );
	WavWriteSetStatus( hWavWrite, MCS_START );

	// start everything
	FW_Main( pvAMPSContext );

	printf( "Main: Leaving.\n" );

	UdpRecvDestroy( hUdpRecv );
	RtpRecvDestroy( hRtpRecv );
	G729DecDestroy( hG729Dec );
	WavWriteDestroy( hWavWrite );

	FW_Cleanup( pvAMPSContext );

	return (AMPS_SUCCESS);
}

e_MsRetCode Test_UdpAppSignalCb( h_MediaComp hUdpRecv_i, h_MediaCompAppCtx hUdpRecvAppCtx_i, h_SigPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = (t_SigPkt*)hSigPkt_i;

	printf( "Test_UdpAppSignalCb: called\n" );

	(poSigPkt->pfnSignalPktDelete)( poSigPkt );
	FW_Free( poSigPkt );

	return (eRetCode);
}

e_MsRetCode Test_RtpAppSignalCb( h_MediaComp hRtpRecv_i, h_MediaCompAppCtx hRtpRecvAppCtx_i, h_SigPkt hSigPkt_i )
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

		case SPT_REMOTE_ADDRESS_CHANGE_Packet:
		{
			printf( "SPT_REMOTE_ADDRESS_CHANGE_Packet: Remote Address = %s:%d\n", poSigPkt->oAddrChangePkt.pcIp, poSigPkt->oAddrChangePkt.nPort );
			break;
		}

		case SPT_CURRENT_SYNC_TIME_Packet:
		{
			poSigPkt->oSyncTimePkt.nSyncTimeMs =  FW_GetCurrentTimeInMilliSec( pvAMPSContext );

			poSigPkt = NULL;
			break;
		}

		default:
			printf( "Unknown signal packet\n" );
	}
	if( NULL != poSigPkt )
	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}
	return (eRetCode);
}

e_MsRetCode Test_MediaCompAppDataCb( h_MediaComp hRtpRecv_i, h_MediaCompAppCtx hRtpRecvAppCtx_i, h_DataPkt hDataPkt_i )
{
	t_DataPkt* poDataPkt = (t_DataPkt*)hDataPkt_i;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	printf( "Test_MediaCompAppDataCb: called\n" );
	printf( "Test_MediaCompAppDataCb: type = %d\n", poDataPkt->eType );

	printf( "Test_MediaCompAppDataCb: length = %d\n", poDataPkt->oGenPkt.nLength );

	(poDataPkt->pfnDataPktDelete)( poDataPkt );
	FW_Free( poDataPkt );

	return (eRetCode);
}

e_MsRetCode Test_G729DecAppSignalCb( h_MediaComp hRtpRecv_i, h_MediaCompAppCtx hRtpRecvAppCtx_i, h_SigPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = (t_SigPkt*)hSigPkt_i;

	printf( "Test_G729DecAppSignalCb: called\n" );

	(poSigPkt->pfnSignalPktDelete)( poSigPkt );
	FW_Free( poSigPkt );

	return (eRetCode);
}

