#ifndef _RTP_PKT_H_
#define _RTP_PKT_H_

#include "media/media.h"
#include "ms_debug.h"

extern const unsigned char ucInvalidEvent_c;

e_MsRetCode AllocRtpPkt( h_DataPkt* phDataPkt_io );

e_MsRetCode RtpDataPktInit( h_DataPkt hDataPkt_i );

e_MsRetCode RtpDataPktDelete( h_DataPkt hDataPkt_i );

// DTMF Signal packet functions
e_MsRetCode DtmfSigPktInit( h_SigPkt hSigPkt_i );

e_MsRetCode DtmfSigPktDelete( h_DataPkt hSigPkt_i );

e_MsRetCode PayloadChangeSigPktInit( h_SigPkt hSigPkt_i );

e_MsRetCode PayloadChangeSigPktDelete( h_DataPkt hSigPkt_i );

#endif  /* _RTP_PKT_H_ */

