#ifndef _TIME_QUEUE_PRIV_H_
#define _TIME_QUEUE_PRIV_H_

#include "media/timequeue.h"

#define MCT_TIMEQUEUE_MAGIC_NUM 0x41238567

typedef struct _t_TimeQueue
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;

	int nMagicNum;

	int           nTimerMs;
	AMPS_TIMER_ID hTimerId;

	t_AMPSDList* poPktList;
} t_TimeQueue;

int TimeQueuePktListFreeLinkDataCallback( void** ppvData_io );

#endif /* _TIME_QUEUE_PRIV_H_ */

