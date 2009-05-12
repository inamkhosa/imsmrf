#include "reseqbuffer_priv.h"

e_MsRetCode ReseqBufferAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_ReseqBuffer* poReseqBuffer = hMediaCompAppCtx_i;

	MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poReseqBuffer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RESEQBUFFER_MAGIC_NUM == poReseqBuffer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		// insert packet in queue

		MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Appending data = %p\n", poDataPkt );

		FW_DListAppend( poReseqBuffer->poPktList, poDataPkt );
		FW_DListSort( &poReseqBuffer->poPktList, ReseqBufferGenTimeCompare );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode ReseqBufferAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_ReseqBuffer* poReseqBuffer = hMediaCompAppCtx_i;

	MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

int ReseqBufferTimeOutNotifyCallBack( AMPS_HANDLE hAMPS_HANDLE_i, void* pvData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_ReseqBuffer* poReseqBuffer = pvData_i;

	MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poReseqBuffer, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_DataPkt* poDataPkt = NULL;

		// Check if there are any data in the queue
		while( NULL != poReseqBuffer->poPktList->poAMPSSListHead )
		{
			// Get the first packet
			poDataPkt = poReseqBuffer->poPktList->poAMPSSListHead->pvData;
			AMPS_DListRemoveFirstNode( &poReseqBuffer->poPktList, NULL );

			MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Sending packet to next layer = %p\n", poDataPkt );

			// Call data callback function of the next layer
			ms_assert_return( NULL != poReseqBuffer->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );

			eRetCode = (poReseqBuffer->oAppData.pfAppDataCb)((h_MediaComp)poReseqBuffer, (h_MediaCompAppCtx)poReseqBuffer->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
		}
		//
		poReseqBuffer->hTimerId = FW_StartTimer( hAMPS_HANDLE_i, poReseqBuffer->nTimerMs, ReseqBufferTimeOutNotifyCallBack, pvData_i );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return ((int)eRetCode);
}

int ReseqBufferPktListFreeLinkDataCallback( void** ppvData_io )
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

int ReseqBufferGenTimeCompare( void* pvData1_i, void* pvData2_i )
{
	t_DataPkt* poPkt1 = pvData1_i;
	t_DataPkt* poPkt2 = pvData2_i;
	if( poPkt1->nPktGenTime > poPkt2->nPktGenTime )
	{
		return (AMPS_SUCCESS);
	}
	else
	{
		return (AMPS_ERROR_FAILURE);
	}
}

