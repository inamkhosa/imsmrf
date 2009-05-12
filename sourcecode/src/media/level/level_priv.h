#ifndef _LEVEL_PRIV_H_
#define _LEVEL_PRIV_H_

#include <glib.h>
#include <math.h>

#include "media/level.h"

#include "media/genpkt.h"

#define MCT_LEVEL_MAGIC_NUM 0x12345678

typedef struct _t_Level
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;

	int nMagicNum;

	gdouble   gdSileceThresholdSq;
	AMPS_BOOL bSoundPresent;        //
} t_Level;

void inline gst_level_calculate_gint16( gpointer data, guint num, guint channels, gdouble *NPS );

#endif /* _LEVEL_PRIV_H_ */

