#include "media/udppkt.h"

e_MsRetCode UdpDataPktInit( h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poDataPkt->oUdpPkt.pucData = NULL;
		poDataPkt->oUdpPkt.nLength = 0;
		memset( poDataPkt->oUdpPkt.pcRemoteIpAddress, 0, sizeof (poDataPkt->oUdpPkt.pcRemoteIpAddress));
		poDataPkt->oUdpPkt.nRemotePort = 0;
		memset( poDataPkt->oUdpPkt.pcLocalIpAddress, 0, sizeof (poDataPkt->oUdpPkt.pcLocalIpAddress));
		poDataPkt->oUdpPkt.nLocalPort = 0;
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode UdpDataPktDelete( h_DataPkt hDataPkt_i )
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
			memset( &poDataPkt->oUdpPkt, 0, sizeof (t_UdpPkt));
		}
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode AllocUdpPkt( h_DataPkt* phDataPkt_io )
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
		poDataPkt->eType = DPT_UDP_Packet;
		poDataPkt->pfnDataPktInit = UdpDataPktInit;
		poDataPkt->pfnDataPktDelete = UdpDataPktDelete;
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

e_MsRetCode AddrChangeSigPktInit( h_SigPkt hSigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = hSigPkt_i;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poSigPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		memset( poSigPkt->oAddrChangePkt.pcIp, 0, AMPS_MAX_IP_STRING_LENGTH );
		poSigPkt->oAddrChangePkt.nPort = 0;
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

e_MsRetCode AddrChangeSigPktDelete( h_DataPkt hSigPkt_i )
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
			memset( &poSigPkt->oAddrChangePkt, 0, sizeof (t_AddrChangePkt));
		}
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

