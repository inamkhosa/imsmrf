#include "mscml_play.h"
/*! @file mscml_play_onentry.c
*
*@brief This file contains the OnEntry functions for all the states of mscml play state machine
*
*/
/**
**@fn e_MsRetCode OnEntry_PSM_InterSeqDelay(   t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on entry of @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode OnEntry_PSM_InterSeqDelay(   t_MscmlOpCtx* poCtx_i )
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

		poCtx_i->oPlayInfo.nPlayOffsetStartTime = 0;

		eRetCode = GetPromptAttrAsMilliSec( poCtx_i->poMscmlReq, "delay", &nDelayInMsg );
		ms_assert( MS_ERR_NONE == eRetCode );

		// Generate START_PLAY_DELAY_TIMER(request.prompt.delay)
		eRetCode =  MOP_Generate_START_PLAY_DELAY_TIMER( poCtx_i, nDelayInMsg );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnEntry_PSM_PlayingSequence(   t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on entry of @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/
e_MsRetCode OnEntry_PSM_PlayingSequence(   t_MscmlOpCtx* poCtx_i )
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
*@fn e_MsRetCode OnEntry_PSM_Start( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on entry of @ref e_MscmlOpState::MOS_Start state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode OnEntry_PSM_Start( t_MscmlOpCtx* poCtx_i )
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
		eRetCode = MOP_Generate_MSCML_RESPONSE( poCtx_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

