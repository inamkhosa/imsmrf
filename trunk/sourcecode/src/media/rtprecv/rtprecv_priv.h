#ifndef _RTP_RECV_PRIV_H_
#define _RTP_RECV_PRIV_H_

#include "media/rtprecv.h"
#include "media/rtppkt.h"

#define MCT_RTPRECV_MAGIC_NUM 0x12563478

typedef struct _t_RtpRecv
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;

	int nMagicNum;

	unsigned int  unMaxTimeStampJump;        // Maximum allowable jump in RTP time stamp without resyncing
	unsigned int  unFirstTimeStamp;          // Value of first timestamp that is received
	int           nFirstPktTime;          // Local time at which the first packet is received
	int           nSamplingDivisor;           // nSamplingDivisor = Sampling Rate(Hz)/ 1000 (ms)
	unsigned char ucPayloadType;        // payload type to receive
	unsigned char ucLastEvent;      // last event received
	unsigned char ucDtmfPayloadType;

	unsigned char ucLastPayloadType;    // payload type of the last packet received
	unsigned int  unPktsRecv;
} t_RtpRecv;

typedef struct _t_RtpDtmfPkt
{
	unsigned char  ucEvent;
	unsigned char  bfVolume : 6;
	unsigned char  bfReservered : 1;
	unsigned char  bfEnd : 1;
	unsigned short usDurationMs;
} t_RtpDtmfPkt;

#endif /* _RTP_RECV_PRIV_H_ */

