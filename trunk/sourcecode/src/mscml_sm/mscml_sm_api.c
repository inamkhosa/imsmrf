#include "mscml_sm.h"
/*! @file mscml_sm_api.c
*
*@brief This file provides the APIs functions for using the functionality of the state machines
*
*/
/**
*@fn e_MsRetCode MOP_CreateSM( e_MscmlOpType eType_i, int nTraceId_i, void* pvFw_i, char* pcSoundDir_i, void** ppvMscmlOpCtx_o )
*
*@brief The function creates a context for state machines(play,play record, play collect)
*
* @param[in] eType_i is event type
*
* @param[in] nTraceId_i is framework traceid
*
* @param[in] pvFw_i is our framework context
*
* @param[in] pcSoundDir_i is a sound directory url
*
* @param[out] ppvMscmlOpCtx_o is an allocated context
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/

e_MsRetCode MOP_CreateSM( e_MscmlOpType eType_i, int nTraceId_i, void* pvFw_i, char* pcSoundDir_i, void** ppvMscmlOpCtx_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MscmlOpCtx* poCtx = NULL;

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != pvFw_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( nTraceId_i != 0, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */
	{
		poCtx = (t_MscmlOpCtx*) FW_Malloc( sizeof (t_MscmlOpCtx));
		if( NULL == poCtx )
		{
			MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_ERROR, "Malloc Failed.\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
		else
		{
			memset( poCtx, 0, sizeof (t_MscmlOpCtx));

			poCtx->eType = eType_i;
			poCtx->eCurrentState = MOS_Start;
			poCtx->nTraceId = nTraceId_i;
			poCtx->pvFwCtx = pvFw_i;
			poCtx->nMagicNum = MSCML_OP_MAGIC_NUM;
			poCtx->pcSoundDir = strdup( pcSoundDir_i );

			switch( eType_i )
			{
				case MOT_PLAY:
					poCtx->pvMscmlSm = (void*)ppf_PlaySm;
					poCtx->pfMscmlOnEntry = pf_PlaySmOnEntry;
					poCtx->pfMscmlOnExit = pf_PlaySmOnExit;
					break;

				case MOT_PLAYCOLLECT:
					poCtx->pvMscmlSm = (void*)ppf_PlayCollectSm;
					poCtx->pfMscmlOnEntry = pf_PlayCollectSmOnEntry;
					poCtx->pfMscmlOnExit = pf_PlayCollectSmOnExit;

					FW_DListInit( &poCtx->uSmInfo.oPlayCollectInfo.poPressedDigitList );
					FW_DListInit( &poCtx->uSmInfo.oPlayCollectInfo.poDRegExHandleList );
					break;

				case MOT_PLAYRECORD:
					poCtx->uSmInfo.oPlayRecordInfo.cPressedDigit = 0;
					poCtx->pvMscmlSm = (void*)ppf_PlayRecordSm;
					poCtx->pfMscmlOnEntry = pf_PlayRecordSmOnEntry;
					poCtx->pfMscmlOnExit = pf_PlayRecordSmOnExit;
					break;
			}
			*ppvMscmlOpCtx_o = poCtx;
		}
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *ppvMscmlOpCtx_o );
		}
	}

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_DestroySM( void* pvMscmlOpCtx_i )
*
*@brief The function destroyes the context of the state machine(play,play record, play collect)
*
* @param[in] pvMscmlOpCtx_i is a context of the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/
e_MsRetCode MOP_DestroySM( void* pvMscmlOpCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MscmlOpCtx* poCtx = pvMscmlOpCtx_i;
	int nTraceId = poCtx->nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	//PRE_CONDITION
	{
	}

	{
		if( NULL != poCtx->oRespInfo.pcResponseId )
		{
			FW_Free( poCtx->oRespInfo.pcResponseId );
		}
		xmlFreeDoc( poCtx->poMscmlReq );
		FW_Free( poCtx->pcSoundDir );

		switch( poCtx->eType )
		{
			case MOT_PLAY:
				MOP_Generate_STOP_INTER_SEQ_TIMER( poCtx );
				MOP_Generate_STOP_PLAY_DURATION_TIMER( poCtx );
				break;

			case MOT_PLAYCOLLECT:
				MOP_Generate_STOP_INTER_SEQ_TIMER( poCtx );
				MOP_Generate_STOP_PLAY_DURATION_TIMER( poCtx );

				MOP_Generate_STOP_COLLECTION_TIMER( poCtx );

				FW_DListFree( &poCtx->uSmInfo.oPlayCollectInfo.poPressedDigitList, NULL );
				FW_DListFree( &poCtx->uSmInfo.oPlayCollectInfo.poDRegExHandleList, RemoveDRegNodeCb );

				break;

			case MOT_PLAYRECORD:
				MOP_Generate_STOP_INTER_SEQ_TIMER( poCtx );
				MOP_Generate_STOP_PLAY_DURATION_TIMER( poCtx );

				MOP_Generate_STOP_RECORDING_SILENCE_TIMER( poCtx );
				MOP_Generate_STOP_RECORDING_DURATION_TIMER( poCtx );

				break;
		}

		FW_Free( pvMscmlOpCtx_i );
	}
	/* - */
	{
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_RegisterEvent( void* pvFw_i, void* pvCtx_i )
*
*@brief The function register an event of type play, playrecord or playcollect
*
* @param[in] pvFw_i is the context of the framework
*
* @param[in] pvCtx_i is a context of the state machine
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/
e_MsRetCode MOP_RegisterEvent( void* pvFw_i, void* pvCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != pvFw_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		//ms_assert_return( NULL != pvCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		int nRetVal = AMPS_SUCCESS;

		nRetVal = FW_EvtSysRegIntEvt( pvFw_i, MSCML_OP_EVENT, MOP_EVENT_Handler, pvCtx_i );
		if( AMPS_SUCCESS != nRetVal )
		{
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

/**
*@fn e_MsRetCode MOP_UnregisterEvent( void* pvFw_i )
*
*The function unregisters an event of type play,playrecord or playcollect
*
* @param[in] pvFw_i is the context of the framework
*
* @return e_MsRetCode, code for success or error
*
* @author suliman shah
*
*/
e_MsRetCode MOP_UnregisterEvent( void* pvFw_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	//PRE_CONDITION
	{
		ms_assert_return( NULL != pvFw_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		int nRetVal = AMPS_SUCCESS;

		nRetVal = FW_EvtSysCancelIntEvt( pvFw_i, MSCML_OP_EVENT, MOP_EVENT_Handler );
		if( AMPS_SUCCESS != nRetVal )
		{
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}

	return (eRetCode);
}

