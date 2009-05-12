#ifndef _PCMU_DEC_H_
#define _PCMU_DEC_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_PcmuDecConfig
{
	t_MediaCompConfig oConfig;
} t_PcmuDecConfig;

e_MsRetCode PcmuDecInit( h_CompConfig hCompConfig_i, h_MediaComp* phPcmuDec_o );

e_MsRetCode PcmuDecSetAppData( h_MediaComp hPcmuDec_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode PcmuDecSetStatus( h_MediaComp hPcmuDec_i, e_MediaCompStatus eStatus );

e_MsRetCode PcmuDecDestroy( h_MediaComp hPcmuDec_i );

e_MsRetCode PcmuDecAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode PcmuDecAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

#endif /* _PCMU_DEC_H_ */

