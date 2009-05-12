#ifndef _RTP_SEND_H_
#define _RTP_SEND_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_RtpSendConfig
{
	t_MediaCompConfig oConfig;
	int               nSamplingRate;
} t_RtpSendConfig;

e_MsRetCode RtpSendInit( h_CompConfig hCompConfig_i, h_MediaComp* phRtpSend_o );

e_MsRetCode RtpSendSetAppData( h_MediaComp hRtpSend_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode RtpSendSetStatus( h_MediaComp hRtpSend_i, e_MediaCompStatus eStatus );

e_MsRetCode RtpSendDestroy( h_MediaComp hRtpSend_i );

e_MsRetCode RtpSendAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode RtpSendAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode RtpSendSetParam( h_MediaComp hRtpSend_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

e_MsRetCode RtpSendGetParam( h_MediaComp hRtpSend_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

#endif /* _RTP_SEND_H_ */

