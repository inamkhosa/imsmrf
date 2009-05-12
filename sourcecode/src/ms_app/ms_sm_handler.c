#include "ms_sm_handler.h"

void MS_SM_Handler( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsCtx* poMsCtx =  (t_MsCtx*)hMsCtx_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hFramework_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvEventHandle_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvMsEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MsEvent* poMsEvent = (t_MsEvent*)pvMsEvent_i;

		switch( poMsEvent->eType )
		{
			case MSCML_RESPONSE:
				MS_SM_HandleMscmlResponse( hFramework_i, hMsCtx_i, pvEventHandle_i, pvMsEvent_i );
				break;

			case SM_STOPPED:
				MS_SM_HandleSmStopped( hFramework_i, hMsCtx_i, pvEventHandle_i, pvMsEvent_i );
				break;

			case START_PLAY:
				MS_SM_HandleStartPlay( hFramework_i, hMsCtx_i, pvEventHandle_i, pvMsEvent_i );
				break;

			case STOP_PLAY:
				MS_SM_HandleStopPlay( hFramework_i, hMsCtx_i, pvEventHandle_i, pvMsEvent_i );
				break;

			case START_RECORDING:
				MS_SM_HandleStartRecording( hFramework_i, hMsCtx_i, pvEventHandle_i, pvMsEvent_i );
				break;

			case STOP_RECORDING:
				MS_SM_HandleStopRecording( hFramework_i, hMsCtx_i, pvEventHandle_i, pvMsEvent_i );
				break;

			case CLEAR_QUARANTINE:
				MS_SM_ClearQuarantineBuffer( hFramework_i, hMsCtx_i, pvEventHandle_i, pvMsEvent_i );
				break;

			case GET_QUARANTINE:
				MS_SM_GetQuarantineEvent( hFramework_i, hMsCtx_i, pvEventHandle_i, pvMsEvent_i );
				break;

			default:
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Event %d not handled\n", poMsEvent->eType );
		}
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return;
}

void MS_SM_HandleMscmlResponse( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsCtx* poMsCtx =  (t_MsCtx*)hMsCtx_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hFramework_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvEventHandle_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvMsEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_String oMscmlContentType = {(unsigned char*)"mediaservercontrol+xml", (unsigned int) sizeof ("mediaservercontrol+xml")-1};
		t_String oContentBody = TSTRING_INITIALIZER;

		t_MsEvent* poMsEvent = (t_MsEvent*)pvMsEvent_i;
		t_MsUserCtrlCtx* poMsUserCtrlCtx = poMsEvent->pvAppCtx;

		ms_assert_return( NULL != poMsUserCtrlCtx, eRetCode = MS_ERR_INTERNAL );
		ms_assert_return( poMsUserCtrlCtx->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );

		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poMsUserCtrlCtx );

		// Encode MSCML
		t_MSCMLResponseParams* poParams = &poMsEvent->uEventParams.oMSCMLResponseParams;

		ms_assert_return( NULL  != poParams->xdpMSCMLResponse, eRetCode = MS_ERR_INVALID_INPUT );

		char* pcRequest = NULL;
		int nCode = 0;
		if( MS_ERR_NONE == GetResponseAttrAsString( poParams->xdpMSCMLResponse, "request", &pcRequest ))
		{
			if( MS_ERR_NONE == GetResponseAttrAsInt( poParams->xdpMSCMLResponse, "code", &nCode ))
			{
				if( 0 == strcasecmp( pcRequest, "play" ))
				{
					if( nCode == SIP_RES_OK )
					{
						SNMP_IncrementPerformanceCtr( poMsCtx, PC_PlaySuccess );
					}
					else
					{
						SNMP_IncrementPerformanceCtr( poMsCtx, PC_PlayFailed );
					}
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_PlayAnswered );
				}
				else if( 0 == strcasecmp( pcRequest, "playcollect" ))
				{
					if( nCode == SIP_RES_OK )
					{
						SNMP_IncrementPerformanceCtr( poMsCtx, PC_PlayCollectSuccess );
					}
					else
					{
						SNMP_IncrementPerformanceCtr( poMsCtx, PC_PlayCollectFailed );
					}
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_PlayCollectAnswered );
				}
				else if( 0 == strcasecmp( pcRequest, "playrecord" ))
				{
					if( nCode == SIP_RES_OK )
					{
						SNMP_IncrementPerformanceCtr( poMsCtx, PC_PlayRecordSuccess );
					}
					else
					{
						SNMP_IncrementPerformanceCtr( poMsCtx, PC_PlayRecordFailed );
					}
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_PlayRecordAnswered );
				}
				else if( 0 == strcasecmp( pcRequest, "stop" ))
				{
					if( nCode == SIP_RES_OK )
					{
						SNMP_IncrementPerformanceCtr( poMsCtx, PC_StopSuccess );
					}
					else
					{
						SNMP_IncrementPerformanceCtr( poMsCtx, PC_StopFailed );
					}
					SNMP_IncrementPerformanceCtr( poMsCtx, PC_StopAnswered );
				}
			}
			xmlFree( pcRequest );
		}
		xmlDocDumpMemory( poParams->xdpMSCMLResponse, &oContentBody.pucData, (int*)&oContentBody.unLength );

		TS_Print( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "MSCML Response", &oContentBody );

		// Send INFO with MSCML message
		eRetCode = MS_SendInfoRequest( hMsCtx_i, poMsUserCtrlCtx, &oMscmlContentType, &oContentBody );

		xmlFree( oContentBody.pucData );
		// Free XML Doc
		xmlFreeDoc( poParams->xdpMSCMLResponse );

		eRetCode = MOP_DestroySM( poMsUserCtrlCtx->pvMscmlOpCtx );
		poMsUserCtrlCtx->pvMscmlOpCtx = NULL;
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return;
}

void MS_SM_HandleSmStopped( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsCtx* poMsCtx =  (t_MsCtx*)hMsCtx_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hFramework_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvEventHandle_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvMsEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MsEvent* poMsEvent = (t_MsEvent*)pvMsEvent_i;
		t_MsUserCtrlCtx* poMsUserCtrlCtx = poMsEvent->pvAppCtx;

		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poMsUserCtrlCtx );
		if( poMsUserCtrlCtx->pvMscmlOpCtx != NULL )
		{
			MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Cleaning old Operation Ctx = %p\n", poMsUserCtrlCtx->pvMscmlOpCtx );

			eRetCode = MOP_DestroySM( poMsUserCtrlCtx->pvMscmlOpCtx );
			poMsUserCtrlCtx->pvMscmlOpCtx = NULL;
			// Check if there is any pending MSCML request
			if( NULL != poMsUserCtrlCtx->poPendingMscmlReq )
			{
				eRetCode = MOP_CreateSM( poMsUserCtrlCtx->ePendingOpCtxType, poMsCtx->nTraceId, poMsCtx->pvFrameworkCtx, poMsCtx->pchSoundDir,  &poMsUserCtrlCtx->pvMscmlOpCtx );
				if( MS_ERR_NONE == eRetCode )
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "MSCML Operation context created successfully\n" );

					// Send MSCML_REQUEST
					eRetCode = MS_SM_SendMscmlRequest( poMsCtx, poMsUserCtrlCtx, poMsUserCtrlCtx->poPendingMscmlReq );
				}
				else
				{
					MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "MSCML Operation context could not be created\n" );
				}
				poMsUserCtrlCtx->poPendingMscmlReq = NULL;
			}
		}
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return;
}

void MS_SM_HandleStartPlay( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsCtx* poMsCtx =  (t_MsCtx*)hMsCtx_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hFramework_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvEventHandle_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvMsEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MsEvent* poMsEvent = (t_MsEvent*)pvMsEvent_i;
		t_MsUserCtrlCtx* poMsUserCtrlCtx = poMsEvent->pvAppCtx;
		int nErrorCode = SIP_RES_OK;
		t_StartPlayParams* poParams = &poMsEvent->uEventParams.oStartPlayParams;

		ms_assert_return( poMsUserCtrlCtx != NULL && poMsUserCtrlCtx->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );

		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poMsUserCtrlCtx );
		if( NULL  != poMsUserCtrlCtx )
		{
			h_MediaComp hFileRead = NULL;

			MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Playing file %s from offset = %d\n", poParams->cpFileName, poParams->nPlayOffset );

			// Get file-source component of TX chain
			eRetCode = Media_ChainGetCompOfType( poMsUserCtrlCtx->hTxChain, MCT_WAVREAD, &hFileRead );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainSetStatus( poMsUserCtrlCtx->hTxChain, MCS_STOP );

				// Set filename and offset of file-source
				eRetCode = Media_CompSetParam( hFileRead, MCC_WAVREAD_FILENAME, (void*)poParams->cpFileName );
				if( MS_ERR_NONE == eRetCode )
				{
					eRetCode = Media_CompSetParam( hFileRead, MCC_WAVREAD_FILEOFFSET, (void*)poParams->nPlayOffset );

					ms_assert( MS_ERR_NONE == eRetCode );

					// Set TX chain mode to start
					eRetCode = Media_ChainSetStatus( poMsUserCtrlCtx->hTxChain, MCS_START );
				}
				else
				{
					nErrorCode = SIP_RES_NOT_FOUND;
				}
			}
			else
			{
				nErrorCode = SIP_RES_SERVER_INTERNAL_ERROR;
			}
		}
		else
		{
			eRetCode = MS_ERR_INTERNAL;
			nErrorCode = SIP_RES_SERVER_INTERNAL_ERROR;
		}
		if( MS_ERR_NONE != eRetCode )
		{
			// Send START_PLAY_ERROR
			t_MsEvent oEvent;
			int nRetVal = 0;

			oEvent.eType = START_PLAY_ERROR;
			oEvent.pvCtx = poMsEvent->pvCtx;
			oEvent.pvAppCtx = poMsUserCtrlCtx;

			oEvent.uEventParams.oPlayErrorParams.nErrorCode = nErrorCode;

			nRetVal = FW_EvtSysSendIntEvt( poMsCtx->pvFrameworkCtx, MSCML_OP_EVENT, &oEvent, sizeof (t_MsEvent));
			if( AMPS_SUCCESS != nRetVal )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt(%d) failed with ret-code = %d.\n", nErrorCode, nRetVal );
			}
		}
		FW_Free( poParams->cpFileName );
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return;
}

void MS_SM_HandleStopPlay( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsCtx* poMsCtx =  (t_MsCtx*)hMsCtx_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hFramework_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvEventHandle_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvMsEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MsEvent* poMsEvent = (t_MsEvent*)pvMsEvent_i;
		t_MsUserCtrlCtx* poMsUserCtrlCtx = poMsEvent->pvAppCtx;

		ms_assert_return( poMsUserCtrlCtx != NULL && poMsUserCtrlCtx->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );

		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poMsUserCtrlCtx );

		// Set TX chain mode to stop
		eRetCode = Media_ChainSetStatus( poMsUserCtrlCtx->hTxChain, MCS_STOP );
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return;
}

void MS_SM_HandleStartRecording( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsCtx* poMsCtx =  (t_MsCtx*)hMsCtx_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hFramework_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvEventHandle_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvMsEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MsEvent* poMsEvent = (t_MsEvent*)pvMsEvent_i;
		t_MsUserCtrlCtx* poMsUserCtrlCtx = poMsEvent->pvAppCtx;

		h_MediaComp hFileWrite = NULL;
		int nErrorCode = SIP_RES_OK;

		ms_assert_return( poMsUserCtrlCtx != NULL && poMsUserCtrlCtx->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );

		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poMsUserCtrlCtx );

		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Recording file name = %s\n", poMsEvent->uEventParams.oStartRecParams.cpRecURL );
		// Get file-write component from RX Chain

		eRetCode = Media_ChainGetCompOfType( poMsUserCtrlCtx->hRxChain, MCT_WAVWRITE, &hFileWrite );
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = Media_CompSetParam( hFileWrite, MCC_WAVWRITE_FILENAME, (void*)poMsEvent->uEventParams.oStartRecParams.cpRecURL );
		}
		else
		{
			eRetCode = MS_ERR_INTERNAL;
			nErrorCode = SIP_RES_SERVER_INTERNAL_ERROR;
		}
		if( MS_ERR_NONE != eRetCode )
		{
			// Send START_RECORDING_ERROR
			t_MsEvent oEvent;
			int nRetVal = 0;

			oEvent.eType = START_RECORDING_ERROR;
			oEvent.pvCtx = poMsEvent->pvCtx;
			oEvent.pvAppCtx = poMsUserCtrlCtx;

			oEvent.uEventParams.oStartRecErrorParams.nErrorCode = nErrorCode;

			nRetVal = FW_EvtSysSendIntEvt( poMsCtx->pvFrameworkCtx, MSCML_OP_EVENT, &oEvent, sizeof (t_MsEvent));
			if( AMPS_SUCCESS != nRetVal )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed with ret-code = %d.\n", nRetVal );
			}
		}
		FW_Free( poMsEvent->uEventParams.oStartRecParams.cpRecURL );
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return;
}

void MS_SM_HandleStopRecording( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsCtx* poMsCtx =  (t_MsCtx*)hMsCtx_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hFramework_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvEventHandle_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvMsEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		h_MediaComp hFileWrite = NULL;
		t_MsEvent* poMsEvent = (t_MsEvent*)pvMsEvent_i;
		t_MsUserCtrlCtx* poMsUserCtrlCtx = poMsEvent->pvAppCtx;
		int nByteWritten = 0;

		ms_assert_return( poMsUserCtrlCtx->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );

		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poMsUserCtrlCtx );

		// Stop RX chain
		//eRetCode = Media_ChainSetStatus( poMsUserCtrlCtx->hRxChain, MCS_STOP );

		// Get File-writing component
		eRetCode = Media_ChainGetCompOfType( poMsUserCtrlCtx->hRxChain, MCT_WAVWRITE, &hFileWrite );
		if( MS_ERR_NONE != eRetCode )
		{
			// Get bytes written for
			eRetCode = Media_CompGetParam( hFileWrite, MCC_WAVWRITE_BYTES_WRITTEN, (void*)&nByteWritten );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			// Send RECORDING_STOPPED
			t_MsEvent oEvent;
			int nRetVal = 0;

			oEvent.eType = RECORDING_STOPPED;
			oEvent.pvCtx = poMsEvent->pvCtx;
			oEvent.pvAppCtx = poMsUserCtrlCtx;

			oEvent.uEventParams.oRecStoppedParams.nReclength = nByteWritten;

			nRetVal = FW_EvtSysSendIntEvt( poMsCtx->pvFrameworkCtx, MSCML_OP_EVENT, &oEvent, sizeof (t_MsEvent));
			if( AMPS_SUCCESS != nRetVal )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt(RECORDING_STOPPED) failed with ret-code = %d.\n", nRetVal );
			}
		}
		else
		{
			// Send STOP_RECORDING_ERROR
			t_MsEvent oEvent;
			int nRetVal = 0;

			oEvent.eType = STOP_RECORDING_ERROR;
			oEvent.pvCtx = poMsEvent->pvCtx;
			oEvent.pvAppCtx = poMsUserCtrlCtx;

			nRetVal = FW_EvtSysSendIntEvt( poMsCtx->pvFrameworkCtx, MSCML_OP_EVENT, &oEvent, sizeof (t_MsEvent));
			if( AMPS_SUCCESS != nRetVal )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt(STOP_RECORDING_ERROR) failed with ret-code = %d.\n", nRetVal );
			}
		}
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return;
}

void MS_SM_ClearQuarantineBuffer( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsCtx* poMsCtx =  (t_MsCtx*)hMsCtx_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hFramework_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvEventHandle_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvMsEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MsEvent* poMsEvent = (t_MsEvent*)pvMsEvent_i;
		t_MsUserCtrlCtx* poMsUserCtrlCtx = poMsEvent->pvAppCtx;

		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poMsUserCtrlCtx );

		FW_DListFreeNodes( &poMsUserCtrlCtx->poQuarantineBuffer, Signal_LListRemoveLinkDataCallback );
	}

	POST_CONDITION
	{
		t_MsEvent* poMsEvent = (t_MsEvent*)pvMsEvent_i;
		t_MsUserCtrlCtx* poMsUserCtrlCtx = poMsEvent->pvAppCtx;
		ms_assert( 0 == poMsUserCtrlCtx->poQuarantineBuffer->uchCount );
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return;
}

void MS_SM_GetQuarantineEvent( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsCtx* poMsCtx =  (t_MsCtx*)hMsCtx_i;

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hFramework_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvEventHandle_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvMsEvent_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MsEvent* poMsEvent = (t_MsEvent*)pvMsEvent_i;
		t_MsUserCtrlCtx* poMsUserCtrlCtx = poMsEvent->pvAppCtx;

		ms_assert_return( poMsUserCtrlCtx->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );

		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "User Ctrl Ctx = %p\n", poMsUserCtrlCtx );

		t_AMPSSList* poListNode = NULL;
		t_MsEvent oEvent;
		int nRetVal = 0;

		oEvent.pvCtx = poMsUserCtrlCtx->pvMscmlOpCtx;
		oEvent.pvAppCtx = poMsUserCtrlCtx;
		oEvent.eType = DTMF_KEY_PRESSED;
		if( poMsUserCtrlCtx->poQuarantineBuffer->uchCount > 0 )
		{
			poListNode = poMsUserCtrlCtx->poQuarantineBuffer->poAMPSSListHead;

			ms_assert( NULL != poListNode );

			t_SigPkt* poSigPkt = poListNode->pvData;

			ms_assert( NULL != poSigPkt );

			oEvent.uEventParams.oDtmfKeyPressedParams.cDtmfKeyVal = TS_NumToAsciiDigit( poSigPkt->oDtmfPkt.ucDtmfKey );
			oEvent.uEventParams.oDtmfKeyPressedParams.bQuarantined = AMPS_TRUE;

			nRetVal = FW_EvtSysSendIntEvt( poMsCtx->pvFrameworkCtx, MSCML_OP_EVENT, &oEvent, sizeof (t_MsEvent));
			if( AMPS_SUCCESS != nRetVal )
			{
				MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			FW_DListRemoveFirstNode( &poMsUserCtrlCtx->poQuarantineBuffer, Signal_LListRemoveLinkDataCallback );
		}
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return;
}

