#ifndef _LEVEL_H_
#define _LEVEL_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_LevelConfig
{
	t_MediaCompConfig oConfig;

	int nSilenceThresholdDb;
} t_LevelConfig;

e_MsRetCode LevelInit( h_CompConfig hCompConfig_i, h_MediaComp* phLevel_o );

e_MsRetCode LevelSetAppData( h_MediaComp hLevel_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode LevelSetStatus( h_MediaComp hLevel_i, e_MediaCompStatus eStatus );

e_MsRetCode LevelDestroy( h_MediaComp hLevel_i );

e_MsRetCode LevelAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode LevelAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode LevelSetParam( h_MediaComp hLevel_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

#endif /* _LEVEL_H_ */

