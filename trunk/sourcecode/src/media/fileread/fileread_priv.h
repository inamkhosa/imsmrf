#ifndef _FILE_READ_PRIV_H_
#define _FILE_READ_PRIV_H_

#include <unistd.h>
#include "media/fileread.h"

#include "media/genpkt.h"
#include "media/filereadpkt.h"

#define MCT_FILEREAD_MAGIC_NUM 0x87654321

typedef struct _t_FileRead
{
	t_MediaComp oComp;

	t_MediaCompAppData oAppData;

	int nMagicNum;

	char*        pcFileName;
	unsigned int unReadSize;
//  unsigned int unMode;
	unsigned int unReadTimerMs;

	//AMPS_FILE_HANDLE hFile;
	FILE*         poFile;
	AMPS_TIMER_ID hReadTimerId;

	unsigned int unNumReadsPerTimeout;
} t_FileRead;

#endif /* _FILE_READ_PRIV_H_ */

