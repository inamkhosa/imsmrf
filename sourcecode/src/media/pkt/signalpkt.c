#include "media/signalpkt.h"

int Signal_LListRemoveLinkDataCallback( void** ppvData_i )
{
	t_SigPkt* poSigPkt = *ppvData_i;

	(poSigPkt->pfnSignalPktDelete)( poSigPkt );
	FW_Free( poSigPkt );

	*ppvData_i = NULL;

	return (AMPS_TRUE);
}

