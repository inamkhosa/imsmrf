#ifndef _PCMU_ENC_H_
#define _PCMU_ENC_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_PcmuEncConfig
{
	t_MediaCompConfig oConfig;
} t_PcmuEncConfig;

e_MsRetCode PcmuEncInit( h_CompConfig hCompConfig_i, h_MediaComp* phPcmuEnc_o );

e_MsRetCode PcmuEncSetAppData( h_MediaComp hPcmuEnc_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode PcmuEncSetStatus( h_MediaComp hPcmuEnc_i, e_MediaCompStatus eStatus );

e_MsRetCode PcmuEncDestroy( h_MediaComp hPcmuEnc_i );

e_MsRetCode PcmuEncAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode PcmuEncAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

#endif /* _PCMU_ENC_H_ */

