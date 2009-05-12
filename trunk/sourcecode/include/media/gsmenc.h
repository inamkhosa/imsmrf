#ifndef _GSM_ENC_H_
#define _GSM_ENC_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_GsmEncConfig
{
	t_MediaCompConfig oConfig;
} t_GsmEncConfig;

e_MsRetCode GsmEncInit( h_CompConfig hCompConfig_i, h_MediaComp* phGsmEnc_o );

e_MsRetCode GsmEncSetAppData( h_MediaComp hGsmEnc_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode GsmEncSetStatus( h_MediaComp hGsmEnc_i, e_MediaCompStatus eStatus );

e_MsRetCode GsmEncDestroy( h_MediaComp hGsmEnc_i );

e_MsRetCode GsmEncAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode GsmEncAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

#endif /* _GSM_ENC_H_ */

