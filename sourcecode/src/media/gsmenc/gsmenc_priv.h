#ifndef _GSM_ENC_PRIV_H_
#define _GSM_ENC_PRIV_H_

#include "codec/gsm/gsm.h"
#include "media/gsmenc.h"

#include "media/genpkt.h"
#include "media/rtppkt.h"
#include "media/udppkt.h"

#define MCT_GSMENC_MAGIC_NUM 0x44332211

typedef struct _t_GsmEnc
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;
	int                nMagicNum;

	gsm          poEncoder;
	unsigned int unStartOfMedia;
} t_GsmEnc;

#endif /* _GSM_ENC_PRIV_H_ */

