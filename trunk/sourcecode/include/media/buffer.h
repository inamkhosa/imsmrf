#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_BufferConfig
{
	t_MediaCompConfig oConfig;

	int nEventId;
} t_BufferConfig;

e_MsRetCode BufferInit( h_CompConfig hCompConfig_i, h_MediaComp* phBuffer_o );

e_MsRetCode BufferSetAppData( h_MediaComp hBuffer_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode BufferSetStatus( h_MediaComp hBuffer_i, e_MediaCompStatus eStatus );

e_MsRetCode BufferDestroy( h_MediaComp hBuffer_i );

e_MsRetCode BufferAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode BufferAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

#endif /* _BUFFER_H_ */

