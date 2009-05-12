/*! @file snmp_comm.c
*
* @ingroup SnmpComm
*
* @brief The file contains implementations for communication with snmp agent.
*
* @author Razi-ud-Din Chaudhry
*
*/

#include "snmp_comm_priv.h"

/*! @fn e_MsRetCode _SNMP_Init( t_SnmpCommConfig* poSnmpCommConfig_i, h_SnmpComm* phSnmpComm_o )
*
* @brief This function initializes the server for snmp communication.
*
* @param[in] poSnmpCommConfig_i is the pointer variable containing the configuration parameters required for initializing snmp communication.
*
* @param[out] phSnmpComm_o is the pointer to the handle required for interaction with the snmp agent.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode _SNMP_Init( t_SnmpCommConfig* poSnmpCommConfig_i, h_SnmpComm* phSnmpComm_o )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;

	MS_TRACE( poSnmpCommConfig_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poSnmpCommConfig_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phSnmpComm_o, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_SnmpRecv* poSnmpRecv = NULL;

		// Allocate t_SnmpRecv structure
		poSnmpRecv = (t_SnmpRecv*)FW_Malloc( sizeof (t_SnmpRecv));
		if( NULL != poSnmpRecv )
		{
			// Initialize t_SnmpRecv structure
			poSnmpRecv->oSnmpCommConfig.nTraceId = poSnmpCommConfig_i->nTraceId;
			strncpy( poSnmpRecv->oSnmpCommConfig.pcLocalIp, poSnmpCommConfig_i->pcLocalIp, sizeof (poSnmpRecv->oSnmpCommConfig.pcLocalIp)-1 );
			poSnmpRecv->oSnmpCommConfig.nLocalPort = poSnmpCommConfig_i->nLocalPort;
			strncpy( poSnmpRecv->oSnmpCommConfig.pcRemoteIp, poSnmpCommConfig_i->pcRemoteIp, sizeof (poSnmpRecv->oSnmpCommConfig.pcRemoteIp)-1 );
			poSnmpRecv->oSnmpCommConfig.nRemotePort = poSnmpCommConfig_i->nRemotePort;
			poSnmpRecv->oSnmpCommConfig.poMsCtx = poSnmpCommConfig_i->poMsCtx;
			poSnmpRecv->oSnmpCommConfig.pvFrameworkCtx = poSnmpCommConfig_i->pvFrameworkCtx;
			poSnmpRecv->nMagicNum = SNMP_MAGIC_NUM;

			int nRetVal = AMPS_SUCCESS;
			// Open UDP Port
//          nRetVal = FW_CreateUDPObject( poSnmpRecv->oSnmpCommConfig.pvFrameworkCtx, AMPS_MEMORY_SCHEME_DYNAMIC, SNMP_MallocCallback, SNMP_FreeCallback, poSnmpRecv->oSnmpCommConfig.nLocalPort, poSnmpRecv->oSnmpCommConfig.pcLocalIp, SNMP_UdpRecvDataCallback, &poSnmpRecv->hUdpNetHandle, poSnmpRecv, NULL );
			nRetVal = FW_CreateUDPObject( poSnmpRecv->oSnmpCommConfig.pvFrameworkCtx, AMPS_MEMORY_SCHEME_STATIC, NULL, NULL, poSnmpRecv->oSnmpCommConfig.nLocalPort, poSnmpRecv->oSnmpCommConfig.pcLocalIp, SNMP_UdpRecvDataCallback, &poSnmpRecv->hUdpNetHandle, poSnmpRecv, NULL );
			if( nRetVal != AMPS_SUCCESS )
			{
				MS_TRACE( poSnmpCommConfig_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Error while creating udp object\n" );
				eRet_Code = MS_ERR_FRAMEWORK;
				FW_Free( poSnmpRecv );
			}
			else
			{
				MS_TRACE( poSnmpCommConfig_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Udp object created successfully\n" );
				*phSnmpComm_o = (h_SnmpComm) poSnmpRecv;
			}
		}
		else
		{
			MS_TRACE( poSnmpCommConfig_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Error while Allocating Memory\n" );
			eRet_Code = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poSnmpCommConfig_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/*! @fn e_MsRetCode _SNMP_SendNotification( h_SnmpComm hSnmpComm_i, t_AMPSDList* poSnmpTrapMsg_i )
*
* @brief This function sends notifications to the snmp agent.
*
* @param[in] hSnmpComm_i is the handle required for interaction with the snmp agent.
*
* @param[in] poSnmpTrapMsg_i is the pointer to the list containing the parameters required for sending notification message to snmp.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode _SNMP_SendNotification( h_SnmpComm hSnmpComm_i, t_AMPSDList* poSnmpTrapMsg_i )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;
	t_SnmpRecv* poSnmpRecv = (t_SnmpRecv*) hSnmpComm_i;

	MS_TRACE( poSnmpRecv->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hSnmpComm_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poSnmpTrapMsg_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		xmlDocPtr pxmlDocPtr = NULL;
		pxmlDocPtr = SP_GetNotification();
		if( NULL == pxmlDocPtr )
		{
			eRet_Code = MS_ERR_INSUFFICIENT_MEMORY;
		}
		else
		{
			char* pcEncodedNotification = NULL;
			int nSize = 0;
			int nTrapIndx = 1;
			t_SnmpTrapMsg* poSnmpTrapMsg = NULL;
			t_AMPSSList* poTrapNode = poSnmpTrapMsg_i->poAMPSSListHead;

			while( NULL != poTrapNode )
			{
				poSnmpTrapMsg = (t_SnmpTrapMsg*)(poTrapNode->pvData);

				// Set oid
				eRet_Code = SP_SetAttrAsString( pxmlDocPtr, nTrapIndx, "OID", poSnmpTrapMsg->pcOid );
				if( MS_ERR_NONE == eRet_Code )
				{
					// Set value
					eRet_Code = SP_SetAttrAsString( pxmlDocPtr, nTrapIndx, "VALUE", poSnmpTrapMsg->pcValue );
				}
				else
				{
					break;
				}
				if( MS_ERR_NONE == eRet_Code )
				{
					// Set severity
					eRet_Code = SP_SetAttrAsString( pxmlDocPtr, nTrapIndx, "SEVERITY", poSnmpTrapMsg->pcSeverity );
				}
				else
				{
					break;
				}
				if( MS_ERR_NONE == eRet_Code )
				{
					// Set error code
					eRet_Code = SP_SetAttrAsUL( pxmlDocPtr, nTrapIndx, "ERRORCODE", ( unsigned long ) poSnmpTrapMsg->nErrorCode );
				}
				else
				{
					break;
				}
				if( MS_ERR_NONE == eRet_Code )
				{
					// Set error description
					eRet_Code = SP_SetAttrAsString( pxmlDocPtr, nTrapIndx, "ERRORDESC", poSnmpTrapMsg->pcErrorDesc );
				}
				else
				{
					break;
				}
				nTrapIndx++;
				poTrapNode = poTrapNode->poAMPSSListNext;
			}
			if( eRet_Code == MS_ERR_NONE )
			{
				// Encode message
				pcEncodedNotification = SP_EncodeMessage( pxmlDocPtr, &nSize );
				if( 0 < nSize )
				{
					MS_TRACE( poSnmpRecv->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Notification to snmp:\n%s\n", pcEncodedNotification );

					// Send notification to snmp
					int nRet_Code = FW_SndBuffer( poSnmpRecv->oSnmpCommConfig.pvFrameworkCtx, poSnmpRecv->hUdpNetHandle, pcEncodedNotification, nSize, poSnmpRecv->oSnmpCommConfig.nRemotePort, poSnmpRecv->oSnmpCommConfig.pcRemoteIp );
					if( AMPS_SUCCESS == nRet_Code )
					{
						eRet_Code = MS_ERR_NONE;
					}
					else
					{
						eRet_Code = MS_ERR_FRAMEWORK;
					}
				}
				else
				{
					eRet_Code = MS_ERR_ENCODE;
				}
				xmlFree( pcEncodedNotification );
			}
			xmlFreeDoc( pxmlDocPtr );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poSnmpRecv->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/*! @fn e_MsRetCode _SNMP_Destroy( h_SnmpComm hSnmpComm_i )
*
* @brief This function stops and close the snmp conmmunication.
*
* @param[in] hSnmpComm_i is the handle required for interaction with the snmp agent.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode _SNMP_Destroy( h_SnmpComm hSnmpComm_i )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;
	t_SnmpRecv* poSnmpRecv = (t_SnmpRecv*) hSnmpComm_i;
	int nTraceId = poSnmpRecv->oSnmpCommConfig.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hSnmpComm_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poSnmpRecv, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poSnmpRecv->nMagicNum == SNMP_MAGIC_NUM, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		// Close udp object
		if( AMPS_SUCCESS == FW_DestroyUDPObject( poSnmpRecv->oSnmpCommConfig.pvFrameworkCtx, poSnmpRecv->hUdpNetHandle ))
		{
			MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Udp object destroyed successfully\n" );
			poSnmpRecv->hUdpNetHandle = NULL;
		}
		else
		{
			MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_ERROR, "Error while destroying udp object\n" );
		}
		// Free t_SnmpRecv structure
		FW_Free( poSnmpRecv );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/*! @fn e_MsRetCode _SNMP_CheckFault( h_SnmpComm hSnmpComm_i, e_FaultCtrs eFaultCtr_i, unsigned long nCurrentValue_i, unsigned long nMaxValue_i )
*
* @brief This function compares the value of the given fault counter with the given boundary value and sends a notification trap message to snmp if both values are equal.
*
* @param[in] hSnmpComm_i is the handle required for interaction with the snmp agent.
*
* @param[in] eFaultCtr_i is the name of the fault counter to be checked.
*
* @param[in] nCurrentValue_i is the current value of the counter to be checked.
*
* @param[in] nMaxValue_i is the boundary value for the fault counter.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode _SNMP_CheckFault( h_SnmpComm hSnmpComm_i, e_FaultCtrs eFaultCtr_i, unsigned long nCurrentValue_i, unsigned long nMaxValue_i )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;
	t_SnmpRecv* poSnmpRecv = (t_SnmpRecv*) hSnmpComm_i;
	int nTraceId = poSnmpRecv->oSnmpCommConfig.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != hSnmpComm_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poSnmpRecv, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poSnmpRecv->nMagicNum == SNMP_MAGIC_NUM, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( nCurrentValue_i == nMaxValue_i ) // check whether given current value is equal to the given boundary value
		{
			t_SnmpTrapMsg oSnmpTrapMsg;
			t_AMPSDList* poSnmpTrapMsg;

			// Initialize dlist
			FW_DListInit( &poSnmpTrapMsg );
			// Initialize value for trap message
			if( FC_InvitesReceived == eFaultCtr_i )
			{
				strncpy( oSnmpTrapMsg.pcOid, "invitesReceived", SNMP_MAX_OID_STRING_LENGTH - 1 );
			}
			else if( FC_InfoReceived == eFaultCtr_i )
			{
				strncpy( oSnmpTrapMsg.pcOid, "infoReceived", SNMP_MAX_OID_STRING_LENGTH - 1 );
			}
			TS_IntToString( oSnmpTrapMsg.pcValue, (int) nCurrentValue_i, 10 );
			strncpy( oSnmpTrapMsg.pcSeverity, "low", SNMP_MAX_SEVERITY_STRING_LENGTH - 1 );
			oSnmpTrapMsg.nErrorCode = SIP_RES_OK;
			strncpy( oSnmpTrapMsg.pcErrorDesc, "OK", SNMP_MAX_ERRORDESC_STRING_LENGTH - 1 );

			// add the trap message to the dlist
			FW_DListAppend( poSnmpTrapMsg, &oSnmpTrapMsg );

			// Send trap message to snmp
			eRet_Code = SNMP_SendNotification( hSnmpComm_i, poSnmpTrapMsg );
			if( MS_ERR_NONE == eRet_Code )
			{
				MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Notification sent to snmp successfully\n" );
			}
			else
			{
				MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_ERROR, "Error while sending notification to snmp\n" );
			}
		}
		else    // Current value  is not equal to the boundary value
		{
			MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "No need to sent notification to snmp\n" );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

