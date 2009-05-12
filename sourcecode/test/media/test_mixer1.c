#include "fw_api.h"
#include "media/media.h"
#include "util/circular_list.h"
#include "ms_ret_code.h"
#include "ms_debug.h"
#include "util/sync_timer.h"
#include "media/mixer.h"
#include "media/udprecv.h"
#include "media/udpsend.h"
#include "media/rtprecv.h"
#include "media/rtpsend.h"
#include "media/pcmuenc.h"
#include "media/pcmudec.h"

typedef enum
{
	APP_INTERNAL_EVENT_MAX
} e_RTP_InteranlEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX
} e_RTP_ExternalEvent;

int Test_EventHandleValidationCallback( void* r_pvContext, int r_nEventID, void* r_pvEventPayload )
{
	return (AMPS_TRUE);
}

e_MsRetCode App_Media_ChainCb( h_MediaComp hMediaComp_i, h_SigPkt h_SigPkt_i, h_MediaCompAppCtx hMediaCompAppCtx1_i, h_MediaCompAppCtx hMediaCompAppCtx2_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	h_SyncTimer hTimer = hMediaCompAppCtx1_i;
	int nTraceId = (int)hMediaCompAppCtx2_i;
	t_SigPkt* poSigPkt = h_SigPkt_i;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		switch( poSigPkt->eType )
		{
			case SPT_SILENCE_DETECTED_Packet:
			{
				break;
			}

			case SPT_SOUND_DETECTED_Packet:
			{
				break;
			}

			case SPT_DTMP_Packet:
			{
				break;
			}

			case SPT_REMOTE_ADDRESS_CHANGE_Packet:
			{
				MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "SPT_REMOTE_ADDRESS_CHANGE_Packet: Remote Address = %s:%d\n", poSigPkt->oAddrChangePkt.pcIp, poSigPkt->oAddrChangePkt.nPort );
				break;
			}

			case SPT_CURRENT_SYNC_TIME_Packet:
			{
				ms_assert( NULL != hTimer );
				ST_GetCurrentTime( hTimer, poSigPkt->oSyncTimePkt.nDelayMs, &poSigPkt->oSyncTimePkt.nSyncTimeMs );

				MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Current Time = %d\n", poSigPkt->oSyncTimePkt.nSyncTimeMs );

				poSigPkt = NULL;
				break;
			}

			default:
				MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_ERROR, "Unknown signal packet\n" );
		}
		if( NULL != poSigPkt )
		{
			(poSigPkt->pfnSignalPktDelete)( poSigPkt );
			FW_Free( poSigPkt );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

int main( int argc, char* argv[] )
{
	void* pvFrameworkCtx = NULL;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	h_SyncTimer hTimer = NULL;

	h_MediaComp hMixer = NULL;
	t_MixerConfig oMixerConfig;
	t_MixerNodeConfig oMixerNodeConfig;

	int nMixerTraceId;
	if( argc < 3 )
	{
		printf( "usage: %s rx_port1 tx_port1 rx_port2 tx_port2 ...\n", argv[0] );
		exit( -1 );
	}
	pvFrameworkCtx = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );
	if( NULL == pvFrameworkCtx )
	{
		printf( "FW_Init: FW_Init failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	FW_SetTraceMode( pvFrameworkCtx, AMPS_TRACE_MODE_BOTH );
	FW_SetTraceLevel( pvFrameworkCtx, 31 );
	FW_EnableLogging( pvFrameworkCtx, "logfile.txt" );

	FW_EvtSysSetEvtHandleValidationCallback( pvFrameworkCtx, Test_EventHandleValidationCallback );
	if( NULL == FW_SchedInit( pvFrameworkCtx, APP_INTERNAL_EVENT_MAX, APP_EXTERNAL_EVENT_MAX, NULL, NULL, NULL, NULL ))
	{
		printf( "FW_SchedInit failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	h_MediaChain hRxChainList[100];
	h_MediaChain hTxChainList[100];

	int nTraceId = FW_GetTraceID( pvFrameworkCtx );
	FW_SetTraceForTraceID( pvFrameworkCtx, nTraceId );

	// Create sync timer
	t_SyncTimerConfig oTimerConfig;

	oTimerConfig.pvFw = pvFrameworkCtx;
	oTimerConfig.nTraceId = nTraceId;

	eRetCode = ST_Init( &oTimerConfig, &hTimer );
	ms_assert( MS_ERR_NONE == eRetCode );

	t_MediaCompConfig oChainConfig;

	oChainConfig.nTraceId = nTraceId;
	oChainConfig.pvFrameworkCtx = pvFrameworkCtx;

	// Create Mixer
	oMixerConfig.oConfig.nTraceId = nTraceId;
	oMixerConfig.oConfig.pvFrameworkCtx = pvFrameworkCtx;
	oMixerConfig.nBufferTimeMs = 100;
	oMixerConfig.nPacketizationTimeMs = 20;

	eRetCode = Media_CompCreate( MCT_MIXER, &oMixerConfig, &hMixer );
	ms_assert( MS_ERR_NONE == eRetCode );

	// Create chains
	int nCount = 0;

	for( nCount = 1; nCount < argc; nCount += 2 )
	{
		eRetCode = Media_ChainCreate( &oChainConfig, &hRxChainList[(nCount-1)/2] );
		ms_assert( MS_ERR_NONE == eRetCode );

		eRetCode = Media_ChainSetAppCb( hRxChainList[(nCount-1)/2], App_Media_ChainCb, (h_MediaCompAppCtx)hTimer, (h_MediaCompAppCtx)nTraceId );

		h_MediaComp hUdpRecv = NULL;
		h_MediaComp hRtpRecv = NULL;
		h_MediaComp hPcmuDec = NULL;
		h_MediaComp hPcmuEnc = NULL;
		h_MediaComp hRtpSend = NULL;
		h_MediaComp hUdpSend = NULL;

		// Create UDP recv component
		t_UdpRecvConfig oUdpRecvConfig;

		memset( &oUdpRecvConfig, 0, sizeof (t_UdpRecvConfig));
		oUdpRecvConfig.oConfig.nTraceId = nTraceId;
		oUdpRecvConfig.oConfig.pvFrameworkCtx = pvFrameworkCtx;
		strcpy( oUdpRecvConfig.pcLocalIp, "127.0.0.1" );
		oUdpRecvConfig.nLocalPort = atoi( argv[nCount] );
		oUdpRecvConfig.hUdpNetHandle = NULL;
		strcpy( oUdpRecvConfig.pcRemoteIp, "127.0.0.1" );
		oUdpRecvConfig.nRemotePort = 0;

		eRetCode = Media_CompCreate( MCT_UDPRECV, (h_CompConfig)&oUdpRecvConfig, &hUdpRecv );
		ms_assert( MS_ERR_NONE == eRetCode );

		eRetCode = Media_ChainAddInChain( hRxChainList[(nCount-1)/2], hUdpRecv );
		ms_assert( MS_ERR_NONE == eRetCode );

		// Create RTP recv component
		t_RtpRecvConfig oRtpRecvConfig;

		oRtpRecvConfig.oConfig.nTraceId = nTraceId;
		oRtpRecvConfig.oConfig.pvFrameworkCtx = pvFrameworkCtx;
		oRtpRecvConfig.ucPayloadType = 0;
		oRtpRecvConfig.ucDtmfPayloadType = 101;
		oRtpRecvConfig.nSamplingRate = 8000;
		oRtpRecvConfig.unMaxTimeStampJump = 500000;

		eRetCode = Media_CompCreate( MCT_RTPRECV, (h_CompConfig)&oRtpRecvConfig, &hRtpRecv );
		ms_assert( MS_ERR_NONE == eRetCode );

		eRetCode = Media_ChainAddInChain( hRxChainList[(nCount-1)/2], hRtpRecv );
		ms_assert( MS_ERR_NONE == eRetCode );

		// Create RTP recv component
		t_PcmuDecConfig oPcmuDecConfig;

		oPcmuDecConfig.oConfig.nTraceId = nTraceId;
		oPcmuDecConfig.oConfig.pvFrameworkCtx = pvFrameworkCtx;

		eRetCode = Media_CompCreate( MCT_PCMUDEC, (h_CompConfig)&oPcmuDecConfig, &hPcmuDec );
		ms_assert( MS_ERR_NONE == eRetCode );

		eRetCode = Media_ChainAddInChain( hRxChainList[(nCount-1)/2], hPcmuDec );
		ms_assert( MS_ERR_NONE == eRetCode );

		eRetCode = Media_ChainSetLink( hRxChainList[(nCount-1)/2], hUdpRecv, hRtpRecv );
		ms_assert( MS_ERR_NONE == eRetCode );
		eRetCode = Media_ChainSetLink( hRxChainList[(nCount-1)/2], hRtpRecv, hPcmuDec );
		ms_assert( MS_ERR_NONE == eRetCode );
		eRetCode = Media_ChainSetLink( hRxChainList[(nCount-1)/2], hPcmuDec, hMixer );
		ms_assert( MS_ERR_NONE == eRetCode );

		// Create TX chain
		eRetCode = Media_ChainCreate( &oChainConfig, &hTxChainList[(nCount-1)/2] );
		ms_assert( MS_ERR_NONE == eRetCode );

		eRetCode = Media_ChainSetAppCb( hTxChainList[(nCount-1)/2], App_Media_ChainCb, (h_MediaCompAppCtx)hTimer, (h_MediaCompAppCtx)nTraceId );

		// Create PCMU Encoder
		t_PcmuEncConfig oPcmuEncConfig;

		oPcmuEncConfig.oConfig.nTraceId = nTraceId;
		oPcmuEncConfig.oConfig.pvFrameworkCtx = pvFrameworkCtx;

		eRetCode = Media_CompCreate( MCT_PCMUENC, (h_CompConfig)&oPcmuEncConfig, &hPcmuEnc );
		ms_assert( MS_ERR_NONE == eRetCode );

		eRetCode = Media_ChainAddInChain( hTxChainList[(nCount-1)/2], hPcmuEnc );
		ms_assert( MS_ERR_NONE == eRetCode );

		// Create RTP sender
		t_RtpSendConfig oRtpSendConfig;

		oRtpSendConfig.oConfig.nTraceId = nTraceId;
		oRtpSendConfig.oConfig.pvFrameworkCtx = pvFrameworkCtx;
		oRtpSendConfig.nSamplingRate = 8000;

		eRetCode = Media_CompCreate( MCT_RTPSEND, (h_CompConfig)&oRtpSendConfig, &hRtpSend );
		ms_assert( MS_ERR_NONE == eRetCode );

		eRetCode = Media_ChainAddInChain( hTxChainList[(nCount-1)/2], hRtpSend );
		ms_assert( MS_ERR_NONE == eRetCode );

		// Create UDP sender
		t_UdpSendConfig oUdpSendConfig;

		memset( &oUdpSendConfig, 0, sizeof (t_UdpSendConfig));
		oUdpSendConfig.oConfig.nTraceId = nTraceId;
		oUdpSendConfig.oConfig.pvFrameworkCtx = pvFrameworkCtx;

		oUdpSendConfig.hUdpNetHandle = NULL;
		strcpy( oUdpSendConfig.pcLocalIp, "127.0.0.1" );
		oUdpSendConfig.nLocalPort = atoi( argv[nCount] )+1;
		strcpy( oUdpSendConfig.pcRemoteIp, "127.0.0.1" );
		oUdpSendConfig.nRemotePort = atoi( argv[nCount+1] );

		eRetCode = Media_CompCreate( MCT_UDPSEND, (h_CompConfig)&oUdpSendConfig, &hUdpSend );
		ms_assert( MS_ERR_NONE == eRetCode );

		eRetCode = Media_ChainAddInChain( hTxChainList[(nCount-1)/2], hUdpSend );
		ms_assert( MS_ERR_NONE == eRetCode );

		// Link components
		eRetCode = Media_ChainSetLink( hTxChainList[(nCount-1)/2], hPcmuEnc, hRtpSend );
		ms_assert( MS_ERR_NONE == eRetCode );
		eRetCode = Media_ChainSetLink( hTxChainList[(nCount-1)/2], hRtpSend, hUdpSend );
		ms_assert( MS_ERR_NONE == eRetCode );

		t_MixerNodeConfig oMixerNodeConfig;

		oMixerNodeConfig.eMode = MrM_FULL;
		oMixerNodeConfig.hSrcMediaComp = hPcmuDec;
		oMixerNodeConfig.hSinkMediaComp = hPcmuEnc;
		Media_ChainFillAppData( hTxChainList[(nCount-1)/2], hPcmuEnc, &oMixerNodeConfig.oSinkAppData );

		eRetCode = Media_CompSetParam( hMixer, MCC_MIXER_ADD_NODE, &oMixerNodeConfig );
		ms_assert( MS_ERR_NONE == eRetCode );

		eRetCode = Media_ChainSetStatus( hRxChainList[(nCount-1)/2], MCS_START );
		eRetCode = Media_ChainSetStatus( hTxChainList[(nCount-1)/2], MCS_START );
	}
	FW_Main( pvFrameworkCtx );

	for( nCount = 1; nCount < argc; nCount += 2 )
	{
		eRetCode = Media_ChainSetStatus( hRxChainList[(nCount-1)/2], MCS_STOP );
		ms_assert( MS_ERR_NONE == eRetCode );
		eRetCode = Media_ChainSetStatus( hTxChainList[(nCount-1)/2], MCS_STOP );
		ms_assert( MS_ERR_NONE == eRetCode );

		eRetCode = Media_ChainDestroy( hRxChainList[(nCount-1)/2] );
		ms_assert( MS_ERR_NONE == eRetCode );
		eRetCode = Media_ChainDestroy( hTxChainList[(nCount-1)/2] );
		ms_assert( MS_ERR_NONE == eRetCode );
	}
	eRetCode = Media_CompDestroy( hMixer );
	ms_assert( MS_ERR_NONE == eRetCode );

	FW_Cleanup( pvFrameworkCtx );

	POST_CONDITION
	{
	}

	return (AMPS_SUCCESS);
}

