#include "mixer_priv.h"

e_MsRetCode MixerAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_Mixer* poMixer = hMediaCompAppCtx_i;

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMixer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_MIXER_MAGIC_NUM == poMixer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_GEN_Packet == poDataPkt->eType ||
		                  DPT_UDP_Packet == poDataPkt->eType ||
		                  DPT_RTP_Packet == poDataPkt->eType ||
		                  DPT_CODEC_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
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

			case DPT_CODEC_Packet:
				pucData = poDataPkt->oCodecPkt.pucData;
				nLength = poDataPkt->oCodecPkt.nLength;
				break;

			default:
				ms_assert_return( 0, eRetCode = MS_ERR_INVALID_PACKET );
		}

		MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "packet length = %d\n", nLength );

		t_CirList* poNode = NULL;
		t_MixerNode* poSrcNode = NULL;

		// Find the source of packet
		poNode = cir_list_find_custom( poMixer->poNodeList, hMediaComp_i, MixerNode_CompareSrc );
		if( NULL != poNode )
		{
			poSrcNode = poNode->pvData;

			MixerNode_SendPacket((cir_list_next( poNode ))->pvData, poDataPkt );

/*			int nCurrentTimeMs = MixerGetCurrentSyncTimeMs( poMixer, poSrcNode );
// if pkt is not too old
if( poDataPkt->nPktGenTime >= (nCurrentTimeMs - poMixer->nBufferTimeMs) )
{
MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Packet %p stored in input queue of node = %p\n", poDataPkt, poSrcNode );

// store the pkt in input queue
poSrcNode->poInPktList = cir_list_insert_sorted( poSrcNode->poInPktList, poDataPkt, DataPkt_GenTimeCompare );

poDataPkt = NULL;
}
else
{
MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_WARNING, "Packet discarded because it is too old =  %p of node =  %p\n", poDataPkt, poSrcNode );
}
MixerMix( poMixer, nCurrentTimeMs );*/
		}
		else
		{
			MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Packet received from a component that is not a mixer source\n" );
		}
		if( NULL != poDataPkt )
		{
			(poDataPkt->pfnDataPktDelete)( poDataPkt );
			FW_Free( poDataPkt );
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

void MixerNode_SendPacket( void* pvData_i, void* pvUserData_i )
{
	t_MixerNode* poMixNode = pvData_i;
	t_DataPkt* poDataPkt = pvUserData_i;
	t_DataPkt* poMixDataPkt = NULL;
	t_Mixer* poMixer = poMixNode->hMixer;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	eRetCode = AllocGenPkt((h_DataPkt*)&poMixDataPkt );
	if( MS_ERR_NONE == eRetCode )
	{
		poMixDataPkt->oGenPkt.pucData = FW_Malloc( poDataPkt->oGenPkt.nLength );
		if( NULL != poMixDataPkt->oGenPkt.pucData )
		{
			poMixDataPkt->oGenPkt.nLength = poDataPkt->oGenPkt.nLength;
			memcpy( poMixDataPkt->oGenPkt.pucData, poDataPkt->oGenPkt.pucData, poDataPkt->oGenPkt.nLength );

			poMixDataPkt->nPktGenTime = poDataPkt->nPktGenTime;
			// Send packet
			ms_assert_return( NULL != poMixNode->oNodeConfig.oSinkAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
			eRetCode = (poMixNode->oNodeConfig.oSinkAppData.pfAppDataCb)( poMixer, (h_MediaCompAppCtx)poMixNode->oNodeConfig.oSinkAppData.hAppDataCtx, (h_DataPkt)poMixDataPkt );
		}
	}
	else
	{
		eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
	}

	POST_CONDITION
	{
	}
}

e_MsRetCode MixerAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_Mixer* poMixer = hMediaCompAppCtx_i;

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

void MixerNode_GetPacket( void* pvData_i, void* pvUserData_i )
{
	t_MixerNode* poMixNode = pvData_i;
	t_MixData* poMixData = pvUserData_i;

	MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );
	MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Getting packet for time = %d\n",  poMixData->nMixTimeMs );

	{
		AMPS_BOOL bMixPkt = AMPS_FALSE;
		if( /*poMixNode->oNodeConfig.eMode != MrM_MUTE  && */ AMPS_TRUE == poMixData->bMixPkt )
		{
			t_CirList* poNode = NULL;
			t_DataPkt* poPkt = NULL;
			if( NULL != poMixNode->poInPktList )
			{
				poNode = cir_list_first( poMixNode->poInPktList );

				do
				{
					poPkt = poNode->pvData;

					MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Pkt %p Generation Time = %d\n",  poPkt, poPkt->nPktGenTime );
					MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Packet %p is generated %d ms ago\n", poPkt, poMixData->nCurrentTimeMs - poPkt->nPktGenTime );

					//MEDIA_TRACE(poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Buffer Time = %d\n",  poMixData->nBufferTimeMs);

					MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Mix_Time -  Gen_Time = %d\n",  poMixData->nMixTimeMs - poPkt->nPktGenTime );
					if( poPkt->nPktGenTime <= (poMixData->nCurrentTimeMs -  poMixData->nBufferTimeMs) )
					{
						MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Pkt %p is too old to be mixed at generation time = %d\n", poPkt, poPkt->nPktGenTime );

						// Delete packet
						poNode = cir_list_remove_link( cir_list_first( poMixNode->poInPktList ), poNode );

						DataPkt_Delete( poPkt, NULL );
					}
					else if( poPkt->nPktGenTime == poMixData->nMixTimeMs )
					{
						bMixPkt = AMPS_TRUE;

						MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Pkt %p is candidate for mixing at generation time = %d\n", poPkt, poPkt->nPktGenTime );

						// Add the packet in mix list
						cir_list_append( poMixData->poMixPktList, poPkt );
						break;
					}
					else if( poPkt->nPktGenTime > poMixData->nMixTimeMs )
					{
						break;
					}
					poNode = cir_list_next( poNode );
				}
				while( poNode != cir_list_first( poMixNode->poInPktList ));
			}
			else
			{
				MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "No packet in input queue\n" );
			}
		}
		else
		{
			MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Mode is MUTE or mixing not to be done\n" );
		}
		poMixData->bMixPkt = bMixPkt;
	}

	MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving\n" );
}

void MixerNode_DeleteOldPacket( void* pvData_i, void* pvUserData_i )
{
	t_MixerNode* poMixNode = pvData_i;
	t_MixData* poMixData = pvUserData_i;

	MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		t_CirList* poNode = NULL;
		t_DataPkt* poPkt = NULL;
		if( NULL != poMixNode->poInPktList )
		{
			poNode = cir_list_first( poMixNode->poInPktList );

			do
			{
				poPkt = poNode->pvData;

				MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Packet %p is generated %d ms ago\n", poPkt, poMixData->nCurrentTimeMs - poPkt->nPktGenTime );
				if( poPkt->nPktGenTime <= (poMixData->nCurrentTimeMs -  poMixData->nBufferTimeMs) )
				{
					MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Pkt %p is too old to be mixed at generation time = %d\n", poPkt, poPkt->nPktGenTime );

					// Delete packet
					poNode = cir_list_remove_link( cir_list_first( poMixNode->poInPktList ), poNode );

					DataPkt_Delete( poPkt, NULL );
				}
				else
				{
					break;
				}
				poNode = cir_list_next( poNode );
			}
			while( poNode != cir_list_first( poMixNode->poInPktList ));
		}
		else
		{
			MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "No packet in input queue\n" );
		}
	}

	MEDIA_TRACE( poMixNode, poMixData->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving\n" );
}

void DataPkt_Mix( void* pvData_i, void* pvUserData_i )
{
	t_DataPkt* poInPkt = pvData_i;
	t_DataPkt* poMixPkt = pvUserData_i;
	int nLength = 0;

	ms_assert( poInPkt->oGenPkt.nLength == poMixPkt->oGenPkt.nLength );

	for( nLength = 0; nLength < poMixPkt->oGenPkt.nLength; nLength++ )
	{
		poMixPkt->oGenPkt.pucData[nLength] += poMixPkt->oGenPkt.pucData[nLength];
	}
	POST_CONDITION
	{
	}
}

void MixerMix( t_Mixer* poMixer_i, int nCurrentTimeMs_i )
{
	t_CirList* poPktNode = NULL;
	t_MixerNode* poMixerNode = NULL;
	t_DataPkt* poDataPkt = NULL;
	t_CirList* poMixNode = NULL;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MEDIA_TRACE( poMixer_i, poMixer_i->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );
	if( NULL != poMixer_i->poNodeList )
	{
		poMixNode = cir_list_first( poMixer_i->poNodeList );

		do
		{
			poMixerNode = poMixNode->pvData;

			MEDIA_TRACE( poMixerNode, poMixer_i->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Mixing for Node = %p\n", poMixerNode );
			if( poMixerNode->nLastPktCurrentTime != nCurrentTimeMs_i )
			{
				if( 0 == poMixerNode->nLastPktGenTime )
				{
					poMixerNode->nLastPktGenTime = MixerGetCurrentSyncTimeMs( poMixer_i, poMixerNode );
				}
				else
				{
					poMixerNode->nLastPktGenTime += poMixer_i->nPacketizationTimeMs;
				}
				// Get the packets to be mixed from sources other than current source
				t_MixData oMixData;

				oMixData.nTraceId = poMixer_i->oComp.nTraceId;
				oMixData.bMixPkt = AMPS_TRUE;
				oMixData.nBufferTimeMs = poMixer_i->nBufferTimeMs;
				oMixData.poMixPktList = NULL;
				oMixData.nCurrentTimeMs = nCurrentTimeMs_i;
				oMixData.nMixTimeMs = poMixerNode->nLastPktGenTime;

				cir_list_foreach( poMixerNode->poInPktList, DataPkt_PrintInfo, (void*)poMixer_i->oComp.nTraceId );

				MixerNode_DeleteOldPacket( poMixNode->pvData, &oMixData );
				cir_list_iterate( cir_list_next( poMixNode ), poMixNode, MixerNode_GetPacket, &oMixData );
				// Check if packets are to be mixed
				if( oMixData.bMixPkt == AMPS_TRUE && cir_list_length( oMixData.poMixPktList ) > 0 )
				{
					// Mix packets
					t_DataPkt* poMixDataPkt = NULL;
					int nLength = poMixer_i->nPacketizationTimeMs * 8;      //FIXME: make sampling rate a configurable parameter

					eRetCode = AllocGenPkt((h_DataPkt*)&poMixDataPkt );
					if( MS_ERR_NONE == eRetCode )
					{
						poMixDataPkt->oGenPkt.pucData = FW_Malloc( nLength );
						poMixDataPkt->oGenPkt.nLength = nLength;
						memset( poMixDataPkt->oGenPkt.pucData, 0, nLength );

						MEDIA_TRACE( poMixerNode, poMixer_i->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Mixing packets\n" );
						cir_list_foreach( oMixData.poMixPktList, DataPkt_PrintInfo, (void*)poMixer_i->oComp.nTraceId );

						cir_list_foreach( oMixData.poMixPktList, DataPkt_Mix, poMixDataPkt );

						poMixDataPkt->nPktGenTime = poMixerNode->nLastPktGenTime;
						// Send packet
						ms_assert_return( NULL != poMixerNode->oNodeConfig.oSinkAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
						eRetCode = (poMixerNode->oNodeConfig.oSinkAppData.pfAppDataCb)((h_MediaComp)poMixer_i, (h_MediaCompAppCtx)poMixerNode->oNodeConfig.oSinkAppData.hAppDataCtx, (h_DataPkt)poMixDataPkt );

						poMixerNode->nLastPktCurrentTime = nCurrentTimeMs_i;
					}
					else
					{
						MEDIA_TRACE( poMixerNode, poMixer_i->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Mixed packet could not be allocated\n" );

						eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
					}
				}
				else
				{
					MEDIA_TRACE( poMixerNode, poMixer_i->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Mixing not to be done\n" );
				}
				cir_list_free( oMixData.poMixPktList );
			}
			else
			{
				MEDIA_TRACE( poMixerNode, poMixer_i->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Mixing already done for this node\n" );
			}
			poMixNode = cir_list_next( poMixNode );
		}
		while( poMixNode != cir_list_first( poMixer_i->poNodeList ));
	}
	else
	{
		MEDIA_TRACE( poMixer_i, poMixer_i->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "No mixing nodes present\n" );
	}
	POST_CONDITION
	{
	}

	MEDIA_TRACE( poMixer_i, poMixer_i->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));
}

int MixerGetCurrentSyncTimeMs( t_Mixer* poMixer_i, t_MixerNode* poNode_i )
{
	// Get current syncronized time
	t_SigPkt oSigPkt;

	memset( &oSigPkt, 0, sizeof (t_SigPkt));

	oSigPkt.eType = SPT_CURRENT_SYNC_TIME_Packet;
	oSigPkt.oSyncTimePkt.nDelayMs = poMixer_i->nPacketizationTimeMs;

	(poNode_i->oNodeConfig.oSinkAppData.pfAppSignalCb)((h_MediaComp)poMixer_i, (h_MediaCompAppCtx)poNode_i->oNodeConfig.oSinkAppData.hAppSignalCtx, (h_DataPkt)&oSigPkt );

	return (oSigPkt.oSyncTimePkt.nSyncTimeMs);
}

