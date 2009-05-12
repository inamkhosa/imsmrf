#ifndef _G729DEC_H_
#define _G729DEC_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_G729DecConfig
{
	t_MediaCompConfig oConfig;
} t_G729DecConfig;

e_MsRetCode G729DecInit( h_CompConfig hCompConfig_i, h_MediaComp* phG729Dec_o );

e_MsRetCode G729DecSetAppData( h_MediaComp hG729Dec_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode G729DecSetStatus( h_MediaComp hG729Dec_i, e_MediaCompStatus eStatus );

e_MsRetCode G729DecDestroy( h_MediaComp hG729Dec_i );

e_MsRetCode G729DecAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode G729DecAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

#endif /* _G729DEC_H_ */

