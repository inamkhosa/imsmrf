/*! @file mscml_play_record.c
*
*@brief This file consist of the whole functionalty of the mscml play record machine(RFC5022)
* The <playrecord> request directs the media server to convert and possibly to transcode the RTP payloads it receives and store them to the specified URL using the requested content codec(s) and file format. This request proceeds in two phases; prompt and record.
*/
#include "mscml_play_record.h"

f_PlayRecordSm ppf_PlayRecordSm[MOS_STATES][MS_EVENT_MAX] = {
	{
		MOP_Dummy,
		PRSM_Start_MSCML_REQUEST,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
	},
	{
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_AnyState_SM_STOP,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		PRSM_PlayingSequence_START_PLAY_ERROR,
		MOP_Dummy,
		PRSM_PlayingSequence_PLAY_ENDED,
		MOP_Dummy,
		PRSM_PlayPhase_DTMF_KEY_PRESSED,
		MOP_Dummy,
		PRSM_PlayPhase_ESCAPE_KEY_PRESSED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		PRSM_PlayPhase_PLAY_DURATION_TIMER_EXPIRED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
	},
	{
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_AnyState_SM_STOP,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		PRSM_PlayPhase_DTMF_KEY_PRESSED,
		MOP_Dummy,
		PRSM_PlayPhase_ESCAPE_KEY_PRESSED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		PRSM_PlayPhase_PLAY_DURATION_TIMER_EXPIRED,
		PRSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
	},
	{
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
	},
	{
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
	},
	{
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_AnyState_SM_STOP,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		PRSM_PlayingBeep_START_PLAY_ERROR,
		MOP_Dummy,
		PRSM_PlayingBeep_PLAY_ENDED,
		MOP_Dummy,
		PRSM_PlayPhase_DTMF_KEY_PRESSED,
		MOP_Dummy,
		PRSM_PlayPhase_ESCAPE_KEY_PRESSED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
	},
	{
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_AnyState_SM_STOP,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		PRSM_RecordingPhase_DTMF_KEY_PRESSED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		PRSM_RecMediaNotStarted_SOUND_DETECTED,
		MOP_Dummy,
		MOP_Dummy,
		PRSM_RecMediaNotStarted_START_RECORDING_ERROR,
		PRSM_RecMediaNotStarted_STOP_RECORDING_ERROR,
		PRSM_RecMediaNotStarted_RECORDING_STOPPED,
		PRSM_RecordingPhase_RECORDING_DURATION_TIMER_EXPIRED,
		PRSM_RecMediaNotStarted_RECORDING_SILENCE_TIMER_EXPIRED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
	},
	{
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_AnyState_SM_STOP,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		PRSM_RecordingPhase_DTMF_KEY_PRESSED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		PRSM_RecMediaStarted_SILENCE_DETECTED,
		PRSM_RecMediaStarted_SOUND_DETECTED,
		MOP_Dummy,
		MOP_Dummy,
		PRSM_RecMediaStarted_START_RECORDING_ERROR,
		PRSM_RecMediaStarted_STOP_RECORDING_ERROR,
		PRSM_RecMediaStarted_RECORDING_STOPPED,
		PRSM_RecordingPhase_RECORDING_DURATION_TIMER_EXPIRED,
		PRSM_RecMediaStarted_RECORDING_SILENCE_TIMER_EXPIRED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
	}
};

f_OnEntry pf_PlayRecordSmOnEntry[MOS_STATES] = {
	OnEntry_PRSM_Start,
	OnEntry_PRSM_PlayingSequence,
	OnEntry_PRSM_InterSeqDelay,
	MOP_OnEntryOnExit_Dummy,
	MOP_OnEntryOnExit_Dummy,
	OnEntry_PRSM_PlayingBeep,
	OnEntry_PRSM_RecMediaNotStarted,
	OnEntry_PRSM_RecMediaStarted,
};

f_OnExit pf_PlayRecordSmOnExit[MOS_STATES] = {
	OnExit_PRSM_Start,
	OnExit_PRSM_PlayingSequence,
	OnExit_PRSM_InterSeqDelay,
	MOP_OnEntryOnExit_Dummy,
	MOP_OnEntryOnExit_Dummy,
	OnExit_PRSM_PlayingBeep,
	OnExit_PRSM_RecMediaNotStarted,
	OnExit_PRSM_RecMediaStarted,
};

/**
*@fn e_MsRetCode PRSM_Start_MSCML_REQUEST( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::MSCML_REQUEST event in @ref e_MscmlOpState::MOS_Start state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PRSM_Start_MSCML_REQUEST( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		poCtx->pvAppCtx = poEvent_i->pvAppCtx;
		poCtx->poMscmlReq = poEvent_i->uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile;
		poEvent_i->uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile = NULL;

		// Append "baseurl" in audio.url attribute
		AppendBaseUrl( poCtx->poMscmlReq, poCtx->pcSoundDir );

		GetRequestAttrAsString( poCtx->poMscmlReq, "playrecord", "id", &poCtx->oRespInfo.pcResponseId );

		// Calculation duration of each file and add a "file_duration" attribute for each audio line
		eRetCode = CalcualateFileDuration( poCtx );
		if( MS_ERR_INVALID_INPUT != eRetCode )
		{
			AMPS_BOOL bClearDigitsInMsg = AMPS_FALSE;   // default value

			GetRequestAttrAsBool( poCtx->poMscmlReq, "playrecord", "cleardigits", &bClearDigitsInMsg );
			if( AMPS_TRUE == bClearDigitsInMsg )
			{
				MOP_Generate_CLEAR_QUARANTINE( poCtx );
			}
			// Retrieve any quarantined events
			MOP_Generate_GET_QUARANTINE( poCtx );

			int nDurationInMsg = nInfiniteValue_c;

			eRetCode = GetPromptAttrAsMilliSec( poCtx->poMscmlReq, "duration", &nDurationInMsg );
			if( MS_ERR_NONE == eRetCode )   // <prompt> present
			{
				//1.If duration is "infinite" or greater than zero
				if( nDurationInMsg != 0 )
				{
					int nRepeatInMsg = 1;
					GetPromptAttrAsInt( poCtx->poMscmlReq, "repeat", &nRepeatInMsg );
					//if repeat_count is lesser than "repeat" in message
					if( poCtx->oPlayInfo.nRepeatCount < nRepeatInMsg )
					{
						//Note Start of Play Duration
						poCtx->oPlayInfo.nPlayDurationStartTime = FW_GetCurrentTimeInMilliSec( poCtx->pvFwCtx );
						// Check if duration is not "infinite"
						if( nDurationInMsg != nInfiniteValue_c )
						{
							//3.Generate START_PLAY_DURATION_TIMER(request.prompt.duration)
							eRetCode = MOP_Generate_START_PLAY_DURATION_TIMER(  poCtx, nDurationInMsg );

							ms_assert_return( NULL != poCtx->oPlayInfo.pvDurationTimerID, eRetCode = MS_ERR_FRAMEWORK );
						}
						// Calculate file and offset to be played
						int nOffsetMs = 0;
						eRetCode = GetPromptAttrAsMilliSec( poCtx->poMscmlReq, "offset", &nOffsetMs );

						MOP_CalculatePlayFileAndOffset( poCtx, nOffsetMs );

						//Note Start of Play Offset
						poCtx->oPlayInfo.nPlayOffsetStartTime = FW_GetCurrentTimeInMilliSec(  poCtx->pvFwCtx ) - poCtx->oPlayInfo.nOffsetMs;

						//Change state to PLAYING_SEQUENCE
						eRetCode = MOP_ChangeState( poCtx, MOS_PlayingSequence );
					}
					else //if repeat_count is equal to repeat in request
					{
						eRetCode = PRSM_ChangeStateTo_BeepOrMediaNotStarted( poCtx );
					}
				}
				else // duration in request is ZERO
				{
					eRetCode = PRSM_ChangeStateTo_BeepOrMediaNotStarted( poCtx );
				}
			}
			else if( MS_ERR_END_OF_LIST == eRetCode )  // no <prompt> found
			{
				eRetCode = PRSM_ChangeStateTo_BeepOrMediaNotStarted( poCtx );
			}
		}
		else
		{
			poCtx->oRespInfo.nResponseCode = SIP_RES_NOT_FOUND;
			//Generate MSCML_RESPONSE
			eRetCode = MOP_Generate_MSCML_RESPONSE( poCtx );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_ChangeStateTo_BeepOrMediaNotStarted( t_MscmlOpCtx* poCtx_i )
*
*@brief The function changes state to @ref e_MscmlOpState::MOS_PlayingBeep or @ref e_MscmlOpState::MOS_RecMediaNotStarted
*
* @param[in] poCtx_i is the context of the play record state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PRSM_ChangeStateTo_BeepOrMediaNotStarted( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	AMPS_BOOL bBeep = AMPS_TRUE;   // Default value is "true"

	int nCurrTime = FW_GetCurrentTimeInMilliSec( poCtx_i->pvFwCtx );
	if( 0 != poCtx_i->oPlayInfo.nPlayDurationStartTime )
	{
		poCtx_i->oRespInfo.nPlayDuration = nCurrTime - poCtx_i->oPlayInfo.nPlayDurationStartTime;
	}
	else
	{
		poCtx_i->oRespInfo.nPlayDuration = 0;
	}
	if( 0 != poCtx_i->oPlayInfo.nPlayOffsetStartTime )
	{
		poCtx_i->oRespInfo.nPlayOffset = nCurrTime - poCtx_i->oPlayInfo.nPlayOffsetStartTime;
	}
	else
	{
		poCtx_i->oRespInfo.nPlayOffset = 0;
	}
	eRetCode = MOP_Generate_STOP_PLAY( poCtx_i );
	eRetCode = MOP_Generate_STOP_PLAY_DURATION_TIMER( poCtx_i );

	GetRequestAttrAsBool( poCtx_i->poMscmlReq, "playrecord", "beep", &bBeep );
	if( AMPS_TRUE == bBeep ) //If beep is to be played
	{
		//Change state to PLAYING_BEEP
		eRetCode = MOP_ChangeState( poCtx_i, MOS_PlayingBeep );
	}
	else
	{
		//Change state to RecMediaNotStarted
		eRetCode = MOP_ChangeState( poCtx_i, MOS_RecMediaNotStarted );
	}
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_ProcessDigit( t_MscmlOpCtx* poCtx_i, char cDigit_i, t_MsEvent* poEvent_i )
*
*@brief The function processes the pressed dtmf digits
*
* @param[in] poCtx_i is the context of the play record state machine
*
* @param[in] cDigit_i is the context of the play record state machine
*
* @param[in] poEvent_i is media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_ProcessDigit( t_MscmlOpCtx* poCtx_i, char cDigit_i, t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		AMPS_BOOL bBarge = AMPS_TRUE;   // Default value is "true"

		GetRequestAttrAsBool( poCtx_i->poMscmlReq, "playrecord", "barge", &bBarge );
		if( AMPS_TRUE == bBarge )
		{
			AMPS_BOOL bKeyMapped = AMPS_FALSE;
			char cKey = '*';     // Default value of "escapekey"

			GetRequestAttrAsChar( poCtx_i->poMscmlReq, "playrecord", "escapekey", &cKey );
			if( tolower( cKey ) == cDigit_i )
			{
				bKeyMapped = AMPS_TRUE;
				eRetCode = ppf_PlayRecordSm[poCtx_i->eCurrentState][ESCAPE_KEY_PRESSED]( poEvent_i );
			}
			else
			{
				eRetCode = PRSM_ChangeStateTo_BeepOrMediaNotStarted( poCtx_i );
			}
		}
		else
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Play can't be barged.\n" );
		}
		MOP_Generate_GET_QUARANTINE( poCtx_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_PlayPhase_ESCAPE_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::ESCAPE_KEY_PRESSED event in play phase
*
* @param[in] poEvent_i is media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PRSM_PlayPhase_ESCAPE_KEY_PRESSED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;
	int nEndOfPlayDuration = 0;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		int nCurrTime = FW_GetCurrentTimeInMilliSec( poCtx->pvFwCtx );
		//set reason to escapekey
		poCtx->oRespInfo.pcReason = "escapekey";
		poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
		eRetCode = MOP_Generate_STOP_PLAY( poCtx );
		if( 0 != poCtx->oPlayInfo.nPlayDurationStartTime )
		{
			poCtx->oRespInfo.nPlayDuration = nCurrTime - poCtx->oPlayInfo.nPlayDurationStartTime;
		}
		else
		{
			poCtx->oRespInfo.nPlayDuration = 0;
		}
		if( 0 != poCtx->oPlayInfo.nPlayOffsetStartTime )
		{
			poCtx->oRespInfo.nPlayOffset = nCurrTime - poCtx->oPlayInfo.nPlayOffsetStartTime;
		}
		else
		{
			poCtx->oRespInfo.nPlayOffset = 0;
		}
		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}
	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_PlayingSequence_START_PLAY_ERROR( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::START_PLAY_ERROR event in @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poEvent_i is media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PRSM_PlayingSequence_START_PLAY_ERROR( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
		//if stoponerror is true
		AMPS_BOOL bPromptStopOnError = AMPS_FALSE;
		GetPromptAttrAsBool( poCtx->poMscmlReq, "stoponerror", &bPromptStopOnError );
		if( bPromptStopOnError == AMPS_TRUE )
		{
			poCtx->oRespInfo.nResponseCode = poEvent_i->uEventParams.oPlayErrorParams.nErrorCode;

			eRetCode = MOP_ChangeState( poCtx, MOS_Start );
		}
		else //stoponerror is false
		{
			eRetCode = MOP_PlayNextFile( poCtx );
			if( MS_ERR_END_OF_LIST == eRetCode )
			{
				eRetCode = PRSM_ChangeStateTo_BeepOrMediaNotStarted( poCtx );
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_PlayingSequence_PLAY_ENDED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::PLAY_ENDED event in @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poEvent_i is media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PRSM_PlayingSequence_PLAY_ENDED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = MOP_PlayNextFile( poCtx );
		if( MS_ERR_END_OF_LIST == eRetCode )
		{
			eRetCode = PRSM_ChangeStateTo_BeepOrMediaNotStarted( poCtx );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_PlayPhase_PLAY_DURATION_TIMER_EXPIRED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::PLAY_DURATION_TIMER_EXPIRED event in play phase
*
* @param[in] poEvent_i is media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_PlayPhase_PLAY_DURATION_TIMER_EXPIRED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = PRSM_ChangeStateTo_BeepOrMediaNotStarted( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::INTER_SEQ_TIMER_EXPIRED event in @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poEvent_i is media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PRSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	{
		//Note Start of Play Offset
		poCtx->oPlayInfo.nPlayOffsetStartTime = FW_GetCurrentTimeInMilliSec(  poCtx->pvFwCtx );
		eRetCode = MOP_ChangeState( poCtx, MOS_PlayingSequence );
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_PlayPhase_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::DTMF_KEY_PRESSED event in play phase
*
* @param[in] poEvent_i is media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_PlayPhase_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = PRSM_ProcessDigit( poCtx, poEvent_i->uEventParams.oDtmfKeyPressedParams.cDtmfKeyVal, poEvent_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_PlayingBeep_START_PLAY_ERROR( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::START_PLAY_ERROR event in @ref e_MscmlOpState::MOS_PlayingBeep state
*
* @param[in] poEvent_i is media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_PlayingBeep_START_PLAY_ERROR( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//Change state to RECORDING_MEDIA NOT Started
		eRetCode = MOP_ChangeState( poCtx, MOS_RecMediaNotStarted );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_PlayingBeep_PLAY_ENDED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::PLAY_ENDED event in @ref e_MscmlOpState::MOS_PlayingBeep state
*
* @param[in] poEvent_i is media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_PlayingBeep_PLAY_ENDED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//Change state to RECORDING_MEDIA NOT Started
		eRetCode = MOP_ChangeState( poCtx, MOS_RecMediaNotStarted );
	}

	POST_CONDITION
	{
	}
	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecordingPhase_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::DTMF_KEY_PRESSED event in play phase
*
* @param[in] poEvent_i is media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_RecordingPhase_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	char cDtmfKey_i;

	cDtmfKey_i = poEvent_i->uEventParams.oDtmfKeyPressedParams.cDtmfKeyVal;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//1.If DTMF_KEY present in rec_stop_mask
		if( PRSM_CheckFor_RecStopMaskDigit( poCtx, cDtmfKey_i ) == MS_ERR_MATCHCOMPLETE )
		{
			poCtx->oRespInfo.pcReason = "digit";
			poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
			eRetCode = MOP_Generate_STOP_RECORDING(  poCtx );
			poCtx->uSmInfo.oPlayRecordInfo.cPressedDigit = cDtmfKey_i;
			MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Digit pressed is : %c\n", poCtx->uSmInfo.oPlayRecordInfo.cPressedDigit );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecMediaNotStarted_SOUND_DETECTED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::SOUND_DETECTED event in @ref e_MscmlOpState::MOS_RecMediaNotStarted state
*
* @param[in] poEvent_i is media server event structure
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_RecMediaNotStarted_SOUND_DETECTED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode =  MOP_ChangeState( poCtx, MOS_RecMediaStarted );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecMediaNotStarted_START_RECORDING_ERROR( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::START_RECORDING_ERROR event in @ref e_MscmlOpState::MOS_RecMediaNotStarted state
*
* @param[in] poEvent_i is media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_RecMediaNotStarted_START_RECORDING_ERROR( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		int iRecordStopTime = 0;
		//media is not started
		poCtx->oRespInfo.nRecDuration = 0;
		poCtx->oRespInfo.nRecLength = 0;

		//set reason to error
		poCtx->oRespInfo.pcReason = "error";
		poCtx->oRespInfo.nResponseCode = SIP_RES_SERVER_INTERNAL_ERROR;

		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecMediaNotStarted_STOP_RECORDING_ERROR( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::STOP_RECORDING_ERROR event in @ref e_MscmlOpState::MOS_RecMediaNotStarted state
*
* @param[in] poEvent_i is media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_RecMediaNotStarted_STOP_RECORDING_ERROR( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		int iRecordStopTime = 0;
		//media is not started
		poCtx->oRespInfo.nRecDuration = 0;
		poCtx->oRespInfo.nRecLength = 0;
		//set reason to error
		poCtx->oRespInfo.pcReason = "error";
		poCtx->oRespInfo.nResponseCode = SIP_RES_SERVER_INTERNAL_ERROR;

		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}
	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecordingPhase_RECORDING_DURATION_TIMER_EXPIRED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::RECORDING_DURATION_TIMER_EXPIRED event in recording phase
*
* @param[in] poEvent_i is media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_RecordingPhase_RECORDING_DURATION_TIMER_EXPIRED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//set reason to max_duration
		poCtx->oRespInfo.pcReason = "max_duration";
		poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
		//Generate STOP_RECORDING
		eRetCode =  MOP_Generate_STOP_RECORDING( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecMediaNotStarted_RECORDING_SILENCE_TIMER_EXPIRED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::RECORDING_SILENCE_TIMER_EXPIRED event in @ref e_MscmlOpState::MOS_RecMediaNotStarted state
*
* @param[in] poEvent_i is media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PRSM_RecMediaNotStarted_RECORDING_SILENCE_TIMER_EXPIRED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//set reason to Init_silence
		poCtx->oRespInfo.pcReason = "initsilence";
		poCtx->oRespInfo.nResponseCode = SIP_RES_OK;

		//Generate STOP_RECORDING
		eRetCode = MOP_Generate_STOP_RECORDING( poCtx );
	}
	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecMediaNotStarted_RECORDING_STOPPED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::RECORDING_STOPPED event in @ref e_MscmlOpState::MOS_RecMediaNotStarted state
*
* @param[in] poEvent_i is media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_RecMediaNotStarted_RECORDING_STOPPED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;
	int iRecordStopTime = 0;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
		//media is not started
		poCtx->oRespInfo.nRecDuration = 0;
		poCtx->oRespInfo.nRecLength = 0;
		//Change state to START
		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}
	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (MS_ERR_INVALID_INPUT);
}

/**
*@fn e_MsRetCode PRSM_RecMediaStarted_SILENCE_DETECTED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::SILENCE_DETECTED event in @ref e_MscmlOpState::MOS_RecMediaStarted state
*
* @param[in] poEvent_i is media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_RecMediaStarted_SILENCE_DETECTED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		int nEndSilenceTimerInMsg = 4000;   // Default value = 4000ms:
		GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playrecord", "endsilence", &nEndSilenceTimerInMsg );
		eRetCode = MOP_Generate_START_RECORDING_SILENCE_TIMER( poCtx, nEndSilenceTimerInMsg );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecMediaStarted_SOUND_DETECTED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::SOUND_DETECTED event in @ref e_MscmlOpState::MOS_RecMediaStarted state
*
* @param[in] poEvent_i is media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_RecMediaStarted_SOUND_DETECTED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
		//Generate STOP_RECORDING_SILENCE_TIMER
		eRetCode = MOP_Generate_STOP_RECORDING_SILENCE_TIMER( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecMediaStarted_START_RECORDING_ERROR( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::START_RECORDING_ERROR event in @ref e_MscmlOpState::MOS_RecMediaStarted state
*
* @param[in] poEvent_i is media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_RecMediaStarted_START_RECORDING_ERROR( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
		//set reason to stopped
		poCtx->oRespInfo.pcReason = "error";
		poCtx->oRespInfo.nResponseCode = SIP_RES_SERVER_INTERNAL_ERROR;
		//Generate STOP_RECORDING
		eRetCode = MOP_Generate_STOP_RECORDING( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecMediaStarted_STOP_RECORDING_ERROR( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::STOP_RECORDING_ERROR event in @ref e_MscmlOpState::MOS_RecMediaStarted state
*
* @param[in] poEvent_i is media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_RecMediaStarted_STOP_RECORDING_ERROR( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//Record length in bytes
		eRetCode = PRSM_CalculateRecordLength( poCtx );
		//set reason to error
		poCtx->oRespInfo.pcReason = "error";
		poCtx->oRespInfo.nResponseCode = SIP_RES_SERVER_INTERNAL_ERROR;

		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecMediaStarted_RECORDING_SILENCE_TIMER_EXPIRED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::RECORDING_SILENCE_TIMER_EXPIRED event in @ref e_MscmlOpState::MOS_RecMediaStarted state
*
* @param[in] poEvent_i is media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_RecMediaStarted_RECORDING_SILENCE_TIMER_EXPIRED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//set reason to end_silence
		poCtx->oRespInfo.pcReason = "endsilence";
		poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
		//Generate STOP_RECORDING
		eRetCode = MOP_Generate_STOP_RECORDING(  poCtx );
	}
	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_RecMediaStarted_RECORDING_STOPPED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::RECORDING_STOPPED event in @ref e_MscmlOpState::MOS_RecMediaStarted state
*
* @param[in] poEvent_i is media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PRSM_RecMediaStarted_RECORDING_STOPPED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
		//Record length in bytes
		eRetCode =  PRSM_CalculateRecordLength( poCtx );
		//Change state to START
		eRetCode =  MOP_ChangeState( poCtx, MOS_Start );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn void PRSM_MSCML_PLAY_COLLECT_EVENT_Handler( AMPS_HANDLE hFw_i, void* pvContext_i, void* pvEventHandle_i, void* pvEvent_i )
*
*@brief The function handles MSCML_PLAY_COLLECT event
*
* @param[in] hFw_i is framework handle
*
* @param[in] pvContext_i is the context of the state machine
*
* @param[in] pvEventHandle_i is used by the frame work
*
* @param[in] pvEvent_i is evnent payload
*
* @return void
*
* @author suliman shah
*
* @version
*/
void PRSM_MSCML_PLAY_COLLECT_EVENT_Handler( AMPS_HANDLE hFw_i, void* pvContext_i, void* pvEventHandle_i, void* pvEvent_i )
{
	t_MsEvent* poEvent = pvEvent_i;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	{
		ms_assert_return( NULL != poEvent, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Before.\n" );
		//PSM_PrintContext( poCtx );

		//Call through the functional pointer
		eRetCode = ppf_PlayRecordSm[poCtx->eCurrentState][poEvent->eType]( poEvent );

		MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "After.\n" );
		//PSM_PrintContext( poCtx );
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
}

/**
*@fn e_MsRetCode OnEntry_PRSM_Start( t_MscmlOpCtx* poCtx )
*
*@brief The function is called on entry of @ref e_MscmlOpState::MOS_Start state
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnEntry_PRSM_Start( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		int nCurrTime = FW_GetCurrentTimeInMilliSec( poCtx->pvFwCtx );
		if( 0 != poCtx->uSmInfo.oPlayRecordInfo.nRecordStartTime )
		{
			poCtx->oRespInfo.nRecDuration = nCurrTime - poCtx->uSmInfo.oPlayRecordInfo.nRecordStartTime;
			//Record length in bytes
			eRetCode =  PRSM_CalculateRecordLength( poCtx );
		}
		else
		{
			poCtx->oRespInfo.nRecDuration = 0;
			poCtx->oRespInfo.nRecLength = 0;
		}
		eRetCode = MOP_Generate_STOP_RECORDING_DURATION_TIMER( poCtx );
		eRetCode = MOP_Generate_MSCML_RESPONSE( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnExit_PRSM_Start( t_MscmlOpCtx* poCtx )
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_Start state
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnExit_PRSM_Start( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//Start of Play Duration
		poCtx->oPlayInfo.nPlayDurationStartTime = FW_GetCurrentTimeInMilliSec( poCtx );
		//poCtx->nPlayDurationStartTime =
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnEntry_PRSM_RecMediaNotStarted( t_MscmlOpCtx* poCtx )
*
*@brief The function is called on entry of @ref e_MscmlOpState::MOS_RecMediaNotStarted state
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnEntry_PRSM_RecMediaNotStarted( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	int nEndOfPlayDuration = 0;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		int nCurrTime = FW_GetCurrentTimeInMilliSec( poCtx->pvFwCtx );
		if( 0 != poCtx->oPlayInfo.nPlayDurationStartTime )
		{
			poCtx->oRespInfo.nPlayDuration = nCurrTime - poCtx->oPlayInfo.nPlayDurationStartTime;
		}
		else
		{
			poCtx->oRespInfo.nPlayDuration = 0;
		}
		if( 0 != poCtx->oPlayInfo.nPlayOffsetStartTime )
		{
			poCtx->oRespInfo.nPlayOffset = nCurrTime - poCtx->oPlayInfo.nPlayOffsetStartTime;
		}
		else
		{
			poCtx->oRespInfo.nPlayOffset = 0;
		}
		eRetCode = MOP_Generate_STOP_PLAY( poCtx );
		eRetCode = MOP_Generate_STOP_PLAY_DURATION_TIMER( poCtx );
		//4.Generate START_RECORDING
		eRetCode = MOP_Generate_START_RECORDING(  poCtx );

		int nInitialSilenceTimerInMsg = 3000;   // Default value = 3000ms
		GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playrecord", "initsilence", &nInitialSilenceTimerInMsg );

		//  Generate START_RECORDING_SILENCE_TIMER(initsilence)
		eRetCode = MOP_Generate_START_RECORDING_SILENCE_TIMER( poCtx, nInitialSilenceTimerInMsg );

		//Get current time and store in record_start_time
		poCtx->uSmInfo.oPlayRecordInfo.nRecordStartTime = FW_GetCurrentTimeInMilliSec( poCtx );
		int nRecordDurationTimerInMsg = nInfiniteValue_c;   // Default value = infinite
		GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playrecord", "duration", &nRecordDurationTimerInMsg );
		//if request.recording.duration is not infinite
		if( nRecordDurationTimerInMsg != nInfiniteValue_c )
		{
			//Generate START_RECORDING_DURATION_TIMER (request.recording.duration)
			eRetCode = MOP_Generate_START_RECORDING_DURATION_TIMER( poCtx, nRecordDurationTimerInMsg );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode e_MsRetCode OnExit_PRSM_RecMediaNotStarted( t_MscmlOpCtx* poCtx )
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_RecMediaNotStarted state
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode OnExit_PRSM_RecMediaNotStarted( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		// 1.Generate STOP_RECORDING_SILENCE_TIMER(if not stopped already)
		eRetCode =  MOP_Generate_STOP_RECORDING_SILENCE_TIMER( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnEntry_PRSM_RecMediaStarted( t_MscmlOpCtx* poCtx )
*
*@brief The function is called on entry to @ref e_MscmlOpState::MOS_RecMediaNotStarted state
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnEntry_PRSM_RecMediaStarted( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (MS_ERR_INVALID_INPUT);
}

/**
*@fn e_MsRetCode OnExit_PRSM_RecMediaStarted( t_MscmlOpCtx* poCtx )
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_RecMediaStarted state
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnExit_PRSM_RecMediaStarted( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = MOP_Generate_STOP_RECORDING_DURATION_TIMER( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnEntry_PRSM_PlayingBeep( t_MscmlOpCtx* poCtx)
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_PlayingBeep state
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnEntry_PRSM_PlayingBeep( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;
	int nEndOfPlayDuration = 0;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//End of Play nDuration
		nEndOfPlayDuration = FW_GetCurrentTimeInMilliSec( poCtx );
		poCtx->oRespInfo.nPlayDuration = nEndOfPlayDuration - poCtx->oPlayInfo.nPlayDurationStartTime;
		//1.Generate START_PLAY(beep.wav)
		eRetCode = MOP_Generate_START_BEEP( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnExit_PRSM_PlayingBeep( t_MscmlOpCtx* poCtx)
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_PlayingBeep state
*
* @param[in] poCtx is the context of the state machine
*
* @return void
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnExit_PRSM_PlayingBeep( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPlayRecordSMInputEvent;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnEntry_PRSM_InterSeqDelay( t_MscmlOpCtx* poCtx)
*
*@brief The function is called on entry of @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnEntry_PRSM_InterSeqDelay( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	int nEndOfPlayDuration = 0;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//poCtx_i->oRespInfo.nPlayOffset = 0;

		int nDelayInMsg = 0;

		poCtx->uSmInfo.oPlayCollectInfo.nInterSeqStartTime = FW_GetCurrentTimeInMilliSec(  poCtx->pvFwCtx );

		eRetCode = GetPromptAttrAsMilliSec( poCtx->poMscmlReq, "delay", &nDelayInMsg );
		ms_assert( MS_ERR_NONE == eRetCode );

		// Generate START_PLAY_DELAY_TIMER(request.prompt.delay)
		eRetCode = MOP_Generate_START_PLAY_DELAY_TIMER(  poCtx, nDelayInMsg );

		ms_assert_return( NULL != poCtx->oPlayInfo.pvDelayTimerID, eRetCode = MS_ERR_FRAMEWORK );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnExit_PRSM_InterSeqDelay( t_MscmlOpCtx* poCtx)
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnExit_PRSM_InterSeqDelay( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = MOP_Generate_STOP_INTER_SEQ_TIMER( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode OnEntry_PRSM_PlayingSequence( t_MscmlOpCtx* poCtx)
*
*@brief The function is called on entry of @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnEntry_PRSM_PlayingSequence( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		poCtx->oPlayInfo.nRepeatCount++;
		eRetCode = MOP_Generate_START_PLAY( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode OnExit_PRSM_PlayingSequence( t_MscmlOpCtx* poCtx)
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode OnExit_PRSM_PlayingSequence( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_CheckFor_RecStopMaskDigit( t_MscmlOpCtx* poCtx, char cPressedKey_i )
*
*@brief The function checks if given dtmfkey is present in the given recstopmaks
*
* @param[in] poCtx is the context of the state machine
*
* @param[in] cPressedKey_i pressed dtmf key
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_CheckFor_RecStopMaskDigit( t_MscmlOpCtx* poCtx, char cPressedKey_i )
{
	t_MsEvent* poPlayRecordSMInputEvent;

	e_MsRetCode eRetCode = MS_ERR_NOTMATCHED;
	int nDtmfCounter = 0;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Digit pressed is: %c\n", cPressedKey_i );

		char* pcRecStopMask;
		if( MS_ERR_NONE != GetRequestAttrAsString( poCtx->poMscmlReq, "playrecord", "recstopmask", &pcRecStopMask ))
		{
			pcRecStopMask = FW_Malloc( sizeof ("0123456789ABCD#*") + 1 );
			strcpy( pcRecStopMask, "0123456789ABCD#*" );
		}

		for( nDtmfCounter = 0; nDtmfCounter < strlen( pcRecStopMask ); nDtmfCounter++ )
		{
			if( cPressedKey_i == tolower( pcRecStopMask[nDtmfCounter] ) )
			{
				eRetCode = MS_ERR_MATCHCOMPLETE;
				break;
			}
		}
		FW_Free( pcRecStopMask );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PRSM_CalculateRecordLength( t_MscmlOpCtx* poCtx )
*
*@brief The function calculates the total size of the recorded file
*
* @param[in] poCtx is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PRSM_CalculateRecordLength( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	{
		struct stat oFileStat;

		char* pcDupRecURL = NULL;

		GetRequestAttrAsString( poCtx->poMscmlReq, "playrecord", "recurl", &pcDupRecURL );
		if( stat( pcDupRecURL, &oFileStat ) == -1 )
		{
			MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "stat function failed.\n" );
			poCtx->oRespInfo.nRecLength = 0;
			eRetCode = MS_ERR_FRAMEWORK;
		}
		else
		{
			MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "file size is : %lld\n", (int) oFileStat.st_size );
			poCtx->oRespInfo.nRecLength = (int) oFileStat.st_size;
		}
		FW_Free( pcDupRecURL );
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

