#ifndef _GSM_DEC_PRIV_H_
#define _GSM_DEC_PRIV_H_

#include "codec/gsm/gsm.h"
#include "media/gsmdec.h"

#include "media/genpkt.h"
#include "media/rtppkt.h"
#include "media/udppkt.h"

#define MCT_GSMDEC_MAGIC_NUM 0x11223344

#define GSM610_BLOCKSIZE        33
#define GSM610_SAMPLES          160

typedef struct _t_GsmDec
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;
	int                nMagicNum;

	gsm poDecoder;
} t_GsmDec;

#endif /* _GSM_DEC_PRIV_H_ */

