#include "timequeue_priv.h"

e_MsRetCode TimeQueueInit( h_CompConfig hCompConfig_i, h_MediaComp* phTimeQueue_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_TimeQueueConfig* poTimeQueueConfig = (t_TimeQueueConfig*)hCompConfig_i;

	MS_TRACE( poTimeQueueConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poTimeQueueConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poTimeQueueConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phTimeQueue_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_TimeQueue* poTimeQueue = NULL;

		// Allocate TimeQueue structure
		poTimeQueue = FW_Malloc( sizeof (t_TimeQueue));
		if( NULL != poTimeQueue )
		{
			poTimeQueue->oComp.eType = MCT_TIMEQUEUE;
			poTimeQueue->oComp.eGroup = MCG_FILTER;
			// Populate TimeQueue structure
			poTimeQueue->oComp.nTraceId = poTimeQueueConfig->oConfig.nTraceId;
			poTimeQueue->oComp.pvFrameworkCtx = poTimeQueueConfig->oConfig.pvFrameworkCtx;
			poTimeQueue->nTimerMs = poTimeQueueConfig->nTimerMs;
			memset( &poTimeQueue->oAppData, 0, sizeof (t_MediaCompAppData));
			poTimeQueue->poPktList = FW_DListInit( &poTimeQueue->poPktList );

			poTimeQueue->nMagicNum = MCT_TIMEQUEUE_MAGIC_NUM;

			poTimeQueue->hTimerId = FW_StartTimer( poTimeQueue->oComp.pvFrameworkCtx, poTimeQueue->nTimerMs, TimeQueueTimeOutNotifyCallBack, poTimeQueue );
			if( NULL != poTimeQueue->hTimerId )
			{
				MS_TRACE( poTimeQueueConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Timer started successfully\n" );

				*phTimeQueue_o = (h_MediaComp) poTimeQueue;
			}
			else
			{
				MS_TRACE( poTimeQueueConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_StartTimer returned with error %d\n" );

				FW_Free( poTimeQueue );

				eRetCode = MS_ERR_FRAMEWORK;
			}
		}
		else
		{
			MS_TRACE( poTimeQueueConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_TimeQueue\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poTimeQueueConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode TimeQueueSetAppData( h_MediaComp hTimeQueue_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_TimeQueue* poTimeQueue = hTimeQueue_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poTimeQueue, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_TIMEQUEUE_MAGIC_NUM == poTimeQueue->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poTimeQueue->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode TimeQueueDestroy( h_MediaComp hTimeQueue_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_TimeQueue* poTimeQueue = hTimeQueue_i;
	int nTraceId = poTimeQueue->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poTimeQueue, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_TIMEQUEUE_MAGIC_NUM == poTimeQueue->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		FW_DeleteTimer( poTimeQueue->oComp.pvFrameworkCtx, poTimeQueue->hTimerId );

		FW_DListFree( &poTimeQueue->poPktList, TimeQueuePktListFreeLinkDataCallback );

		memset( poTimeQueue, 0, sizeof (t_TimeQueue));
		FW_Free( poTimeQueue );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode TimeQueueSetStatus( h_MediaComp hTimeQueue_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_TimeQueue* poTimeQueue = hTimeQueue_i;

	MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poTimeQueue, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_TIMEQUEUE_MAGIC_NUM == poTimeQueue->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eStatus )
		{
			case MCS_START:
			{
				break;
			}

			case MCS_STOP:
			{
				break;
			}

			default:
			{
				MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

