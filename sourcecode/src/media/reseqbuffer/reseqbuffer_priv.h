#ifndef _TIME_QUEUE_PRIV_H_
#define _TIME_QUEUE_PRIV_H_

#include "media/reseqbuffer.h"

#define MCT_RESEQBUFFER_MAGIC_NUM 0x41322111

typedef struct _t_ReseqBuffer
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;

	int nMagicNum;

	int           nTimerMs;
	AMPS_TIMER_ID hTimerId;

	t_AMPSDList* poPktList;
} t_ReseqBuffer;

int ReseqBufferPktListFreeLinkDataCallback( void** ppvData_io );

int ReseqBufferGenTimeCompare( void* pvData1_i, void* pvData2_i );

#endif /* _TIME_QUEUE_PRIV_H_ */

