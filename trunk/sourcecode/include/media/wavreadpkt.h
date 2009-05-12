#ifndef _WAVREAD_PKT_H_
#define _WAVREAD_PKT_H_

#include "media/media.h"
#include "ms_debug.h"

e_MsRetCode WavReadSigPktInit( h_SigPkt hSigPkt_i );

e_MsRetCode WavReadSigPktDelete( h_DataPkt hSigPkt_i );

#endif  /* _WAVREAD_PKT_H_ */

