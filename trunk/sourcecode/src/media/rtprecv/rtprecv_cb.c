#include "rtprecv_priv.h"

e_MsRetCode RtpRecvAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_RtpRecv* poRtpRecv = hMediaCompAppCtx_i;

	MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poRtpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_UDP_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RTPRECV_MAGIC_NUM == poRtpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		Rtp_Packet poRtpPkt = NULL;
		unsigned char ucPType = 0;

		MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Packet info = %p(%d)\n", poDataPkt->oUdpPkt.pucData, poDataPkt->oUdpPkt.nLength );

		poRtpPkt = rtp_packet_new_take_data( poDataPkt->oUdpPkt.pucData, poDataPkt->oUdpPkt.nLength );
		// Check if rtp parsing is successful
		if( NULL != poRtpPkt )
		{
			poRtpRecv->unPktsRecv++;

			ucPType = rtp_packet_get_payload_type( poRtpPkt );

			MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Payload type of received pkt = %d\n", ucPType );
			// Check if packet has the same payload type, as it is configured to receive
			if( ucPType == poRtpRecv->ucPayloadType )
			{
				MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Payload type same as configured.\n" );

				poRtpRecv->ucLastPayloadType = ucPType;

				unsigned int unCurrTimeStamp = rtp_packet_get_timestamp( poRtpPkt );

				// delete the previous packet data
				(poDataPkt->pfnDataPktDelete)( poDataPkt );

				AllocRtpPkt((h_DataPkt*)&poDataPkt );

				// Populate Rtp packet information
				poDataPkt->oRtpPkt.poRtpPkt = poRtpPkt;
				// if first packet or timestamp makes a very long jump
				if( 0 == poRtpRecv->unFirstTimeStamp  || (unCurrTimeStamp - poRtpRecv->unFirstTimeStamp) > poRtpRecv->unMaxTimeStampJump )
				{
					MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Resyncing\n" );

					poRtpRecv->unFirstTimeStamp = unCurrTimeStamp;

					t_SigPkt oSigPkt;
					memset( &oSigPkt, 0, sizeof (t_SigPkt));

					oSigPkt.eType = SPT_CURRENT_SYNC_TIME_Packet;
					oSigPkt.oSyncTimePkt.nDelayMs = 10;     // minimum allowable packetization time

					ms_assert_return( NULL != poRtpRecv->oAppData.pfAppSignalCb, eRetCode = MS_ERR_NOT_INITIALIZED );
					eRetCode = (poRtpRecv->oAppData.pfAppSignalCb)((h_MediaComp)poRtpRecv, (h_MediaCompAppCtx)poRtpRecv->oAppData.hAppSignalCtx, (h_DataPkt)&oSigPkt );

					poRtpRecv->nFirstPktTime = oSigPkt.oSyncTimePkt.nSyncTimeMs;
				}
				poDataPkt->nPktGenTime = ((unCurrTimeStamp - poRtpRecv->unFirstTimeStamp) / poRtpRecv->nSamplingDivisor) + poRtpRecv->nFirstPktTime;

				MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "rtp time stamp = %u\n", rtp_packet_get_timestamp( poRtpPkt ));

				MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "rtp time stamp of first packet = %u\n", poRtpRecv->unFirstTimeStamp );

				MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "difference in timestamp = %u\n", rtp_packet_get_timestamp( poRtpPkt ) - poRtpRecv->unFirstTimeStamp );

				MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "time of first packet = %d\n", poRtpRecv->nFirstPktTime );

				MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Packet generated at time = %d\n", poDataPkt->nPktGenTime );

				MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "difference in packet generation time = %u\n", poDataPkt->nPktGenTime - poRtpRecv->nFirstPktTime );

				// Call data callback function of the next layer
				ms_assert_return( NULL != poRtpRecv->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
				eRetCode = (poRtpRecv->oAppData.pfAppDataCb)((h_MediaComp)poRtpRecv, (h_MediaCompAppCtx)poRtpRecv->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
			}
			// Check if it is a DTMF packet
			else if( poRtpRecv->ucDtmfPayloadType == ucPType )
			{
				MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "DTMF Event received\n" );

				// Delete the previous packet
				(poDataPkt->pfnDataPktDelete)( poDataPkt );
				FW_Free( poDataPkt );

				t_RtpDtmfPkt* poRtpDtmfPkt = (t_RtpDtmfPkt*)rtp_packet_get_payload( poRtpPkt );
				// Check if this event is different from the last event received
				if( poRtpDtmfPkt->ucEvent != poRtpRecv->ucLastEvent && 0 == poRtpDtmfPkt->bfEnd )
				{
					MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Event different for pervious event = %d, current event = %d\n", poRtpRecv->ucLastEvent, poRtpDtmfPkt->ucEvent );

					// Update last event
					poRtpRecv->ucLastEvent = poRtpDtmfPkt->ucEvent;

					// Allocate signal packet
					t_SigPkt* poSigPkt = FW_Malloc( sizeof (t_SigPkt));
					// Check if allocation is successful
					if( NULL != poSigPkt )
					{
						MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Signal packet allocated\n" );

						// Initialize signal packet
						poSigPkt->eType = SPT_DTMP_Packet;
						poSigPkt->pfnSignalPktInit =  DtmfSigPktInit;
						poSigPkt->pfnSignalPktDelete =  DtmfSigPktDelete;

						(poSigPkt->pfnSignalPktInit)( poSigPkt );

						// Populate values
						poSigPkt->oDtmfPkt.ucDtmfKey = poRtpDtmfPkt->ucEvent;
						poSigPkt->oDtmfPkt.ucVolume = (unsigned char)poRtpDtmfPkt->bfVolume;

						MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Singal callback function\n" );

						// Call signal callback function
						ms_assert_return( NULL != poRtpRecv->oAppData.pfAppSignalCb, eRetCode = MS_ERR_NOT_INITIALIZED );
						eRetCode = (poRtpRecv->oAppData.pfAppSignalCb)((h_MediaComp)poRtpRecv, (h_MediaCompAppCtx)poRtpRecv->oAppData.hAppSignalCtx, (h_DataPkt)poSigPkt );
					}
					else
					{
						eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
					}
				}
				else    // Event is same as the previous one
				{
					MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Event same as previous event\n" );
					// Check if event has ended
					if( 1 == poRtpDtmfPkt->bfEnd )
					{
						MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Event ended\n" );

						// Reset last event
						poRtpRecv->ucLastEvent = ucInvalidEvent_c;
					}
					else
					{
						MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Event not ended\n" );
					}
				}
				// Free memory
				rtp_packet_free( poRtpPkt );
			}
			else
			{
				MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Payload type different from configured , which is = %d\n", poRtpRecv->ucPayloadType );
				// Generate signal for the first time
				if( poRtpRecv->ucLastPayloadType != ucPType )
				{
					poRtpRecv->ucLastPayloadType = ucPType;

					// Allocate signal packet
					t_SigPkt* poSigPkt = FW_Malloc( sizeof (t_SigPkt));
					// Check if allocation is successful
					if( NULL != poSigPkt )
					{
						// Initialize signal packet
						poSigPkt->eType = SPT_PAYLOAD_TYPE_CHANGE_Packet;
						poSigPkt->pfnSignalPktInit =  PayloadChangeSigPktInit;
						poSigPkt->pfnSignalPktDelete =  PayloadChangeSigPktDelete;

						(poSigPkt->pfnSignalPktInit)( poSigPkt );

						// Populate values
						poSigPkt->oPayloadChangePkt.ucPayloadType = poRtpRecv->ucLastPayloadType;

						MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "SPT_PAYLOAD_TYPE_CHANGE_Packet Calling Singal callback function\n" );

						// Call signal callback function
						ms_assert_return( NULL != poRtpRecv->oAppData.pfAppSignalCb, eRetCode = MS_ERR_NOT_INITIALIZED );
						eRetCode = (poRtpRecv->oAppData.pfAppSignalCb)((h_MediaComp)poRtpRecv, (h_MediaCompAppCtx)poRtpRecv->oAppData.hAppSignalCtx, (h_DataPkt)poSigPkt );
					}
					else
					{
						MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_WARNING, "Could not generate signal: PAYLOAD_TYPE_CHANGE\n" );
					}
				}
				// Free memory
				rtp_packet_free( poRtpPkt );

				// Delete the previous packet
				(poDataPkt->pfnDataPktDelete)( poDataPkt );
				FW_Free( poDataPkt );

				eRetCode = MS_ERR_INVALID_PACKET;
			}
		}
		else
		{
			eRetCode = MS_ERR_INTERNAL;
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode RtpRecvAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_RtpRecv* poRtpRecv = hMediaCompAppCtx_i;

	MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poRtpRecv, poRtpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

