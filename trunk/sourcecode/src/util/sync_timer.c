#include "fw_api.h"
#include "util/sync_timer.h"

typedef struct _t_SyncTimer
{
	void* pvFw;
	int   nBaseTimeMs;
//	int nDelayMs;
	int nTraceId;
} t_SyncTimer;

e_MsRetCode ST_Init( t_SyncTimerConfig* poConfig_i, h_SyncTimer* phSyncTimer_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	{
		t_SyncTimer* poTimer = NULL;

		poTimer = FW_Malloc( sizeof (poTimer));
		if( NULL != poTimer )
		{
			poTimer->pvFw = poConfig_i->pvFw;
			//poTimer->nDelayMs = poConfig_i->nDelayMs;
			poTimer->nTraceId = poConfig_i->nTraceId;
			poTimer->nBaseTimeMs =  FW_GetCurrentTimeInMilliSec( poTimer->pvFw );

			*phSyncTimer_o = (h_SyncTimer)poTimer;
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	return (eRetCode);
}

e_MsRetCode ST_GetCurrentTime( h_SyncTimer hSyncTimer_i, int nDelayMs_i, int* pnCurrentTimeMs_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	{
		t_SyncTimer* poTimer = NULL;
		poTimer = (t_SyncTimer*)hSyncTimer_i;
		int nCurrentTimeMs = FW_GetCurrentTimeInMilliSec( poTimer->pvFw );

		int nCurrentDelayMs = (nCurrentTimeMs - poTimer->nBaseTimeMs) % nDelayMs_i;
		if( nCurrentDelayMs < (nDelayMs_i/2) )
		{
			*pnCurrentTimeMs_o = nCurrentTimeMs - nCurrentDelayMs;
		}
		else
		{
			*pnCurrentTimeMs_o = nCurrentTimeMs + (nDelayMs_i - nCurrentDelayMs);
		}
	}
	return (eRetCode);
}

/*e_MsRetCode ST_SetDelay(h_SyncTimer hSyncTimer_i, int nDelayMs_i)
{
e_MsRetCode eRetCode = MS_ERR_NONE;

{
t_SyncTimer* poTimer = NULL;
poTimer = (t_SyncTimer*)hSyncTimer_i;

poTimer->nDelayMs = nDelayMs_i;
}

return eRetCode;
}*/

e_MsRetCode ST_Destroy( h_SyncTimer hSyncTimer_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	{
		FW_Free( hSyncTimer_i );
	}
	return (eRetCode);
}

