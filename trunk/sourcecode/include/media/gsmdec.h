#ifndef _GSM_DEC_H_
#define _GSM_DEC_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_GsmDecConfig
{
	t_MediaCompConfig oConfig;
} t_GsmDecConfig;

e_MsRetCode GsmDecInit( h_CompConfig hCompConfig_i, h_MediaComp* phGsmDec_o );

e_MsRetCode GsmDecSetAppData( h_MediaComp hGsmDec_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode GsmDecSetStatus( h_MediaComp hGsmDec_i, e_MediaCompStatus eStatus );

e_MsRetCode GsmDecDestroy( h_MediaComp hGsmDec_i );

e_MsRetCode GsmDecAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode GsmDecAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode GsmDecSetParam( h_MediaComp hGsmDec_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

#endif /* _GSM_DEC_H_ */

