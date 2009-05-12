#ifndef _WAV_WRITE_H_
#define _WAV_WRITE_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_WavWriteConfig
{
	t_MediaCompConfig oConfig;

	char*        pcFileName;
	unsigned int unMode;
} t_WavWriteConfig;

e_MsRetCode WavWriteInit( h_CompConfig hCompConfig_i, h_MediaComp* phWavWrite_o );

e_MsRetCode WavWriteSetAppData( h_MediaComp hWavWrite_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode WavWriteSetStatus( h_MediaComp hWavWrite_i, e_MediaCompStatus eStatus );

e_MsRetCode WavWriteDestroy( h_MediaComp hWavWrite_i );

e_MsRetCode WavWriteAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode WavWriteAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode WavWriteGetParam( h_MediaComp hWavWrite_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

e_MsRetCode WavWriteSetParam( h_MediaComp hWavWrite_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

int WavWriteDataCallback( AMPS_HANDLE hAMPS_HANDLE_i, AMPS_APP_HANDLE hAMPS_APP_HANDLE_i, AMPS_NET_HANDLE hAMPS_NET_HANDLE_i, unsigned char* pucBuff_i, int nBuffLength_i, int nPort_i, char* pcIPAddress_i );

#endif /* _WAV_WRITE_H_ */

