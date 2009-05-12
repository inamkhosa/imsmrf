#ifndef _PCMU_DEC_PRIV_H_
#define _PCMU_DEC_PRIV_H_

#include "codec/g711/g711.h"
#include "media/pcmudec.h"

#include "media/genpkt.h"
#include "media/rtppkt.h"
#include "media/udppkt.h"

#define MCT_PCMUDEC_MAGIC_NUM 0x12435687

typedef struct _t_PcmuDec
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;
	int                nMagicNum;
} t_PcmuDec;

#endif /* _PCMU_DEC_PRIV_H_ */

