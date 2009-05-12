#ifndef _FILE_READ_H_
#define _FILE_READ_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_FileReadConfig
{
	t_MediaCompConfig oConfig;

	char*        pcFileName;
	unsigned int unReadSize;
//  unsigned int unMode;
	unsigned int unReadTimerMs;
	unsigned int unNumReadsPerTimeout;      // number of file-read when the timer expires
} t_FileReadConfig;

e_MsRetCode FileReadInit( h_CompConfig hCompConfig_i, h_MediaComp* phFileRead_o );

e_MsRetCode FileReadSetAppData( h_MediaComp hFileRead_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode FileReadSetStatus( h_MediaComp hFileRead_i, e_MediaCompStatus eStatus_i );

e_MsRetCode FileReadDestroy( h_MediaComp hFileRead_i );

int FileReadTimeOutNotifyCallBack( AMPS_HANDLE hAMPS_HANDLE_i, void* pvData_i );

e_MsRetCode FileReadAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode FileReadSetParam( h_MediaComp hFileRead_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

#endif /* _FILE_READ_H_ */

