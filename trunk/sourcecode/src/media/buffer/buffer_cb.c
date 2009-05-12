#include "buffer_priv.h"

e_MsRetCode BufferAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_Buffer* poBuffer = hMediaCompAppCtx_i;

	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );
	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "hDataPkt_i = %p\n", hDataPkt_i );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poBuffer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		//ms_assert_return( DPT_GEN_Packet == poDataPkt->eType	, eRetCode = MS_ERR_PRE_COND_FAILED);
		ms_assert_return( MCT_BUFFER_MAGIC_NUM == poBuffer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nRetVal = 0;

		MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, " Sending event with id = %d\n", poBuffer->nEventId );

		nRetVal = FW_EvtSysSendIntEvt( poBuffer->oComp.pvFrameworkCtx, poBuffer->nEventId, poDataPkt, sizeof (t_DataPkt));
		if( AMPS_SUCCESS != nRetVal )
		{
			MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );

			eRetCode = MS_ERR_FRAMEWORK;
		}
		// Delete packet, but not packet data
		FW_Free( poDataPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode BufferAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_Buffer* poBuffer = hMediaCompAppCtx_i;

	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poBuffer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poSigPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_BUFFER_MAGIC_NUM == poBuffer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

void BufferEventHandler( void* pvFrameworkCtx_i, void* pvBufferCtx_i, void* pvEventHandle_i, void* pvDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Buffer* poBuffer = (t_Buffer*)pvBufferCtx_i;
	t_DataPkt* poDataPkt = pvDataPkt_i;

	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );
	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "pvBufferCtx_i = %p,\n", pvBufferCtx_i );
	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "pvEventHandle_i = %p,\n", pvEventHandle_i );
	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "pvDataPkt_i = %p,\n", pvDataPkt_i );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poBuffer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_BUFFER_MAGIC_NUM == poBuffer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_DataPkt* poDataPktCopy = NULL;

		poDataPktCopy = FW_Malloc( sizeof (t_DataPkt));
		if( NULL != poDataPktCopy )
		{
			// Copy the contents of original packet
			memcpy( poDataPktCopy, poDataPkt, sizeof (t_DataPkt));

			// Call data callback function of the next layer
			ms_assert_return( NULL != poBuffer->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );

			eRetCode = (poBuffer->oAppData.pfAppDataCb)((h_MediaComp)poBuffer, (h_MediaCompAppCtx)poBuffer->oAppData.hAppDataCtx, (h_DataPkt)poDataPktCopy );
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));
}

