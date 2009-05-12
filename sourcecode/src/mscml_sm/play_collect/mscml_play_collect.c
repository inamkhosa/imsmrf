/*! @file mscml_play_collect.c
*@brief This file consist of the whole functionality for the mscml PlayCollect state machine
*The client issues a <playcollect> request to play an announcement (optional) and collect digits. The <playcollect> request is executed in two phases, prompt and collect. If the client specifies prompt content to be played, using the <prompt> element or prompturl attribute, the media server plays the content before starting the collection phase. If no prompt content is specified, the collect phase begins immediately.(RFC5022)
*/

#include "mscml_play_collect.h"

f_MscmlOpSm ppf_PlayCollectSm[MOS_STATES][MS_EVENT_MAX] = {
	{
		MOP_Dummy,
		PCSM_Start_MSCML_REQUEST,
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
		PCSM_PlayingSequence_START_PLAY_ERROR,
		MOP_Dummy,
		PCSM_PlayingSequence_PLAY_ENDED,
		MOP_Dummy,
		PCSM_PlayingSequence_DTMF_KEY_PRESSED,
		PCSM_AnyState_RETURN_KEY_PRESSED,
		PCSM_AnyState_ESCAPE_KEY_PRESSED,
		PCSM_PlayingSequence_FF_KEY_PRESSED,
		PCSM_PlayingSequence_RW_KEY_PRESSED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		PCSM_PlayingPhase_PLAY_DURATION_TIMER_EXPIRED,
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
		PCSM_InterSeqDelay_DTMF_KEY_PRESSED,
		PCSM_AnyState_RETURN_KEY_PRESSED,
		PCSM_AnyState_ESCAPE_KEY_PRESSED,
		PCSM_InterSeqDelay_FF_KEY_PRESSED,
		PCSM_InterSeqDelay_RW_KEY_PRESSED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
		PCSM_PlayingPhase_PLAY_DURATION_TIMER_EXPIRED,
		PCSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED,
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
		PCSM_CollectingDigits_DTMF_KEY_PRESSED,
		PCSM_AnyState_RETURN_KEY_PRESSED,
		PCSM_AnyState_ESCAPE_KEY_PRESSED,
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
		PCSM_CollectingDigits_COLLECTION_TIMER_EXPIRED,
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
		PCSM_DigitsCollected_DTMF_KEY_PRESSED,
		PCSM_AnyState_RETURN_KEY_PRESSED,
		PCSM_AnyState_ESCAPE_KEY_PRESSED,
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
		PCSM_DigitsCollected_COLLECTION_TIMER_EXPIRED,
		MOP_Dummy,
		MOP_Dummy,
	}
};

f_OnEntry pf_PlayCollectSmOnEntry[MOS_STATES] = {
	OnEntry_PCSM_Start,
	OnEntry_PCSM_PlayingSequence,
	OnEntry_PCSM_InterSeqDelay,
	OnEntry_PCSM_CollectingDigits,
	OnEntry_PCSM_DigitsCollected,
};

f_OnExit pf_PlayCollectSmOnExit[MOS_STATES] = {
	OnExit_PCSM_Start,
	OnExit_PCSM_PlayingSequence,
	OnExit_PCSM_InterSeqDelay,
	OnExit_PCSM_CollectingDigits,
	OnExit_PCSM_DigitsCollected,
};
/**
*@fn e_MsRetCode PCSM_Start_MSCML_REQUEST( t_MsEvent* poEvent_i )
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

e_MsRetCode PCSM_Start_MSCML_REQUEST( t_MsEvent* poEvent_i )
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
		poCtx->pvAppCtx = poEvent_i->pvAppCtx;
		poCtx->poMscmlReq = poEvent_i->uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile;
		poEvent_i->uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile = NULL;

		// Append "baseurl" in audio.url attribute
		AppendBaseUrl( poCtx->poMscmlReq, poCtx->pcSoundDir );

		GetRequestAttrAsString( poCtx->poMscmlReq, "playcollect", "id", &poCtx->oRespInfo.pcResponseId );

		CreateRegExList( poCtx );

		// Calculation duration of each file and add a "file_duration" attribute for each audio line
		eRetCode = CalcualateFileDuration( poCtx );
		if( MS_ERR_INVALID_INPUT != eRetCode )
		{
			AMPS_BOOL bClearDigitsInMsg = AMPS_FALSE;   // default value

			GetRequestAttrAsBool( poCtx->poMscmlReq, "playcollect", "cleardigits", &bClearDigitsInMsg );
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
						//1.Generate START_COLLECTION_TIMER(first digit timer)
						int nFirstDigitTimerInMsg = 5000;   // Default value = 5000ms

						GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "firstdigittimer", &nFirstDigitTimerInMsg );

						// Start collection timer(firstdigittimer)
						eRetCode = MOP_Generate_START_COLLECTION_TIMER(  poCtx, nFirstDigitTimerInMsg );

						eRetCode = MOP_ChangeState( poCtx, MOS_CollectingDigits );
					}
				}
				else // duration in request is ZERO
				{
					//1.Generate START_COLLECTION_TIMER(first digit timer)
					int nFirstDigitTimerInMsg = 5000;   // Default value = 5000ms

					GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "firstdigittimer", &nFirstDigitTimerInMsg );

					// Start collection timer(firstdigittimer)
					eRetCode = MOP_Generate_START_COLLECTION_TIMER(  poCtx, nFirstDigitTimerInMsg );

					eRetCode = MOP_ChangeState( poCtx, MOS_CollectingDigits );
				}
			}
			else if( MS_ERR_END_OF_LIST == eRetCode )  // no <prompt> found
			{
				//1.Generate START_COLLECTION_TIMER(first digit timer)
				int nFirstDigitTimerInMsg = 5000;   // Default value = 5000ms

				GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "firstdigittimer", &nFirstDigitTimerInMsg );

				// Start collection timer(firstdigittimer)
				eRetCode = MOP_Generate_START_COLLECTION_TIMER(  poCtx, nFirstDigitTimerInMsg );

				eRetCode = MOP_ChangeState( poCtx, MOS_CollectingDigits );
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
*@fn e_MsRetCode PCSM_PlayingSequence_START_PLAY_ERROR( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::START_PLAY_ERROR event in @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_PlayingSequence_START_PLAY_ERROR( t_MsEvent* poEvent_i )
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
				//1.Generate START_COLLECTION_TIMER(first digit timer)
				int nFirstDigitTimerInMsg = 5000;   // Default value = 5000ms

				GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "firstdigittimer", &nFirstDigitTimerInMsg );

				// Start collection timer(firstdigittimer)
				eRetCode = MOP_Generate_START_COLLECTION_TIMER(  poCtx, nFirstDigitTimerInMsg );

				eRetCode = MOP_ChangeState( poCtx, MOS_CollectingDigits );
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
*@fn e_MsRetCode PCSM_PlayingSequence_PLAY_ENDED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::PLAY_ENDED event in @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_PlayingSequence_PLAY_ENDED( t_MsEvent* poEvent_i )
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
			//1.Generate START_COLLECTION_TIMER(first digit timer)
			int nFirstDigitTimerInMsg = 5000;   // Default value = 5000ms

			GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "firstdigittimer", &nFirstDigitTimerInMsg );

			// Start collection timer(firstdigittimer)
			eRetCode = MOP_Generate_START_COLLECTION_TIMER(  poCtx, nFirstDigitTimerInMsg );

			eRetCode = MOP_ChangeState( poCtx, MOS_CollectingDigits );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PCSM_PlayingSequence_FF_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::FF_KEY_PRESSED event in @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_PlayingSequence_FF_KEY_PRESSED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	int nSeqOffset = 0;

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
		int nCurrentTimeMs = 0;
		int nSkipIntervalMs = 6000;     // Default value = 6s

		nCurrentTimeMs = FW_GetCurrentTimeInMilliSec( poCtx->pvFwCtx );

		GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "skipinterval", &nSkipIntervalMs );

		MOP_CalculatePlayFileAndOffset( poCtx, nCurrentTimeMs - poCtx->oPlayInfo.nPlayOffsetStartTime + nSkipIntervalMs );

		eRetCode = MOP_Generate_START_PLAY( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PCSM_PlayingSequence_RW_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::RW_KEY_PRESSED event in @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_PlayingSequence_RW_KEY_PRESSED( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	int nSeqOffset = 0;

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
		int nCurrentTimeMs = 0;
		int nSkipIntervalMs = 6000;     // Default value = 6s
		int nOffsetMs = 0;
		int nSeqDuration = 0;

		GetPromptAttrAsInt( poCtx->poMscmlReq, "seq_duration", &nSeqDuration );

		ms_assert( 0 < nSeqDuration );

		nCurrentTimeMs = FW_GetCurrentTimeInMilliSec( poCtx->pvFwCtx );

		GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "skipinterval", &nSkipIntervalMs );

		nOffsetMs = nCurrentTimeMs - poCtx->oPlayInfo.nPlayOffsetStartTime - nSkipIntervalMs;
		if( 0 > nOffsetMs )
		{
			poCtx->oPlayInfo.nRepeatCount -= 1;
			nOffsetMs = nSeqDuration + nOffsetMs;
		}
		MOP_CalculatePlayFileAndOffset( poCtx, nOffsetMs );

		eRetCode = MOP_Generate_START_PLAY( poCtx );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PCSM_PlayingSequence_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::DTMF_KEY_PRESSED event in @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_PlayingSequence_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
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
		// Map DTMF_KEY to Special Key
		AMPS_BOOL bKeyMapped = AMPS_FALSE;
		{
			char cKey = 0;
			char cDtmfKey = poEvent_i->uEventParams.oDtmfKeyPressedParams.cDtmfKeyVal;

			eRetCode = GetRequestAttrAsChar( poCtx->poMscmlReq, "playcollect", "ffkey", &cKey );
			if( MS_ERR_NONE == eRetCode )
			{
				if( tolower( cKey ) == cDtmfKey )
				{
					bKeyMapped = AMPS_TRUE;
					eRetCode = ppf_PlayCollectSm[poCtx->eCurrentState][FF_KEY_PRESSED]( poEvent_i );
				}
			}
			else
			{
				eRetCode = GetRequestAttrAsChar( poCtx->poMscmlReq, "playcollect", "rwkey", &cKey );
				if( MS_ERR_NONE == eRetCode )
				{
					if( tolower( cKey ) == cDtmfKey )
					{
						bKeyMapped = AMPS_TRUE;
						eRetCode = ppf_PlayCollectSm[poCtx->eCurrentState][RW_KEY_PRESSED]( poEvent_i );
					}
				}
			}
		}
		if( AMPS_FALSE == bKeyMapped )
		{
			eRetCode = PCSM_ProcessDigit( poCtx, poEvent_i->uEventParams.oDtmfKeyPressedParams.cDtmfKeyVal, poEvent_i );
			if( MS_ERR_MATCHED == eRetCode )
			{
				eRetCode = MOP_ChangeState( poCtx, MOS_CollectingDigits );
			}
			else if( MS_ERR_MATCHCOMPLETE == eRetCode )
			{
				eRetCode = MOP_ChangeState( poCtx, MOS_DigitsCollected );
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
*@fn e_MsRetCode PCSM_PlayingPhase_PLAY_DURATION_TIMER_EXPIRED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::PLAY_DURATION_TIMER_EXPIRED event in @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_PlayingPhase_PLAY_DURATION_TIMER_EXPIRED( t_MsEvent* poEvent_i )
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
		eRetCode = MOP_ChangeState( poCtx, MOS_CollectingDigits );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PCSM_InterSeqDelay_FF_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::FF_KEY_PRESSED event in @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_InterSeqDelay_FF_KEY_PRESSED( t_MsEvent* poEvent_i )
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
		int nCurrentTimeMs = 0;
		int nSkipIntervalMs = 6000;     // Default value = 6s
		int nDelayMs = 0;
		int nInterSeqDelay = 0;

		nCurrentTimeMs = FW_GetCurrentTimeInMilliSec( poCtx->pvFwCtx );

		GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "skipinterval", &nSkipIntervalMs );

		GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "delay", &nInterSeqDelay );

		nDelayMs = nCurrentTimeMs - poCtx->uSmInfo.oPlayCollectInfo.nInterSeqStartTime + nSkipIntervalMs - nInterSeqDelay;
		// Still in interseq_delay ?
		if( nDelayMs < 0 )
		{
			// Stop the timer
			FW_DeleteTimer( poCtx->pvFwCtx, poCtx->oPlayInfo.pvDelayTimerID );

			eRetCode = MOP_Generate_START_PLAY_DELAY_TIMER(  poCtx, nInterSeqDelay + nDelayMs );

			ms_assert_return( NULL != poCtx->oPlayInfo.pvDelayTimerID, eRetCode = MS_ERR_FRAMEWORK );
		}
		else
		{
			poCtx->oPlayInfo.nOffsetMs = nDelayMs;
			MOP_ChangeState( poCtx, MOS_PlayingSequence );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PCSM_InterSeqDelay_RW_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::RW_KEY_PRESSED event in @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_InterSeqDelay_RW_KEY_PRESSED( t_MsEvent* poEvent_i )
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
		int nCurrentTimeMs = 0;
		int nSkipIntervalMs = 6000;     // Default value = 6s
		int nDelayMs = 0;
		int nInterSeqDelay = 0;

		nCurrentTimeMs = FW_GetCurrentTimeInMilliSec( poCtx->pvFwCtx );

		GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "skipinterval", &nSkipIntervalMs );

		GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "delay", &nInterSeqDelay );

		nDelayMs = nCurrentTimeMs - poCtx->uSmInfo.oPlayCollectInfo.nInterSeqStartTime - nSkipIntervalMs;
		// Still in interseq_delay ?
		if( nDelayMs > 0 )
		{
			// Stop the timer
			FW_DeleteTimer( poCtx->pvFwCtx, poCtx->oPlayInfo.pvDelayTimerID );

			eRetCode = MOP_Generate_START_PLAY_DELAY_TIMER(  poCtx, nInterSeqDelay - nDelayMs );

			ms_assert_return( NULL != poCtx->oPlayInfo.pvDelayTimerID, eRetCode = MS_ERR_FRAMEWORK );
		}
		else
		{
			int nSeqDuration = 0;

			eRetCode = GetPromptAttrAsInt( poCtx->poMscmlReq, "seq_duration", &nSeqDuration );

			poCtx->oPlayInfo.nOffsetMs = nSeqDuration - nDelayMs;
			poCtx->oPlayInfo.nRepeatCount--;

			MOP_ChangeState( poCtx, MOS_PlayingSequence );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PCSM_InterSeqDelay_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::DTMF_KEY_PRESSED event in @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_InterSeqDelay_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
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
		// Map DTMF_KEY to Special Key
		AMPS_BOOL bKeyMapped = AMPS_FALSE;
		{
			char cKey = '\0';
			char cDtmfKey = poEvent_i->uEventParams.oDtmfKeyPressedParams.cDtmfKeyVal;

			eRetCode = GetRequestAttrAsChar( poCtx->poMscmlReq, "playcollect", "ffkey", &cKey );
			if( MS_ERR_NONE == eRetCode )
			{
				if( tolower( cKey ) == cDtmfKey )
				{
					bKeyMapped = AMPS_TRUE;
					eRetCode = ppf_PlayCollectSm[poCtx->eCurrentState][FF_KEY_PRESSED]( poEvent_i );
				}
			}
			else
			{
				eRetCode = GetRequestAttrAsChar( poCtx->poMscmlReq, "playcollect", "rwkey", &cKey );
				if( MS_ERR_NONE == eRetCode )
				{
					if( tolower( cKey ) == cDtmfKey )
					{
						bKeyMapped = AMPS_TRUE;
						eRetCode = ppf_PlayCollectSm[poCtx->eCurrentState][RW_KEY_PRESSED]( poEvent_i );
					}
				}
			}
		}
		if( AMPS_FALSE == bKeyMapped )
		{
			eRetCode = PCSM_ProcessDigit( poCtx, poEvent_i->uEventParams.oDtmfKeyPressedParams.cDtmfKeyVal, poEvent_i );
			if( MS_ERR_MATCHED == eRetCode )
			{
				eRetCode = MOP_ChangeState( poCtx, MOS_CollectingDigits );
			}
			else if( MS_ERR_MATCHCOMPLETE == eRetCode )
			{
				eRetCode = MOP_ChangeState( poCtx, MOS_DigitsCollected );
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
*@fn e_MsRetCode PCSM_ProcessDigit( t_MscmlOpCtx* poCtx_i, char cDigit_i, t_MsEvent* poEvent_i )
*
*@brief The function processes the pressed dtmf keys
*
* @param[in] poCtx_i is state machine context
*
* @param[in] cDigit_i is the pressed dtmfkey
*
* @param[in] poEvent_i is the Media server event structure
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_ProcessDigit( t_MscmlOpCtx* poCtx_i, char cDigit_i, t_MsEvent* poEvent_i )
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

		GetRequestAttrAsBool( poCtx_i->poMscmlReq, "playcollect", "barge", &bBarge );
		if( AMPS_TRUE == bBarge )
		{
			int nMaxDigitsInMsg = -1;

			GetRequestAttrAsInt( poCtx_i->poMscmlReq, "playcollect", "maxdigits", &nMaxDigitsInMsg );
			if( 0 == nMaxDigitsInMsg )
			{
				eRetCode = MS_ERR_MATCHCOMPLETE;
			}
			else if( 0 < nMaxDigitsInMsg )
			{
				AMPS_BOOL bKeyMapped = AMPS_FALSE;
				char cKey = '*';     // Default value of "escapekey"

				GetRequestAttrAsChar( poCtx_i->poMscmlReq, "playcollect", "escapekey", &cKey );
				if( tolower( cKey ) == cDigit_i )
				{
					bKeyMapped = AMPS_TRUE;
					eRetCode = ppf_PlayCollectSm[poCtx_i->eCurrentState][ESCAPE_KEY_PRESSED]( poEvent_i );
				}
				else
				{
					cKey = '#';      // Default value of "returnkey"

					GetRequestAttrAsChar( poCtx_i->poMscmlReq, "playcollect", "returnkey", &cKey );
					if( tolower( cKey ) == cDigit_i )
					{
						bKeyMapped = AMPS_TRUE;
						eRetCode = ppf_PlayCollectSm[poCtx_i->eCurrentState][RETURN_KEY_PRESSED]( poEvent_i );
					}
				}
				if( bKeyMapped == AMPS_FALSE )
				{
					FW_DListAppend( poCtx_i->uSmInfo.oPlayCollectInfo.poPressedDigitList, (void*)(int)(cDigit_i));
					if( poCtx_i->uSmInfo.oPlayCollectInfo.poPressedDigitList->uchCount < nMaxDigitsInMsg )
					{
						eRetCode = MS_ERR_MATCHED;
					}
					else
					{
						eRetCode = MS_ERR_MATCHCOMPLETE;
					}
				}
			}
			else
			{
				t_AMPSSList* poNode = NULL;
				hDRegEx hExp = NULL;

				for( poNode = poCtx_i->uSmInfo.oPlayCollectInfo.poDRegExHandleList->poAMPSSListHead;
				     poNode != NULL;
				     poNode = poNode->poAMPSSListNext )
				{
					hExp = (hDRegEx)poNode->pvData;

					MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Matching with exp = %p\n", hExp );

					eRetCode = re_match( hExp, cDigit_i );
					if( MS_ERR_MATCHCOMPLETE == eRetCode )
					{
						poCtx_i->uSmInfo.oPlayCollectInfo.hMatchedExp = hExp;
						break;
					}
				}
				if( NULL == poNode )
				{
					eRetCode = MS_ERR_MATCHED;
				}
				else
				{
					eRetCode = MS_ERR_MATCHCOMPLETE;
				}
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
*@fn e_MsRetCode PCSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::INTER_SEQ_TIMER_EXPIRED event in @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED( t_MsEvent* poEvent_i )
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
*@fn e_MsRetCode PCSM_AnyState_RETURN_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::RETURN_KEY_PRESSED event
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_AnyState_RETURN_KEY_PRESSED( t_MsEvent* poEvent_i )
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
		poCtx->oRespInfo.pcReason = "returnkey";
		poCtx->oRespInfo.nResponseCode = SIP_RES_OK;

		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PCSM_AnyState_ESCAPE_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::ESCAPE_KEY_PRESSED event
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PCSM_AnyState_ESCAPE_KEY_PRESSED( t_MsEvent* poEvent_i )
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
		poCtx->oRespInfo.pcReason = "escapekey";
		poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PCSM_CollectingDigits_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::DTMF_KEY_PRESSED event in @ref e_MscmlOpState::MOS_CollectingDigits state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_CollectingDigits_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
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
		eRetCode = PCSM_ProcessDigit( poCtx, poEvent_i->uEventParams.oDtmfKeyPressedParams.cDtmfKeyVal, poEvent_i );
		if( MS_ERR_MATCHCOMPLETE == eRetCode )
		{
			eRetCode = MOP_ChangeState( poCtx, MOS_DigitsCollected );
		}
		else if( MS_ERR_MATCHED == eRetCode )
		{
			// Restart collection timer
			MOP_Generate_STOP_COLLECTION_TIMER( poCtx );

			int nInterDigitTimer = 2000;    // default value

			GetRequestAttrAsMilliSec( poCtx->poMscmlReq, "playcollect", "interdigittimer", &nInterDigitTimer );

			eRetCode = MOP_Generate_START_COLLECTION_TIMER(  poCtx, nInterDigitTimer );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PCSM_CollectingDigits_COLLECTION_TIMER_EXPIRED( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::COLLECTION_TIMER_EXPIRED event in @ref e_MscmlOpState::MOS_CollectingDigits state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_CollectingDigits_COLLECTION_TIMER_EXPIRED( t_MsEvent* poEvent_i )
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
		poCtx->oRespInfo.pcReason = "timeout";
		poCtx->oRespInfo.nResponseCode = SIP_RES_OK;

		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PCSM_DigitsCollected_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
*
*@brief This function handles @ref e_MsEvent::DTMF_KEY_PRESSED event in @ref e_MscmlOpState::MOS_DigitsCollected state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode PCSM_DigitsCollected_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i )
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
		eRetCode = PCSM_ProcessDigit( poCtx, poEvent_i->uEventParams.oDtmfKeyPressedParams.cDtmfKeyVal, poEvent_i );
	}
	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PCSM_DigitsCollected_COLLECTION_TIMER_EXPIRED( t_MsEvent* poEvent_i )
*
*@brief The function is called when collection timer expires
*
* @param[in] poEvent_i is the Media server event structure
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode PCSM_DigitsCollected_COLLECTION_TIMER_EXPIRED( t_MsEvent* poEvent_i )
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
		poCtx->oRespInfo.pcReason = "match";
		poCtx->oRespInfo.nResponseCode = SIP_RES_OK;

		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode OnEntry_PCSM_Start( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on entry of the @ref e_MscmlOpState::MOS_Start state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode OnEntry_PCSM_Start( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = MOP_Generate_STOP_COLLECTION_TIMER( poCtx_i );
		eRetCode = MOP_Generate_MSCML_RESPONSE( poCtx_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnExit_PCSM_Start( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_Start state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode OnExit_PCSM_Start( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnEntry_PCSM_PlayingSequence( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on entry of @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode OnEntry_PCSM_PlayingSequence( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		poCtx_i->oPlayInfo.nRepeatCount++;

		//1.Generate START_PLAY
		eRetCode = MOP_Generate_START_PLAY( poCtx_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode OnExit_PCSM_PlayingSequence( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode OnExit_PCSM_PlayingSequence( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnEntry_PCSM_InterSeqDelay( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on entry of @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode OnEntry_PCSM_InterSeqDelay( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	int nEndOfPlayDuration = 0;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//poCtx_i->oRespInfo.nPlayOffset = 0;

		int nDelayInMsg = 0;

		poCtx_i->uSmInfo.oPlayCollectInfo.nInterSeqStartTime = FW_GetCurrentTimeInMilliSec(  poCtx_i->pvFwCtx );

		eRetCode = GetPromptAttrAsMilliSec( poCtx_i->poMscmlReq, "delay", &nDelayInMsg );
		ms_assert( MS_ERR_NONE == eRetCode );

		// Generate START_PLAY_DELAY_TIMER(request.prompt.delay)
		eRetCode = MOP_Generate_START_PLAY_DELAY_TIMER(  poCtx_i, nDelayInMsg );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnExit_PCSM_InterSeqDelay( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnExit_PCSM_InterSeqDelay( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		eRetCode = MOP_Generate_STOP_INTER_SEQ_TIMER( poCtx_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode OnEntry_PCSM_CollectingDigits( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on entry of @ref e_MscmlOpState::MOS_CollectingDigits state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnEntry_PCSM_CollectingDigits( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
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
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnExit_PCSM_CollectingDigits( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_CollectingDigits state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/
e_MsRetCode OnExit_PCSM_CollectingDigits( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//1.Generate STOP_COLLECTION_TIMER
		eRetCode = MOP_Generate_STOP_COLLECTION_TIMER( poCtx_i );
	}

	POST_CONDITION
	{
	}
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode OnEntry_PCSM_DigitsCollected( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_DigitsCollected state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode OnEntry_PCSM_DigitsCollected( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	POST_CONDITION
	{
		int nTimer = 1000;      //default value of "extradigittimer"
		int nMaxDigitsInMsg = -1;

		GetRequestAttrAsInt( poCtx_i->poMscmlReq, "playcollect", "maxdigits", &nMaxDigitsInMsg );
		if( -1 == nMaxDigitsInMsg )
		{
			nTimer = 2000;      // ""defaults to the value of the interdigittimer attribute""
			GetRequestAttrAsMilliSec( poCtx_i->poMscmlReq, "playcollect", "interdigittimer", &nTimer );

			GetRequestAttrAsMilliSec( poCtx_i->poMscmlReq, "playcollect", "interdigitcriticaltimer", &nTimer );
		}
		else
		{
			GetRequestAttrAsMilliSec( poCtx_i->poMscmlReq, "playcollect", "extradigittimer", &nTimer );
		}
		// Generate START_COLLECTION_TIMER(extradigit/interdigitcriticaltimer timer)
		eRetCode = MOP_Generate_START_COLLECTION_TIMER( poCtx_i, nTimer );
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode OnExit_PCSM_DigitsCollected( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_DigitsCollected state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return return an error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode OnExit_PCSM_DigitsCollected( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//eRetCode = MOP_Generate_STOP_COLLECTION_TIMER( poCtx_i );
	}

	POST_CONDITION
	{
	}
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn void PCSM_MSCML_PLAY_COLLECT_EVENT_Handler( AMPS_HANDLE hFw_i, void* pvContext_i, void* pvEventHandle_i, void* pvEvent_i )
*
*@brief The function handles MSCML_PLAY_RECORD event for play collect state machine
*
* @param[in] hFw_i is framework handle
*
* @param[in] pvContext_i is the context of the state machine
*
* @param[in] pvEventHandle_i is media server event structure
*
* @param[in] pvEvent_i is evnent payload
*
* @return void
*
* @author suliman shah
*
* @version
*/

void PCSM_MSCML_PLAY_COLLECT_EVENT_Handler( AMPS_HANDLE hFw_i, void* pvContext_i, void* pvEventHandle_i, void* pvEvent_i )
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

		eRetCode = ppf_PlayCollectSm[poCtx->eCurrentState][poEvent->eType]( poEvent );

		MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "After.\n" );
		//PSM_PrintContext( poCtx );
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
}

/**
*@fn e_MsRetCode CreateRegExList( t_MscmlOpCtx* poCtx_i )
*
*@brief The function creates a list of regular expressions
*
* @param[in] poCtx_i is the context of the state machine
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
* @version
*/

e_MsRetCode CreateRegExList( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	{
		int nRegExIndex = 1;
		t_DRegExConfig oDRegExConfig;
		hDRegEx hDRegEx = NULL;
		char* pcExpression = NULL;
		char* pcName = NULL;

		do
		{
			eRetCode =  GetPatternAttrAsString( poCtx_i->poMscmlReq, "regex", nRegExIndex, "value", &pcExpression );
			if( MS_ERR_NONE == eRetCode )
			{
				strncpy( oDRegExConfig.pcExpression, pcExpression, MAX_DREG_EX_EXPRESSION_LENGTH - 1 );

				GetPatternAttrAsString( poCtx_i->poMscmlReq, "regex", nRegExIndex, "name", &pcName );
				if( NULL != pcName )
				{
					strncpy( oDRegExConfig.pcName, pcName, MAX_DREG_EX_NAME_LENGTH - 1 );

					xmlFree( pcName );
				}
				else
				{
					strncpy( oDRegExConfig.pcName, "", MAX_DREG_EX_NAME_LENGTH - 1 );
				}
				oDRegExConfig.nDRegTraceID = poCtx_i->nTraceId;

				eRetCode = re_create( &hDRegEx, &oDRegExConfig );

				MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "RegEx handle = %p\n", hDRegEx );

				FW_DListAppend( poCtx_i->uSmInfo.oPlayCollectInfo.poDRegExHandleList, hDRegEx );

				xmlFree( pcExpression );
			}
			nRegExIndex++;
		}
		while( MS_ERR_NONE == eRetCode );
	}
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

