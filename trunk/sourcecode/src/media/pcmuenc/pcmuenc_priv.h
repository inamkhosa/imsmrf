#ifndef _PCMU_ENC_PRIV_H_
#define _PCMU_ENC_PRIV_H_

#include "codec/g711/g711.h"
#include "media/pcmuenc.h"
#include "media/codecpkt.h"

#define MCT_PCMUENC_MAGIC_NUM 0x78653412

typedef struct _t_PcmuEnc
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;
	int                nMagicNum;

	unsigned int unStartOfMedia;
} t_PcmuEnc;

#endif /* _PCMU_ENC_PRIV_H_ */

