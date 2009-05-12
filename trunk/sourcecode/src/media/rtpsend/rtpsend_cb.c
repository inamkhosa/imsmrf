#include "rtpsend_priv.h"

e_MsRetCode RtpSendAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_RtpSend* poRtpSend = hMediaCompAppCtx_i;

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_CODEC_Packet == poDataPkt->eType ||
		                  DPT_RTP_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );

		ms_assert_return( MCT_RTPSEND_MAGIC_NUM == poRtpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		unsigned char* pucData = NULL;
		int nLength = 0;
		unsigned char ucPayloadType = 0;
		unsigned int unStartOfMedia = 0;

		MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Packet type = %d\n", poDataPkt->eType );

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

			case DPT_CODEC_Packet:
				pucData = poDataPkt->oCodecPkt.pucData;
				nLength = poDataPkt->oCodecPkt.nLength;
				ucPayloadType = poDataPkt->oCodecPkt.ucPayloadType;
				unStartOfMedia = poDataPkt->oCodecPkt.unStartOfMedia;
				break;

			case DPT_RTP_Packet:
				pucData = rtp_packet_get_payload( poDataPkt->oRtpPkt.poRtpPkt );
				nLength = rtp_packet_get_payload_len( poDataPkt->oRtpPkt.poRtpPkt );
				ucPayloadType = rtp_packet_get_payload_type( poDataPkt->oRtpPkt.poRtpPkt );
				unStartOfMedia = rtp_packet_get_marker( poDataPkt->oRtpPkt.poRtpPkt );
				break;

			default:
				ms_assert_return( 0, eRetCode = MS_ERR_INVALID_PACKET );
		}

		Rtp_Packet poRtpPkt = NULL;
		//t_CodecPkt* poCodecPkt = &poDataPkt->oCodecPkt;

		MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Input data length = %d\n", nLength );
		MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Payload type = %d\n", ucPayloadType );
		MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Data pointer = %p\n", pucData );

		poRtpPkt = rtp_packet_new_allocate( nLength, 0, 0 );
		if( NULL != poRtpPkt )
		{
			poRtpSend->usSeqNo++;
			// Calculate RTP timestamp of next packet
			if( 0 == poRtpSend->nFirstPktGenTimeMs )
			{
				poRtpSend->nFirstPktGenTimeMs = poDataPkt->nPktGenTime;
			}
			MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Difference from fist pkt generation time = %d\n", poDataPkt->nPktGenTime - poRtpSend->nFirstPktGenTimeMs );

			poRtpSend->unTimestamp = poRtpSend->unFirstPktTimestamp + (poDataPkt->nPktGenTime - poRtpSend->nFirstPktGenTimeMs) * poRtpSend->nSamplingMultiplier;

			rtp_packet_set_csrc_count( poRtpPkt, 0 );
			rtp_packet_set_extension( poRtpPkt, 0 );
			rtp_packet_set_padding( poRtpPkt, 0 );
			rtp_packet_set_version( poRtpPkt, RTP_VERSION );
			rtp_packet_set_payload_type( poRtpPkt, ucPayloadType );
			rtp_packet_set_marker( poRtpPkt, unStartOfMedia );
			rtp_packet_set_ssrc( poRtpPkt, poRtpSend->unSsrc );
			rtp_packet_set_seq( poRtpPkt, poRtpSend->usSeqNo );
			rtp_packet_set_timestamp( poRtpPkt, poRtpSend->unTimestamp );

			MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "RTP packet with timestamp = %u, seq no = %u\n", poRtpSend->unTimestamp, poRtpSend->usSeqNo );

			memcpy( rtp_packet_get_payload( poRtpPkt ), pucData, nLength );

			// Make Data packet
			(poDataPkt->pfnDataPktDelete)( poDataPkt );

			AllocRtpPkt((h_DataPkt*)&poDataPkt );

			poDataPkt->oRtpPkt.poRtpPkt = poRtpPkt;

			poRtpSend->unPktsSent++;

			MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Data callback function = %p\n", poRtpSend->oAppData.pfAppDataCb );

			// Call data callback function of the next layer
			ms_assert_return( NULL != poRtpSend->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
			eRetCode = (poRtpSend->oAppData.pfAppDataCb)((h_MediaComp)poRtpSend, (h_MediaCompAppCtx)poRtpSend->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpSendAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_RtpSend* poRtpSend = hMediaCompAppCtx_i;

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poRtpSend, poRtpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

