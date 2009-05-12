#ifndef _FILE_WRITE_H_
#define _FILE_WRITE_H_

#include "media/media.h"
#include "ms_debug.h"

typedef struct _t_FileWriteConfig
{
	t_MediaCompConfig oConfig;

	char*        pcFileName;
	unsigned int unMode;
} t_FileWriteConfig;

e_MsRetCode FileWriteInit( h_CompConfig hCompConfig_i, h_MediaComp* phFileWrite_o );

e_MsRetCode FileWriteSetAppData( h_MediaComp hFileWrite_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode FileWriteSetStatus( h_MediaComp hFileWrite_i, e_MediaCompStatus eStatus );

e_MsRetCode FileWriteDestroy( h_MediaComp hFileWrite_i );

e_MsRetCode FileWriteAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode FileWriteAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode FileWriteGetParam( h_MediaComp hFileWrite_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

e_MsRetCode FileWriteSetParam( h_MediaComp hFileWrite_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

int FileWriteDataCallback( AMPS_HANDLE hAMPS_HANDLE_i, AMPS_APP_HANDLE hAMPS_APP_HANDLE_i, AMPS_NET_HANDLE hAMPS_NET_HANDLE_i, unsigned char* pucBuff_i, int nBuffLength_i, int nPort_i, char* pcIPAddress_i );

#endif /* _FILE_WRITE_H_ */

