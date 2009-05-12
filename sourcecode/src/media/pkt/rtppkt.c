#include "media/rtppkt.h"

const unsigned char ucInvalidEvent_c = 255;

e_MsRetCode RtpDataPktInit( h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poDataPkt->oRtpPkt.poRtpPkt = NULL;
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode RtpDataPktDelete( h_DataPkt hDataPkt_i )
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
			rtp_packet_free( poDataPkt->oRtpPkt.poRtpPkt );
			memset( &poDataPkt->oRtpPkt, 0, sizeof (t_RtpPkt));
		}
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode DtmfSigPktInit( h_SigPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = hSigPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poSigPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poSigPkt->oDtmfPkt.ucDtmfKey = ucInvalidEvent_c;
		poSigPkt->oDtmfPkt.ucVolume = 0;
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode DtmfSigPktDelete( h_DataPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = hSigPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poSigPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( 0 == --poSigPkt->unRefCount )
		{
			memset( &poSigPkt->oDtmfPkt, 0, sizeof (t_DtmfPkt));
		}
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode AllocRtpPkt( h_DataPkt* phDataPkt_io )
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
		poDataPkt->eType = DPT_RTP_Packet;
		poDataPkt->pfnDataPktInit = RtpDataPktInit;
		poDataPkt->pfnDataPktDelete = RtpDataPktDelete;
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

e_MsRetCode PayloadChangeSigPktInit( h_SigPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = hSigPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poSigPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poSigPkt->oPayloadChangePkt.ucPayloadType = 0;
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode PayloadChangeSigPktDelete( h_DataPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = hSigPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poSigPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( 0 == --poSigPkt->unRefCount )
		{
			memset( &poSigPkt->oPayloadChangePkt, 0, sizeof (t_PayloadChangePkt));
		}
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

