#ifndef _UDP_RECV_H_
#define _UDP_RECV_H_

#include "media/media.h"
#include "ms_debug.h"

#include "media/udppkt.h"

typedef struct _t_UdpRecvConfig
{
	t_MediaCompConfig oConfig;
	char              pcLocalIp[AMPS_MAX_IP_STRING_LENGTH];
	int               nLocalPort;
	char              pcRemoteIp[AMPS_MAX_IP_STRING_LENGTH];
	int               nRemotePort;
	AMPS_NET_HANDLE   hUdpNetHandle;
	AMPS_BOOL         bManipulateHandle;        // Whether this component should allocate/deallocate net-handle or not
} t_UdpRecvConfig;

e_MsRetCode UdpRecvInit( h_CompConfig hCompConfig_i, h_MediaComp* phUdpRecv_o );

e_MsRetCode UdpRecvSetAppData( h_MediaComp hUdpRecv_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode UdpRecvSetStatus( h_MediaComp hUdpRecv_i, e_MediaCompStatus eStatus );

e_MsRetCode UdpRecvDestroy( h_MediaComp hUdpRecv_i );

e_MsRetCode UdpRecvGetParam( h_MediaComp hUdpRecv_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

e_MsRetCode UdpRecvSetParam( h_MediaComp hUdpRecv_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

#endif /* _UDP_RECV_H_ */

