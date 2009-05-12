/*! @file mscml_play.c
*
*@brief This file consist of the whole functionalty of the mscml play state machine
* The client issues a <play> request to play an announcement without interruption and with no digit collection. One use, for example, is to announce the name of a new participant to the entire conference. (RFC5022)
*/

#include "mscml_play.h"

f_MscmlOpSm ppf_PlaySm[MOS_STATES][MS_EVENT_MAX] = {
	{
		MOP_Dummy,
		PSM_Start_MSCML_REQUEST,
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
		PSM_PlayingSequence_START_PLAY_ERROR,
		MOP_Dummy,
		PSM_PlayingSequence_PLAY_ENDED,
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
		PSM_PlayingSequence_PLAY_DURATION_TIMER_EXPIRED,
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
		PSM_InterSeqDelay_PLAY_DURATION_TIMER_EXPIRED,
		PSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED,
		MOP_Dummy,
		MOP_Dummy,
		MOP_Dummy,
	}
};

f_OnEntry pf_PlaySmOnEntry[MOS_STATES] = {
	OnEntry_PSM_Start,
	OnEntry_PSM_PlayingSequence,
	OnEntry_PSM_InterSeqDelay,
};

f_OnExit pf_PlaySmOnExit[MOS_STATES] = {
	OnExit_PSM_Start,
	OnExit_PSM_PlayingSequence,
	OnExit_PSM_InterSeqDelay,
};

/**
*@fn e_MsRetCode PSM_Start_MSCML_REQUEST(   t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::MSCML_REQUEST event in @ref e_MscmlOpState::MOS_Start state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
*@version
*/
e_MsRetCode PSM_Start_MSCML_REQUEST(   t_MsEvent* poEvent_i )
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

		MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Sound directory = %s\n", poCtx->pcSoundDir );

		// Append "baseurl" in audio.url attribute
		AppendBaseUrl( poCtx->poMscmlReq, poCtx->pcSoundDir );

		GetRequestAttrAsString( poCtx->poMscmlReq, "play", "id", &poCtx->oRespInfo.pcResponseId );

		// Calculation duration of each file and add a "file_duration" attribute for each audio line
		eRetCode = CalcualateFileDuration( poCtx );
		if( MS_ERR_INVALID_INPUT != eRetCode )
		{
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
						poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
						//Generate MSCML_RESPONSE
						eRetCode = MOP_Generate_MSCML_RESPONSE( poCtx );
					}
				}
				else // duration in request is ZERO
				{
					poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
					//Generate MSCML_RESPONSE
					eRetCode = MOP_Generate_MSCML_RESPONSE( poCtx );
				}
			}
			else if( MS_ERR_END_OF_LIST == eRetCode )  // no <prompt> found
			{
				poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
				//Generate MSCML_RESPONSE
				eRetCode = MOP_Generate_MSCML_RESPONSE( poCtx );
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
*@fn e_MsRetCode PSM_PlayingSequence_START_PLAY_ERROR(   t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::START_PLAY_ERROR event in @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
*/
e_MsRetCode PSM_PlayingSequence_START_PLAY_ERROR(   t_MsEvent* poEvent_i )
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

			//Change state to START
			eRetCode = MOP_ChangeState( poCtx, MOS_Start );
		}
		else //stoponerror is false
		{
			eRetCode = MOP_PlayNextFile( poCtx );
			if( MS_ERR_END_OF_LIST == eRetCode )
			{
				poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
				//Change state to PSM_Start
				eRetCode = MOP_ChangeState( poCtx, MOS_Start );
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
*@fn e_MsRetCode PSM_PlayingSequence_PLAY_ENDED(   t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::PLAY_ENDED event in @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
*@version
*/

e_MsRetCode PSM_PlayingSequence_PLAY_ENDED(   t_MsEvent* poEvent_i )
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
			poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
			//Change state to PSM_Start
			eRetCode = MOP_ChangeState( poCtx, MOS_Start );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode PSM_PlayingSequence_PLAY_DURATION_TIMER_EXPIRED(   t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::PLAY_DURATION_TIMER_EXPIRED event in @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
*/

e_MsRetCode PSM_PlayingSequence_PLAY_DURATION_TIMER_EXPIRED(   t_MsEvent* poEvent_i )
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
		poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
		//Change state to PSM_Start
		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PSM_InterSeqDelay_PLAY_DURATION_TIMER_EXPIRED(   t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::PLAY_DURATION_TIMER_EXPIRED event in @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
*/
e_MsRetCode PSM_InterSeqDelay_PLAY_DURATION_TIMER_EXPIRED(   t_MsEvent* poEvent_i )
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
		poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
		//Change state to START
		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode PSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED(   t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::INTER_SEQ_TIMER_EXPIRED event in @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poEvent_i is the Media server event structure
*
* @return error code of type e_MsRetCode
*
* @author suliman shah
*
*/

e_MsRetCode PSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED(   t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//Note Start of Play Offset
	poCtx->oPlayInfo.nPlayOffsetStartTime = FW_GetCurrentTimeInMilliSec(  poCtx->pvFwCtx );

	eRetCode = MOP_ChangeState( poCtx, MOS_PlayingSequence );

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

