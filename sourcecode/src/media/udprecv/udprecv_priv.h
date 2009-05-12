#ifndef _UDP_RECV_PRIV_H_
#define _UDP_RECV_PRIV_H_

#include "media/udprecv.h"

#define MCT_UDPRECV_MAGIC_NUM 0x86754231

typedef struct _t_UdpRecv
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;

	int nMagicNum;

	char pcLocalIp[AMPS_MAX_IP_STRING_LENGTH];
	int  nLocalPort;

	struct in_addr oLastRemoteAddr;
	int nLastRemotePort;

	AMPS_NET_HANDLE hUdpNetHandle;
	AMPS_BOOL       bManipulateHandle;
} t_UdpRecv;

int UdpRecvDataCallback( AMPS_HANDLE hAMPS_HANDLE_i, AMPS_APP_HANDLE hAMPS_APP_HANDLE_i, AMPS_NET_HANDLE hAMPS_NET_HANDLE_i, unsigned char* pucBuff_i, int nBuffLength_i, int nPort_i, char* pcIPAddress_i );

#endif /* _UDP_RECV_PRIV_H_ */

