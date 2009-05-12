#ifndef _MIXER_H_
#define _MIXER_H_

#include "media/media.h"
#include "ms_debug.h"

typedef enum
{
	MrM_FULL,
	MrM_PARKED,
	MrM_MUTE,
	MrM_PRIVATE,
	MrM_PREFERRED
} e_MixerMode;      // same as e_MixMode

typedef struct _t_MixerConfig
{
	t_MediaCompConfig oConfig;
	int               nBufferTimeMs;
	int               nPacketizationTimeMs;
} t_MixerConfig;

typedef struct _t_MixerNodeConfig
{
	e_MixerMode        eMode;
	h_MediaComp        hSrcMediaComp;
	h_MediaComp        hSinkMediaComp;
	t_MediaCompAppData oSinkAppData;
} t_MixerNodeConfig;

e_MsRetCode MixerInit( h_CompConfig hCompConfig_i, h_MediaComp* phMixer_o );

e_MsRetCode MixerSetAppData( h_MediaComp hMixer_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode MixerSetStatus( h_MediaComp hMixer_i, e_MediaCompStatus eStatus );

e_MsRetCode MixerDestroy( h_MediaComp hMixer_i );

e_MsRetCode MixerAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode MixerAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode MixerGetParam( h_MediaComp hMixer_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

e_MsRetCode MixerSetParam( h_MediaComp hMixer_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

#endif /* _MIXER_H_ */

