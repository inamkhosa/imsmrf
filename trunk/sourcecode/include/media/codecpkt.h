#ifndef _CODEC_PKT_H_
#define _CODEC_PKT_H_

#include "media/media.h"
#include "ms_debug.h"

e_MsRetCode CodecDataPktInit( h_DataPkt hDataPkt_i );

e_MsRetCode CodecDataPktDelete( h_DataPkt hDataPkt_i );

e_MsRetCode AllocCodecPkt( h_DataPkt* phDataPkt_io );

#endif  /* _CODEC_PKT_H_ */

