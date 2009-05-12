#include "buffer_priv.h"

e_MsRetCode BufferInit( h_CompConfig hCompConfig_i, h_MediaComp* phBuffer_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_BufferConfig* poBufferConfig = hCompConfig_i;

	MS_TRACE( poBufferConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poBufferConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poBufferConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phBuffer_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		int nRetVal = 0;
		t_Buffer* poBuffer = NULL;

		// Allocate Buffer structure
		poBuffer = FW_Malloc( sizeof (t_Buffer));
		if( NULL != poBuffer )
		{
			MS_TRACE( poBufferConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Buffer Ctx Allocated = %p\n", poBuffer );

			poBuffer->oComp.eType = MCT_BUFFER;
			poBuffer->oComp.eGroup = MCG_FILTER;
			// Populate Buffer structure
			poBuffer->oComp.nTraceId = poBufferConfig->oConfig.nTraceId;
			poBuffer->oComp.pvFrameworkCtx = poBufferConfig->oConfig.pvFrameworkCtx;
			memset( &poBuffer->oAppData, 0, sizeof (t_MediaCompAppData));

			poBuffer->nMagicNum = MCT_BUFFER_MAGIC_NUM;
			poBuffer->nEventId = poBufferConfig->nEventId;

			MS_TRACE( poBufferConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, " Registering event with id = %d\n", poBuffer->nEventId );

			// Register Event
			nRetVal = FW_EvtSysRegIntEvt( poBuffer->oComp.pvFrameworkCtx, poBuffer->nEventId, BufferEventHandler, (void*)poBuffer );
			if( AMPS_SUCCESS != nRetVal )
			{
				MS_TRACE( poBufferConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, " FW_EvtSysRegIntEvt failed.\n" );

				memset( poBuffer,  0, sizeof (t_Buffer));
				FW_Free( poBuffer );

				eRetCode = MS_ERR_FRAMEWORK;
			}
			else
			{
				*phBuffer_o = (h_MediaComp) poBuffer;
			}
		}
		else
		{
			MS_TRACE( poBufferConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_Buffer\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poBufferConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode BufferSetAppData( h_MediaComp hBuffer_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Buffer* poBuffer = hBuffer_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poBuffer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_BUFFER_MAGIC_NUM == poBuffer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poBuffer->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode BufferDestroy( h_MediaComp hBuffer_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Buffer* poBuffer = hBuffer_i;
	int nTraceId = poBuffer->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poBuffer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_BUFFER_MAGIC_NUM == poBuffer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nRetVal = 0;

		// Register Event
		nRetVal = FW_EvtSysRegIntEvt( poBuffer->oComp.pvFrameworkCtx, poBuffer->nEventId, NULL, NULL );
		// Check for registeration error
		if( AMPS_SUCCESS != nRetVal )
		{
			MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_ERROR, " FW_EvtSysRegIntEvt failed.\n" );

			eRetCode = MS_ERR_FRAMEWORK;
		}
		else
		{
			memset( poBuffer, 0, sizeof (t_Buffer));
			FW_Free( poBuffer );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode BufferSetStatus( h_MediaComp hBuffer_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Buffer* poBuffer = hBuffer_i;

	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poBuffer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_BUFFER_MAGIC_NUM == poBuffer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
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
				MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poBuffer, poBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

