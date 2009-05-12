#include "reseqbuffer_priv.h"

e_MsRetCode ReseqBufferInit( h_CompConfig hCompConfig_i, h_MediaComp* phReseqBuffer_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_ReseqBufferConfig* poReseqBufferConfig = (t_ReseqBufferConfig*)hCompConfig_i;

	MS_TRACE( poReseqBufferConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poReseqBufferConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poReseqBufferConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phReseqBuffer_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_ReseqBuffer* poReseqBuffer = NULL;

		// Allocate ReseqBuffer structure
		poReseqBuffer = FW_Malloc( sizeof (t_ReseqBuffer));
		if( NULL != poReseqBuffer )
		{
			poReseqBuffer->oComp.eType = MCT_RESEQBUFFER;
			poReseqBuffer->oComp.eGroup = MCG_FILTER;
			// Populate ReseqBuffer structure
			poReseqBuffer->oComp.nTraceId = poReseqBufferConfig->oConfig.nTraceId;
			poReseqBuffer->oComp.pvFrameworkCtx = poReseqBufferConfig->oConfig.pvFrameworkCtx;
			poReseqBuffer->nTimerMs = poReseqBufferConfig->nTimerMs;
			memset( &poReseqBuffer->oAppData, 0, sizeof (t_MediaCompAppData));
			poReseqBuffer->poPktList = FW_DListInit( &poReseqBuffer->poPktList );

			poReseqBuffer->nMagicNum = MCT_RESEQBUFFER_MAGIC_NUM;

			*phReseqBuffer_o = (h_MediaComp) poReseqBuffer;
		}
		else
		{
			MS_TRACE( poReseqBufferConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_ReseqBuffer\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poReseqBufferConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode ReseqBufferSetAppData( h_MediaComp hReseqBuffer_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_ReseqBuffer* poReseqBuffer = hReseqBuffer_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poReseqBuffer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RESEQBUFFER_MAGIC_NUM == poReseqBuffer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poReseqBuffer->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode ReseqBufferDestroy( h_MediaComp hReseqBuffer_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_ReseqBuffer* poReseqBuffer = hReseqBuffer_i;
	int nTraceId = poReseqBuffer->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poReseqBuffer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RESEQBUFFER_MAGIC_NUM == poReseqBuffer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		FW_DListFree( &poReseqBuffer->poPktList, ReseqBufferPktListFreeLinkDataCallback );

		memset( poReseqBuffer, 0, sizeof (t_ReseqBuffer));
		FW_Free( poReseqBuffer );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode ReseqBufferSetStatus( h_MediaComp hReseqBuffer_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_ReseqBuffer* poReseqBuffer = hReseqBuffer_i;

	MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poReseqBuffer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_RESEQBUFFER_MAGIC_NUM == poReseqBuffer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eStatus )
		{
			case MCS_START:
			{
				if( NULL == poReseqBuffer->hTimerId )
				{
					poReseqBuffer->hTimerId = FW_StartTimer( poReseqBuffer->oComp.pvFrameworkCtx, poReseqBuffer->nTimerMs, ReseqBufferTimeOutNotifyCallBack, poReseqBuffer );
					if( NULL != poReseqBuffer->hTimerId )
					{
						MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Timer started successfully\n" );
					}
					else
					{
						MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_StartTimer returned with error %d\n" );

						FW_Free( poReseqBuffer );

						eRetCode = MS_ERR_FRAMEWORK;
					}
				}
				break;
			}

			case MCS_STOP:
			{
				if( NULL != poReseqBuffer->hTimerId )
				{
					FW_DeleteTimer( poReseqBuffer->oComp.pvFrameworkCtx, poReseqBuffer->hTimerId );
					poReseqBuffer->hTimerId = NULL;
				}
				break;
			}

			default:
			{
				MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poReseqBuffer, poReseqBuffer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

