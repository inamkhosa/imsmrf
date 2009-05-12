#ifndef _WAV_READ_H_
#define _WAV_READ_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_WavReadConfig
{
	t_MediaCompConfig oConfig;

	char*        pcFileName;
	unsigned int unReadSize;
//  unsigned int unMode;
	unsigned int unReadTimerMs;
	unsigned int unNumReadsPerTimeout;      // number of file-read when the timer expires
} t_WavReadConfig;

e_MsRetCode WavReadInit( h_CompConfig hCompConfig_i, h_MediaComp* phWavRead_o );

e_MsRetCode WavReadSetAppData( h_MediaComp hWavRead_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode WavReadSetStatus( h_MediaComp hWavRead_i, e_MediaCompStatus eStatus_i );

e_MsRetCode WavReadDestroy( h_MediaComp hWavRead_i );

int WavReadTimeOutNotifyCallBack( AMPS_HANDLE hAMPS_HANDLE_i, void* pvData_i );

e_MsRetCode WavReadAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode WavReadSetParam( h_MediaComp hWavRead_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

#endif /* _WAV_READ_H_ */

