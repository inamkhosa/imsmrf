#include "mscml_play.h"
/*! @file mscml_play_onexit.c
*
*@brief This file contains the OnExit functions for all the states of mscml play state machine
*
*/
/**
*@fn e_MsRetCode OnExit_PSM_InterSeqDelay( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_InterSeqDelay state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/
e_MsRetCode OnExit_PSM_InterSeqDelay( t_MscmlOpCtx* poCtx_i )
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
*@fn e_MsRetCode OnExit_PSM_PlayingSequence( t_MscmlOpCtx* poCtx_i )
*
*@brief The function is called on exit of @ref e_MscmlOpState::MOS_PlayingSequence state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/
e_MsRetCode OnExit_PSM_PlayingSequence( t_MscmlOpCtx* poCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	MS_TRACE( poCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

/**
*@fn e_MsRetCode OnExit_PSM_Start( t_MscmlOpCtx* poCtx_i )
*
*@brief The function handles is called on exit of @ref e_MscmlOpState::MOS_Start state
*
* @param[in] poCtx_i is the context of the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/
e_MsRetCode OnExit_PSM_Start( t_MscmlOpCtx* poCtx_i )
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

