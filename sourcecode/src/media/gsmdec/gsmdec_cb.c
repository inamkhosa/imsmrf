#include "gsmdec_priv.h"

e_MsRetCode GsmDecAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_GsmDec* poGsmDec = hMediaCompAppCtx_i;

	MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poGsmDec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_GEN_Packet == poDataPkt->eType ||
		                  DPT_UDP_Packet == poDataPkt->eType ||
		                  DPT_RTP_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_GSMDEC_MAGIC_NUM == poGsmDec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
		MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Packet info = %p(%d)\n", pucData, nLength );

		unsigned char* pucDecodeBuffer = NULL;
		int nFrame = (float)nLength / 32.5;

		MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Processing %d frames\n", nFrame );
		// Allocate decode buffer, double the size of encoded data
		pucDecodeBuffer = (unsigned char*) FW_Malloc( nFrame * 160 * sizeof (gsm_signal) );
		// Check if allocation is successful
		if( NULL != pucDecodeBuffer )
		{
			MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Decoding buffer allocated successfully = %p\n", pucDecodeBuffer );
			// Decode data

			int nFrameCount = 0;

			while( nFrameCount < nFrame )
			{
				gsm_decode( poGsmDec->poDecoder, (gsm_byte*)(&pucData[nFrameCount * sizeof (gsm_frame)]), (gsm_signal*)(&pucDecodeBuffer[nFrameCount * 160 * sizeof (gsm_signal)]) );

				nFrameCount++;
			}
			// Delete the previous packet data
			(poDataPkt->pfnDataPktDelete)( poDataPkt );

			// Allocate Generic packet
			AllocGenPkt((h_DataPkt*)&poDataPkt );

			// Update data packet
			poDataPkt->oGenPkt.pucData = pucDecodeBuffer;
			poDataPkt->oGenPkt.nLength = nFrame * 160 * sizeof (gsm_signal);

			MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Data callback function = %p\n", poGsmDec->oAppData.pfAppDataCb );

			// Call data callback function of the next layer
			ms_assert_return( NULL != poGsmDec->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
			eRetCode = (poGsmDec->oAppData.pfAppDataCb)((h_MediaComp)poGsmDec, (h_MediaCompAppCtx)poGsmDec->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode GsmDecAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_GsmDec* poGsmDec = hMediaCompAppCtx_i;

	MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}
	MEDIA_TRACE( poGsmDec, poGsmDec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

