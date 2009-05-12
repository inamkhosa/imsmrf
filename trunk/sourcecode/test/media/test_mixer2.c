#include "fw_api.h"
#include "media/mixer.h"
#include "media/fileread.h"
#include "media/filewrite.h"

typedef enum
{
	APP_INTERNAL_EVENT_MAX
} e_RTP_InteranlEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX
} e_RTP_ExternalEvent;

e_MsRetCode Test_AppSignalCb( h_MediaComp hUdpSend_i, h_MediaCompAppCtx hUdpSendAppCtx_i, h_SigPkt hSigPkt_i );

int main( int argc, char* argv[] )
{
	void* pvAMPSContext = NULL;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	h_MediaComp hMixer = NULL;
	t_MixerConfig oMixerConfig;
	t_MixerNode oMixerNode;
	int nMixerTraceId;

	h_MediaComp hFileWrite1 = NULL;
	t_FileWriteConfig oFileWriteConfig1;
	t_MediaCompAppData oFileWriteAppData1;
	int nFileWriteTraceId1;

	h_MediaComp hFileWrite2 = NULL;
	t_FileWriteConfig oFileWriteConfig2;
	t_MediaCompAppData oFileWriteAppData2;
	int nFileWriteTraceId2;

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

	eRetCode = Media_CompCreate( MCT_FILEREAD, &oFileReadConfig, &hFileRead );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileReadInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Initialize FileWrite1
	nFileWriteTraceId1 = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nFileWriteTraceId1 );

	oFileWriteConfig1.oConfig.nTraceId = nFileWriteTraceId1;
	oFileWriteConfig1.oConfig.pvFrameworkCtx = pvAMPSContext;
	oFileWriteConfig1.pcFileName = "output1.txt";
	oFileWriteConfig1.unMode = 1;

	eRetCode = Media_CompCreate( MCT_FILEWRITE, &oFileWriteConfig1, &hFileWrite1 );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileWriteInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Initialize FileWrite2
	nFileWriteTraceId2 = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nFileWriteTraceId2 );

	oFileWriteConfig2.oConfig.nTraceId = nFileWriteTraceId2;
	oFileWriteConfig2.oConfig.pvFrameworkCtx = pvAMPSContext;
	oFileWriteConfig2.pcFileName = "output2.txt";
	oFileWriteConfig2.unMode = 1;

	eRetCode = Media_CompCreate( MCT_FILEWRITE, &oFileWriteConfig2, &hFileWrite2 );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileWriteInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Initialize Mixer
	nMixerTraceId = FW_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nMixerTraceId );

	oMixerConfig.oConfig.nTraceId = nMixerTraceId;
	oMixerConfig.oConfig.pvFrameworkCtx = pvAMPSContext;

	eRetCode = Media_CompCreate( MCT_MIXER, &oMixerConfig, &hMixer );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "MixerInit: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	oMixerNode.hMediaComp = hFileRead;
	oMixerNode.eMode = (e_MixerMode)MrM_FULL;
	oMixerNode.oAppData.hAppDataCtx = hMixer;
	oMixerNode.oAppData.hAppSignalCtx = NULL;
	oMixerNode.oAppData.pfAppDataCb = MixerAppDataCb;
	oMixerNode.oAppData.pfAppSignalCb = NULL;

	eRetCode = Media_CompSetParam( hMixer, MCC_MIXER_ADD_SOURCE, &oMixerNode );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "MixerSetParam(MCC_MIXER_ADD_SOURCE): error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	oMixerNode.hMediaComp = hFileWrite1;
	oMixerNode.eMode = (e_MixerMode)MrM_FULL;

	//Media_ChainFillAppData(NULL, hMixer, &oAppData);

	oMixerNode.oAppData.hAppDataCtx = hFileWrite1;
	oMixerNode.oAppData.hAppSignalCtx = NULL;
	oMixerNode.oAppData.pfAppDataCb = FileWriteAppDataCb;
	oMixerNode.oAppData.pfAppSignalCb = NULL;

	eRetCode = Media_CompSetParam( hMixer, MCC_MIXER_ADD_SINK, &oMixerNode );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "MixerSetParam(MCC_MIXER_ADD_SINK): error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	oMixerNode.hMediaComp = hFileWrite2;
	oMixerNode.eMode = (e_MixerMode)MrM_FULL;
	oMixerNode.oAppData.hAppDataCtx = hFileWrite2;
	oMixerNode.oAppData.hAppSignalCtx = NULL;
	oMixerNode.oAppData.pfAppDataCb = FileWriteAppDataCb;
	oMixerNode.oAppData.pfAppSignalCb = NULL;

	eRetCode = Media_CompSetParam( hMixer, MCC_MIXER_ADD_SINK, &oMixerNode );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "MixerSetParam(MCC_MIXER_ADD_SINK): error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	oFileWriteAppData1.hAppDataCtx = NULL;
	oFileWriteAppData1.pfAppDataCb = NULL;
	oFileWriteAppData1.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = Media_CompSetAppData( hFileWrite1, &oFileWriteAppData1 );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileWriteSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	oFileWriteAppData2.hAppDataCtx = NULL;
	oFileWriteAppData2.pfAppDataCb = NULL;
	oFileWriteAppData2.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = Media_CompSetAppData( hFileWrite2, &oFileWriteAppData2 );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileWriteSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Configure FileRead
	oFileReadAppData.hAppDataCtx = hMixer;
	oFileReadAppData.pfAppDataCb = MixerAppDataCb;
	oFileReadAppData.pfAppSignalCb = Test_AppSignalCb;

	eRetCode = Media_CompSetAppData( hFileRead, &oFileReadAppData );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileReadSetAppData: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	// Set Status FileRead
	eRetCode = Media_CompSetStatus( hFileRead, MCS_START );
	if( eRetCode != MS_ERR_NONE )
	{
		printf( "FileReadSetStatus: error = %s\n", get_ms_ret_code_string( eRetCode ) );
		exit( -1 );
	}
	FW_Main( pvAMPSContext );

	printf( "Main: Leaving.\n" );

	Media_CompDestroy( hFileRead );
	Media_CompDestroy( hFileWrite1 );
	Media_CompDestroy( hFileWrite2 );
	Media_CompDestroy( hMixer );

	FW_Cleanup( pvAMPSContext );

	return (AMPS_SUCCESS);
}

e_MsRetCode Test_AppSignalCb( h_MediaComp hUdpSend_i, h_MediaCompAppCtx hUdpSendAppCtx_i, h_SigPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = (t_SigPkt*)hSigPkt_i;

	(poSigPkt->pfnSignalPktDelete)( poSigPkt );
	FW_Free( poSigPkt );

	return (eRetCode);
}

