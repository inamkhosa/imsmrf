#ifndef _RTP_RECV_H_
#define _RTP_RECV_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_RtpRecvConfig
{
	t_MediaCompConfig oConfig;

	int           nSamplingRate;      // Sampling Rate in Hz
	unsigned char ucPayloadType;
	unsigned char ucDtmfPayloadType;
	unsigned int  unMaxTimeStampJump;        // Maximum allowable jump in RTP time stamp
} t_RtpRecvConfig;

e_MsRetCode RtpRecvInit( h_CompConfig hCompConfig_i, h_MediaComp* phRtpRecv_o );

e_MsRetCode RtpRecvSetAppData( h_MediaComp hRtpRecv_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode RtpRecvSetStatus( h_MediaComp hRtpRecv_i, e_MediaCompStatus eStatus );

e_MsRetCode RtpRecvDestroy( h_MediaComp hRtpRecv_i );

e_MsRetCode RtpRecvAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode RtpRecvAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode RtpRecvSetParam( h_MediaComp hRtpRecv_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

e_MsRetCode RtpRecvGetParam( h_MediaComp hRtpRecv_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

#endif /* _RTP_RECV_H_ */

