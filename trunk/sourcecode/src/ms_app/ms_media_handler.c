#include "ms_media_handler.h"

e_MsRetCode MS_Media_RxChainCb( h_MediaComp hMediaComp_i, h_SigPkt h_SigPkt_i, h_MediaCompAppCtx hMediaCompAppCtx1_i, h_MediaCompAppCtx hMediaCompAppCtx2_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsCtx* poMsCtx = (t_MsCtx*)hMediaCompAppCtx1_i;
	t_MsUserCtrlCtx* poUserCtrlCtx = (t_MsUserCtrlCtx*)hMediaCompAppCtx2_i;
	t_SigPkt* poSigPkt = h_SigPkt_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaCompAppCtx1_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaCompAppCtx2_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != h_SigPkt_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poUserCtrlCtx );

		switch( poSigPkt->eType )
		{
			case SPT_SILENCE_DETECTED_Packet:
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "SPT_SILENCE_DETECTED_Packet\n" );
				if( NULL != poUserCtrlCtx->pvMscmlOpCtx )
				{
					t_MsEvent oPSMInputEvent;
					int nRetVal = 0;

					oPSMInputEvent.pvCtx = poUserCtrlCtx->pvMscmlOpCtx;
					oPSMInputEvent.pvAppCtx = poUserCtrlCtx;
					oPSMInputEvent.eType = SILENCE_DETECTED;

					nRetVal = FW_EvtSysSendIntEvt( poMsCtx->pvFrameworkCtx, MSCML_OP_EVENT, &oPSMInputEvent, sizeof (t_MsEvent));
					if( AMPS_SUCCESS != nRetVal )
					{
						MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
						eRetCode = MS_ERR_FRAMEWORK;
					}
				}
				else
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_WARNING, "MSCML operation has ended\n" );
				}
				break;
			}

			case SPT_SOUND_DETECTED_Packet:
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "SPT_SOUND_DETECTED_Packet\n" );
				if( NULL != poUserCtrlCtx->pvMscmlOpCtx )
				{
					t_MsEvent oPSMInputEvent;
					int nRetVal = 0;

					oPSMInputEvent.pvCtx = poUserCtrlCtx->pvMscmlOpCtx;
					oPSMInputEvent.pvAppCtx = poUserCtrlCtx;
					oPSMInputEvent.eType = SOUND_DETECTED;

					nRetVal = FW_EvtSysSendIntEvt( poMsCtx->pvFrameworkCtx, MSCML_OP_EVENT, &oPSMInputEvent, sizeof (t_MsEvent));
					if( AMPS_SUCCESS != nRetVal )
					{
						MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
						eRetCode = MS_ERR_FRAMEWORK;
					}
				}
				else
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_WARNING, "MSCML operation has ended\n" );
				}
				break;
			}

			case SPT_DTMP_Packet:
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "SPT_DTMP_Packet:DTMF digit = %d, volume = %d\n", poSigPkt->oDtmfPkt.ucDtmfKey, poSigPkt->oDtmfPkt.ucVolume );
				if( poUserCtrlCtx->bDtmfClamp == AMPS_FALSE )
				{
					// If an operation is currently being executed AND all the qurantined events are processed

					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "MSCML Op Ctx = %p\n", poUserCtrlCtx->pvMscmlOpCtx );

					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Qurantine Buffer size = %d\n", poUserCtrlCtx->poQuarantineBuffer->uchCount );
					if( NULL != poUserCtrlCtx->pvMscmlOpCtx &&  poUserCtrlCtx->poQuarantineBuffer->uchCount == 0 )
					{
						t_MsEvent oPSMInputEvent;
						int nRetVal = 0;

						oPSMInputEvent.pvCtx = poUserCtrlCtx->pvMscmlOpCtx;
						oPSMInputEvent.pvAppCtx = poUserCtrlCtx;
						oPSMInputEvent.eType = DTMF_KEY_PRESSED;

						oPSMInputEvent.uEventParams.oDtmfKeyPressedParams.cDtmfKeyVal = TS_NumToAsciiDigit( poSigPkt->oDtmfPkt.ucDtmfKey );
						oPSMInputEvent.uEventParams.oDtmfKeyPressedParams.bQuarantined = AMPS_FALSE;

						nRetVal = FW_EvtSysSendIntEvt( poMsCtx->pvFrameworkCtx, MSCML_OP_EVENT, &oPSMInputEvent, sizeof (t_MsEvent));
						if( AMPS_SUCCESS != nRetVal )
						{
							MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
							eRetCode = MS_ERR_FRAMEWORK;
						}
					}
					else
					{
						MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Event %p quarantined\n", poSigPkt );
						FW_DListAppend( poUserCtrlCtx->poQuarantineBuffer, poSigPkt );

						poSigPkt = NULL;
					}
				}
				else
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Ignoring DTMF key as DTMF is clamped\n" );
				}
				break;
			}

			case SPT_REMOTE_ADDRESS_CHANGE_Packet:
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "SPT_REMOTE_ADDRESS_CHANGE_Packet: Remote Address = %s:%d\n", poSigPkt->oAddrChangePkt.pcIp, poSigPkt->oAddrChangePkt.nPort );

				// Set Remote port of UdpSend to the IP:port received
				h_MediaComp hUdpSend;

				eRetCode = Media_ChainGetCompOfType( poUserCtrlCtx->hRxChain, MCT_UDPSEND, &hUdpSend );
				if( MS_ERR_NONE == eRetCode )
				{
					Media_CompSetParam( hUdpSend, MCC_UDPSEND_REMOTE_IP,  (void*)poSigPkt->oAddrChangePkt.pcIp );

					Media_CompSetParam( hUdpSend, MCC_UDPSEND_REMOTE_PORT,  (void*)poSigPkt->oAddrChangePkt.nPort );
				}
				break;
			}

			case SPT_CURRENT_SYNC_TIME_Packet:
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "SPT_CURRENT_SYNC_TIME_Packet\n" );

				t_MsConfCtx* poMsConfCtx = poUserCtrlCtx->poMsConfCtx;
				if( NULL != poMsConfCtx )
				{
					ms_assert( NULL != poMsConfCtx->hTimer );
					ST_GetCurrentTime( poMsConfCtx->hTimer, poSigPkt->oSyncTimePkt.nDelayMs, &poSigPkt->oSyncTimePkt.nSyncTimeMs );
				}
				else
				{
					poSigPkt->oSyncTimePkt.nSyncTimeMs = FW_GetCurrentTimeInMilliSec( poMsCtx->pvFrameworkCtx );
				}
				poSigPkt = NULL;
				break;
			}

			case SPT_PAYLOAD_TYPE_CHANGE_Packet:
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Payload type changed to = %d\n", poSigPkt->oPayloadChangePkt.ucPayloadType );

				// Check if we have negotiated this payload type
				t_AMPSSList* poNegInfoNode = NULL;
				t_SdpNegMediaInfo* poNegMediaInfo = NULL;

				for( poNegInfoNode = poUserCtrlCtx->poNegInfoList->poAMPSSListHead; poNegInfoNode != NULL; poNegInfoNode = poNegInfoNode->poAMPSSListNext )
				{
					poNegMediaInfo = poNegInfoNode->pvData;
					if( poNegMediaInfo->ucPayloadType == poSigPkt->oPayloadChangePkt.ucPayloadType )
					{
						break;
					}
					else
					{
						poNegMediaInfo = NULL;
					}
				}
				eRetCode = MS_Media_ChainSetMode( poMsCtx, poUserCtrlCtx->poMsConfCtx, poUserCtrlCtx, poUserCtrlCtx->eCurrentMode, poNegMediaInfo );

				break;
			}

			default:
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Unknown signal packet = %d\n", poSigPkt->eType );
		}
		if( NULL != poSigPkt )
		{
			(poSigPkt->pfnSignalPktDelete)( poSigPkt );
			FW_Free( poSigPkt );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_TxChainCb( h_MediaComp hMediaComp_i, h_SigPkt h_SigPkt_i, h_MediaCompAppCtx hMediaCompAppCtx1_i, h_MediaCompAppCtx hMediaCompAppCtx2_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsCtx* poMsCtx = (t_MsCtx*)hMediaCompAppCtx1_i;
	t_MsUserCtrlCtx* poUserCtrlCtx = (t_MsUserCtrlCtx*)hMediaCompAppCtx2_i;
	t_SigPkt* poSigPkt = h_SigPkt_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaCompAppCtx1_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaCompAppCtx2_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != h_SigPkt_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poUserCtrlCtx );

		switch( poSigPkt->eType )
		{
			case SPT_EOF_Detected:
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "SPT_EOF_Detected\n" );
				if( NULL != poUserCtrlCtx->pvMscmlOpCtx )
				{
					t_MsEvent oPSMInputEvent;
					int nRetVal = 0;

					oPSMInputEvent.pvCtx = poUserCtrlCtx->pvMscmlOpCtx;
					oPSMInputEvent.pvAppCtx = poUserCtrlCtx;
					oPSMInputEvent.eType = PLAY_ENDED;
					nRetVal = FW_EvtSysSendIntEvt( poMsCtx->pvFrameworkCtx, MSCML_OP_EVENT, &oPSMInputEvent, sizeof (t_MsEvent));
					if( AMPS_SUCCESS != nRetVal )
					{
						MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
						eRetCode = MS_ERR_FRAMEWORK;
					}
				}
				else
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_WARNING, "MSCML operation has ended\n" );
				}
				break;
			}

			case SPT_CURRENT_SYNC_TIME_Packet:
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "SPT_CURRENT_SYNC_TIME_Packet\n" );

				t_MsConfCtx* poMsConfCtx = poUserCtrlCtx->poMsConfCtx;
				if( NULL != poMsConfCtx )
				{
					ms_assert( NULL != poMsConfCtx->hTimer );
					ST_GetCurrentTime( poMsConfCtx->hTimer, poSigPkt->oSyncTimePkt.nDelayMs, &poSigPkt->oSyncTimePkt.nSyncTimeMs );
				}
				else
				{
					poSigPkt->oSyncTimePkt.nSyncTimeMs = FW_GetCurrentTimeInMilliSec( poMsCtx->pvFrameworkCtx );
				}
				poSigPkt = NULL;
				break;
			}

			default:
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Unknown signal packet = %d\n", poSigPkt->eType );
		}
		if( NULL != poSigPkt )
		{
			(poSigPkt->pfnSignalPktDelete)( poSigPkt );
			FW_Free( poSigPkt );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

