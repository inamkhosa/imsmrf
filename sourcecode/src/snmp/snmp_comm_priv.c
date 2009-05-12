/*! @file snmp_comm_priv.c
*
* @ingroup SnmpComm
*
* @brief The file contains implementation of additional functions for communication with snmp agent.
*
* @author Razi-ud-Din Chaudhry
*
*/

#include "snmp_comm_priv.h"

/*! @fn int SNMP_UdpRecvDataCallback( AMPS_HANDLE hAMPS_HANDLE_i, AMPS_APP_HANDLE hAMPS_APP_HANDLE_i, AMPS_NET_HANDLE hAMPS_NET_HANDLE_i, unsigned char* pucBuff_i, int nBuffLength_i, int nPort_i, char* pcIPAddress_i )
*
* @brief This callback function receives requests from snmp and sends the generated response back on the same ip and port from where the message was received.
*
* @param[in] hAMPS_HANDLE_i is the handle of Amps context.
*
* @param[in] hAMPS_APP_HANDLE_i is the handle of application required for interaction with the snmp agent.
*
* @param[in] hAMPS_NET_HANDLE_i is the net handle required by Amps for generating events.
*
* @param[in] pucBuff_i is the buffer containing the message received from snmp.
*
* @param[in] nBuffLength_i is the size of the buffer containing the message.
*
* @param[in] nPort_i is the port of the snmp used for communication.
*
* @param[in] pcIPAddress_i is the ip address of the snmp used for communication.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

int SNMP_UdpRecvDataCallback( AMPS_HANDLE hAMPS_HANDLE_i, AMPS_APP_HANDLE hAMPS_APP_HANDLE_i, AMPS_NET_HANDLE hAMPS_NET_HANDLE_i, unsigned char* pucBuff_i, int nBuffLength_i, int nPort_i, char* pcIPAddress_i )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;
	t_SnmpRecv* poSnmpRecv = (t_SnmpRecv*) hAMPS_APP_HANDLE_i;

	MS_TRACE( poSnmpRecv->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poSnmpRecv, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( SNMP_MAGIC_NUM == poSnmpRecv->nMagicNum, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( hAMPS_HANDLE_i == poSnmpRecv->oSnmpCommConfig.pvFrameworkCtx, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( hAMPS_NET_HANDLE_i == poSnmpRecv->hUdpNetHandle, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( hAMPS_APP_HANDLE_i == poSnmpRecv, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poSnmpRecv->oSnmpCommConfig.poMsCtx, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pucBuff_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( 0 < nBuffLength_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pcIPAddress_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( 0 < nPort_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		MS_TRACE( poSnmpRecv->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Data Received : %s\n", pucBuff_i );

		xmlDocPtr pxmlDocPtr = NULL;
		// Parse the request
		pxmlDocPtr = SP_ParseXML( pucBuff_i );
		if( NULL == pxmlDocPtr )
		{
			eRet_Code = MS_ERR_INSUFFICIENT_MEMORY;
		}
		else
		{
			xmlDocPtr pxmlRespPtr = NULL;
			// Generate the response template
			pxmlRespPtr = SP_GetResponse();
			if( NULL == pxmlRespPtr )
			{
				eRet_Code = MS_ERR_INSUFFICIENT_MEMORY;
			}
			else
			{
			}
			char* pcEncodedResponse = NULL;
			int nSize = 0;
			int nVarIndx = 1;

			while( MS_ERR_NONE == eRet_Code )
			{
				e_PerformanceCtrs ePerformanceCtrs;
				e_FaultCtrs eFaultCtrs;
				char* pcOpcode = NULL;
				char* pcOid = NULL;
				char* pcAttrValue = NULL;
				t_String oString = TSTRING_INITIALIZER;
				unsigned long ulAttrValue = -1;

				// Get the opcode from the message
				eRet_Code = SP_GetVariableAttrAsString( pxmlDocPtr, nVarIndx, "OPCODE", &pcOpcode );
				if( eRet_Code == MS_ERR_NONE )
				{
					// Set the opcode in the response
					eRet_Code = SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "OPCODE", pcOpcode );
				}
				else if( MS_ERR_INVALID_INPUT != eRet_Code )
				{
					nVarIndx++;
					continue;
				}
				else
				{
					break;
				}
				if( eRet_Code == MS_ERR_NONE )
				{
					// Get the oid from the message
					eRet_Code = SP_GetVariableAttrAsString( pxmlDocPtr, nVarIndx, "OID", &pcOid );
					if( eRet_Code == MS_ERR_NONE )
					{
						// Set the oid in the response
						eRet_Code = SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "OID", pcOid );
					}
					else
					{
						nVarIndx++;
						xmlFree( pcOpcode );
						eRet_Code = MS_ERR_NONE;
						continue;
					}
				}
				if( eRet_Code == MS_ERR_NONE )
				{
					if( 0 == xmlStrcasecmp( (xmlChar*) pcOpcode, (xmlChar*) "get" ))
					{
						MS_TRACE( poSnmpRecv->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Get request received\n" );

						// Get index of the given performance counter
						ePerformanceCtrs = SNMP_GetPerformanceCtrIndx( poSnmpRecv, pcOid );
						// Get index of the given fault counter
						eFaultCtrs = SNMP_GetFaultCtrIndx( poSnmpRecv, pcOid );
						if( PC_IncomingRTPPackets == ePerformanceCtrs )
						{
							// Get value of the given performance counter
							eRet_Code = SNMP_GetNumberOfIncomingRTPPackets( poSnmpRecv, poSnmpRecv->oSnmpCommConfig.poMsCtx, &ulAttrValue );
							if( MS_ERR_NONE != eRet_Code )
							{
								ulAttrValue = poSnmpRecv->oSnmpCommConfig.poMsCtx->pulPerformanceCtrs[ePerformanceCtrs];
							}
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( PC_OutgoingRTPPackets == ePerformanceCtrs )
						{
							// Get value of the given performance counter
							eRet_Code = SNMP_GetNumberOfOutgoingRTPPackets( poSnmpRecv, poSnmpRecv->oSnmpCommConfig.poMsCtx, &ulAttrValue );
							if( MS_ERR_NONE != eRet_Code )
							{
								ulAttrValue = poSnmpRecv->oSnmpCommConfig.poMsCtx->pulPerformanceCtrs[ePerformanceCtrs];
							}
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( PC_MaxCtrValue != ePerformanceCtrs )
						{
							// Get value of the given performance counter
							ulAttrValue = poSnmpRecv->oSnmpCommConfig.poMsCtx->pulPerformanceCtrs[ePerformanceCtrs];
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( FC_MaxCtrValue != eFaultCtrs )
						{
							// Get value of the given fault counter
							ulAttrValue = poSnmpRecv->oSnmpCommConfig.poMsCtx->pulFaultCtrs[eFaultCtrs];
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "userName" ))
						{
							// Get value of user name
							pcAttrValue = TS_GetStringZ( &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalUri.oUser );
							// Set the value in the response
							SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "VALUE", pcAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "silenceThresholdDB" ))
						{
							// Get value of silence threshold db
							ulAttrValue = poSnmpRecv->oSnmpCommConfig.poMsCtx->nSilenceThresholdDb;
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "jitterBuffer" ))
						{
							// Get value of jitter buffer
							ulAttrValue = poSnmpRecv->oSnmpCommConfig.poMsCtx->nJitterBufferMs;
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "localSdpFile" ))
						{
							// Get value of local sdp file
							eRet_Code = MSC_GetItemAsTString( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "ms", "local_sdp_file", &oString );
							// Set the value in the response
							pcAttrValue = TS_GetStringZ( &oString );
							SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "VALUE", pcAttrValue );
							TS_Free( &oString );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "sipPort" ))
						{
							// Get value of sip port
							eRet_Code = MSC_GetItemAsUL( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "sip", "port", &ulAttrValue );
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "frameworkPort" ))
						{
							// Get value of framework port
							eRet_Code = MSC_GetItemAsUL( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "framework", "port", &ulAttrValue );
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "frameworkTracelevel" ))
						{
							// Get value of trace level
							eRet_Code = MSC_GetItemAsUL( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "framework", "tracelevel", &ulAttrValue );
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "systemLocalIP" ))
						{
							// Get value of system local ip
							pcAttrValue = TS_GetStringZ( &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalIp );
							// Set the value in the response
							SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "VALUE", pcAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "systemDomainName" ))
						{
							// Get value of system domain name
							pcAttrValue = TS_GetStringZ( &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalUri.oHost );
							// Set the value in the response
							SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "VALUE", pcAttrValue );
						}
						else
						{
							// Get the value from the message
							eRet_Code = SP_GetVariableAttrAsString( pxmlDocPtr, nVarIndx, "VALUE", &pcAttrValue );
							// Set the value in the response
							eRet_Code = SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "VALUE", pcAttrValue );
							eRet_Code = MS_ERR_NOT_FOUND;
						}
						xmlFree( pcAttrValue );
					}
					else if( 0 == xmlStrcasecmp( (xmlChar*) pcOpcode, (xmlChar*) "set" ))
					{
						MS_TRACE( poSnmpRecv->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Set request recieved\n" );

						// Get index of the given performance counter
						ePerformanceCtrs = SNMP_GetPerformanceCtrIndx( poSnmpRecv, pcOid );
						// Get index of the given fault counter
						eFaultCtrs = SNMP_GetFaultCtrIndx( poSnmpRecv, pcOid );
						if( PC_MaxCtrValue != ePerformanceCtrs )
						{
							// Get the value from the message
							eRet_Code = SP_GetVariableAttrAsUL( pxmlDocPtr, nVarIndx, "VALUE", &ulAttrValue );
							if( eRet_Code == MS_ERR_NONE )
							{
								// Set value of the given performance counter
								poSnmpRecv->oSnmpCommConfig.poMsCtx->pulPerformanceCtrs[ePerformanceCtrs] = ulAttrValue;
							}
							else
							{
								// Invalid Value
							}
							// Get value of the given performance counter
							ulAttrValue = poSnmpRecv->oSnmpCommConfig.poMsCtx->pulPerformanceCtrs[ePerformanceCtrs];
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( FC_MaxCtrValue != eFaultCtrs )
						{
							// Get the value from the message
							eRet_Code = SP_GetVariableAttrAsUL( pxmlDocPtr, nVarIndx, "VALUE", &ulAttrValue );
							if( eRet_Code == MS_ERR_NONE )
							{
								// Set value of the given fault counter
								poSnmpRecv->oSnmpCommConfig.poMsCtx->pulFaultCtrs[eFaultCtrs] = ulAttrValue;
							}
							else
							{
								// Invalid Value
							}
							// Get value of the given fault counter
							ulAttrValue = poSnmpRecv->oSnmpCommConfig.poMsCtx->pulFaultCtrs[eFaultCtrs];
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "userName" ))
						{
							eRet_Code = SP_GetVariableAttrAsString( pxmlDocPtr, nVarIndx, "VALUE", &pcAttrValue );
							if( eRet_Code == MS_ERR_NONE )
							{
								// Set value of user name
								TS_Set( pcAttrValue, &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalUri.oUser );

								eRet_Code = MSC_SetItemAsTString( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "ms", "username", &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalUri.oUser );

//								printf( "pcOid: %s_pcAttrValue: %s\n", pcOid, pcAttrValue );
							}
							else
							{
								// Invalid Value
							}
							// Get value of user name
							xmlFree( pcAttrValue );
							pcAttrValue = TS_GetStringZ( &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalUri.oUser );
							// Set the value in the response
							SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "VALUE", pcAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "silenceThresholdDB" ))
						{
							eRet_Code = SP_GetVariableAttrAsUL( pxmlDocPtr, nVarIndx, "VALUE", &ulAttrValue );
							if( eRet_Code == MS_ERR_NONE )
							{
								// Set value of silence threshold db
								poSnmpRecv->oSnmpCommConfig.poMsCtx->nSilenceThresholdDb = ulAttrValue;

								eRet_Code = MSC_SetItemAsUL( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "ms", "silence_threshold_db", ulAttrValue );
							}
							else
							{
								// Invalid Value
							}
							// Get value of silence threshold db
							ulAttrValue = poSnmpRecv->oSnmpCommConfig.poMsCtx->nSilenceThresholdDb;
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "jitterBuffer" ))
						{
							eRet_Code = SP_GetVariableAttrAsUL( pxmlDocPtr, nVarIndx, "VALUE", &ulAttrValue );
							if( eRet_Code == MS_ERR_NONE )
							{
								// Set value of jitter buffer
								poSnmpRecv->oSnmpCommConfig.poMsCtx->nJitterBufferMs = ulAttrValue;

								eRet_Code = MSC_SetItemAsUL( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "ms", "jitter_buffer_ms", ulAttrValue );
							}
							else
							{
								// Invalid Value
							}
							// Get value of jitter buffer
							ulAttrValue = poSnmpRecv->oSnmpCommConfig.poMsCtx->nJitterBufferMs;
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "localSdpFile" ))
						{
							eRet_Code = SP_GetVariableAttrAsString( pxmlDocPtr, nVarIndx, "VALUE", &pcAttrValue );
							if( eRet_Code == MS_ERR_NONE )
							{
								// Set value of local sdp file
								TS_Set( pcAttrValue, &oString );
								eRet_Code = MSC_SetItemAsTString( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "ms", "local_sdp_file", &oString );
								TS_Free( &oString );
							}
							else
							{
								// Invalid Value
							}
							// Get value of local sdp file
							MSC_GetItemAsTString( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "ms", "local_sdp_file", &oString );
							// Set the value in the response
							xmlFree( pcAttrValue );
							pcAttrValue = TS_GetStringZ( &oString );
							SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "VALUE", pcAttrValue );
							TS_Free( &oString );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "sipPort" ))
						{
							eRet_Code = SP_GetVariableAttrAsUL( pxmlDocPtr, nVarIndx, "VALUE", &ulAttrValue );
							if( eRet_Code == MS_ERR_NONE )
							{
								// Set value of sip port
								eRet_Code = MSC_SetItemAsUL( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "sip", "port", ulAttrValue );
							}
							else
							{
								// Invalid Value
							}
							// Get value of sip port
							MSC_GetItemAsUL( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "sip", "port", &ulAttrValue );
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "frameworkPort" ))
						{
							eRet_Code = SP_GetVariableAttrAsUL( pxmlDocPtr, nVarIndx, "VALUE", &ulAttrValue );
							if( eRet_Code == MS_ERR_NONE )
							{
								// Set value of framework port
								eRet_Code = MSC_SetItemAsUL( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "framework", "port", ulAttrValue );
							}
							else
							{
								// Invalid Value
							}
							// Get value of framework port
							MSC_GetItemAsUL( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "framework", "port", &ulAttrValue );
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "frameworkTracelevel" ))
						{
							eRet_Code = SP_GetVariableAttrAsUL( pxmlDocPtr, nVarIndx, "VALUE", &ulAttrValue );
							if( eRet_Code == MS_ERR_NONE )
							{
								// Set value of trace level
								eRet_Code = MSC_SetItemAsUL( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "framework", "tracelevel", ulAttrValue );
							}
							else
							{
								// Invalid Value
							}
							// Get value of trace level
							MSC_GetItemAsUL( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "framework", "tracelevel", &ulAttrValue );
							// Set the value in the response
							SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "VALUE", ulAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "systemLocalIP" ))
						{
							eRet_Code = SP_GetVariableAttrAsString( pxmlDocPtr, nVarIndx, "VALUE", &pcAttrValue );
							if( eRet_Code == MS_ERR_NONE )
							{
								// Set value of system local ip
								TS_Set( pcAttrValue, &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalIp );

								eRet_Code = MSC_SetItemAsTString( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "system", "local_ip", &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalIp );
							}
							else
							{
								// Invalid Value
							}
							// Get value of system local ip
							xmlFree( pcAttrValue );
							pcAttrValue = TS_GetStringZ( &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalIp );
							// Set the value in the response
							SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "VALUE", pcAttrValue );
						}
						else if( 0 == xmlStrcasecmp( pcOid, (xmlChar*) "systemDomainName" ))
						{
							eRet_Code = SP_GetVariableAttrAsString( pxmlDocPtr, nVarIndx, "VALUE", &pcAttrValue );
							if( eRet_Code == MS_ERR_NONE )
							{
								// Set value of system domain name
								TS_Set( pcAttrValue, &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalUri.oHost );

								eRet_Code = MSC_SetItemAsTString( poSnmpRecv->oSnmpCommConfig.poMsCtx->poDoc, "system", "domainname", &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalUri.oHost );
							}
							else
							{
								// Invalid Value
							}
							// Get value of system domain name
							xmlFree( pcAttrValue );
							pcAttrValue = TS_GetStringZ( &poSnmpRecv->oSnmpCommConfig.poMsCtx->oLocalUri.oHost );
							// Set the value in the response
							SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "VALUE", pcAttrValue );
						}
						else
						{
							// Get the value from the message
							eRet_Code = SP_GetVariableAttrAsString( pxmlDocPtr, nVarIndx, "VALUE", &pcAttrValue );
							// Set the value in the response
							eRet_Code = SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "VALUE", pcAttrValue );
							eRet_Code = MS_ERR_NOT_FOUND;
						}
						xmlFree( pcAttrValue );
					}
					else
					{
						MS_TRACE( poSnmpRecv->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Invalid value of opcode : %s\n", pcOpcode );

						// Get the value from the message
						eRet_Code = SP_GetVariableAttrAsString( pxmlDocPtr, nVarIndx, "VALUE", &pcAttrValue );
						// Set the value in the response
						eRet_Code = SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "VALUE", pcAttrValue );
						eRet_Code = MS_ERR_NOT_IMPLEMENTED;
					}
				}
				xmlFree( pcOid );
				xmlFree( pcOpcode );

				// Calculate the appropriate error code and description
				e_SIPResStatusCodes eStatusCode;
				char pcErrorDesc[SNMP_MAX_ERRORDESC_STRING_LENGTH];

				switch( eRet_Code )
				{
					case MS_ERR_NONE:
						eStatusCode = SIP_RES_OK;
						strcpy( pcErrorDesc, "OK" );
						break;

					case MS_ERR_NOT_IMPLEMENTED:
						eStatusCode = SIP_RES_BAD_REQUEST;
						strcpy( pcErrorDesc, "Bad Request" );
						break;

					case MS_ERR_NOT_FOUND:
						eStatusCode = SIP_RES_NOT_FOUND;
						strcpy( pcErrorDesc, "Not Found" );
						break;

					default:
						eStatusCode = SIP_RES_SERVER_INTERNAL_ERROR;
						strcpy( pcErrorDesc, "Server Internal Error" );
						break;
				}

				// Set the error code and description in the response
				eRet_Code = SP_SetAttrAsUL( pxmlRespPtr, nVarIndx, "ERRORCODE", eStatusCode );
				if( MS_ERR_NONE == eRet_Code )
				{
					eRet_Code = SP_SetAttrAsString( pxmlRespPtr, nVarIndx, "ERRORDESC", pcErrorDesc );
				}
				else
				{
					MS_TRACE( poSnmpRecv->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Error while setting error code\n" );
				}
				nVarIndx++;
			}
			if( NULL != pxmlRespPtr )
			{
				// Encode the response
				pcEncodedResponse = SP_EncodeMessage( pxmlRespPtr, &nSize );
				MS_TRACE( poSnmpRecv->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Response to snmp: %s\n", pcEncodedResponse );

				// Send the response to snmp
				int nRet_Code = FW_SndBuffer( poSnmpRecv->oSnmpCommConfig.pvFrameworkCtx, poSnmpRecv->hUdpNetHandle, pcEncodedResponse, nSize, nPort_i, pcIPAddress_i );
				if( AMPS_SUCCESS == nRet_Code )
				{
					eRet_Code = MS_ERR_NONE;
				}
				else
				{
					eRet_Code = MS_ERR_FRAMEWORK;
				}
				xmlFree( pcEncodedResponse );
				xmlFreeDoc( pxmlRespPtr );
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

/*! @fn void* SNMP_MallocCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, int r_nSize )
*
* @brief This callback function allocates the memory for the buffer receiving the message.
*
* @param[in] hAMPS_HANDLE_i is the handle of Amps context.
*
* @param[in] hAMPS_APP_HANDLE_i is the handle of application required for interaction with the snmp agent.
*
* @param[in] r_nSize is the size of the buffer.
*
* @return A pointer to the allocated memory as void.
*
* @author Razi-ud-Din Chaudhry
*
*/

// void* SNMP_MallocCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, int r_nSize )
// {
//  return (FW_Malloc( r_nSize ));
// }

/*! @fn void SNMP_FreeCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, void* r_pvData )
*
* @brief This callback function frees the memory for the buffer receiving the message.
*
* @param[in] hAMPS_HANDLE_i is the handle of Amps context.
*
* @param[in] hAMPS_APP_HANDLE_i is the handle of application required for interaction with the snmp agent.
*
* @param[in] r_pvData is the buffer to be freed.
*
* @author Razi-ud-Din Chaudhry
*
*/

// void SNMP_FreeCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, void* r_pvData )
// {
//  FW_Free( r_pvData );
// }

/*! @fn e_PerformanceCtrs SNMP_GetPerformanceCtrIndx( t_SnmpRecv* poSnmpRecv_i, char* pcAttribute_i )
*
* @brief This function provides the index for the given performance counter.
*
* @param[in] poSnmpRecv_i is the structure required for interaction.
*
* @param[in] pcAttribute_i is the name of the performance counter whose index is to be determined.
*
* @return Index of the given performance counter.
*
* @author Razi-ud-Din Chaudhry
*
*/

e_PerformanceCtrs SNMP_GetPerformanceCtrIndx( t_SnmpRecv* poSnmpRecv_i, char* pcAttribute_i )
{
	e_PerformanceCtrs ePerformanceCtr = PC_MaxCtrValue;

	MS_TRACE( poSnmpRecv_i->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poSnmpRecv_i, ePerformanceCtr = PC_MaxCtrValue );
		ms_assert_return( NULL != pcAttribute_i, ePerformanceCtr = PC_MaxCtrValue );
	}

	{
		if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "inviteAttemptConf" ))
		{
			ePerformanceCtr = PC_InviteAttemptConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "inviteFailedConf" ))
		{
			ePerformanceCtr = PC_InviteFailedConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "inviteSuccessConf" ))
		{
			ePerformanceCtr = PC_InviteSuccessConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "inviteAnsweredConf" ))
		{
			ePerformanceCtr = PC_InviteAnsweredConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "infoAttemptConf" ))
		{
			ePerformanceCtr = PC_InfoAttemptConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "infoFailedConf" ))
		{
			ePerformanceCtr = PC_InfoFailedConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "infoSuccessConf" ))
		{
			ePerformanceCtr = PC_InfoSuccessConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "infoAnsweredConf" ))
		{
			ePerformanceCtr = PC_InfoAnsweredConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "cancelAttemptConf" ))
		{
			ePerformanceCtr = PC_CancelAttemptConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "cancelFailedConf" ))
		{
			ePerformanceCtr = PC_CancelFailedConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "cancelSuccessConf" ))
		{
			ePerformanceCtr = PC_CancelSuccessConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "cancelAnsweredConf" ))
		{
			ePerformanceCtr = PC_CancelAnsweredConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "byeAttemptConf" ))
		{
			ePerformanceCtr = PC_ByeAttemptConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "PC_ByeFailedConf" ))
		{
			ePerformanceCtr = PC_ByeFailedConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "byeSuccessConf" ))
		{
			ePerformanceCtr = PC_ByeSuccessConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "byeAnsweredConf" ))
		{
			ePerformanceCtr = PC_ByeAnsweredConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "inviteAttemptNonConf" ))
		{
			ePerformanceCtr = PC_InviteAttemptNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "inviteFailedNonConf" ))
		{
			ePerformanceCtr = PC_InviteFailedNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "inviteSuccessNonConf" ))
		{
			ePerformanceCtr = PC_InviteSuccessNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "inviteAnsweredNonConf" ))
		{
			ePerformanceCtr = PC_InviteAnsweredNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "infoAttemptNonConf" ))
		{
			ePerformanceCtr = PC_InfoAttemptNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "infoFailedNonConf" ))
		{
			ePerformanceCtr = PC_InfoFailedNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "infoSuccessNonConf" ))
		{
			ePerformanceCtr = PC_InfoSuccessNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "infoAnsweredNonConf" ))
		{
			ePerformanceCtr = PC_InfoAnsweredNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "cancelAttemptNonConf" ))
		{
			ePerformanceCtr = PC_CancelAttemptNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "cancelFailedNonConf" ))
		{
			ePerformanceCtr = PC_CancelFailedNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "cancelSuccessNonConf" ))
		{
			ePerformanceCtr = PC_CancelSuccessNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "cancelAnsweredNonConf" ))
		{
			ePerformanceCtr = PC_CancelAnsweredNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "byeAttemptNonConf" ))
		{
			ePerformanceCtr = PC_ByeAttemptNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "byeFailedNonConf" ))
		{
			ePerformanceCtr = PC_ByeFailedNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "byeSuccessNonConf" ))
		{
			ePerformanceCtr = PC_ByeSuccessNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "byeAnsweredNonConf" ))
		{
			ePerformanceCtr = PC_ByeAnsweredNonConf;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "incomingRTPPackets" ))
		{
			ePerformanceCtr = PC_IncomingRTPPackets;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "outgoingRTPPackets" ))
		{
			ePerformanceCtr = PC_OutgoingRTPPackets;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureConfAttempt" ))
		{
			ePerformanceCtr = PC_ConfigureConfAttempt;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureConfFailed" ))
		{
			ePerformanceCtr = PC_ConfigureConfFailed;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureConfSuccess" ))
		{
			ePerformanceCtr = PC_ConfigureConfSuccess;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureConfAnswered" ))
		{
			ePerformanceCtr = PC_ConfigureConfAnswered;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureLegAttempt" ))
		{
			ePerformanceCtr = PC_ConfigureLegAttempt;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureLegFailed" ))
		{
			ePerformanceCtr = PC_ConfigureLegFailed;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureLegSuccess" ))
		{
			ePerformanceCtr = PC_ConfigureLegSuccess;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureLegAnswered" ))
		{
			ePerformanceCtr = PC_ConfigureLegAnswered;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureTeamAttempt" ))
		{
			ePerformanceCtr = PC_ConfigureTeamAttempt;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureTeamFailed" ))
		{
			ePerformanceCtr = PC_ConfigureTeamFailed;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureTeamSuccess" ))
		{
			ePerformanceCtr = PC_ConfigureTeamSuccess;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "configureTeamAnswered" ))
		{
			ePerformanceCtr = PC_ConfigureTeamAnswered;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playAttempt" ))
		{
			ePerformanceCtr = PC_PlayAttempt;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playFailed" ))
		{
			ePerformanceCtr = PC_PlayFailed;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playSuccess" ))
		{
			ePerformanceCtr = PC_PlaySuccess;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playAnswered" ))
		{
			ePerformanceCtr = PC_PlayAnswered;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playCollectAttempt" ))
		{
			ePerformanceCtr = PC_PlayCollectAttempt;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playCollectFailed" ))
		{
			ePerformanceCtr = PC_PlayCollectFailed;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playCollectSuccess" ))
		{
			ePerformanceCtr = PC_PlayCollectSuccess;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playCollectAnswered" ))
		{
			ePerformanceCtr = PC_PlayCollectAnswered;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playRecordAttempt" ))
		{
			ePerformanceCtr = PC_PlayRecordAttempt;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playRecordFailed" ))
		{
			ePerformanceCtr = PC_PlayRecordFailed;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playRecordSuccess" ))
		{
			ePerformanceCtr = PC_PlayRecordSuccess;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "playRecordAnswered" ))
		{
			ePerformanceCtr = PC_PlayRecordAnswered;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "stopAttempt" ))
		{
			ePerformanceCtr = PC_StopAttempt;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "stopFailed" ))
		{
			ePerformanceCtr = PC_StopFailed;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "stopSuccess" ))
		{
			ePerformanceCtr = PC_StopSuccess;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "stopAnswered" ))
		{
			ePerformanceCtr = PC_StopAnswered;
		}
		else
		{
			ePerformanceCtr = PC_MaxCtrValue;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poSnmpRecv_i->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %d\n", ePerformanceCtr );
	return (ePerformanceCtr);
}

/*! @fn e_FaultCtrs SNMP_GetFaultCtrIndx( t_SnmpRecv* poSnmpRecv_i, char* pcAttribute_i )
*
* @brief This function provides the index for the given fault counter.
*
* @param[in] poSnmpRecv_i is the structure required for interaction.
*
* @param[in] pcAttribute_i is the name of the fault counter whose index is to be determined.
*
* @return Index of the given fault counter.
*
* @author Razi-ud-Din Chaudhry
*
*/

e_FaultCtrs SNMP_GetFaultCtrIndx( t_SnmpRecv* poSnmpRecv_i, char* pcAttribute_i )
{
	e_FaultCtrs eFaultCtr = FC_MaxCtrValue;

	MS_TRACE( poSnmpRecv_i->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poSnmpRecv_i, eFaultCtr = FC_MaxCtrValue );
		ms_assert_return( NULL != pcAttribute_i, eFaultCtr = FC_MaxCtrValue );
	}

	{
		if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "invitesReceived" ))
		{
			eFaultCtr = FC_InvitesReceived;
		}
		else if( 0 == xmlStrcasecmp( pcAttribute_i, (xmlChar*) "infoReceived" ))
		{
			eFaultCtr = FC_InfoReceived;
		}
		else
		{
			eFaultCtr = FC_MaxCtrValue;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poSnmpRecv_i->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %d\n", eFaultCtr );
	return (eFaultCtr);
}

/*! @fn e_MsRetCode SNMP_GetNumberOfIncomingRTPPackets( t_SnmpRecv* poSnmpRecv_i, t_MsCtx* poMsCtx_i, unsigned long* pulValue_o )
*
* @brief This function provides the total number of RTP packets received by the media server.
*
* @param[in] poSnmpRecv_i is the structure required for interaction.
*
* @param[in] poMsCtx_i is the media server context.
*
* @param[out] pulValue_o is the pointer to the value indicating the number of RTP packets.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode SNMP_GetNumberOfIncomingRTPPackets( t_SnmpRecv* poSnmpRecv_i, t_MsCtx* poMsCtx_i, unsigned long* pulValue_o )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;

	MS_TRACE( poSnmpRecv_i->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poSnmpRecv_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsCtx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pulValue_o, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		*pulValue_o = poSnmpRecv_i->oSnmpCommConfig.poMsCtx->pulPerformanceCtrs[PC_IncomingRTPPackets];

		t_AMPSSList* poConfList = NULL;
		t_MsConfCtx* poConfCtx = NULL;
		t_AMPSSList* poUserList = NULL;
		t_MsUserCtrlCtx* poUserCtx = NULL;

		poConfList = poMsCtx_i->poConfCtxList->poAMPSSListHead;

		while( NULL != poConfList )
		{
			poConfCtx = (t_MsConfCtx*)(poConfList->pvData);
			poUserList = poConfCtx->poUserCtrlCtxList->poAMPSSListHead;

			while( NULL != poUserList )
			{
				poUserCtx = (t_MsUserCtrlCtx*)(poUserList->pvData);

				unsigned int unValue;
				eRet_Code = MS_GetNumOfRTPPackets( poMsCtx_i, poUserCtx->hRxChain, MCT_RTPRECV, MCC_RTPRECV_NUM_PKTS_RECV, &unValue );
				if( MS_ERR_NONE == eRet_Code )
				{
					*pulValue_o += unValue;
				}
				poUserList = poUserList->poAMPSSListNext;
			}
			poUserList = poUserList->poAMPSSListNext;
		}
		poUserList = poMsCtx_i->poUserCtrlCtxList->poAMPSSListHead;

		while( NULL != poUserList )
		{
			poUserCtx = (t_MsUserCtrlCtx*)(poUserList->pvData);

			unsigned int unValue;
			eRet_Code = MS_GetNumOfRTPPackets( poMsCtx_i, poUserCtx->hRxChain, MCT_RTPRECV, MCC_RTPRECV_NUM_PKTS_RECV, &unValue );
			if( MS_ERR_NONE == eRet_Code )
			{
				*pulValue_o += unValue;
			}
			poUserList = poUserList->poAMPSSListNext;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poSnmpRecv_i->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/*! @fn e_MsRetCode SNMP_GetNumberOfOutgoingRTPPackets( t_SnmpRecv* poSnmpRecv_i, t_MsCtx* poMsCtx_i, unsigned long* pulValue_o )
*
* @brief This function provides the total number of RTP packets sent by the media server.
*
* @param[in] poSnmpRecv_i is the structure required for interaction.
*
* @param[in] poMsCtx_i is the media server context.
*
* @param[out] pulValue_o is the pointer to the value indicating the number of RTP packets.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode SNMP_GetNumberOfOutgoingRTPPackets( t_SnmpRecv* poSnmpRecv_i, t_MsCtx* poMsCtx_i, unsigned long* pulValue_o )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;

	MS_TRACE( poSnmpRecv_i->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poSnmpRecv_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsCtx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pulValue_o, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		*pulValue_o = poSnmpRecv_i->oSnmpCommConfig.poMsCtx->pulPerformanceCtrs[PC_OutgoingRTPPackets];

		t_AMPSSList* poConfList = NULL;
		t_MsConfCtx* poConfCtx = NULL;
		t_AMPSSList* poUserList = NULL;
		t_MsUserCtrlCtx* poUserCtx = NULL;

		poConfList = poMsCtx_i->poConfCtxList->poAMPSSListHead;

		while( NULL != poConfList )
		{
			poConfCtx = (t_MsConfCtx*)(poConfList->pvData);
			poUserList = poConfCtx->poUserCtrlCtxList->poAMPSSListHead;

			while( NULL != poUserList )
			{
				poUserCtx = (t_MsUserCtrlCtx*)(poUserList->pvData);

				unsigned int unValue;
				eRet_Code = MS_GetNumOfRTPPackets( poMsCtx_i, poUserCtx->hTxChain, MCT_RTPSEND, MCC_RTPSEND_NUM_PKTS_SENT, &unValue );
				if( MS_ERR_NONE == eRet_Code )
				{
					*pulValue_o += unValue;
				}
				poUserList = poUserList->poAMPSSListNext;
			}
			poUserList = poUserList->poAMPSSListNext;
		}
		poUserList = poMsCtx_i->poUserCtrlCtxList->poAMPSSListHead;

		while( NULL != poUserList )
		{
			poUserCtx = (t_MsUserCtrlCtx*)(poUserList->pvData);

			unsigned int unValue;
			eRet_Code = MS_GetNumOfRTPPackets( poMsCtx_i, poUserCtx->hTxChain, MCT_RTPSEND, MCC_RTPSEND_NUM_PKTS_SENT, &unValue );
			if( MS_ERR_NONE == eRet_Code )
			{
				*pulValue_o += unValue;
			}
			poUserList = poUserList->poAMPSSListNext;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poSnmpRecv_i->oSnmpCommConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

