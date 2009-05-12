#include "udprecv_priv.h"

e_MsRetCode UdpRecvInit( h_CompConfig hCompConfig_i, h_MediaComp* phUdpRecv_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpRecvConfig* poUdpRecvConfig = (t_UdpRecvConfig*)hCompConfig_i;

	MS_TRACE( poUdpRecvConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpRecvConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUdpRecvConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phUdpRecv_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_UdpRecv* poUdpRecv = NULL;

		// Allocate UdpRecv structure
		poUdpRecv = FW_Malloc( sizeof (t_UdpRecv));
		if( NULL != poUdpRecv )
		{
			poUdpRecv->oComp.eType = MCT_UDPRECV;
			poUdpRecv->oComp.eGroup = MCG_SOURCE;
			// Populate UdpRecv structure
			poUdpRecv->oComp.nTraceId = poUdpRecvConfig->oConfig.nTraceId;
			poUdpRecv->oComp.pvFrameworkCtx = poUdpRecvConfig->oConfig.pvFrameworkCtx;
			strncpy( poUdpRecv->pcLocalIp, poUdpRecvConfig->pcLocalIp, sizeof (poUdpRecv->pcLocalIp)-1 );
			poUdpRecv->nLocalPort = poUdpRecvConfig->nLocalPort;

			struct in_addr oRemoteAddr;
			inet_ntoa( poUdpRecvConfig->pcRemoteIp, &poUdpRecv->oLastRemoteAddr );
			poUdpRecv->nLastRemotePort = poUdpRecvConfig->nRemotePort;

			memset( &poUdpRecv->oAppData, 0, sizeof (t_MediaCompAppData));

			poUdpRecv->nMagicNum = MCT_UDPRECV_MAGIC_NUM;
			poUdpRecv->hUdpNetHandle = poUdpRecvConfig->hUdpNetHandle;
			poUdpRecv->bManipulateHandle = poUdpRecvConfig->bManipulateHandle;
			if( 0 != strcmp( poUdpRecv->pcLocalIp, "0.0.0.0" ) )
			{
				UdpRecvSetStatus( poUdpRecv, MCS_START );
			}
			*phUdpRecv_o = (h_MediaComp) poUdpRecv;
		}
		else
		{
			MS_TRACE( poUdpRecvConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_UdpRecv\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( *phUdpRecv_o, poUdpRecvConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode UdpRecvSetAppData( h_MediaComp hUdpRecv_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpRecv* poUdpRecv = hUdpRecv_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPRECV_MAGIC_NUM == poUdpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poUdpRecv->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode UdpRecvDestroy( h_MediaComp hUdpRecv_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpRecv* poUdpRecv = hUdpRecv_i;
	int nTraceId = poUdpRecv->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPRECV_MAGIC_NUM == poUdpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		// Should this component close the net-handle ?
		if( AMPS_TRUE == poUdpRecv->bManipulateHandle )
		{
			// Is net-handle already closed
			if( NULL != poUdpRecv->hUdpNetHandle )
			{
				MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling FW_DestroyUDPObject\n" );
				FW_DestroyUDPObject( poUdpRecv->oComp.pvFrameworkCtx, poUdpRecv->hUdpNetHandle );

				poUdpRecv->hUdpNetHandle = NULL;
			}
			else
			{
				MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Net-handle already closed\n" );
			}
		}
		else
		{
			MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Net-handle is not to be closed by this component\n" );
		}
		memset( poUdpRecv, 0, sizeof (t_UdpRecv));
		FW_Free( poUdpRecv );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode UdpRecvSetStatus( h_MediaComp hUdpRecv_i, e_MediaCompStatus eStatus_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpRecv* poUdpRecv = hUdpRecv_i;

	MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPRECV_MAGIC_NUM == poUdpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eStatus_i )
		{
			case MCS_START:
			{
				MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "MCS_START\n" );
				// Should this component open its own net-handle ?
				if( AMPS_TRUE == poUdpRecv->bManipulateHandle )
				{
					// Is net-handle already open ?
					if( NULL == poUdpRecv->hUdpNetHandle )
					{
						int nRetVal = AMPS_SUCCESS;

						MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Opening net handle on local-ip:local-port = %s:%d.\n", poUdpRecv->pcLocalIp, poUdpRecv->nLocalPort );

						// Open UDP Port
						nRetVal = FW_CreateUDPObject( poUdpRecv->oComp.pvFrameworkCtx, AMPS_MEMORY_SCHEME_DYNAMIC, MediaMallocCallback, MediaFreeCallback, poUdpRecv->nLocalPort, poUdpRecv->pcLocalIp, UdpRecvDataCallback, &poUdpRecv->hUdpNetHandle, poUdpRecv, NULL );
						if( AMPS_SUCCESS != nRetVal )
						{
							MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_CreateUDPObject failed = %d.\n", nRetVal );
							eRetCode = MS_ERR_FRAMEWORK;
						}
						else
						{
							int nLocalPort = 0;
							char pchLocalIp[AMPS_MAX_IP_STRING_LENGTH] = "";

							FW_GetLocalIPAddressFromNetHandle( poUdpRecv->oComp.pvFrameworkCtx, poUdpRecv->hUdpNetHandle, pchLocalIp );

							FW_GetLocalPortFromNetHandle( poUdpRecv->oComp.pvFrameworkCtx, poUdpRecv->hUdpNetHandle, &nLocalPort );

							MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "UDP object created on local-ip:local-port = %s:%d.\n", pchLocalIp, nLocalPort );
						}
					}
					else
					{
						MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Net-handle already opened\n" );
					}
				}
				else
				{
					MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Net-handle is not to be opened by this component\n" );
				}
				break;
			}

			case MCS_STOP:
			{
				MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "MCS_STOP\n" );
				// Should this component close the net-handle ?
				if( AMPS_TRUE == poUdpRecv->bManipulateHandle )
				{
					// Is net-handle already closed
					if( NULL != poUdpRecv->hUdpNetHandle )
					{
						MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling FW_DestroyUDPObject\n" );
						FW_DestroyUDPObject( poUdpRecv->oComp.pvFrameworkCtx, poUdpRecv->hUdpNetHandle );

						poUdpRecv->hUdpNetHandle = NULL;
					}
					else
					{
						MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Net-handle already closed\n" );
					}
				}
				else
				{
					MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Net-handle is not to be closed by this component\n" );
				}
				break;
			}

			default:
			{
				MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus_i );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode UdpRecvGetParam( h_MediaComp hUdpRecv_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpRecv* poUdpRecv = hUdpRecv_i;

	MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPRECV_MAGIC_NUM == poUdpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_UDPRECV_LOCAL_PORT:
			{
				int* pnLocalPort = (int*)pvConfigData_i;
				if( NULL != poUdpRecv->hUdpNetHandle )
				{
					FW_GetLocalPortFromNetHandle( poUdpRecv->oComp.pvFrameworkCtx, poUdpRecv->hUdpNetHandle, pnLocalPort );
				}
				else
				{
					eRetCode = MS_ERR_INVALID_HANDLE;
				}
				break;
			}

			case MCC_UDPRECV_NET_HANDLE:
			{
				AMPS_NET_HANDLE* poUdpNetHandle = (AMPS_NET_HANDLE*)pvConfigData_i;
				*poUdpNetHandle = poUdpRecv->hUdpNetHandle;
				break;
			}

			default:
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode UdpRecvSetParam( h_MediaComp hUdpRecv_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpRecv* poUdpRecv = hUdpRecv_i;

	MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering type = %d, data = %p\n", eConfig_i, pvConfigData_i );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPRECV_MAGIC_NUM == poUdpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_UDPRECV_REMOTE_IP:
			{
				strcpy((char*)pvConfigData_i, inet_ntoa( &poUdpRecv->oLastRemoteAddr ));
				break;
			}

			case MCC_UDPRECV_REMOTE_PORT:
			{
				poUdpRecv->nLastRemotePort = (int)pvConfigData_i;
				break;
			}

			case MCC_UDPRECV_LOCAL_PORT:
			{
				poUdpRecv->nLocalPort = (int)pvConfigData_i;
				break;
			}

			default:
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

