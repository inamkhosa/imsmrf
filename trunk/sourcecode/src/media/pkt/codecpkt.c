#include "media/codecpkt.h"

e_MsRetCode CodecDataPktInit( h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poDataPkt->oCodecPkt.pucData = NULL;
		poDataPkt->oCodecPkt.nLength = 0;
		poDataPkt->oCodecPkt.ucPayloadType = 0;
		poDataPkt->oCodecPkt.unNumSamples = 0;
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode CodecDataPktDelete( h_DataPkt hDataPkt_i )
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
			if( NULL != poDataPkt->oCodecPkt.pucData )
			{
				FW_Free( poDataPkt->oCodecPkt.pucData );
			}
			memset( &poDataPkt->oCodecPkt, 0, sizeof (t_CodecPkt));
		}
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode AllocCodecPkt( h_DataPkt* phDataPkt_io )
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
		poDataPkt->eType = DPT_CODEC_Packet;
		poDataPkt->pfnDataPktInit = CodecDataPktInit;
		poDataPkt->pfnDataPktDelete = CodecDataPktDelete;
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

