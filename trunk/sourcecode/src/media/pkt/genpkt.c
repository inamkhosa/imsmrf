#include "media/genpkt.h"

e_MsRetCode GenDataPktInit( h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poDataPkt->oGenPkt.pucData = NULL;
		poDataPkt->oGenPkt.nLength = 0;
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode GenDataPktDelete( h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( 0 == --poDataPkt->unRefCount )
		{
			if( NULL != poDataPkt->oGenPkt.pucData )
			{
				FW_Free( poDataPkt->oGenPkt.pucData );
			}
			memset( &poDataPkt->oGenPkt, 0, sizeof (t_GenPkt));
		}
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode AllocGenPkt( h_DataPkt* phDataPkt_io )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = *phDataPkt_io;
	if( NULL == poDataPkt )
	{
		// Allocate data packet
		poDataPkt = FW_Malloc( sizeof (t_DataPkt) );
	}
	if( NULL != poDataPkt )
	{
		// Initialize data packet
		poDataPkt->eType = DPT_GEN_Packet;
		poDataPkt->pfnDataPktInit = GenDataPktInit;
		poDataPkt->pfnDataPktDelete = GenDataPktDelete;
		poDataPkt->unRefCount = 1;
		poDataPkt->pvAppData = NULL;

		(poDataPkt->pfnDataPktInit)( poDataPkt );

		*phDataPkt_io = poDataPkt;
	}
	else
	{
		eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
	}
	POST_CONDITION
	{
	}

	return (eRetCode);
}

