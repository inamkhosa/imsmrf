#ifndef _SYNC_TIMER_H_
#define _SYNC_TIMER_H_

#include "ms_ret_code.h"
#include "ms_debug.h"

typedef void* h_SyncTimer;

typedef struct _t_SyncTimerConfig
{
	void* pvFw;
//	int nDelayMs;
	int nTraceId;
} t_SyncTimerConfig;

e_MsRetCode ST_Init( t_SyncTimerConfig* poConfig_i, h_SyncTimer* phSyncTimer_o );

e_MsRetCode ST_GetCurrentTime( h_SyncTimer hSyncTimer_i, int nDelayMs_i, int* pnCurrentTimeMs_o );

//e_MsRetCode ST_SetDelay(h_SyncTimer hSyncTimer_i, int nDelayMs_i);

e_MsRetCode ST_Destroy( h_SyncTimer hSyncTimer_i );

#endif /* _SYNC_TIMER_H_ */

