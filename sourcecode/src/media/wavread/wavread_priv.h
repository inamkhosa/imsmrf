#ifndef _WAV_READ_PRIV_H_
#define _WAV_READ_PRIV_H_

#include <sndfile.h>

#include "media/wavread.h"

#include "media/genpkt.h"
#include "media/wavreadpkt.h"

#define MCT_WAVREAD_MAGIC_NUM 0x87654321

typedef struct _t_WavRead
{
	t_MediaComp oComp;

	t_MediaCompAppData oAppData;

	int nMagicNum;

	char*        pcFileName;
	unsigned int unReadSize;
//  unsigned int unMode;
	unsigned int unReadTimerMs;

	SNDFILE*      poFile;
	SF_INFO       oSfInfo;
	AMPS_TIMER_ID hReadTimerId;

	int          nLastPktGenTimeMs;
	unsigned int unNumReadsPerTimeout;
} t_WavRead;

#endif /* _WAV_READ_PRIV_H_ */

