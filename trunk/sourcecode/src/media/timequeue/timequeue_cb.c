#include "timequeue_priv.h"

e_MsRetCode TimeQueueAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_TimeQueue* poTimeQueue = hMediaCompAppCtx_i;

	MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poTimeQueue, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_TIMEQUEUE_MAGIC_NUM == poTimeQueue->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
//      int nRetVal = 0;
		// insert packet in queue

		MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Appending data = %p\n", poDataPkt );

		FW_DListAppend( poTimeQueue->poPktList, poDataPkt );
//      if( AMPS_SUCCESS != nRetVal )
//      {
//          MEDIA_TRACE(poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_DListAppend returned with ret-code = %d\n", nRetVal );
//
//          eRetCode = MS_ERR_FRAMEWORK;
//      }
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode TimeQueueAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_TimeQueue* poTimeQueue = hMediaCompAppCtx_i;

	MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

int TimeQueueTimeOutNotifyCallBack( AMPS_HANDLE hAMPS_HANDLE_i, void* pvData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_TimeQueue* poTimeQueue = pvData_i;

	MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poTimeQueue, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_DataPkt* poDataPkt = NULL;

		// Check if there are any data in the queue
		while( NULL != poTimeQueue->poPktList->poAMPSSListHead )
		{
			// Get the first packet
			poDataPkt = poTimeQueue->poPktList->poAMPSSListHead->pvData;
			AMPS_DListRemoveFirstNode( &poTimeQueue->poPktList, NULL );

			MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Sending packet to next layer = %p\n", poDataPkt );

			// Call data callback function of the next layer
			ms_assert_return( NULL != poTimeQueue->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );

			eRetCode = (poTimeQueue->oAppData.pfAppDataCb)((h_MediaComp)poTimeQueue, (h_MediaCompAppCtx)poTimeQueue->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
		}
		//
		poTimeQueue->hTimerId = FW_StartTimer( hAMPS_HANDLE_i, poTimeQueue->nTimerMs, TimeQueueTimeOutNotifyCallBack, pvData_i );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poTimeQueue, poTimeQueue->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return ((int)eRetCode);
}

int TimeQueuePktListFreeLinkDataCallback( void** ppvData_io )
{
	{
		t_DataPkt* poDataPkt = *ppvData_io;

		(poDataPkt->pfnDataPktDelete)( poDataPkt );

		memset( poDataPkt, 0, sizeof (t_DataPkt) );

		FW_Free( poDataPkt );

		*ppvData_io = NULL;
	}
	return ((int)AMPS_SUCCESS);
}

