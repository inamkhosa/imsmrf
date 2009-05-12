#include "gsmenc_priv.h"

e_MsRetCode GsmEncAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_GsmEnc* poGsmEnc = hMediaCompAppCtx_i;

	MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poGsmEnc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_GEN_Packet == poDataPkt->eType ||
		                  DPT_UDP_Packet == poDataPkt->eType ||
		                  DPT_RTP_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_GSMENC_MAGIC_NUM == poGsmEnc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
		float fEncodeLength = 0;
		int nSignalLength = 0;
		if( nLength >= 320 )
		{
			pucEncodeBuffer = (unsigned char*) FW_Malloc( sizeof (gsm_frame) * (nLength/320) );

			while( nSignalLength < nLength )
			{
				// Encode data
				gsm_encode( poGsmEnc->poEncoder, (gsm_signal*)(&pucData[nSignalLength]), (gsm_byte*)(&pucEncodeBuffer[(int)ceil( fEncodeLength )]) );

				fEncodeLength += 32.5;

				nSignalLength += 320;
			}
			// Delete the previous packet data
			(poDataPkt->pfnDataPktDelete)( poDataPkt );

			// Change packet type to CODEC
			AllocCodecPkt((h_DataPkt*)&poDataPkt );

			poDataPkt->oCodecPkt.ucPayloadType = 3;    // GSM
			poDataPkt->oCodecPkt.unNumSamples = nLength;
			poDataPkt->oCodecPkt.unStartOfMedia = poGsmEnc->unStartOfMedia;
			poDataPkt->oCodecPkt.nLength = (int)ceil( fEncodeLength );
			poDataPkt->oCodecPkt.pucData = pucEncodeBuffer;

			poGsmEnc->unStartOfMedia = 0;

			MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Encoded data length = %d\n", poDataPkt->oCodecPkt.nLength );
			MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Number of samples   = %d\n", poDataPkt->oCodecPkt.unNumSamples );

			MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Data callback function = %p\n", poGsmEnc->oAppData.pfAppDataCb );

			// Call data callback function of the next layer
			ms_assert_return( NULL != poGsmEnc->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
			eRetCode = (poGsmEnc->oAppData.pfAppDataCb)((h_MediaComp)poGsmEnc, (h_MediaCompAppCtx)poGsmEnc->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
		}
		else
		{
			eRetCode = MS_ERR_INVALID_PACKET;
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode GsmEncAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_GsmEnc* poGsmEnc = hMediaCompAppCtx_i;

	MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}
	MEDIA_TRACE( poGsmEnc, poGsmEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

