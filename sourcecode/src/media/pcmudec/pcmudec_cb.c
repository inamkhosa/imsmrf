#include "pcmudec_priv.h"

e_MsRetCode PcmuDecAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_PcmuDec* poPcmuDec = hMediaCompAppCtx_i;

	MEDIA_TRACE( poPcmuDec, poPcmuDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poPcmuDec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_GEN_Packet == poDataPkt->eType ||
		                  DPT_UDP_Packet == poDataPkt->eType ||
		                  DPT_RTP_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_PCMUDEC_MAGIC_NUM == poPcmuDec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		unsigned char* pucData = NULL;
		int nLength = 0;

		switch( poDataPkt->eType )
		{
			case DPT_GEN_Packet:
				pucData = poDataPkt->oGenPkt.pucData;
				nLength = poDataPkt->oGenPkt.nLength;
				break;

			case DPT_UDP_Packet:
				pucData = poDataPkt->oUdpPkt.pucData;
				nLength = poDataPkt->oUdpPkt.nLength;
				break;

			case DPT_RTP_Packet:
				pucData = rtp_packet_get_payload( poDataPkt->oRtpPkt.poRtpPkt );
				nLength = rtp_packet_get_payload_len( poDataPkt->oRtpPkt.poRtpPkt );
				break;

			default:
				ms_assert_return( 0, eRetCode = MS_ERR_INVALID_PACKET );
		}

		unsigned char* pucDecodeBuffer = NULL;

		// Allocate decode buffer, double the size of encoded data
		pucDecodeBuffer = (unsigned char*) FW_Malloc( nLength * 2 );
		// Check if allocation is successful
		if( NULL != pucDecodeBuffer )
		{
			MEDIA_TRACE( poPcmuDec, poPcmuDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Decoding buffer allocated successfully = %p\n", pucDecodeBuffer );
			// Decode data
			mulaw_dec( (char*)pucData, (char*)pucDecodeBuffer, nLength );

			// Delete the previous packet data
			(poDataPkt->pfnDataPktDelete)( poDataPkt );

			// Allocate Generic packet
			AllocGenPkt((h_DataPkt*)&poDataPkt );

			// Update data packet
			poDataPkt->oGenPkt.pucData = pucDecodeBuffer;
			poDataPkt->oGenPkt.nLength = nLength * 2;

			MEDIA_TRACE( poPcmuDec, poPcmuDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Data callback function = %p\n", poPcmuDec->oAppData.pfAppDataCb );

			// Call data callback function of the next layer
			ms_assert_return( NULL != poPcmuDec->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
			eRetCode = (poPcmuDec->oAppData.pfAppDataCb)((h_MediaComp)poPcmuDec, (h_MediaCompAppCtx)poPcmuDec->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poPcmuDec, poPcmuDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode PcmuDecAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_PcmuDec* poPcmuDec = hMediaCompAppCtx_i;

	MEDIA_TRACE( poPcmuDec, poPcmuDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}
	MEDIA_TRACE( poPcmuDec, poPcmuDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

