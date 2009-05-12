#include "ms_mscml_handler.h"

e_MsRetCode MS_HandleConfigureConference( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, xmlDocPtr poMsmclDoc_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsConfCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsConfCtx_i->nMagicNum == MS_CONF_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
		ms_assert_return( NULL != poMsmclDoc_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureConfAttempt );

		int nReservedTalkers = -1;

		eRetCode = GetRequestAttrAsInt( poMsmclDoc_i, "configure_conference", "reservedtalkers", &nReservedTalkers );
		if( MS_ERR_NONE == eRetCode )
		{
			poMsConfCtx_i->nAvailableCtrlCtx = nReservedTalkers;

			AMPS_BOOL bReserveConfMedia = AMPS_TRUE;
			eRetCode = GetRequestAttrAsBool( poMsmclDoc_i, "configure_conference", "reserveconfmedia", &bReserveConfMedia );
			if( MS_ERR_NONE == eRetCode )
			{
				int nCtrlCtxCount = 0;

				for( nCtrlCtxCount = 0; nCtrlCtxCount < poMsConfCtx_i->nAvailableCtrlCtx; nCtrlCtxCount++ )
				{
					t_MsUserCtrlCtx* poMsUserCtrlCtx = NULL;
					eRetCode = MS_CreateUserCtrlCtx( poMsCtx_i, bReserveConfMedia, &poMsUserCtrlCtx );
					if( MS_ERR_NONE == eRetCode )
					{
						FW_DListAppend( poMsConfCtx_i->poUserCtrlCtxList, poMsUserCtrlCtx );
					}
				}
			}
		}
		else if( MS_ERR_END_OF_LIST == eRetCode )
		{
			poMsConfCtx_i->nAvailableCtrlCtx = -1;
			eRetCode = MS_ERR_NONE;
		}
		// Generate MSCML <response>
		xmlDocPtr poMscmlResp = NULL;

		poMscmlResp = GetMscmlResponse();
		if( NULL != poMscmlResp )
		{
			SetResponseAttrAsString( poMscmlResp, "request", "configure_conference" );
			if( MS_ERR_NONE == eRetCode )
			{
				SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureConfSuccess );

				SetResponseAttrAsString( poMscmlResp, "text", "OK" );
				SetResponseAttrAsInt( poMscmlResp, "code", SIP_RES_OK );
			}
			else
			{
				SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureConfFailed );

				SetResponseAttrAsString( poMscmlResp, "text", "Error" );
				SetResponseAttrAsInt( poMscmlResp, "code", SIP_RES_SERVER_INTERNAL_ERROR );
			}
			SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureConfAnswered );

			eRetCode = MS_SendMscmlInInfo( poMsCtx_i, poMsConfCtx_i->poConfCtrlCtx, poMscmlResp );

			xmlFreeDoc( poMscmlResp );
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_HandleConfigureLeg( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsConfCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsConfCtx_i->nMagicNum == MS_CONF_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
		ms_assert_return( NULL != poMsUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsUserCtrlCtx_i->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
		ms_assert_return( NULL != poMsmclDoc_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureLegAttempt );

		char* pchLegId = NULL;
		char* pchMixMode = NULL;

		// Generate MSCML Response
		xmlDocPtr poMscmlResp = NULL;

		poMscmlResp = GetMscmlResponse();
		if( NULL != poMscmlResp )
		{
			SetResponseAttrAsString( poMscmlResp, "request", "configure_leg" );
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = GetRequestAttrAsBool( poMsmclDoc_i, "configure_leg", "dtmfclamp", &poMsUserCtrlCtx_i->bDtmfClamp );

			eRetCode = GetRequestAttrAsString( poMsmclDoc_i, "configure_leg", "id", &pchLegId );
			if( MS_ERR_NONE == eRetCode )
			{
				TS_Assign((char*)pchLegId, &poMsUserCtrlCtx_i->oTeamMemberId );

				TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leg Id = ", &poMsUserCtrlCtx_i->oTeamMemberId );
			}
			eRetCode = GetRequestAttrAsString( poMsmclDoc_i, "configure_leg", "mixmode", &pchMixMode );
			if( MS_ERR_NONE != eRetCode )
			{
				// Set Default value
				pchMixMode = xmlStrdup((xmlChar*)"full" );
			}
			TS_PrintN( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Mode = ", pchMixMode, strlen((char*)pchMixMode ));

			e_MixMode eCurrentMode = MM_FULL;
			if( 0 == xmlStrcasecmp( pchMixMode, (xmlChar*)"parked" ) )
			{
				eCurrentMode = MM_PARKED;
			}
			else if( 0 == xmlStrcasecmp( pchMixMode, (xmlChar*)"full" ) )
			{
				eCurrentMode = MM_FULL;
			}
			else if( 0 == xmlStrcasecmp( pchMixMode, (xmlChar*)"mute" ) )
			{
				eCurrentMode = MM_MUTE;
			}
			else if( 0 == xmlStrcasecmp( pchMixMode, (xmlChar*)"private" ) )
			{
				eCurrentMode = MM_PRIVATE;
			}
			else if( 0 == xmlStrcasecmp( pchMixMode, (xmlChar*)"preferred" ) )
			{
				eCurrentMode = MM_PREFERRED;
			}
			else
			{
				eCurrentMode = MM_FULL;
			}
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = MS_Media_ChainSetMode( poMsCtx_i, poMsConfCtx_i, poMsUserCtrlCtx_i, eCurrentMode, NULL );
				if( MS_ERR_NONE == eRetCode )
				{
					SetResponseAttrAsString( poMscmlResp, "text", "MIXMODE_SUCC" );
				}
				else
				{
					SetResponseAttrAsString( poMscmlResp, "text", "MIXMODE_FAILURE" );
				}
				xmlFree( pchMixMode );
			}
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = MS_HandleConfigureTeam( poMsCtx_i, poMsConfCtx_i, poMsUserCtrlCtx_i, poMsmclDoc_i, poMscmlResp );
			}
			if( MS_ERR_NONE == eRetCode )
			{
				SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureLegSuccess );

				SetResponseAttrAsInt( poMscmlResp, "code", SIP_RES_OK );
			}
			else
			{
				SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureLegFailed );

				SetResponseAttrAsInt( poMscmlResp, "code", SIP_RES_SERVER_INTERNAL_ERROR );
			}
			SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureLegAnswered );

			eRetCode = MS_SendMscmlInInfo( poMsCtx_i, poMsUserCtrlCtx_i, poMscmlResp );

			xmlFreeDoc( poMscmlResp );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_HandleConfigureTeam( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i, xmlDocPtr poMscmlResp_io )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsConfCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsConfCtx_i->nMagicNum == MS_CONF_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
		ms_assert_return( NULL != poMsUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsUserCtrlCtx_i->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
		ms_assert_return( NULL != poMsmclDoc_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMscmlResp_io, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		char* pchAction = NULL;

		eRetCode = GetConfigureTeamAttrAsString( poMsmclDoc_i, "action", &pchAction );
		if( MS_ERR_NONE == eRetCode )
		{
			SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureTeamAttempt );

			t_MsUserCtrlCtx* poUserCtrlCtx1 = NULL;     // ctx of user to perform action on
			// if no id is provided with configure_leg, then use the current user-ctrl ctx
			if( poMsUserCtrlCtx_i->oTeamMemberId.pucData == NULL )
			{
				poUserCtrlCtx1 = poMsUserCtrlCtx_i;
			}
			else
			{
				// Get User-Ctrl Context of user on which to perform action
				eRetCode = MS_GetUserCtxFromId( poMsCtx_i, poMsConfCtx_i, &poMsUserCtrlCtx_i->oTeamMemberId, &poUserCtrlCtx1 );

				ms_assert( eRetCode == MS_ERR_NONE );
				ms_assert( poUserCtrlCtx1 == poMsUserCtrlCtx_i );
			}
			unsigned int unTeamMateIndex = 1;

			while( eRetCode == MS_ERR_NONE )
			{
				t_String oTeamMateId = TSTRING_INITIALIZER;
				xmlChar* pchTeamMateId = NULL;
				t_MsUserCtrlCtx* poUserCtrlCtx2 = NULL;

				eRetCode = GetTeamMateId( poMsmclDoc_i, unTeamMateIndex++, &pchTeamMateId );
				if( MS_ERR_NONE == eRetCode )
				{
					TS_Assign((char*)pchTeamMateId, &oTeamMateId );

					eRetCode = MS_GetUserCtxFromId( poMsCtx_i, poMsConfCtx_i, &oTeamMateId, &poUserCtrlCtx2 );
					if( MS_ERR_NONE == eRetCode )
					{
						if( 0 == xmlStrcasecmp( pchAction, (const xmlChar*)"add" ))
						{
							eRetCode = MS_Team_AddTeamMate( poMsCtx_i, poMsConfCtx_i, poUserCtrlCtx1,  poUserCtrlCtx2 );
						}
						else if( 0 == xmlStrcasecmp( pchAction, (const xmlChar*)"delete" ))
						{
							eRetCode = MS_Team_DeleteTeamMate( poMsCtx_i, poMsConfCtx_i, poUserCtrlCtx1,  poUserCtrlCtx2 );
						}
					}
					else
					{
						TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_WARNING, "No user control context found agaist team-mate id (skipped) = ", &oTeamMateId );
					}
					TS_Free( &oTeamMateId );
				}
				else
				{
					eRetCode = MS_ERR_NONE;
					break;
				}
			}
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = MS_Media_ChainSetMode( poMsCtx_i, poMsConfCtx_i, poMsUserCtrlCtx_i, poMsUserCtrlCtx_i->eCurrentMode, NULL );
				if( 0 == xmlStrcasecmp( pchAction, (const xmlChar*)"add" ))
				{
					SetResponseAttrAsString( poMscmlResp_io, "text", "TEAM_ADD_SUCC" );
				}
				else if( 0 == xmlStrcasecmp( pchAction, (const xmlChar*)"delete" ))
				{
					SetResponseAttrAsString( poMscmlResp_io, "text", "TEAM_DEL_SUCC" );
				}
				char* pcId = NULL;
				pcId = TS_GetStringZ( &poUserCtrlCtx1->oTeamMemberId );

				ms_assert( NULL != pcId );

				SetTeamAttrAsString( poMscmlResp_io, "id", pcId );
				FW_Free( pcId );

				// team size is double the size of team mate member list
				SetTeamAttrAsInt( poMscmlResp_io, "numteam", (int)poUserCtrlCtx1->poTeamMateList->uchCount * 2 );

				t_AMPSSList* poListNode = NULL;
				t_MsUserCtrlCtx* poTeamMateUserCtrlCtx = NULL;

				for( poListNode = poUserCtrlCtx1->poTeamMateList->poAMPSSListHead; poListNode != NULL; poListNode = poListNode->poAMPSSListNext )
				{
					poTeamMateUserCtrlCtx = poListNode->pvData;

					pcId = TS_GetStringZ( &poTeamMateUserCtrlCtx->oTeamMemberId );

					ms_assert( NULL != pcId );

					AddTeamMateId( poMscmlResp_io, (xmlChar*)pcId );

					FW_Free( pcId );
				}
			}
			else
			{
				if( 0 == xmlStrcasecmp( pchAction, (const xmlChar*)"add" ))
				{
					SetResponseAttrAsString( poMscmlResp_io, "text", "TEAM_ADD_SUCC" );
				}
				else if( 0 == xmlStrcasecmp( pchAction, (const xmlChar*)"delete" ))
				{
					SetResponseAttrAsString( poMscmlResp_io, "text", "TEAM_DEL_SUCC" );
				}
			}
			xmlFree( pchAction );
			if( MS_ERR_NONE == eRetCode )
			{
				SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureTeamSuccess );
			}
			else
			{
				SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureTeamFailed );
			}
			SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_ConfigureTeamAnswered );
		}
		else
		{
			eRetCode = MS_ERR_NONE;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_HandlePlay( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsUserCtrlCtx_i->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
		ms_assert_return( NULL != poMsmclDoc_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_PlayAttempt );
		// Check if a request is already is already executing
		if( poMsUserCtrlCtx_i->pvMscmlOpCtx != NULL )
		{
			eRetCode =  MS_SM_SendSmStop( poMsCtx_i, poMsUserCtrlCtx_i );

			// Store the MSCML Request
			poMsUserCtrlCtx_i->ePendingOpCtxType = MOT_PLAY;
			poMsUserCtrlCtx_i->poPendingMscmlReq = poMsmclDoc_i;
		}
		else
		{
			eRetCode = MS_Media_ChainSetMode( poMsCtx_i, poMsConfCtx_i, poMsUserCtrlCtx_i, MM_PARKED, NULL );

			//	Create a PLAY context
			eRetCode = MOP_CreateSM( MOT_PLAY, poMsCtx_i->nTraceId, poMsCtx_i->pvFrameworkCtx, poMsCtx_i->pchSoundDir, &poMsUserCtrlCtx_i->pvMscmlOpCtx );
			if( MS_ERR_NONE == eRetCode )
			{
				MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "PLAY context created successfully\n" );

				//MSCML_OP_EVENT = MSCML_PLAY_EVENT;

				// Send MSCML_REQUEST
				eRetCode = MS_SM_SendMscmlRequest( poMsCtx_i, poMsUserCtrlCtx_i, poMsmclDoc_i );
			}
			else
			{
				MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "PlAY context not created\n" );
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_HandlePlayRecord( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsUserCtrlCtx_i->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
		ms_assert_return( NULL != poMsmclDoc_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_PlayRecordAttempt );
		// Check if a request is already is already executing
		if( poMsUserCtrlCtx_i->pvMscmlOpCtx != NULL )
		{
			eRetCode =  MS_SM_SendSmStop( poMsCtx_i, poMsUserCtrlCtx_i );

			// Store the MSCML Request
			poMsUserCtrlCtx_i->ePendingOpCtxType = MOT_PLAYRECORD;
			poMsUserCtrlCtx_i->poPendingMscmlReq = poMsmclDoc_i;
		}
		else
		{
			eRetCode = MS_Media_ChainSetMode( poMsCtx_i, poMsConfCtx_i, poMsUserCtrlCtx_i, MM_PARKED, NULL );

			//	Create a PLAYRECORD context
			eRetCode = MOP_CreateSM( MOT_PLAYRECORD, poMsCtx_i->nTraceId, poMsCtx_i->pvFrameworkCtx, poMsCtx_i->pchSoundDir,  &poMsUserCtrlCtx_i->pvMscmlOpCtx );
			if( MS_ERR_NONE == eRetCode )
			{
				MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "PLAYRECORD context created successfully\n" );

				// Send MSCML_REQUEST
				eRetCode = MS_SM_SendMscmlRequest( poMsCtx_i, poMsUserCtrlCtx_i, poMsmclDoc_i );
			}
			else
			{
				MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "PlAYRECORD context not created\n" );
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_HandlePlayCollect( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsUserCtrlCtx_i->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
		ms_assert_return( NULL != poMsmclDoc_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_PlayCollectAttempt );
		// Check if a request is already is already executing
		if( poMsUserCtrlCtx_i->pvMscmlOpCtx != NULL )
		{
			eRetCode =  MS_SM_SendSmStop( poMsCtx_i, poMsUserCtrlCtx_i );

			// Store the MSCML Request
			poMsUserCtrlCtx_i->ePendingOpCtxType = MOT_PLAYCOLLECT;
			poMsUserCtrlCtx_i->poPendingMscmlReq = poMsmclDoc_i;
		}
		else
		{
			eRetCode = MS_Media_ChainSetMode( poMsCtx_i, poMsConfCtx_i, poMsUserCtrlCtx_i, MM_PARKED, NULL );

			//	Create a PLAYCOLLECT context
			eRetCode = MOP_CreateSM( MOT_PLAYCOLLECT, poMsCtx_i->nTraceId, poMsCtx_i->pvFrameworkCtx, poMsCtx_i->pchSoundDir,  &poMsUserCtrlCtx_i->pvMscmlOpCtx );
			if( MS_ERR_NONE == eRetCode )
			{
				MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "PLAYCOLLECT context created successfully\n" );

				// Send MSCML_REQUEST
				eRetCode = MS_SM_SendMscmlRequest( poMsCtx_i, poMsUserCtrlCtx_i, poMsmclDoc_i );
			}
			else
			{
				MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "PLAYCOLLECT context not created\n" );
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_HandleStop( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsUserCtrlCtx_i->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
		ms_assert_return( NULL != poMsmclDoc_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode =  MS_SM_SendSmStop( poMsCtx_i, poMsUserCtrlCtx_i );

		// Generate MSCML Response
		xmlDocPtr poMscmlResp = NULL;

		poMscmlResp = GetMscmlResponse();
		if( NULL != poMscmlResp )
		{
			SetResponseAttrAsString( poMscmlResp, "request", "stop" );
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
		if( MS_ERR_NONE == eRetCode )
		{
			SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_StopSuccess );

			SetResponseAttrAsString( poMscmlResp, "text", "OK" );
			SetResponseAttrAsInt( poMscmlResp, "code", SIP_RES_OK );
		}
		else
		{
			SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_StopFailed );

			SetResponseAttrAsString( poMscmlResp, "text", "Error" );
			SetResponseAttrAsInt( poMscmlResp, "code", SIP_RES_SERVER_INTERNAL_ERROR );
		}
		SNMP_IncrementPerformanceCtr( poMsCtx_i, PC_StopAnswered );

		eRetCode = MS_SendMscmlInInfo( poMsCtx_i, poMsUserCtrlCtx_i, poMscmlResp );

		xmlFreeDoc( poMscmlResp );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_HandleMscmlMessage( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i,  SIP_MSG_HANDLE hSipMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsUserCtrlCtx_i->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM || poMsUserCtrlCtx_i->nMagicNum == MS_CONF_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_String oMscmlMsg = TSTRING_INITIALIZER;
		if( MS_ERR_NONE == MS_GetMscmlFromMsg( poMsCtx_i, hSipMsg_i, &oMscmlMsg ))
		{
			xmlDocPtr poMscmlDoc = NULL;
			xmlNodePtr poNode = NULL;

			TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "MSCML Message = ", &oMscmlMsg );

			// Parse Mscml Message
			poMscmlDoc = xmlParseMemory( (const char*)oMscmlMsg.pucData, (int)oMscmlMsg.unLength );
			if( NULL != poMscmlDoc )
			{
				MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "MSCML message parsed successfully\n" );
				if( 0 == xmlSchemaValidateDoc( poMsCtx_i->poMscmlValidCtx, poMscmlDoc ) )
				{
					poNode = xmlDocGetRootElement( poMscmlDoc );
					if( poNode != NULL )
					{
						AMPS_BOOL bNodeFound = AMPS_FALSE;

						// Search "request" node
						for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
						{
							if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
							{
								bNodeFound = AMPS_TRUE;
								break;
							}
						}
						if( bNodeFound == AMPS_TRUE )
						{
							for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
							{
								MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Processing %s\n", poNode->name );
								if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"play" ))
								{
									eRetCode = MS_HandlePlay( poMsCtx_i, poMsConfCtx_i, poMsUserCtrlCtx_i, poMscmlDoc );

									break;
								}
								else if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playcollect" ))
								{
									eRetCode = MS_HandlePlayCollect( poMsCtx_i, poMsConfCtx_i, poMsUserCtrlCtx_i, poMscmlDoc );

									break;
								}
								else if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playrecord" ))
								{
									eRetCode = MS_HandlePlayRecord( poMsCtx_i, poMsConfCtx_i, poMsUserCtrlCtx_i, poMscmlDoc );

									break;
								}
								else if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"configure_conference" ))
								{
									eRetCode = MS_HandleConfigureConference( poMsCtx_i, poMsConfCtx_i, poMscmlDoc );

									break;
								}
								else if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"configure_leg" ))
								{
									eRetCode = MS_HandleConfigureLeg( poMsCtx_i, poMsConfCtx_i, poMsUserCtrlCtx_i, poMscmlDoc );

									break;
								}
								else if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"stop" ))
								{
									eRetCode = MS_HandleStop( poMsCtx_i, poMsConfCtx_i, poMsUserCtrlCtx_i, poMscmlDoc );

									break;
								}
							}
						}
						else
						{
							eRetCode = MS_ERR_INVALID_INPUT;
						}
					}
				}
				else
				{
					MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "MSCML message not validated successfully\n" );
					eRetCode = MS_ERR_INVALID_INPUT;
				}
			}
			else
			{
				MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "MSCML message not parsed successfully\n" );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
			TS_Free( &oMscmlMsg );
			if( MS_ERR_NONE != eRetCode )
			{
				// If we know what type of request was received
				if( NULL != poNode )
				{
					eRetCode = MS_ERR_NONE;     // Dont return MSCML errors in SIP Response, Send MSCML <response>

					// Generate MSCML <response>
					xmlDocPtr poMscmlResp = NULL;

					poMscmlResp = GetMscmlResponse();
					if( NULL != poMscmlResp )
					{
						SetResponseAttrAsString( poMscmlResp, "request", (xmlChar*)poNode->name );
						SetResponseAttrAsString( poMscmlResp, "text", "Bad Request" );
						SetResponseAttrAsInt( poMscmlResp, "code", SIP_RES_BAD_REQUEST );

						eRetCode = MS_SendMscmlInInfo( poMsCtx_i, poMsUserCtrlCtx_i, poMscmlResp );

						// Free XML Doc
						xmlFreeDoc( poMscmlResp );
					}
					else
					{
						eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
					}
				}
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_SM_SendMscmlRequest( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsmclDoc_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsUserCtrlCtx_i->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
	}

	{
		t_MsEvent oMsEvent;

		oMsEvent.pvCtx = poMsUserCtrlCtx_i->pvMscmlOpCtx;
		oMsEvent.pvAppCtx = (void*)poMsUserCtrlCtx_i;
		oMsEvent.eType = MSCML_REQUEST;
		oMsEvent.uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile = poMsmclDoc_i;
		if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poMsCtx_i->pvFrameworkCtx, MSCML_OP_EVENT, &oMsEvent, sizeof (t_MsEvent)))
		{
			MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_SM_SendSmStop( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( poMsUserCtrlCtx_i->nMagicNum == MS_USER_CTRL_CTX_MAGIC_NUM, eRetCode = MS_ERR_INVALID_HANDLE );
	}

	{
		t_MsEvent oMsEvent;

		oMsEvent.pvCtx = poMsUserCtrlCtx_i->pvMscmlOpCtx;
		oMsEvent.pvAppCtx = (void*)poMsUserCtrlCtx_i;
		oMsEvent.eType = SM_STOP;
		if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( poMsCtx_i->pvFrameworkCtx, MSCML_OP_EVENT, &oMsEvent, sizeof (t_MsEvent)))
		{
			MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
			eRetCode = MS_ERR_FRAMEWORK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_SendMscmlInInfo( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMscmlMsg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMscmlMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_String oMscmlContentType = {(unsigned char*)"mediaservercontrol+xml", (unsigned int) sizeof ("mediaservercontrol+xml")-1};
		t_String oContentBody = TSTRING_INITIALIZER;

		// Encode MSCML
		xmlDocDumpMemory( poMscmlMsg_i, &oContentBody.pucData, (int*)&oContentBody.unLength );

		TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "response =", &oContentBody );

		// Send INFO with MSCML message
		eRetCode = MS_SendInfoRequest( poMsCtx_i, poMsUserCtrlCtx_i, &oMscmlContentType, &oContentBody );

		xmlFree( oContentBody.pucData );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

