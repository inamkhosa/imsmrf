#include "mscml_sm.h"
/*! @file mscml_sm.c
*
*@brief This file contains the common functionality of all the state machines(Play, PlayRecord and PlayCollect)
*
*/
/**
*@fn e_MsRetCode MOP_Dummy( t_MsEvent* poEvent_i )
*
*@brief This is a dummy function which does nothing
*
* @param[in] poEvent_i is the context for the media server
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

f_FreeEvent pf_FreeEventData[MS_EVENT_MAX] =
{

	MOP_FreeEvent_MSCML_RESPONSE,	//	MSCML_RESPONSE		/*!< Mscml response event of the state machines */
	MOP_FreeEvent_MSCML_REQUEST,	//	MSCML_REQUEST		/*!< Mscml request event of the state machines */
	MOP_FreeEvent_NONE,	//	SM_STOP			/*!< Stop state machine */
	MOP_FreeEvent_SM_STOPPED,	//	SM_STOPPED		/*!< Indicates that state machine has stopped */
	MOP_FreeEvent_START_PLAY,	//	START_PLAY		/*!< Start playing */
	MOP_FreeEvent_NONE,	//	STOP_PLAY		/*!< Stop playing */
	MOP_FreeEvent_NONE,	//	START_PLAY_ERROR	/*!< Error in starting file play */
	MOP_FreeEvent_NONE,	//	PLAY_ENDED		/*!< File playing has ended */
	MOP_FreeEvent_NONE,	//	DTMF_KEY_PRESSED	/*!< DTMF key is pressed */
	MOP_FreeEvent_NONE,	//	RETURN_KEY_PRESSED	/*!< Return key is pressed */
	MOP_FreeEvent_NONE,	//	ESCAPE_KEY_PRESSED	/*!< Escape key is pressed */
	MOP_FreeEvent_NONE,	//	FF_KEY_PRESSED		/*!< Forward key is pressed */
	MOP_FreeEvent_NONE,	//	RW_KEY_PRESSED		/*!< Rewind key is pressed */
	MOP_FreeEvent_NONE,	//	SILENCE_DETECTED	/*!< Silence is detected (after sound) in audio */
	MOP_FreeEvent_NONE,	//	SOUND_DETECTED		/*!< Sound is detected (after silence) in audio */
	MOP_FreeEvent_NONE,	//	STOP_RECORDING		/*!< Stop recording media */
	MOP_FreeEvent_START_RECORDING,	//	START_RECORDING		/*!< Start recording media */
	MOP_FreeEvent_NONE,	//	START_RECORDING_ERROR	/*!< Error in starting recording */
	MOP_FreeEvent_NONE,	//	STOP_RECORDING_ERROR	/*!< Error in stopping recording */
	MOP_FreeEvent_NONE,	//	RECORDING_STOPPED	/*!< Recording has stopped */
	MOP_FreeEvent_NONE,	//	RECORDING_DURATION_TIMER_EXPIRED	/*!< Recording duration timer expired */
	MOP_FreeEvent_NONE,	//	RECORDING_SILENCE_TIMER_EXPIRED		/*!< Recording silence timer expired */
	MOP_FreeEvent_NONE,	//	PLAY_DURATION_TIMER_EXPIRED		/*!< Time of total duration of play has ended */
	MOP_FreeEvent_NONE,	//	INTER_SEQ_TIMER_EXPIRED			/*!< Time of inter-sequence delay has ended */
	MOP_FreeEvent_NONE,	//	COLLECTION_TIMER_EXPIRED		/*!< One of digit collection timers has ended */
	MOP_FreeEvent_NONE,	//	GET_QUARANTINE		/*!< Get quarantine buffer */
	MOP_FreeEvent_NONE	//	CLEAR_QUARANTINE	/*!< Clear quarantine buffer */
};

e_MsRetCode MOP_Dummy( t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_OnEntryOnExit_Dummy( t_MscmlOpCtx* poCtx )
*
*@brief This is a dummy function which does nothing
*
* @param[in] poCtx is the context for the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_OnEntryOnExit_Dummy( t_MscmlOpCtx* poCtx )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	{
		eRetCode = MS_ERR_INVALID_INPUT;
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_ChangeState(  t_MscmlOpCtx* poCtx_i, e_MscmlOpState eState )
*
*@brief This function changes state for the state machine
*
* @param[in] poCtx_i is the context for the state machine
*
* @param[in] eState next state
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_ChangeState(  t_MscmlOpCtx* poCtx_i, e_MscmlOpState eState )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MOS_STATES > eState, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		//1.if current state is different from new state
		if( poCtx_i->eCurrentState != eState )
		{
			//1.Call current state onexit function.
			eRetCode = poCtx_i->pfMscmlOnExit[poCtx_i->eCurrentState]( poCtx_i );
			if( MS_ERR_NONE == eRetCode )
			{
				//2.Change state variable
				poCtx_i->eCurrentState = eState;
				//3.Call onentry on current (new) state.
				eRetCode = poCtx_i->pfMscmlOnEntry[poCtx_i->eCurrentState]( poCtx_i );
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_START_PLAY( t_MscmlOpCtx* poCtx_i )
*
*@brief The function generates @ref e_MsEvent::START_PLAY event
*
* @param[in] poCtx_i is the context for the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_START_PLAY( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCtx_i->oPlayInfo.poCurAudioNode, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_MsEvent oEvent;

		oEvent.pvCtx = poCtx_i;
		oEvent.pvAppCtx = poCtx_i->pvAppCtx;

		oEvent.eType = START_PLAY;
		oEvent.uEventParams.oStartPlayParams.nPlayOffset = poCtx_i->oPlayInfo.nOffsetMs;
		oEvent.uEventParams.oStartPlayParams.cpFileName = xmlGetProp( poCtx_i->oPlayInfo.poCurAudioNode, "url" );
		if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx_i->pvFwCtx, PLAYER_REQ, &oEvent, sizeof (t_MsEvent)))
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_START_BEEP( t_MscmlOpCtx* poCtx_i )
*
*@brief The function generates @ref e_MsEvent::START_PLAY event with parameter "beep.wav"
*
* @param[in] poCtx_i is the context for the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_START_BEEP( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_MsEvent oEvent;
		oEvent.pvCtx = poCtx_i;
		oEvent.pvAppCtx = poCtx_i->pvAppCtx;
		char* pcBeepFileName;
		pcBeepFileName = FW_Malloc( (strlen( poCtx_i->pcSoundDir ) + strlen( "beep.wav" )) + 1 );
		strcpy( pcBeepFileName, poCtx_i->pcSoundDir );
		strcat( pcBeepFileName, "beep.wav" );
		oEvent.eType = START_PLAY;
		oEvent.uEventParams.oStartPlayParams.nPlayOffset = 0;
		oEvent.uEventParams.oStartPlayParams.cpFileName = pcBeepFileName;
		if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx_i->pvFwCtx, PLAYER_REQ, &oEvent, sizeof (t_MsEvent)))
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_STOP_PLAY( t_MscmlOpCtx* poCtx_i )
*
*@brief The function generates @ref e_MsEvent::STOP_PLAY event
*
* @param[in] poCtx_i is the context for the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_STOP_PLAY( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_MsEvent oEvent;
		oEvent.pvCtx = poCtx_i;
		oEvent.pvAppCtx = poCtx_i->pvAppCtx;
		oEvent.eType = STOP_PLAY;
		if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx_i->pvFwCtx, PLAYER_REQ, &oEvent, sizeof (t_MsEvent)))
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_MSCML_RESPONSE(  t_MscmlOpCtx* poCtx_i )
*
*@brief The function generates @ref e_MsEvent::MSCML_RESPONSE event
*
* @param[in] poCtx_i is the context for the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_MSCML_RESPONSE(  t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlDocPtr pxdResponseFile;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_MsEvent oEvent;
		oEvent.eType = MSCML_RESPONSE;
		oEvent.pvCtx = poCtx_i;
		oEvent.pvAppCtx = poCtx_i->pvAppCtx;

		pxdResponseFile = MOP_GenerateMscmlResponse( poCtx_i );
		if( NULL == pxdResponseFile )
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "MOP_GenerateMscmlResponse failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
		//xmlDocDump( stdout, pxdResponseFile );
		oEvent.uEventParams.oMSCMLResponseParams.xdpMSCMLResponse = pxdResponseFile;
		if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx_i->pvFwCtx, MSCML_PLAY_RES, &oEvent, sizeof (t_MsEvent)))
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_STOP_INTER_SEQ_TIMER(  t_MscmlOpCtx* poCtx_i )
*
*@brief The function stops intersequence timer
*
* @param[in] poCtx_i is the context for the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_STOP_INTER_SEQ_TIMER(  t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	if( poCtx_i->oPlayInfo.pvDelayTimerID != NULL )
	{
		// Stop the timer
		if( AMPS_SUCCESS != FW_DeleteTimer( poCtx_i->pvFwCtx, poCtx_i->oPlayInfo.pvDelayTimerID ))
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_DeleteTimer Failed\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
		poCtx_i->oPlayInfo.pvDelayTimerID = NULL;
	}
	POST_CONDITION
	{
	}
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_STOP_PLAY_DURATION_TIMER(  t_MscmlOpCtx* poCtx_i )
*
*@brief The function stops play duration timer
*
* @param[in] poCtx_i is the context for the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_STOP_PLAY_DURATION_TIMER(  t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		if( poCtx_i->oPlayInfo.pvDurationTimerID != NULL )
		{
			// Stop the timer
			if( AMPS_SUCCESS != FW_DeleteTimer( poCtx_i->pvFwCtx, poCtx_i->oPlayInfo.pvDurationTimerID ))
			{
				MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_DeleteTimer Failed\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			poCtx_i->oPlayInfo.pvDurationTimerID = NULL;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_STOP_COLLECTION_TIMER(  t_MscmlOpCtx* poCtx_i )
*
*@brief The function stops collection timer
*
* @param[in] poCtx_i is the context for the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_STOP_COLLECTION_TIMER(  t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		if( poCtx_i->uSmInfo.oPlayCollectInfo.pvCollectionTimerID != NULL )
		{
			// Stop the timer
			if( AMPS_SUCCESS != FW_DeleteTimer( poCtx_i->pvFwCtx, poCtx_i->uSmInfo.oPlayCollectInfo.pvCollectionTimerID ))
			{
				MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_DeleteTimer Failed\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			poCtx_i->uSmInfo.oPlayCollectInfo.pvCollectionTimerID = NULL;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_START_COLLECTION_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i )
*
*@brief The function starts collection timer
*
* @param[in] poCtx_i is the context for the state machine
*
* @param[in] nTimer_i is timer value
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_START_COLLECTION_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Starting timer of %d ms\n", nTimer_i );
		if( nTimer_i > MIN_TIMER_MS )   // minimum value of timer supported in Framework
		{
			poCtx_i->uSmInfo.oPlayCollectInfo.pvCollectionTimerID = FW_StartTimer( poCtx_i->pvFwCtx, nTimer_i, MOP_CollectionTimerExpired, poCtx_i );
		}
		else
		{
			eRetCode = MOP_CollectionTimerExpired( poCtx_i->pvFwCtx, (void*)poCtx_i );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_START_PLAY_DURATION_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i )
*
*@brief The function starts play duration timer
*
* @param[in] poCtx_i is the context for the state machine
*
* @param[in] nTimer_i is timer value
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_START_PLAY_DURATION_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Starting timer of %d ms\n", nTimer_i );
		if( nTimer_i > MIN_TIMER_MS )   // minimum value of timer supported in Framework
		{
			poCtx_i->oPlayInfo.pvDurationTimerID = FW_StartTimer( poCtx_i->pvFwCtx, nTimer_i, MOP_DurationTimerExpired, poCtx_i );
		}
		else
		{
			eRetCode = MOP_DurationTimerExpired( poCtx_i->pvFwCtx, (void*)poCtx_i );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_START_PLAY_DELAY_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i )
*
*@brief The function starts intersequence timer
*
* @param[in] poCtx_i is the context for the state machine
*
* @param[in] nTimer_i is timer value in milliseconds
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_START_PLAY_DELAY_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Starting timer of %d ms\n", nTimer_i );
		if( nTimer_i > MIN_TIMER_MS )   // minimum value of timer supported in Framework
		{
			poCtx_i->oPlayInfo.pvDelayTimerID = FW_StartTimer( poCtx_i->pvFwCtx, nTimer_i, MOP_DelayTimerExpired, poCtx_i );
		}
		else
		{
			eRetCode = MOP_DelayTimerExpired( poCtx_i->pvFwCtx, (void*)poCtx_i );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_START_RECORDING_SILENCE_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i )
*
*@brief The function starts recording silence timer
*
* @param[in] poCtx_i is the context for the state machine
*
* @param[in] nTimer_i is timer value in milliseconds
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_START_RECORDING_SILENCE_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Starting timer of %d ms\n", nTimer_i );
		if( nTimer_i > MIN_TIMER_MS )   // minimum value of timer supported in Framework
		{
			poCtx_i->uSmInfo.oPlayRecordInfo.pvSilenceTimerID = FW_StartTimer( poCtx_i->pvFwCtx, nTimer_i, MOP_RecSilenceTimerExpired, poCtx_i );
		}
		else
		{
			eRetCode = MOP_RecSilenceTimerExpired( poCtx_i->pvFwCtx, (void*)poCtx_i );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_START_RECORDING_DURATION_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i )
*
*@brief The function generates @ref e_MsEvent::START_RECORDING_DURATION_TIMER event
*
* @param[in] poCtx_i is the context for the state machine
*
* @param[in] nTimer_i is timer value in milliseconds
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_START_RECORDING_DURATION_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Starting timer of %d ms\n", nTimer_i );
		if( nTimer_i > MIN_TIMER_MS )   // minimum value of timer supported in Framework
		{
			poCtx_i->uSmInfo.oPlayRecordInfo.pvRecordTimerID  = FW_StartTimer( poCtx_i->pvFwCtx, nTimer_i, MOP_RecDurationTimerExpired, poCtx_i );
		}
		else
		{
			eRetCode = MOP_RecDurationTimerExpired( poCtx_i->pvFwCtx, (void*)poCtx_i );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_GET_QUARANTINE( t_MscmlOpCtx* poCtx_i )
*
*@brief The function generates @ref e_MsEvent::GET_QUARANTINE event
*
* @param[in] poCtx_i is the context for the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_GET_QUARANTINE( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_MsEvent oEvent;
		oEvent.pvCtx = poCtx_i;
		oEvent.pvAppCtx = poCtx_i->pvAppCtx;
		oEvent.eType = GET_QUARANTINE;
		if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx_i->pvFwCtx, PLAYER_REQ, &oEvent, sizeof (t_MsEvent)))
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_CLEAR_QUARANTINE( t_MscmlOpCtx* poCtx_i )
*
*@brief This function generates @ref e_MsEvent::CLEAR_QUARANTINE event
*
* @param[in] poCtx_i is the context for the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_CLEAR_QUARANTINE( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_MsEvent oEvent;
		oEvent.pvCtx = poCtx_i;
		oEvent.pvAppCtx = poCtx_i->pvAppCtx;

		oEvent.eType = CLEAR_QUARANTINE;
		if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx_i->pvFwCtx, PLAYER_REQ, &oEvent, sizeof (t_MsEvent)))
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/**
*@fn int MOP_DurationTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i )
*
*@brief The function is called when the duration time expires
*
* @param[in] hFw_i is framework handle
*
* @param[in] pvTimerData_i is payload
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

int MOP_DurationTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)pvTimerData_i;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	poCtx->oPlayInfo.pvDurationTimerID = NULL;

	t_MsEvent oEvent;
	oEvent.eType = PLAY_DURATION_TIMER_EXPIRED;
	oEvent.pvCtx = poCtx;
	oEvent.pvAppCtx = poCtx->pvAppCtx;
	if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx->pvFwCtx, MSCML_OP_EVENT, &oEvent, sizeof (t_MsEvent)))
	{
		MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (AMPS_SUCCESS);
}

/**
*@fn int MOP_RecDurationTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i )
*
*@brief The function is called when record duration time expires
*
* @param[in] hFw_i is framework handle
*
* @param[in] pvTimerData_i is payload
*
* @return integer value
*
* @author suliman shah
*
*/

int MOP_RecDurationTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)pvTimerData_i;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	poCtx->uSmInfo.oPlayRecordInfo.pvRecordTimerID = NULL;

	t_MsEvent oEvent;
	oEvent.eType = RECORDING_DURATION_TIMER_EXPIRED;
	oEvent.pvCtx = poCtx;
	oEvent.pvAppCtx = poCtx->pvAppCtx;
	if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx->pvFwCtx, MSCML_OP_EVENT, &oEvent, sizeof (t_MsEvent)))
	{
		MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (AMPS_SUCCESS);
}

/**
*@fn int MOP_RecSilenceTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i )
*
*@brief The function is called when record silence time expires
*
* @param[in] hFw_i is framework handle
*
* @param[in] pvTimerData_i is payload
*
* @return integer value
*
* @author suliman shah
*
*/

int MOP_RecSilenceTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)pvTimerData_i;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	poCtx->uSmInfo.oPlayRecordInfo.pvSilenceTimerID = NULL;

	t_MsEvent oEvent;
	oEvent.eType = RECORDING_SILENCE_TIMER_EXPIRED;
	oEvent.pvCtx = poCtx;
	oEvent.pvAppCtx = poCtx->pvAppCtx;
	if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx->pvFwCtx, MSCML_OP_EVENT, &oEvent, sizeof (t_MsEvent)))
	{
		MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (AMPS_SUCCESS);
}

/**
*@fn int MOP_DelayTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i );
*
*@brief The function is called when delay time expires
*
* @param[in] hFw_i is framework handle
*
* @param[in] pvTimerData_i is payload
*
* @return integer value
*
* @author suliman shah
*
*/

int MOP_DelayTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)pvTimerData_i;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != pvTimerData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		poCtx->oPlayInfo.pvDelayTimerID = NULL;
		t_MsEvent oEvent;
		oEvent.eType = INTER_SEQ_TIMER_EXPIRED;
		oEvent.pvCtx = poCtx;
		oEvent.pvAppCtx = poCtx->pvAppCtx;
		if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx->pvFwCtx, MSCML_OP_EVENT, &oEvent, sizeof (t_MsEvent)))
		{
			MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
			return (AMPS_ERROR_FAILURE);
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (AMPS_SUCCESS);
}

/**
*@fn int MOP_CollectionTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i )
*
*@brief The function is called when collection time expires
*
* @param[in] hFw_i is framework handle
*
* @param[in] pvTimerData_i is payload
*
* @return integer value
*
* @author suliman shah
*
*/

int MOP_CollectionTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)pvTimerData_i;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	poCtx->uSmInfo.oPlayCollectInfo.pvCollectionTimerID = NULL;

	t_MsEvent oEvent;
	oEvent.eType = COLLECTION_TIMER_EXPIRED;
	oEvent.pvCtx = poCtx;
	oEvent.pvAppCtx = poCtx->pvAppCtx;
	if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx->pvFwCtx, MSCML_OP_EVENT, &oEvent, sizeof (t_MsEvent)))
	{
		MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (AMPS_SUCCESS);
}

/**
*@fn e_MsRetCode MOP_PlayNextFile( t_MscmlOpCtx* poCtx_i )
*
*@brief The function plays next audio file
*
* @param[in] poCtx_i event configuration structure
*
* @return integer value
*
* @author suliman shah
*
*/

e_MsRetCode MOP_PlayNextFile( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		//If more file to play in sequence
		xmlNodePtr poNode = NULL;
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poCtx_i->oPlayInfo.poCurAudioNode->next; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"audio" ) )
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			poCtx_i->oPlayInfo.poCurAudioNode = poNode;
			poCtx_i->oPlayInfo.nOffsetMs = 0;
			//Generate START_PLAY
			eRetCode = MOP_Generate_START_PLAY( poCtx_i );
		}
		else // sequence ended
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Sequence has ended.\n" );

			for( poNode = poCtx_i->oPlayInfo.poCurAudioNode->parent->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"audio" ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			ms_assert( NULL != poNode );

			// Re Initailize to the first file
			poCtx_i->oPlayInfo.poCurAudioNode = poNode;
			poCtx_i->oPlayInfo.nOffsetMs = 0;

			int nRepeatInMsg = 1;
			GetPromptAttrAsInt( poCtx_i->poMscmlReq, "repeat", &nRepeatInMsg );
			//if repeat_count is lesser than "repeat" in message
			if( poCtx_i->oPlayInfo.nRepeatCount < nRepeatInMsg )
			{
				int nDelayInMsg = 0; // default value

				GetPromptAttrAsMilliSec( poCtx_i->poMscmlReq, "delay", &nDelayInMsg );
				//If inter-sequence delay in request is greater than 0
				if( nDelayInMsg > 0 )
				{
					//Change state to INTER_SEQ_DELAY
					eRetCode = MOP_ChangeState( poCtx_i, MOS_InterSeqDelay );
				}
				else
				{
					// Play the next file from start
					poCtx_i->oPlayInfo.nPlayOffsetStartTime = FW_GetCurrentTimeInMilliSec(  poCtx_i->pvFwCtx );
					poCtx_i->oPlayInfo.nRepeatCount++;

					//Generate START_PLAY
					eRetCode = MOP_Generate_START_PLAY( poCtx_i );
				}
			}
			else
			{
				eRetCode = MS_ERR_END_OF_LIST;
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_AnyState_SM_STOP( t_MsEvent* poEvent_i )
*
*@brief The function handles @ref e_MsEvent::SM_STOP event
*
* @param[in] poEvent_i event configuration structure
*
* @return integer value
*
* @author suliman shah
*
*/

e_MsRetCode MOP_AnyState_SM_STOP(   t_MsEvent* poEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent_i->pvCtx;

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	{
		poCtx->oRespInfo.nResponseCode = SIP_RES_OK;
		//Change state to START
		eRetCode = MOP_ChangeState( poCtx, MOS_Start );
	}

	MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_CalculatePlayFileAndOffset(  t_MscmlOpCtx* poCtx_i, int nOffsetMs_i )
*
*@brief The function calculates offset for an audio sequence
*
* @param[in] poCtx_i event configuration structure
*
* @param[in] nOffsetMs_i is offset given in request message
*
* @return integer value
*
* @author suliman shah
*
*/

e_MsRetCode MOP_CalculatePlayFileAndOffset(  t_MscmlOpCtx* poCtx_i, int nOffsetMs_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poCtx_i->poMscmlReq, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		xmlNodePtr poNode = NULL;
		xmlNodePtr poPromptNode = NULL;

		int nSeqDuration = 0;

		eRetCode = GetPromptAttrAsInt( poCtx_i->poMscmlReq, "seq_duration", &nSeqDuration );

		poNode = xmlDocGetRootElement( poCtx_i->poMscmlReq );
		if( poNode != NULL )
		{
			// Search "request" node
			AMPS_BOOL bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				bNodePresent = AMPS_FALSE;

				for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
				{
					if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"play" ) ||
					    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playrecord" ) ||
					    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playcollect" ) )
					{
						bNodePresent = AMPS_TRUE;
						break;
					}
				}
				if( AMPS_TRUE == bNodePresent )
				{
					bNodePresent = AMPS_FALSE;

					for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
					{
						if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"prompt" ) )
						{
							bNodePresent = AMPS_TRUE;
							break;
						}
					}
					if( AMPS_TRUE == bNodePresent )
					{
						poPromptNode = poNode;
					}
					else
					{
						eRetCode = MS_ERR_END_OF_LIST;
					}
				}
				else
				{
					eRetCode = MS_ERR_INVALID_INPUT;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		if( MS_ERR_NONE == eRetCode )
		{
			/*RFC 5022: 6.1.1. "offset":If the offset value is greater than the total time of the sequence, it will "wrap" to the beginning and continue from there until the media server reaches the specified offset."*/

			ms_assert( 0 < nSeqDuration );

			while( nOffsetMs_i > nSeqDuration )
			{
				nOffsetMs_i -= nSeqDuration;
			}
			int nFileDuration = 0;      // duration of sequence, incremented for each file

			for( poNode = poPromptNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"audio" ) )
				{
					t_String oProp = TSTRING_INITIALIZER;

					oProp.pucData = xmlGetProp( poNode, "file_duration" );
					ms_assert( NULL != oProp.pucData );
					oProp.unLength = strlen( oProp.pucData );

					nFileDuration += TS_StringToInt( &oProp, 10 );

					TS_Free( &oProp );
					if( nOffsetMs_i < nFileDuration )
					{
						break;
					}
				}
			}
			poCtx_i->oPlayInfo.poCurAudioNode = poNode;
			poCtx_i->oPlayInfo.nOffsetMs = nOffsetMs_i;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn xmlDocPtr MOP_GenerateMscmlResponse( t_MscmlOpCtx* poCtx_i )
*
*@brief The function generates mscml response
*
* @param[in] poCtx_i event configuration structure
*
* @return mscml response as xmlDocPtr
*
* @author suliman shah
*
*/

xmlDocPtr MOP_GenerateMscmlResponse( t_MscmlOpCtx* poCtx_i )
{
	xmlDocPtr poMscmlDoc = NULL;
	char pcText[40] = "";
	char pcDigitPressed[2] = "";

	poMscmlDoc = GetMscmlResponse();

	SetResponseAttrAsInt( poMscmlDoc, "code", poCtx_i->oRespInfo.nResponseCode );
	if( NULL != poCtx_i->oRespInfo.pcResponseId )
	{
		SetResponseAttrAsString( poMscmlDoc, "id", poCtx_i->oRespInfo.pcResponseId );
	}
	if( poCtx_i->oRespInfo.nResponseCode >= 200 && poCtx_i->oRespInfo.nResponseCode < 300 )
	{
		strcpy( pcText, "OK" );
	}
	else if( poCtx_i->oRespInfo.nResponseCode >= 400 && poCtx_i->oRespInfo.nResponseCode < 500 )
	{
		strcpy( pcText, "Client Error" );
	}
	else if( poCtx_i->oRespInfo.nResponseCode >= 500 && poCtx_i->oRespInfo.nResponseCode < 600 )
	{
		strcpy( pcText, "Server Error" );
	}
	else
	{
		strcpy( pcText, "Unknown Error" );
	}
	SetResponseAttrAsString( poMscmlDoc, "text", pcText );

	xmlChar* poUrl = xmlGetProp( poCtx_i->oPlayInfo.poCurAudioNode, "url" );

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "The value of context is : %s\n", poUrl );

	SetResponseAttrAsInt( poMscmlDoc, "playduration", poCtx_i->oRespInfo.nPlayDuration );

	SetResponseAttrAsInt( poMscmlDoc, "playoffset", poCtx_i->oRespInfo.nPlayOffset );
	if( poCtx_i->oRespInfo.nResponseCode < 200 || poCtx_i->oRespInfo.nResponseCode >= 300 )
	{
		xmlChar* poUrl = xmlGetProp( poCtx_i->oPlayInfo.poCurAudioNode, "url" );
		SetResponseErrorInfoAttrAsInt( poMscmlDoc, "code", poCtx_i->oRespInfo.nResponseCode );
		SetResponseErrorInfoAttrAsString( poMscmlDoc, "text", pcText );
		SetResponseErrorInfoAttrAsString( poMscmlDoc, "context", poUrl );

		xmlFree( poUrl );
	}

	switch( poCtx_i->eType )
	{
		case MOT_PLAY:
			SetResponseAttrAsString( poMscmlDoc, "request", "play" );
			break;

		case MOT_PLAYCOLLECT:
			SetResponseAttrAsString( poMscmlDoc, "request", "playcollect" );
			if( NULL != poCtx_i->oRespInfo.pcReason )
			{
				SetResponseAttrAsString( poMscmlDoc, "reason", poCtx_i->oRespInfo.pcReason );
			}
			char pcMatchBuffer[MAX_DREG_EX_MATCH_BUFFER_LENGTH] = "";
			int nDigitIndex = 0;
			if( NULL != poCtx_i->uSmInfo.oPlayCollectInfo.hMatchedExp )
			{
				char pcRegExName[MAX_DREG_EX_NAME_LENGTH] = "";
				nDigitIndex = MAX_DREG_EX_NAME_LENGTH;

				re_get_name( poCtx_i->uSmInfo.oPlayCollectInfo.hMatchedExp, pcRegExName, &nDigitIndex );

				pcRegExName[nDigitIndex] = '\0';

				SetResponseAttrAsString( poMscmlDoc, "name", pcRegExName );

				nDigitIndex = MAX_DREG_EX_MATCH_BUFFER_LENGTH;

				re_get_current_match( poCtx_i->uSmInfo.oPlayCollectInfo.hMatchedExp, pcMatchBuffer, &nDigitIndex );

				pcMatchBuffer[nDigitIndex] = '\0';
			}
			else
			{
				t_AMPSSList* poNode = NULL;
				nDigitIndex = 0;

				for( poNode = poCtx_i->uSmInfo.oPlayCollectInfo.poPressedDigitList->poAMPSSListHead; poNode != NULL; poNode = poNode->poAMPSSListNext )
				{
					pcMatchBuffer[nDigitIndex++] = (char)poNode->pvData;
				}
				pcMatchBuffer[nDigitIndex] = '\0';
			}
			SetResponseAttrAsString( poMscmlDoc, "digits", pcMatchBuffer );

			break;

		case MOT_PLAYRECORD:
			SetResponseAttrAsString( poMscmlDoc, "request", "playrecord" );
			if( NULL != poCtx_i->oRespInfo.pcReason )
			{
				SetResponseAttrAsString( poMscmlDoc, "reason", poCtx_i->oRespInfo.pcReason );
			}
			if( poCtx_i->uSmInfo.oPlayRecordInfo.cPressedDigit != 0 )
			{
				pcDigitPressed[0] = poCtx_i->uSmInfo.oPlayRecordInfo.cPressedDigit;
				pcDigitPressed[1] = '\0';
				SetResponseAttrAsString( poMscmlDoc, "digits", pcDigitPressed );

				MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "The pressed DTMF digit is: %s\n", pcDigitPressed );
			}
			else
			{
				SetResponseAttrAsString( poMscmlDoc, "digits", "" );
			}
			SetResponseAttrAsString( poMscmlDoc, "request", "playrecord" );
			SetResponseAttrAsInt( poMscmlDoc, "reclength", poCtx_i->oRespInfo.nRecLength );
			SetResponseAttrAsInt( poMscmlDoc, "recduration", poCtx_i->oRespInfo.nRecDuration );

			break;
	}

	return (poMscmlDoc);
}

/**
*@fn void MOP_EVENT_Handler( AMPS_HANDLE hFw_i, void* pvContext_i, void* pvEventHandle_i, void* pvEvent_i )
*
*@brief The function handles the state machine events
*
* @param[in] hFw_i event configuration structure
*
* @param[in] pvContext_i event configuration structure
*
* @param[in] pvEventHandle_i event configuration structure
*
* @param[in] pvEvent_i event configuration structure
*
* @return integer value
*
* @author suliman shah
*
*/

// Main State Machine
void MOP_EVENT_Handler( AMPS_HANDLE hFw_i, void* pvContext_i, void* pvEventHandle_i, void* pvEvent_i )
{
	t_MsEvent* poEvent = pvEvent_i;
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MscmlOpCtx* poCtx = (t_MscmlOpCtx*)poEvent->pvCtx;

	//MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	{
		ms_assert_return( NULL != poEvent, eRetCode = MS_ERR_PRE_COND_FAILED );
		//ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( NULL != poCtx && MSCML_OP_MAGIC_NUM == poCtx->nMagicNum )
		{
			MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Before.\n" );
			MOP_PrintContext( poCtx );
	
			MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Event type of received event = %d\n", poEvent->eType );
	
			switch( poCtx->eType )
			{
				case MOT_PLAY:
					eRetCode = ppf_PlaySm[poCtx->eCurrentState][poEvent->eType]( poEvent );
					break;
	
				case MOT_PLAYCOLLECT:
					eRetCode = ppf_PlayCollectSm[poCtx->eCurrentState][poEvent->eType]( poEvent );
					break;
	
				case MOT_PLAYRECORD:
					eRetCode = ppf_PlayRecordSm[poCtx->eCurrentState][poEvent->eType]( poEvent );
					break;
			}

			MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "After.\n" );
			MOP_PrintContext( poCtx );
		}
	}
	MOP_FreeEvent(poEvent);

	//MS_TRACE( poCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
}

/**
*@fn int RemoveDRegNodeCb( void** ppvData_io )
*
*This is a callback function which copies payload
*
* @param[out] ppvData_io is a pointer to the payload of the callback
*
* @return int value
*
* @author suliman shah
*
*/

int RemoveDRegNodeCb( void** ppvData_io )
{
	hDRegEx hExp = *ppvData_io;

	ms_assert( NULL != hExp );

	re_destroy( &hExp );
}

/**
*@fn e_MsRetCode MOP_PrintContext( t_MscmlOpCtx* poCtx_i )
*
*@brief The function prints the context of a state machine
*
* @param[in]  poCtx_i is a pointer of the type context
*
* @return e_MsRetCode, error code
*
* @author suliman shah
*
*/

e_MsRetCode MOP_PrintContext( t_MscmlOpCtx* poCtx_i )
{
	xmlChar* poAudioFile = xmlGetProp( poCtx_i->oPlayInfo.poCurAudioNode, "url" );

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Current state = %d\n", poCtx_i->eCurrentState );

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Current Audio file = %s\n", poAudioFile );
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Offset = %d\n", poCtx_i->oPlayInfo.nOffsetMs );
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Code = %d\n", poCtx_i->oRespInfo.nResponseCode );
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Play Duration = %d\n", poCtx_i->oRespInfo.nPlayDuration );
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Play Offset = %d\n", poCtx_i->oRespInfo.nPlayOffset );
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Delay Timer Id= %p\n", poCtx_i->oPlayInfo.pvDelayTimerID );
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Duration Timer Id= %p\n", poCtx_i->oPlayInfo.pvDurationTimerID );
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Duration Start Time = %d\n", poCtx_i->oPlayInfo.nPlayDurationStartTime );
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Offset Start Time = %d\n", poCtx_i->oPlayInfo.nPlayOffsetStartTime );

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Repeat Count = %d\n", poCtx_i->oPlayInfo.nRepeatCount );

	int nCurrTime = FW_GetCurrentTimeInMilliSec( poCtx_i->pvFwCtx );
	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Diff in duration = %d\n", nCurrTime - poCtx_i->oPlayInfo.nPlayDurationStartTime );

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Diff in offset = %d\n", nCurrTime - poCtx_i->oPlayInfo.nPlayOffsetStartTime );

	switch( poCtx_i->eType )
	{
		case MOT_PLAYCOLLECT:
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Collection Timer id = %p\n", poCtx_i->uSmInfo.oPlayCollectInfo.pvCollectionTimerID );
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Inter Seq Start Time = %d\n", poCtx_i->uSmInfo.oPlayCollectInfo.nInterSeqStartTime );
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Matched Reg Exp Handle = %p\n", poCtx_i->uSmInfo.oPlayCollectInfo.hMatchedExp );

			char pcMatchBuffer[30] = "";
			t_AMPSSList* poNode = NULL;
			int nDigitIndex = 0;

			for( poNode = poCtx_i->uSmInfo.oPlayCollectInfo.poPressedDigitList->poAMPSSListHead; poNode != NULL; poNode = poNode->poAMPSSListNext )
			{
				pcMatchBuffer[nDigitIndex++] = (char)poNode->pvData;
			}
			pcMatchBuffer[nDigitIndex] = '\0';

			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Digits Pressed = \"%s\"\n", pcMatchBuffer );

			hDRegEx hExp = NULL;

			for( poNode = poCtx_i->uSmInfo.oPlayCollectInfo.poDRegExHandleList->poAMPSSListHead;
			     poNode != NULL;
			     poNode = poNode->poAMPSSListNext )
			{
				hExp = (hDRegEx)poNode->pvData;
				nDigitIndex = 30;

				re_get_current_match( hExp, pcMatchBuffer, &nDigitIndex );

				pcMatchBuffer[nDigitIndex] = '\0';

				MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Exp[%p] = \"%s\"\n", hExp, pcMatchBuffer );
			}
			break;
		}

		case MOT_PLAYRECORD:
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Recording Timer id = %p\n", poCtx_i->uSmInfo.oPlayRecordInfo.pvRecordTimerID );
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Silence Timer id = %p\n", poCtx_i->uSmInfo.oPlayRecordInfo.pvSilenceTimerID );

			break;
		}
	}

	xmlFree( poAudioFile );
}

/**
*@fn e_MsRetCode MOP_Generate_START_RECORDING( t_MscmlOpCtx* poCtx_i  )
*
*@brief This function generates @ref e_MsEvent::START_RECORDING event
*
* @param[in] poCtx_i is a pointer of the type context
*
* @return e_MsRetCode, error code
*
* @author suliman shah
*
*/

e_MsRetCode MOP_Generate_START_RECORDING( t_MscmlOpCtx* poCtx_i  )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_MsEvent oEvent;
		char* pcRecURL = NULL;

		oEvent.pvCtx = poCtx_i;
		oEvent.pvAppCtx = poCtx_i->pvAppCtx;

		oEvent.eType = START_RECORDING;

		GetRequestAttrAsString( poCtx_i->poMscmlReq, "playrecord", "recurl", &pcRecURL );

		oEvent.uEventParams.oStartRecParams.cpRecURL = pcRecURL;
		if( oEvent.uEventParams.oStartRecParams.cpRecURL == NULL )
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_Malloc Failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
		MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "oEvent.uEventParams.oStartRecParams.cpRecURL = %s.\n", oEvent.uEventParams.oStartRecParams.cpRecURL );
		if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx_i->pvFwCtx, PLAYER_REQ, &oEvent, sizeof (t_MsEvent)))
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_STOP_RECORDING_SILENCE_TIMER( t_MscmlOpCtx* poCtx_i )
*
*@brief The function stops recording silence timer
*
* @param[in] poCtx_i is a pointer of the type context
*
* @return e_MsRetCode, error code
*
* @author suliman shah
*
*/
e_MsRetCode MOP_Generate_STOP_RECORDING_SILENCE_TIMER(  t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		if( poCtx_i->uSmInfo.oPlayRecordInfo.pvSilenceTimerID != NULL )
		{
			// Stop the timer
			if( AMPS_SUCCESS != FW_DeleteTimer( poCtx_i->pvFwCtx, poCtx_i->uSmInfo.oPlayRecordInfo.pvSilenceTimerID ))
			{
				MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_DeleteTimer Failed\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			poCtx_i->uSmInfo.oPlayRecordInfo.pvSilenceTimerID = NULL;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_STOP_RECORDING( t_MscmlOpCtx* poCtx_i )
*
*@brief The function generates @ref e_MsEvent::STOP_RECORDING event
*
* @param[in] poCtx_i is a pointer of the type context
*
* @return e_MsRetCode, error code
*
* @author suliman shah
*
*/
e_MsRetCode MOP_Generate_STOP_RECORDING( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		// PlayerInputEvent Player Sturcture is used here
		t_MsEvent oEvent;
		oEvent.pvCtx = poCtx_i;
		oEvent.pvAppCtx = poCtx_i->pvAppCtx;
		oEvent.eType = STOP_RECORDING;
		if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poCtx_i->pvFwCtx, PLAYER_REQ, &oEvent, sizeof (t_MsEvent)))
		{
			MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_Generate_STOP_RECORDING_DURATION_TIMER( t_MscmlOpCtx* poCtx_i )
*
*@brief The function stops recording duration timer
*
* @param[in] poCtx_i is a pointer of the type context
*
* @return e_MsRetCode, error code
*
* @author suliman shah
*
*/
e_MsRetCode MOP_Generate_STOP_RECORDING_DURATION_TIMER( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MSCML_OP_MAGIC_NUM == poCtx_i->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		if( poCtx_i->uSmInfo.oPlayRecordInfo.pvRecordTimerID != NULL )
		{
			// Stop the timer
			if( AMPS_SUCCESS != FW_DeleteTimer( poCtx_i->pvFwCtx, poCtx_i->uSmInfo.oPlayRecordInfo.pvRecordTimerID ))
			{
				MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_DeleteTimer Failed\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			poCtx_i->uSmInfo.oPlayRecordInfo.pvRecordTimerID = NULL;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/*! @fn e_MsRetCode CalcualateFileDuration( xmlDocPtr poMscmlDoc_i )
*
* @brief This function calculates the duration of the audio file in the given MSCML message. This duration is stored as a new attribute named 'file_duration' in audio node. Also the attribute named 'seq_duration' is added in the prompt node.
*
* @param[in] poMscmlDoc_i is the xml document pointer for the MSCML request message.
*
* @return e_MsRetCode the code for success or error
*
*/

//e_MsRetCode CalcualateFileDuration( xmlDocPtr poMscmlDoc_i )
e_MsRetCode CalcualateFileDuration( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poCtx_i->poMscmlReq );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"play" ) ||
				    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playrecord" ) ||
				    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playcollect" ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				bNodePresent = AMPS_FALSE;

				for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
				{
					if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"prompt" ) )
					{
						bNodePresent = AMPS_TRUE;
						break;
					}
				}
				if( AMPS_TRUE == bNodePresent )
				{
					bNodePresent = AMPS_FALSE;
					int nSeqDuration = 0;
					xmlNodePtr poPromptNode = poNode;

					for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
					{
						if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"audio" ) )
						{
							xmlChar* poUrl = NULL;

							poUrl = xmlGetProp( poNode, (xmlChar*)"url" );

							{
								SNDFILE* poFile = NULL;
								SF_INFO oSfInfo;

								poFile = sf_open( poUrl, SFM_READ, &oSfInfo );
								if( NULL != poFile )
								{
									if( oSfInfo.samplerate > 0 )
									{
										xmlAttrPtr poAttr = NULL;
										int nFileDuration = 0;
										unsigned char pucFileDuration[33] = "";

										nFileDuration = (oSfInfo.frames * 1000) / oSfInfo.samplerate;
										TS_IntToString( pucFileDuration, nFileDuration, 10 );

										nSeqDuration += nFileDuration;

										poAttr = xmlNewProp( poNode, "file_duration", pucFileDuration );
									}
									else
									{
										eRetCode = MS_ERR_INVALID_INPUT;
										break;
									}
									sf_close( poFile );
								}
								else
								{
									poCtx_i->oPlayInfo.poCurAudioNode = poNode;
									eRetCode = MS_ERR_INVALID_INPUT;
									break;
								}
							}

							xmlFree( poUrl );
						}
					}
					// Add "seq_duration" property
					xmlAttrPtr poAttr = NULL;
					unsigned char pucSeqDuration[33] = "";

					TS_IntToString( pucSeqDuration, nSeqDuration, 10 );

					poAttr = xmlNewProp( poPromptNode, "seq_duration", pucSeqDuration );
				}
				else
				{
					eRetCode = MS_ERR_END_OF_LIST;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_FreeEvent( t_MsEvent* poEvent_i )
*
* @brief The function performs a deep-free of the event
*
* @param[in] poEvent_i is the event to de-allocate
*
* @return e_MsRetCode, error code
*
* @author Waqqas Jabbar
*
*/
e_MsRetCode MOP_FreeEvent(t_MsEvent* poEvent_i )
{
	pf_FreeEventData[poEvent_i->eType](poEvent_i);

	//FW_Free(poEvent_i);

	return MS_ERR_NONE;
}

e_MsRetCode MOP_FreeEvent_NONE(t_MsEvent* poEvent_i )
{
	return MS_ERR_NONE;
}

e_MsRetCode MOP_FreeEvent_MSCML_RESPONSE(t_MsEvent* poEvent_i )
{
	if( NULL != poEvent_i->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse )
	{
		xmlFreeDoc(poEvent_i->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse);
	}

	return MS_ERR_NONE;
}

e_MsRetCode MOP_FreeEvent_MSCML_REQUEST(t_MsEvent* poEvent_i )
{
	if( NULL != poEvent_i->uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile )
	{
		xmlFreeDoc(poEvent_i->uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile);
	}

	return MS_ERR_NONE;
}

e_MsRetCode MOP_FreeEvent_SM_STOPPED(t_MsEvent* poEvent_i )
{
	if( NULL != poEvent_i->uEventParams.oSMStoppedParams.xdpMSCMLResponse )
	{
		xmlFreeDoc(poEvent_i->uEventParams.oSMStoppedParams.xdpMSCMLResponse);
	}

	return MS_ERR_NONE;
}

e_MsRetCode MOP_FreeEvent_START_PLAY(t_MsEvent* poEvent_i )
{
	if( NULL != poEvent_i->uEventParams.oStartPlayParams.cpFileName )
	{
		FW_Free(poEvent_i->uEventParams.oStartPlayParams.cpFileName);
	}

	return MS_ERR_NONE;
}

e_MsRetCode MOP_FreeEvent_START_RECORDING(t_MsEvent* poEvent_i )
{
	if( NULL != poEvent_i->uEventParams.oStartRecParams.cpRecURL )
	{
		FW_Free(poEvent_i->uEventParams.oStartRecParams.cpRecURL);
	}

	return MS_ERR_NONE;
}







