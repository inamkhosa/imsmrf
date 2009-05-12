#include "level_priv.h"

e_MsRetCode LevelAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_Level* poLevel = hMediaCompAppCtx_i;

	MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poLevel, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_GEN_Packet == poDataPkt->eType ||
		                  DPT_UDP_Packet == poDataPkt->eType ||
		                  DPT_RTP_Packet == poDataPkt->eType ||
		                  DPT_CODEC_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_LEVEL_MAGIC_NUM == poLevel->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		gdouble gdPeakSq = 0.0;
		double dLevelDb = 0.0;

		unsigned char* pucData = NULL;
		int nLength = 0;
		t_SigPkt* poSigPkt = NULL;

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

			case DPT_CODEC_Packet:
				pucData = poDataPkt->oCodecPkt.pucData;
				nLength = poDataPkt->oCodecPkt.nLength;
				break;

			default:
				ms_assert_return( 0, eRetCode = MS_ERR_INVALID_PACKET );
		}

		MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "packet length = %d\n", nLength );

		gst_level_calculate_gint16( pucData, nLength, 1, &gdPeakSq );

		MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "PeakSq = %f, ThresholdSq = %f\n", gdPeakSq, poLevel->gdSileceThresholdSq );
		if( gdPeakSq < poLevel->gdSileceThresholdSq )
		{
			MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Volume less than silence threshold\n" );
			// Check if sound was present previously
			if( AMPS_TRUE == poLevel->bSoundPresent )
			{
				dLevelDb = 10 * log10( gdPeakSq );

				MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Generating SILENCE_DETECTED event, level = %e(db)\n", dLevelDb );
				// Generate SILENCE_DETECTED event

				poSigPkt = FW_Malloc( sizeof (t_SigPkt));
				if( NULL != poSigPkt )
				{
					poSigPkt->eType = SPT_SILENCE_DETECTED_Packet;
					poSigPkt->pfnSignalPktInit = NULL;
					poSigPkt->pfnSignalPktDelete = NULL;
					poSigPkt->unRefCount = 1;
					poSigPkt->oLevelPkt.dLevelDb = dLevelDb;

					// Call signal callback function
					ms_assert_return( NULL != poLevel->oAppData.pfAppSignalCb, eRetCode = MS_ERR_NOT_INITIALIZED );
					eRetCode = (poLevel->oAppData.pfAppSignalCb)((h_MediaComp)poLevel, (h_MediaCompAppCtx)poLevel->oAppData.hAppDataCtx, (h_SigPkt)poSigPkt );
				}
				else
				{
					eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
				}
				poLevel->bSoundPresent = AMPS_FALSE;
			}
		}
		else
		{
			MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Volume greater than silence threshold\n" );
			// Check if sound was absent previously
			if( AMPS_FALSE == poLevel->bSoundPresent )
			{
				dLevelDb = 10 * log10( gdPeakSq );

				// Generate SOUND_DETECTED event

				MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Generating SOUND_DETECTED event, level = %f(db)\n", dLevelDb );

				poSigPkt = FW_Malloc( sizeof (t_SigPkt));
				if( NULL != poSigPkt )
				{
					poSigPkt->eType = SPT_SOUND_DETECTED_Packet;
					poSigPkt->pfnSignalPktInit = NULL;
					poSigPkt->pfnSignalPktDelete = NULL;
					poSigPkt->unRefCount = 1;
					poSigPkt->oLevelPkt.dLevelDb = dLevelDb;

					// Call signal callback function
					ms_assert_return( NULL != poLevel->oAppData.pfAppSignalCb, eRetCode = MS_ERR_NOT_INITIALIZED );
					eRetCode = (poLevel->oAppData.pfAppSignalCb)((h_MediaComp)poLevel, (h_MediaCompAppCtx)poLevel->oAppData.hAppSignalCtx, (h_SigPkt)poSigPkt );
				}
				else
				{
					eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
				}
				poLevel->bSoundPresent = AMPS_TRUE;
			}
		}
		MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Data callback function = %p\n", poLevel->oAppData.pfAppDataCb );

		// Call data callback function of the next layer
		ms_assert_return( NULL != poLevel->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
		eRetCode = (poLevel->oAppData.pfAppDataCb)((h_MediaComp)poLevel, (h_MediaCompAppCtx)poLevel->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode LevelAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_Level* poLevel = hMediaCompAppCtx_i;

	MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poLevel, poLevel->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

