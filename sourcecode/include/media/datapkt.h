#ifndef _DATA_PKT_H_
#define _DATA_PKT_H_

#include "fw_api.h"
#include "ms_ret_code.h"

#include "librtp/rtp-packet.h"

typedef enum
{
	DPT_UDP_Packet = 1,
	DPT_RTP_Packet = 2,
	DPT_GEN_Packet = 3,
	DPT_CODEC_Packet = 4
} e_DataPktType;

typedef void*  h_DataPkt;

typedef e_MsRetCode (*f_DataPktInit)( h_DataPkt hDataPkt_i );
typedef e_MsRetCode (*f_DataPktDelete)( h_DataPkt hDataPkt_i );

typedef struct _t_UdpPkt
{
	unsigned char* pucData;
	int            nLength;
	char           pcLocalIpAddress[AMPS_MAX_IP_STRING_LENGTH];
	int            nLocalPort;
	char           pcRemoteIpAddress[AMPS_MAX_IP_STRING_LENGTH];
	int            nRemotePort;
} t_UdpPkt;

typedef struct _t_RtpPkt
{
	Rtp_Packet poRtpPkt;
} t_RtpPkt;

typedef struct _t_GenPkt
{
	unsigned char* pucData;
	int            nLength;
} t_GenPkt;

typedef struct _t_CodecPkt
{
	unsigned char* pucData;
	int            nLength;
	unsigned char  ucPayloadType;
	unsigned int   unNumSamples;
	unsigned int   unStartOfMedia;
} t_CodecPkt;

typedef struct _t_DataPkt
{
	e_DataPktType   eType;
	unsigned int    unRefCount;
	int             nPktGenTime;    // Time at which the packet is generated
	f_DataPktInit   pfnDataPktInit;
	f_DataPktDelete pfnDataPktDelete;
	void*           pvAppData;
	union
	{
		t_UdpPkt oUdpPkt;
		t_RtpPkt oRtpPkt;
		t_GenPkt oGenPkt;
		t_CodecPkt oCodecPkt;
	};
} t_DataPkt;

e_MsRetCode AllocDataPkt( h_DataPkt* phDataPkt_io );

int DataPkt_GenTimeCompare( void* pvData1_i, void* pvData2_i );

void DataPkt_PrintInfo( void* pvData_i, void* pvUserData_i );

void DataPkt_Delete( void* pvData_i, void* pvUserData_i );

#endif /* _DATA_PKT_H_ */

