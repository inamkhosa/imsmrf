#ifndef _UDP_PKT_H_
#define _UDP_PKT_H_

#include "media/media.h"
#include "ms_debug.h"

e_MsRetCode UdpDataPktInit( h_DataPkt hDataPkt_i );

e_MsRetCode UdpDataPktDelete( h_DataPkt hDataPkt_i );

e_MsRetCode AllocUdpPkt( h_DataPkt* phDataPkt_io );

e_MsRetCode AddrChangeSigPktInit( h_SigPkt hSigPkt_i );

e_MsRetCode AddrChangeSigPktDelete( h_DataPkt hSigPkt_i );

#endif  /* _UDP_PKT_H_ */

