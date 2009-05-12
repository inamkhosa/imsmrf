#ifndef _NULL_H_
#define _NULL_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_NullConfig
{
	t_MediaCompConfig oConfig;
} t_NullConfig;

e_MsRetCode NullInit( h_CompConfig hCompConfig_i, h_MediaComp* phNull_o );

e_MsRetCode NullSetAppData( h_MediaComp hNull_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode NullSetStatus( h_MediaComp hNull_i, e_MediaCompStatus eStatus );

e_MsRetCode NullDestroy( h_MediaComp hNull_i );

e_MsRetCode NullAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode NullAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

#endif /* _NULL_H_ */

