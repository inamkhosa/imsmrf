#include "fw_api.h"
#include "media/udprecv.h"
#include "media/rtprecv.h"
#include "media/buffer.h"

typedef enum
{
	APP_BUFFER1_EVENT = 2,
	APP_INTERNAL_EVENT_MAX
} e_RTP_InteranlEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX = 1
} e_RTP_ExternalEvent;

e_MsRetCode Test_RtpAppDataCb( h_MediaComp hRtpRecv_i, h_MediaCompAppCtx hRtpRecvAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode Test_RtpAppSignalCb( h_MediaComp hRtpRecv_i, h_MediaCompAppCtx hRtpRecvAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode Test_UdpAppSignalCb( h_MediaComp hUdpRecv_i, h_MediaCompAppCtx hUdpRecvAppCtx_i, h_SigPkt h_SigPkt_i );

int Test_EventHandleValidationCallback( void* r_pvContext, int r_nEventID, void* r_pvEventPayload );

int main( void )
{
	void* pvAMPSContext = NULL;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	h_MediaComp hUdpRecv = NULL;
	t_UdpRecvConfig oUdpRecvConfig;
	t_MediaCompAppData oUdpAppData;
	int nUdpRecvTraceId;

	h_MediaComp hBuffer = NULL;
	t_BufferConfig oBufferConfig;
	t_MediaCompAppData oBufferAppData;
	int nBufferTraceId;

	h_MediaComp hRtpRecv = NULL;
	t_RtpRecvConfig oRtpRecvConfig;
	t_MediaCompAppData oRtpAppData;
	int nRtpRecvTraceId;

	pvAMPSContext = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );
	if( NULL == pvAMPSContext )
	{
		printf( "FW_Init: AMPS_Init is failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	FW_SetTraceMode( pvAMPSContext, AMPS_TRACE_MODE_BOTH );
	FW_SetTraceLevel( pvAMPSContext, 31 );
	FW_EnableLogging( pvAMPSContext, "logfile.txt" );
	//AMPS_SetTraceForAMPS(pvAMPSContext);
	//AMPS_SetTraceForEventSystem(pvAMPSContext);
	//AMPS_SetTraceForSched(pvAMPSContext);

	FW_EvtSysSetEvtHandleValidationCallback( pvAMPSContext, Test_EventHandleValidationCallback );
	if( NULL == FW_SchedInit( pvAMPSContext, APP_INTERNAL_EVENT_MAX, APP_EXTERNAL_EVENT_MAX, NULL, NULL, NULL, NULL ))
	{
		printf( "FW_SchedInit failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	// Initialize UdpRecv

	nUdpRecvTraceId = FW_GetTraceID( pvAMPSContext );
	FW_SetTraceForTraceID( pvAMPSContext, nUdpRecvTraceId );

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
	// Initialize Buffer

	nBufferTraceId = FW_GetTraceID( pvAMPSContext );
	FW_SetTraceForTraceID( pvAMPSContext, nBufferTraceId );

	oBufferConfig.oConfig.nTraceId = nBufferTraceId;
	oBufferConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	oBufferConfig.nEventId = APP_BUFFER1_EVENT;

	eRetCode = BufferInit( &oBufferConfig, &hBuffer );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "BufferInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Initialize RtpRecv
	nRtpRecvTraceId = FW_GetTraceID( pvAMPSContext );
	FW_SetTraceForTraceID( pvAMPSContext, nRtpRecvTraceId );

	oRtpRecvConfig.oConfig.nTraceId = nRtpRecvTraceId;
	oRtpRecvConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	oRtpRecvConfig.ucPayloadType = 0;
	oRtpRecvConfig.ucDtmfPayloadType = 101;

	eRetCode = RtpRecvInit( &oRtpRecvConfig, &hRtpRecv );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "RtpRecvInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// set UdpRecv application context data
	oUdpAppData.hAppDataCtx = hBuffer;
	oUdpAppData.pfAppDataCb = BufferAppDataCb;
	oUdpAppData.pfAppSignalCb = Test_UdpAppSignalCb;

	eRetCode = UdpRecvSetAppData( hUdpRecv, &oUdpAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpRecvSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// set Buffer application context data
	oBufferAppData.hAppDataCtx = hRtpRecv;
	oBufferAppData.pfAppDataCb = Test_RtpAppDataCb;
	oBufferAppData.pfAppSignalCb = Test_UdpAppSignalCb;

	eRetCode = BufferSetAppData( hBuffer, &oBufferAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "BufferSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// set RtpRecv application context data
	oRtpAppData.hAppDataCtx = NULL;
	oRtpAppData.pfAppDataCb = Test_RtpAppDataCb;
	oRtpAppData.pfAppSignalCb = Test_RtpAppSignalCb;

	eRetCode = RtpRecvSetAppData( hRtpRecv, &oRtpAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "RtpRecvSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// start everything
	FW_Main( pvAMPSContext );

	printf( "Main: Leaving.\n" );

	UdpRecvDestroy( hUdpRecv );
	BufferDestroy( hBuffer );
	RtpRecvDestroy( hRtpRecv );

	FW_Cleanup( pvAMPSContext );

	return (AMPS_SUCCESS);
}

e_MsRetCode Test_UdpAppSignalCb( h_MediaComp hUdpRecv_i, h_MediaCompAppCtx hUdpRecvAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	return (eRetCode);
}

e_MsRetCode Test_RtpAppDataCb( h_MediaComp hRtpRecv_i, h_MediaCompAppCtx hRtpRecvAppCtx_i, h_DataPkt hDataPkt_i )
{
	t_DataPkt* poDataPkt = (t_DataPkt*)hDataPkt_i;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	printf( "Test_RtpAppDataCb: called\n" );

	printf( "packet type = %d\n", poDataPkt->eType );

	switch( poDataPkt->eType )
	{
		case DPT_GEN_Packet:
			printf( "packet length = %d\n", poDataPkt->oGenPkt.nLength );
			printf( "packet data = %s\n", poDataPkt->oGenPkt.pucData );
			break;

		case DPT_UDP_Packet:
			printf( "packet length = %d\n", poDataPkt->oUdpPkt.nLength );
			printf( "packet data = %s\n", poDataPkt->oUdpPkt.pucData );
			break;

		case DPT_RTP_Packet:
			printf( "packet length = %d\n", rtp_packet_get_payload_len( poDataPkt->oRtpPkt.poRtpPkt ) );
			break;

		default:
			printf( "unknown packet type = %d\n", poDataPkt->eType );
	}

	(poDataPkt->pfnDataPktDelete)( poDataPkt );
	FW_Free( poDataPkt );

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

		default:
			printf( "Unknown signal packet\n" );
	}

	(poSigPkt->pfnSignalPktDelete)( poSigPkt );
	FW_Free( poSigPkt );

	return (eRetCode);
}

int Test_EventHandleValidationCallback( void* r_pvContext, int r_nEventID, void* r_pvEventPayload )
{
	return (AMPS_SUCCESS);
}

