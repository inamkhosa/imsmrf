#ifndef _UDP_SEND_PRIV_H_
#define _UDP_SEND_PRIV_H_

#include "media/udpsend.h"

#define MCT_UDPSEND_MAGIC_NUM 0x87564312

typedef struct _t_UdpSend
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;

	int nMagicNum;

	char pcLocalIp[AMPS_MAX_IP_STRING_LENGTH];
	int  nLocalPort;
	char pcRemoteIp[AMPS_MAX_IP_STRING_LENGTH];
	int  nRemotePort;

	AMPS_NET_HANDLE hUdpNetHandle;
	AMPS_BOOL       bManipulateHandle;        // Whether this component should allocate/deallocate net-handle or not
} t_UdpSend;

int UdpSendDataCallback( AMPS_HANDLE hAMPS_HANDLE_i, AMPS_APP_HANDLE hAMPS_APP_HANDLE_i, AMPS_NET_HANDLE hAMPS_NET_HANDLE_i, unsigned char* pucBuff_i, int nBuffLength_i, int nPort_i, char* pcIPAddress_i );

#endif /* _UDP_SEND_PRIV_H_ */

