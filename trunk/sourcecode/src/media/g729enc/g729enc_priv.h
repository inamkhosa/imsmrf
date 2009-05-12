#ifndef _G729ENC_PRIV_H_
#define _G729ENC_PRIV_H_

#include "codec/g729/typedef.h"
#include "codec/g729/basic_op.h"
#include "codec/g729/ld8k.h"

#include "media/g729enc.h"

#include "media/genpkt.h"
#include "media/rtppkt.h"
#include "media/udppkt.h"

#define MCT_G729ENC_MAGIC_NUM 0x43332313

typedef struct _t_G729Enc
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;
	int                nMagicNum;

	unsigned int unStartOfMedia;
	Word16       pn16_AnalysisParam[PRM_SIZE];          /* Analysis parameters.                  */
	Word16       pn16_SynthesisBuffer[L_FRAME];           /* Buffer for synthesis speech           */
	Word16       serial[SERIAL_SIZE];    /* Output bitstream buffer               */
} t_G729Enc;

#endif /* _G729ENC_PRIV_H_ */

