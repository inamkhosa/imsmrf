#ifndef _FILE_WRITE_PRIV_H_
#define _FILE_WRITE_PRIV_H_

#include "media/filewrite.h"

#define MCT_FILEWRITE_MAGIC_NUM 0x21436587

typedef struct _t_FileWrite
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;

	int nMagicNum;

	char*        pcFileName;
	unsigned int unMode;

	AMPS_FILE_HANDLE hFile;
	int              nByteWritten;
} t_FileWrite;

#endif /* _FILE_WRITE_PRIV_H_ */

