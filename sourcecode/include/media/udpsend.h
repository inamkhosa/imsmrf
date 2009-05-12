#ifndef _UDP_SEND_H_
#define _UDP_SEND_H_

#include "media/media.h"
#include "ms_debug.h"

#include "media/udppkt.h"

typedef struct _t_UdpSendConfig
{
	t_MediaCompConfig oConfig;

	char            pcLocalIp[AMPS_MAX_IP_STRING_LENGTH];
	int             nLocalPort;
	char            pcRemoteIp[AMPS_MAX_IP_STRING_LENGTH];
	int             nRemotePort;
	AMPS_NET_HANDLE hUdpNetHandle;
	AMPS_BOOL       bManipulateHandle;        // Whether this component should allocate/deallocate net-handle or not
} t_UdpSendConfig;

e_MsRetCode UdpSendInit( h_CompConfig hCompConfig_i, h_MediaComp* phUdpSend_o );

e_MsRetCode UdpSendSetAppData( h_MediaComp hUdpSend_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode UdpSendSetStatus( h_MediaComp hUdpSend_i, e_MediaCompStatus eStatus );

e_MsRetCode UdpSendDestroy( h_MediaComp hUdpSend_i );

e_MsRetCode UdpSendAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );

e_MsRetCode UdpSendAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode UdpSendSetParam( h_MediaComp hUdpSend_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

e_MsRetCode UdpSendGetParam( h_MediaComp hUdpSend_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

int UdpSendDataCallback( AMPS_HANDLE hAMPS_HANDLE_i, AMPS_APP_HANDLE hAMPS_APP_HANDLE_i, AMPS_NET_HANDLE hAMPS_NET_HANDLE_i, unsigned char* pucBuff_i, int nBuffLength_i, int nPort_i, char* pcIPAddress_i );

#endif /* _UDP_SEND_H_ */

