#include "g729enc_priv.h"

e_MsRetCode G729EncAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_G729Enc* poG729Enc = hMediaCompAppCtx_i;

	MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poG729Enc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_GEN_Packet == poDataPkt->eType ||
		                  DPT_UDP_Packet == poDataPkt->eType ||
		                  DPT_RTP_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_G729ENC_MAGIC_NUM == poG729Enc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		unsigned char* pucData = NULL;
		int nLength = 0;
		unsigned char* pucDecodeBuffer = NULL;

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

		unsigned char* pucEncodeBuffer = NULL;
		int nEncodeLength = 0;
		int nSignalLength = 0;
		if( nLength >= (L_FRAME * 2) )
		{
			pucEncodeBuffer = (unsigned char*) FW_Malloc( M * (nLength/ (L_FRAME * 2) ) );

			MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Encoder buffer pointer = %p\n", pucEncodeBuffer );

			while( nSignalLength < nLength )
			{
				//Encode data
				MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Encoding frame number = %d\n", nEncodeLength / M + 1 );

				Pre_Process((Word16*)(&pucData[nSignalLength]), L_FRAME );

				Coder_ld8k( poG729Enc->pn16_AnalysisParam, poG729Enc->pn16_SynthesisBuffer );

				prm2bits_ld8k( poG729Enc->pn16_AnalysisParam, poG729Enc->serial );

				memcpy( &pucEncodeBuffer[nEncodeLength], poG729Enc->serial, M );

				nEncodeLength += M;
				nSignalLength += (L_FRAME * 2);
			}
			//Delete the previous packet data
			(poDataPkt->pfnDataPktDelete)( poDataPkt );

			//Change packet type to CODEC
			AllocCodecPkt((h_DataPkt*)&poDataPkt );

			poDataPkt->oCodecPkt.ucPayloadType = 18;    // G729
			poDataPkt->oCodecPkt.unNumSamples = nLength;
			poDataPkt->oCodecPkt.unStartOfMedia = poG729Enc->unStartOfMedia;
			poDataPkt->oCodecPkt.nLength = nEncodeLength;
			poDataPkt->oCodecPkt.pucData = pucEncodeBuffer;

			poG729Enc->unStartOfMedia = 0;

			MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Encoded data length = %d\n", poDataPkt->oCodecPkt.nLength );
			MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Number of samples   = %d\n", poDataPkt->oCodecPkt.unNumSamples );

			MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Data callback function = %p\n", poG729Enc->oAppData.pfAppDataCb );

			//Call data callback function of the next layer
			ms_assert_return( NULL != poG729Enc->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
			eRetCode = (poG729Enc->oAppData.pfAppDataCb)((h_MediaComp)poG729Enc, (h_MediaCompAppCtx)poG729Enc->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
		}
		else
		{
			eRetCode = MS_ERR_INVALID_PACKET;
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode G729EncAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_G729Enc* poG729Enc = hMediaCompAppCtx_i;

	MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}
	MEDIA_TRACE( poG729Enc, poG729Enc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

