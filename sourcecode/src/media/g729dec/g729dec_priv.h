#ifndef _G729DEC_PRIV_H_
#define _G729DEC_PRIV_H_

#include "codec/g729/typedef.h"
#include "codec/g729/basic_op.h"
#include "codec/g729/ld8k.h"

#include "media/g729dec.h"

#include "media/genpkt.h"
#include "media/rtppkt.h"
#include "media/udppkt.h"

#define MCT_G729DEC_MAGIC_NUM 0x42322212

typedef struct _t_G729Dec
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;
	int                nMagicNum;

	unsigned int unStartOfMedia;
	Word16       synth_buf[L_FRAME+M], * synth;           /* Buffer for synthesis speech           */
	Word16       parm[PRM_SIZE+1];          /* Analysis parameters.                  */
	Word16       serial[SERIAL_SIZE];          /* Serial stream               */
	Word16       pst_out[L_FRAME];             /* Postfilter output           */
	Word16       Az_dec[MP1*2], * ptr_Az;       /* Decoded Az for post-filter  */
	Word16       T0_first;                     /* Pitch lag in 1st subframe   */
	Word16       voicing;                      /* voicing from previous frame */
	Word16       sf_voic;                      /* voicing for subframe        */
} t_G729Dec;

#endif /* _G729DEC_PRIV_H_ */

