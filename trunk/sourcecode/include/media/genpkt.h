#ifndef _GEN_PKT_H_
#define _GEN_PKT_H_

#include "media/media.h"
#include "ms_debug.h"

e_MsRetCode GenDataPktInit( h_DataPkt hDataPkt_i );

e_MsRetCode GenDataPktDelete( h_DataPkt hDataPkt_i );

e_MsRetCode AllocGenPkt( h_DataPkt* phDataPkt_io );

#endif  /* _GEN_PKT_H_ */

