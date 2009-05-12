#ifndef _TIME_QUEUE_H_
#define _TIME_QUEUE_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_TimeQueueConfig
{
	t_MediaCompConfig oConfig;

	int nTimerMs;
} t_TimeQueueConfig;

e_MsRetCode TimeQueueInit( h_CompConfig hCompConfig_i, h_MediaComp* phTimeQueue_o );

e_MsRetCode TimeQueueSetAppData( h_MediaComp hTimeQueue_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode TimeQueueSetStatus( h_MediaComp hTimeQueue_i, e_MediaCompStatus eStatus );

e_MsRetCode TimeQueueDestroy( h_MediaComp hTimeQueue_i );

int TimeQueueTimeOutNotifyCallBack( AMPS_HANDLE hAMPS_HANDLE_i, void* pvData_i );

e_MsRetCode TimeQueueAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode TimeQueueAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

#endif /* _TIME_QUEUE_H_ */

