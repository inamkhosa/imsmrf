#include "fw_api.h"
#include "media/udprecv.h"
#include "media/level.h"

typedef enum
{
	APP_INTERNAL_EVENT_MAX
} e_RTP_InteranlEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX
} e_RTP_ExternalEvent;

e_MsRetCode Test_AppDataCb( h_MediaComp hUdpRecv_i, h_MediaCompAppCtx hUdpRecvAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode Test_AppSignalCb( h_MediaComp hUdpRecv_i, h_MediaCompAppCtx hUdpRecvAppCtx_i, h_SigPkt h_SigPkt_i );

int main( int argc, char* argv[] )
{
	void* pvAMPSContext = NULL;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	h_MediaComp hUdpRecv = NULL;
	t_UdpRecvConfig oUdpRecvConfig;
	t_MediaCompAppData oUdpRecvAppData;
	int nUdpRecvTraceId;

	h_MediaComp hLevel = NULL;
	t_LevelConfig oLevelConfig;
	t_MediaCompAppData oLevelAppData;
	int nLevelTraceId;
	if( argc != 2 )
	{
		printf( "usage %s silence_threshold_db\n", argv[0] );
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
	nLevelTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nLevelTraceId );

	oLevelConfig.oConfig.nTraceId = nLevelTraceId;
	oLevelConfig.oConfig.pvFrameworkCtx = pvAMPSContext;
	oLevelConfig.nSilenceThresholdDb = atof( argv[1] );

	eRetCode = LevelInit( &oLevelConfig, &hLevel );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "LevelInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	oUdpRecvAppData.hAppDataCtx = hLevel;
	oUdpRecvAppData.pfAppDataCb = LevelAppDataCb;
	oUdpRecvAppData.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = UdpRecvSetAppData( hUdpRecv, &oUdpRecvAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "UdpRecvSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	oLevelAppData.hAppDataCtx = NULL;
	oLevelAppData.pfAppDataCb = Test_AppDataCb;
	oLevelAppData.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = LevelSetAppData( hLevel, &oLevelAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "LevelSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	FW_Main( pvAMPSContext );

	printf( "Main: Leaving.\n" );

	FW_Cleanup( pvAMPSContext );

	return (AMPS_SUCCESS);
}

e_MsRetCode Test_AppDataCb( h_MediaComp hUdpRecv_i, h_MediaCompAppCtx hUdpRecvAppCtx_i, h_DataPkt hDataPkt_i )
{
	t_DataPkt* poDataPkt = (t_DataPkt*)hDataPkt_i;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	printf( "Test_AppDataCb: called\n" );

	(poDataPkt->pfnDataPktDelete)( poDataPkt );
	FW_Free( poDataPkt );

	return (eRetCode);
}

e_MsRetCode Test_AppSignalCb( h_MediaComp hUdpRecv_i, h_MediaCompAppCtx hUdpRecvAppCtx_i, h_SigPkt hSigPkt_i )
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

		case SPT_SILENCE_DETECTED_Packet:
		{
			printf( "SPT_SILENCE_DETECTED_Packet, volume(db) = %f\n", poSigPkt->oLevelPkt.dLevelDb );
			break;
		}

		case SPT_SOUND_DETECTED_Packet:
		{
			printf( "SPT_SOUND_DETECTED_Packet, volume(db) = %f\n", poSigPkt->oLevelPkt.dLevelDb );
			break;
		}

		default:
		{
			printf( "Unknown signal packet\n" );
		}
	}

	(poSigPkt->pfnSignalPktDelete)( poSigPkt );
	FW_Free( poSigPkt );

	return (eRetCode);
}

