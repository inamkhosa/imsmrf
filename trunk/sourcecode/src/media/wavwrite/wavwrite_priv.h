#ifndef _WAV_WRITE_PRIV_H_
#define _WAV_WRITE_PRIV_H_

#include <sndfile.h>

#include "media/wavwrite.h"

#define MCT_WAVWRITE_MAGIC_NUM 0x34217865

typedef struct _t_WavWrite
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;

	int nMagicNum;

	char*        pcFileName;
	unsigned int unMode;

	SNDFILE* poFile;
	SF_INFO  oSfInfo;
	int      nByteWritten;
} t_WavWrite;

#endif /* _WAV_WRITE_PRIV_H_ */

