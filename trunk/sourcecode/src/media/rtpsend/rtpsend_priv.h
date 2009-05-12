#ifndef _RTP_SEND_PRIV_H_
#define _RTP_SEND_PRIV_H_

#include "librtp/rtp.h"
#include "media/rtpsend.h"

#include "media/codecpkt.h"
#include "media/rtppkt.h"

#define MCT_RTPSEND_MAGIC_NUM 0x87346512

typedef struct _t_RtpSend
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;

	int nMagicNum;

	unsigned short int usSeqNo;     // Sequence number of rtp packet
	unsigned int       unSsrc;            // RTP SSRC
	unsigned int       unTimestamp;       // RTP Timestamp
	int                nFirstPktGenTimeMs;     // Geneation time of first packet that is sent
	int                nSamplingMultiplier;    // Mulitiplier to change ms into samples (based on sampling rate)
	unsigned int       unPktsSent;
	unsigned int       unFirstPktTimestamp;   // Starting value of timestamp
} t_RtpSend;

#endif /* _RTP_SEND_PRIV_H_ */

