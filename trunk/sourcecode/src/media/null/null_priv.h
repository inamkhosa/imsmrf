#ifndef _NULL_PRIV_H_
#define _NULL_PRIV_H_

#include "media/null.h"

#define MCT_NULL_MAGIC_NUM 0x19823764

typedef struct _t_Null
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;
	int                nMagicNum;
} t_Null;

#endif /* _NULL_PRIV_H_ */

