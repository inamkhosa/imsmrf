#ifndef _G729ENC_H_
#define _G729ENC_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_G729EncConfig
{
	t_MediaCompConfig oConfig;
} t_G729EncConfig;

e_MsRetCode G729EncInit( h_CompConfig hCompConfig_i, h_MediaComp* phG729Enc_o );

e_MsRetCode G729EncSetAppData( h_MediaComp hG729Enc_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode G729EncSetStatus( h_MediaComp hG729Enc_i, e_MediaCompStatus eStatus );

e_MsRetCode G729EncDestroy( h_MediaComp hG729Enc_i );

e_MsRetCode G729EncAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode G729EncAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

#endif /* _G729ENC_H_ */

