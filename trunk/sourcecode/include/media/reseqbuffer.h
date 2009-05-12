#ifndef _RESEQBUFFER_H_
#define _RESEQBUFFER_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_ReseqBufferConfig
{
	t_MediaCompConfig oConfig;

	int nTimerMs;
} t_ReseqBufferConfig;

e_MsRetCode ReseqBufferInit( h_CompConfig hCompConfig_i, h_MediaComp* phReseqBuffer_o );

e_MsRetCode ReseqBufferSetAppData( h_MediaComp hReseqBuffer_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode ReseqBufferSetStatus( h_MediaComp hReseqBuffer_i, e_MediaCompStatus eStatus );

e_MsRetCode ReseqBufferDestroy( h_MediaComp hReseqBuffer_i );

int ReseqBufferTimeOutNotifyCallBack( AMPS_HANDLE hAMPS_HANDLE_i, void* pvData_i );

e_MsRetCode ReseqBufferAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode ReseqBufferAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

#endif /* _RESEQBUFFER_H_ */

