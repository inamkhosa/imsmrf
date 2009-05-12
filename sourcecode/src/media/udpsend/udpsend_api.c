#include "udpsend_priv.h"

e_MsRetCode UdpSendInit( h_CompConfig hCompConfig_i, h_MediaComp* phUdpSend_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpSendConfig* poUdpSendConfig_i = (t_UdpSendConfig*)hCompConfig_i;

	MS_TRACE( poUdpSendConfig_i->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpSendConfig_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUdpSendConfig_i->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phUdpSend_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_UdpSend* poUdpSend = NULL;

		// Allocate UdpSend structure
		poUdpSend = FW_Malloc( sizeof (t_UdpSend));
		if( NULL != poUdpSend )
		{
			int nRetVal = AMPS_SUCCESS;

			poUdpSend->oComp.eType = MCT_UDPSEND;
			poUdpSend->oComp.eGroup = MCG_SINK;
			poUdpSend->nMagicNum = MCT_UDPSEND_MAGIC_NUM;

			poUdpSend->oComp.nTraceId = poUdpSendConfig_i->oConfig.nTraceId;
			poUdpSend->oComp.pvFrameworkCtx = poUdpSendConfig_i->oConfig.pvFrameworkCtx;
			strncpy( poUdpSend->pcLocalIp, poUdpSendConfig_i->pcLocalIp, sizeof (poUdpSend->pcLocalIp)-1 );
			poUdpSend->nLocalPort = poUdpSendConfig_i->nLocalPort;
			strncpy( poUdpSend->pcRemoteIp, poUdpSendConfig_i->pcRemoteIp, sizeof (poUdpSend->pcRemoteIp)-1 );
			poUdpSend->nRemotePort = poUdpSendConfig_i->nRemotePort;
			memset( &poUdpSend->oAppData, 0, sizeof (t_MediaCompAppData));

			poUdpSend->hUdpNetHandle = poUdpSendConfig_i->hUdpNetHandle;
			poUdpSend->bManipulateHandle = poUdpSendConfig_i->bManipulateHandle;
			if( 0 != strcmp( poUdpSend->pcLocalIp, "0.0.0.0" ) )
			{
				UdpSendSetStatus( poUdpSend, MCS_START );
			}
			*phUdpSend_o = (h_MediaComp) poUdpSend;
		}
		else
		{
			MS_TRACE( poUdpSendConfig_i->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_UdpSend\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phUdpSend_o );
		}
	}

	MS_TRACE( poUdpSendConfig_i->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode UdpSendSetAppData( h_MediaComp hUdpSend_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpSend* poUdpSend = hUdpSend_i;
	t_MediaCompAppData* poAppData = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData, eRetCode = MS_ERR_PRE_COND_FAILED );
		//ms_assert_return( NULL != poAppData->pfAppDataCb	, eRetCode = MS_ERR_PRE_COND_FAILED);
		ms_assert_return( NULL != poAppData->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPSEND_MAGIC_NUM == poUdpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poUdpSend->oAppData = *poAppData;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode UdpSendDestroy( h_MediaComp hUdpSend_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpSend* poUdpSend = hUdpSend_i;
	int nTraceId = poUdpSend->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		// Should this component close the net-handle ?
		if( AMPS_TRUE == poUdpSend->bManipulateHandle )
		{
			// Is net-handle already closed
			if( NULL != poUdpSend->hUdpNetHandle )
			{
				MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling FW_DestroyUDPObject\n" );
				FW_DestroyUDPObject( poUdpSend->oComp.pvFrameworkCtx, poUdpSend->hUdpNetHandle );

				poUdpSend->hUdpNetHandle = NULL;
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
		memset( poUdpSend, 0, sizeof (t_UdpSend));
		FW_Free( poUdpSend );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode UdpSendSetStatus( h_MediaComp hUdpSend_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpSend* poUdpSend = hUdpSend_i;

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPSEND_MAGIC_NUM == poUdpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eStatus )
		{
			case MCS_START:
			{
				// Should this component open its own net-handle ?
				if( AMPS_TRUE == poUdpSend->bManipulateHandle )
				{
					// Is net-handle already open ?
					if( NULL == poUdpSend->hUdpNetHandle )
					{
						int nRetVal = AMPS_SUCCESS;
						// Open UDP Port
						nRetVal = FW_CreateUDPObject( poUdpSend->oComp.pvFrameworkCtx, AMPS_MEMORY_SCHEME_DYNAMIC, MediaMallocCallback, MediaFreeCallback, poUdpSend->nLocalPort, poUdpSend->pcLocalIp, UdpSendDataCallback, &poUdpSend->hUdpNetHandle, poUdpSend, NULL );
						if( AMPS_SUCCESS != nRetVal )
						{
							MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_CreateUDPObject failed = %d.\n", nRetVal );
							eRetCode = MS_ERR_FRAMEWORK;
						}
						else
						{
							int nLocalPort = 0;
							char pchLocalIp[AMPS_MAX_IP_STRING_LENGTH] = "";

							FW_GetLocalIPAddressFromNetHandle( poUdpSend->oComp.pvFrameworkCtx, poUdpSend->hUdpNetHandle, pchLocalIp );

							FW_GetLocalPortFromNetHandle( poUdpSend->oComp.pvFrameworkCtx, poUdpSend->hUdpNetHandle, &nLocalPort );

							MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "UDP object created on local-ip:local-port = %s:%d.\n", pchLocalIp, nLocalPort );
						}
					}
					else
					{
						MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Net-handle already opened\n" );
					}
				}
				else
				{
					MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Net-handle is not to be opened by this component\n" );
				}
				break;
			}

			case MCS_STOP:
			{
				// Should this component close the net-handle ?
				if( AMPS_TRUE == poUdpSend->bManipulateHandle )
				{
					// Is net-handle already closed
					if( NULL != poUdpSend->hUdpNetHandle )
					{
						MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling FW_DestroyUDPObject\n" );
						FW_DestroyUDPObject( poUdpSend->oComp.pvFrameworkCtx, poUdpSend->hUdpNetHandle );

						poUdpSend->hUdpNetHandle = NULL;
					}
					else
					{
						MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Net-handle already closed\n" );
					}
				}
				else
				{
					MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Net-handle is not to be closed by this component\n" );
				}
				break;
			}

			default:
			{
				MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode UdpSendGetParam( h_MediaComp hUdpSend_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpSend* poUdpSend = hUdpSend_i;

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPSEND_MAGIC_NUM == poUdpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
//          case MCC_UDPSEND_LOCAL_PORT:
//          {
//              int* pnLocalPort = (int*)pvConfigData_i;
//
//
//              if( NULL != poUdpSend->hUdpNetHandle )
//              {
//                  FW_GetLocalPortFromNetHandle( poUdpSend->oComp.pvFrameworkCtx, poUdpSend->hUdpNetHandle, pnLocalPort );
//              }
//              else
//              {
//                  eRetCode = MS_ERR_INVALID_HANDLE;
//              }
//
//              break;
//          }

			case MCC_UDPSEND_NET_HANDLE:
			{
				AMPS_NET_HANDLE* poUdpNetHandle = (AMPS_NET_HANDLE*)pvConfigData_i;
				*poUdpNetHandle = poUdpSend->hUdpNetHandle;
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

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode UdpSendSetParam( h_MediaComp hUdpSend_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpSend* poUdpSend = hUdpSend_i;

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPSEND_MAGIC_NUM == poUdpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_UDPSEND_REMOTE_IP:
			{
				char* pcRemoteIp = (char*)pvConfigData_i;

				MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Remote IP = %s\n", pcRemoteIp );

				strncpy( poUdpSend->pcRemoteIp, pcRemoteIp, AMPS_MAX_IP_STRING_LENGTH );
				break;
			}

			case MCC_UDPSEND_REMOTE_PORT:
			{
				poUdpSend->nRemotePort = (int)pvConfigData_i;
				break;
			}

			case MCC_UDPSEND_NET_HANDLE:
			{
				poUdpSend->hUdpNetHandle = (AMPS_NET_HANDLE)pvConfigData_i;
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

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

