#include "fw_api.h"
#include "util/sync_timer.h"

typedef enum
{
	APP_INTERNAL_EVENT_MAX = 1
} e_AppInteranlEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX = 1
} e_AppExternalEvent;

AMPS_TIMER_ID hTimerId;
int nTimerMs;
int nDelayMs;

int Test_EventHandleValidationCallback( void* r_pvContext, int r_nEventID, void* r_pvEventPayload )
{
	return (AMPS_TRUE);
}

int AppTimeOutNotifyCallBack( AMPS_HANDLE hAMPS_HANDLE_i, void* pvData_i )
{
	h_SyncTimer hTimer1 = pvData_i;
	int nCurrSyncTime = 0;

	ST_GetCurrentTime( hTimer1, nDelayMs, &nCurrSyncTime );

	printf( "Sync Time = %d\n", nCurrSyncTime );

	hTimerId = FW_StartTimer( hAMPS_HANDLE_i, nTimerMs, AppTimeOutNotifyCallBack, pvData_i );
}

int main( int nArgc_i, char* ppcArgv_i[] )
{
	h_SyncTimer hTimer1 = NULL;
	t_SyncTimerConfig oTimer1Config;
	e_MsRetCode eRetCode = MS_ERR_NONE;
	if( 3 != nArgc_i )
	{
		printf( "%s timer delay\n", ppcArgv_i[0] );
		return (AMPS_ERROR_FAILURE);
	}
	nTimerMs = atoi( ppcArgv_i[1] );
	nDelayMs = atoi( ppcArgv_i[2] );

	void* pvAMPSContext = NULL;

	pvAMPSContext = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );
	if( NULL == pvAMPSContext )
	{
		printf( "FW_Init has failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	FW_SetTraceMode( pvAMPSContext, AMPS_TRACE_MODE_BOTH );
	FW_SetTraceLevel( pvAMPSContext, 31 );
	FW_EnableLogging( pvAMPSContext, "logfile.txt" );

	FW_EvtSysSetEvtHandleValidationCallback( pvAMPSContext, Test_EventHandleValidationCallback );
	if( NULL == FW_SchedInit( pvAMPSContext, APP_INTERNAL_EVENT_MAX, APP_EXTERNAL_EVENT_MAX, NULL, NULL, NULL, NULL ))
	{
		printf( "FW_SchedInit failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	//oTimer1Config.nDelayMs = nDelayMs;
	oTimer1Config.pvFw = pvAMPSContext;
	oTimer1Config.nTraceId = FW_GetTraceID( pvAMPSContext );
	FW_SetTraceForTraceID( pvAMPSContext, oTimer1Config.nTraceId );

	eRetCode = ST_Init( &oTimer1Config, &hTimer1 );
	if( MS_ERR_NONE != eRetCode )
	{
		printf( "ST_Init failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	hTimerId = FW_StartTimer( pvAMPSContext, nTimerMs, AppTimeOutNotifyCallBack, hTimer1 );

	FW_Main( pvAMPSContext );

	FW_DeleteTimer( pvAMPSContext, hTimerId  );

	FW_Cleanup( pvAMPSContext );

	ST_Destroy( hTimer1 );

	return (AMPS_SUCCESS);
}

