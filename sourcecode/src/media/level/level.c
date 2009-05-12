#include "level_priv.h"

void inline gst_level_calculate_gint16( gpointer data, guint num, guint channels, gdouble *NPS )
{
	gint16* in = (gint16*)data;
	register guint j;
	gdouble square = 0.0;     /* Square */
	gdouble peaksquare = 0.0; /* Peak Square Sample */
	gdouble normalizer;               /* divisor to get a [-1.0, 1.0] range */

	normalizer = (gdouble) (G_GINT64_CONSTANT( 1 ) << (15 * 2));

	for( j = 0; j < num; j += channels )
	{
		square = ((gdouble) in[j]) * in[j];
		if( square > peaksquare )
		{
			peaksquare = square;
		}
	}
	*NPS = peaksquare / normalizer;
}

