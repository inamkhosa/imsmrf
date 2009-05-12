#include "media/datapkt.h"
#include "media/genpkt.h"
#include "media/udppkt.h"
#include "media/rtppkt.h"
#include "media/codecpkt.h"

e_MsRetCode AllocDataPkt( h_DataPkt* phDataPkt_io )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = *phDataPkt_io;
	if( NULL != poDataPkt )
	{
		switch( poDataPkt->eType )
		{
			case DPT_GEN_Packet:
				eRetCode = AllocGenPkt( phDataPkt_io );
				break;

			case DPT_UDP_Packet:
				eRetCode = AllocUdpPkt( phDataPkt_io );
				break;

			case DPT_RTP_Packet:
				eRetCode = AllocRtpPkt( phDataPkt_io );
				break;

			case DPT_CODEC_Packet:
				eRetCode = AllocCodecPkt( phDataPkt_io );

			default:
				eRetCode = MS_ERR_INVALID_PACKET;
		}
	}
	else
	{
		eRetCode = MS_ERR_INVALID_PACKET;
	}
	POST_CONDITION
	{
	}

	return (eRetCode);
}

int DataPkt_GenTimeCompare( void* pvData1_i, void* pvData2_i )
{
	t_DataPkt* poPkt1 = pvData1_i;
	t_DataPkt* poPkt2 = pvData2_i;
	if( poPkt1->nPktGenTime > poPkt2->nPktGenTime )
	{
		return (-1);
	}
	else if( poPkt1->nPktGenTime == poPkt2->nPktGenTime )
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

void DataPkt_PrintInfo( void* pvData_i, void* pvUserData_i )
{
	int nTraceId = (int)pvUserData_i;
	t_DataPkt* poDataPkt = pvData_i;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Pkt = %p\n", poDataPkt );
	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Pkt Type = %d\n", poDataPkt->eType );
	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Pkt Generation Time= %d\n", poDataPkt->nPktGenTime );
}

void DataPkt_Delete( void* pvData_i, void* pvUserData_i )
{
	t_DataPkt* poPkt = pvData_i;

	(poPkt->pfnDataPktDelete)( poPkt );
	FW_Free( poPkt );
}

