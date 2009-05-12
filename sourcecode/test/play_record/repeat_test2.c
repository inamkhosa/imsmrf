// Test For Repeat>=1, SeqofFiles =1 , Duration= Infinite
#include "unit_tests.h"
//Unit Test State Machine States(UTSM_States)
enum e_SMRepeatTest2States
{
	RT2_W4Start_Play,
	RT2_W4Mscml_Response,
};
enum e_SMRepeatTest2States eSMRT2CurrentState;
int nRepeat;
char RT2_XFile[] = "<?xml version=\"1.0\"?>\
        <MediaServerControl version=\"1.0\">\
        <request>\
        <playrecord id=\"5556123\"\
        recurl=\"/home/suliman_shah/mediaserver/trunk/qa/sipp/non-conference/play/name.wav\"\
        recencoding=\"msgsm\"\
        initsilence=\"700\" endsilence=\"400\" duration=\"300\"\
        barge=\"yes\"\
        beep=\"no\"\
        mode=\"overwrite\"\
        cleardigits=\"no\"\
        escapekey=\"*\"\
        recstopmask=\"0123456789#*\">\
        <prompt stoponerror=\"yes\"\
        baseurl=\"/home/suliman_shah/mediaserver/trunk/qa/sipp/sound/\"\
        locale=\"en_US\" offset=\"50\" gain=\"0\" rate=\"0\"\
        delay=\"500\" duration=\"infinite\" repeat=\"4\">\
        <audio url=\"11.wav\" encoding=\"ulaw\"/>\
        <variable type=\"dig\" subtype=\"ndn\" value=\"3014170700\"/>\
        </prompt>\
        </playrecord>\
        </request>\
        </MediaServerControl>"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   ;

/******************************************************************************
** Function Name:
**          UT_Test_Mscml_Play_Events_Registerar
**
** Parameters:
**        char* pcMyString_i
**           type: input
**           detail : this is a C style NULL terminated char array whose
**                  length is to be calculated
**
**        BOOL  bLogEnable_i
**            type: input
**            detail: This is a flag indicating whether logging should
**                  be done inside this function or not
**
**
** Return:
**      An integer specifying the size in characters of pcMyString_i
**
**
** Additional Details:
**      Any additional details about the function may be added here
**      if Required
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
int execute_test( void* pvMSContext_i )
{
	xmlDocPtr docPtr;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	docPtr = xmlParseMemory( RT2_XFile, strlen( RT2_XFile ) );
	if( docPtr == NULL )
	{
		fprintf( stderr, "xmlParseMemory Cannot Parse the XML File\n" );
		return (0);
	}
	//xmlDocDump(stdout,docPtr);

	t_MsEvent oPSMInputEvent;
	oPSMInputEvent.pvCtx = pvMSContext_i;
	oPSMInputEvent.eType = MSCML_REQUEST;
	oPSMInputEvent.uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile = docPtr;

	eSMRT2CurrentState = RT2_W4Start_Play;
	nRepeat = 5;
	if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( g_pvUTContext, MSCML_OP_EVENT, &oPSMInputEvent, sizeof (t_MsEvent)))
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (0);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
	return (1);
}

/******************************************************************************
** Function Name:
**          CalculateStringLength
**
** Parameters:
**        char* pcMyString_i
**           type: input
**           detail : this is a C style NULL terminated char array whose
**                  length is to be calculated
**
**        BOOL  bLogEnable_i
**            type: input
**            detail: This is a flag indicating whether logging should
**                  be done inside this function or not
**
**
** Return:
**      An integer specifying the size in characters of pcMyString_i
**
**
** Additional Details:
**      Any additional details about the function may be added here
**      if Required
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
// MSCML_PLAY_RES
void UT_Test_Mscml_Play_MSCML_PLAY_RES_Handler( void* pvMSContext_i, void* pvParseXMLPContext_i, void* pvEventHandle_i, void* pvEventPayload_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n\n\n" );
	t_MsEvent* poPSMOutputEvent;
	poPSMOutputEvent = (t_MsEvent*) pvEventPayload_i;
	//Testing Lines*~*~*~*~*~**~*~*~**~*~*~*~*~**~*~*~*~**~*~*~*~*~*~**~*~*~*~*~**~*~*~**~*~*
	if( eSMRT2CurrentState == RT2_W4Mscml_Response && poPSMOutputEvent->eType == MSCML_RESPONSE && nRepeat == 0 )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "UNIT TEST SUCCESSFUL\n" );
	}
	else
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "UNIT TEST2 FAILED: nRepeat = %d\n", nRepeat );
		exit( 0 );
	}
	xmlDocDump( stdout, poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse );
	printf( "\n\n\n" );

	switch( poPSMOutputEvent->eType )
	{
		case MSCML_RESPONSE:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, " MSCML_RESPONSE\n:" );
			//Memory CleanUp
			MOP_DestroySM( poPSMOutputEvent->pvCtx );
			exit( 0 );
			break;

		case STOP_PLAY:
			break;

		case START_PLAY:
			break;

		case STOP_RECORDING:
			break;

		case START_RECORDING:
			break;

			/*case SM_START:
			{ break;
			}*/

		case SM_STOP:
		{ break;
		}

		case SM_STOPPED:
		{ break;
		}

		case MSCML_REQUEST:
		{ break;
		}

		case START_PLAY_ERROR:
		{ break;
		}

			/*case STOP_PLAY_ERROR:
			{break;
			}*/

		case PLAY_ENDED:
		{ break;
		}

			/*case PLAY_STARTED:
			{break;
			}*/

		case DTMF_KEY_PRESSED:
		{ break;
		}

		case RETURN_KEY_PRESSED:
		{ break;
		}

		case ESCAPE_KEY_PRESSED:
		{ break;
		}

		case FF_KEY_PRESSED:
		{ break;
		}

		case RW_KEY_PRESSED:
		{ break;
		}

		case SILENCE_DETECTED:
		{ break;
		}

		case SOUND_DETECTED:
		{ break;
		}

		case START_RECORDING_ERROR:
		{ break;
		}

		case STOP_RECORDING_ERROR:
		{ break;
		}

		case RECORDING_STOPPED:
		{ break;
		}

		case RECORDING_DURATION_TIMER_EXPIRED:
		{ break;
		}

		case RECORDING_SILENCE_TIMER_EXPIRED:
		{ break;
		}

		case PLAY_DURATION_TIMER_EXPIRED:
		{ break;
		}

		case INTER_SEQ_TIMER_EXPIRED:
		{ break;
		}

		case COLLECTION_TIMER_EXPIRED:
		{ break;
		}

		case GET_QUARANTINE:

			break;

		case CLEAR_QUARANTINE:
			break;
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
}

/******************************************************************************
** Function Name:
**          CalculateStringLength
**
** Parameters:
**        char* pcMyString_i
**           type: input
**           detail : this is a C style NULL terminated char array whose
**                  length is to be calculated
**
**        BOOL  bLogEnable_i
**            type: input
**            detail: This is a flag indicating whether logging should
**                  be done inside this function or not
**
**
** Return:
**      An integer specifying the size in characters of pcMyString_i
**
**
** Additional Details:
**      Any additional details about the function may be added here
**      if Required
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
//PLAYER_REQ
void UT_Player_PLAYER_REQ_Handler( void* pvMSContext_i, void* pvParseXMLContext_i, void* pvEventHandle_i, void* pvEventPayload_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	t_MsEvent* poPSMInputEvent;
	poPSMInputEvent = (t_MsEvent*) pvEventPayload_i;

	t_MsEvent* poPlayerInputEvent;
	poPlayerInputEvent = (t_MsEvent*) pvEventPayload_i;

	switch( poPlayerInputEvent->eType )
	{
		case START_PLAY:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "START_PLAY START_PLAY START_PLAY START_PLAY.\n" );
			if( MS_ERR_NONE != Generate_PLAY_ENDED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
				return;
			}
			break;

		case STOP_PLAY:
			break;

		case MSCML_RESPONSE:

			break;

		case STOP_RECORDING:
			if( MS_ERR_NONE != Generate_RECORDING_STOPPED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Generate_RECORDING_STOPPED_For_Mscml_Play failed.\n" );
				return;
			}
			break;

		case START_RECORDING:
			break;

			/*case SM_START:
			{ break;
			}*/

		case SM_STOP:
		{ break;
		}

		case SM_STOPPED:
		{ break;
		}

		case MSCML_REQUEST:
		{ break;
		}

		case START_PLAY_ERROR:
		{ break;
		}

			/*case STOP_PLAY_ERROR:
			{break;
			}*/

		case PLAY_ENDED:
		{ break;
		}

			/*case PLAY_STARTED:
			{break;
			}*/

		case DTMF_KEY_PRESSED:
		{ break;
		}

		case RETURN_KEY_PRESSED:
		{ break;
		}

		case ESCAPE_KEY_PRESSED:
		{ break;
		}

		case FF_KEY_PRESSED:
		{ break;
		}

		case RW_KEY_PRESSED:
		{ break;
		}

		case SILENCE_DETECTED:
		{ break;
		}

		case SOUND_DETECTED:
		{ break;
		}

		case START_RECORDING_ERROR:
		{ break;
		}

		case STOP_RECORDING_ERROR:
		{ break;
		}

		case RECORDING_STOPPED:
		{ break;
		}

		case RECORDING_DURATION_TIMER_EXPIRED:
		{ break;
		}

		case RECORDING_SILENCE_TIMER_EXPIRED:
		{ break;
		}

		case PLAY_DURATION_TIMER_EXPIRED:
		{ break;
		}

		case INTER_SEQ_TIMER_EXPIRED:
		{ break;
		}

		case COLLECTION_TIMER_EXPIRED:
		{ break;
		}

		case GET_QUARANTINE:

			break;

		case CLEAR_QUARANTINE:
			break;
	}
	// Repeat Test 2
	if( poPlayerInputEvent->eType == START_PLAY && nRepeat >= 0 && eSMRT2CurrentState == RT2_W4Start_Play )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "PLAY STARTED FOR nRepeat ==%d\n", nRepeat );
		if( nRepeat == 1 )
		{
			eSMRT2CurrentState = RT2_W4Mscml_Response;
		}
		nRepeat = nRepeat - 1;
	}
	else
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "UNIT TEST2 FAILED nRepeat = %d.\n", nRepeat );
		//exit( 0 );
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
}

