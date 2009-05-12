#include "unit_tests.h"
// Test For escapekey
//Unit Test State Machine States(UTSM_States)
enum e_EscapeKeyTest1States
{
	RT1_W4Mscml_Response
};
enum e_EscapeKeyTest1States eRT1CurrentState;
char RT1_XFile[] = "<?xml version=\"1.0\"?>\
        <MediaServerControl version=\"1.0\">\
        <request>\
        <playrecord id=\"5556123\"\
        recurl=\"/home/suliman_shah/mediaserver/trunk/qa/sipp/non-conference/play/name.wav\"\
        recencoding=\"msgsm\"\
        initsilence=\"500\" endsilence=\"400\" duration=\"300\"\
        barge=\"yes\"\
        beep=\"no\"\
        mode=\"overwrite\"\
        cleardigits=\"no\"\
        escapekey=\"*\"\
        recstopmask=\"0123456789#*\">\
        <prompt stoponerror=\"yes\"\
        baseurl=\"/home/suliman_shah/mediaserver/trunk/qa/sipp/sound/\"\
        locale=\"en_US\" offset=\"50\" gain=\"0\" rate=\"0\"\
        delay=\"500\" duration=\"infinite\" repeat=\"1\">\
        <audio url=\"11.wav\" encoding=\"ulaw\"/>\
        <audio url=\"12.wav\" encoding=\"ulaw\"/>\
        <variable type=\"dig\" subtype=\"ndn\" value=\"3014170700\"/>\
        </prompt>\
        </playrecord>\
        </request>\
        </MediaServerControl>"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      ;

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
int execute_test( void* pvMSContext_i )
{
	xmlDocPtr docPtr;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	docPtr = xmlParseMemory( RT1_XFile, strlen( RT1_XFile ) );
	if( docPtr == NULL )
	{
		fprintf( stderr, "xmlParseMemory Cannot Parse the XML File\n" );
		return (0);
	}
	t_MsEvent oPSMInputEvent;
	oPSMInputEvent.pvCtx = pvMSContext_i;
	oPSMInputEvent.eType = MSCML_REQUEST;

	oPSMInputEvent.uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile = docPtr;
	eRT1CurrentState = RT1_W4Mscml_Response;
	if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( g_pvUTContext, MSCML_OP_EVENT, &oPSMInputEvent, sizeof (t_MsEvent)))
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (0);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	return (1);
}

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
// MSCML_PLAY_RES
void UT_Test_Mscml_Play_MSCML_PLAY_RES_Handler( void* pvMSContext_i, void* pvParseXMLPContext_i, void* pvEventHandle_i, void* pvEventPayload_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n\n\n" );

	//have changed this from t_MsEvent to t_MsEvent
	t_MsEvent* poPSMOutputEvent;
	poPSMOutputEvent = (t_MsEvent*) pvEventPayload_i;
	if( pvMSContext_i == NULL || poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Failed: NULL Valued Argument.\n" );
		return;
	}
	if( eRT1CurrentState == RT1_W4Mscml_Response && poPSMOutputEvent->eType == MSCML_RESPONSE )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "SUCCESS: UNIT TEST MSCML_RESPONSE\n:" );
	}
	else
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "UNIT TEST FAILED\n:" );
	}
	xmlDocDump( stdout, poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse );
	printf( "\n\n\n" );

	switch( poPSMOutputEvent->eType )
	{
		case MSCML_RESPONSE:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " MSCML_RESPONSE\n:" );

			//Memory CleanUp
			MOP_DestroySM( poPSMOutputEvent->pvCtx );
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
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
}

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
//PLAYER_REQ
void UT_Player_PLAYER_REQ_Handler( void* pvMSContext_i, void* pvParseXMLContext_i, void* pvEventHandle_i, void* pvEventPayload_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	printf( "PLAYER_REQPLAYER_REQPLAYER_REQPLAYER_REQPLAYER_REQPLAYER_REQ:\n" );
	t_MsEvent* poPSMInputEvent;
	poPSMInputEvent = (t_MsEvent*) pvEventPayload_i;
	if( pvMSContext_i == NULL || poPSMInputEvent == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Failed: NULL Valued Argument.\n" );
		return;
	}
	//t_PlayerInputEvent structure for the Player module
	//have changed this from t_MsEvent to t_MsEvent

	t_MsEvent* poPlayerInputEvent;
	poPlayerInputEvent = (t_MsEvent*) pvEventPayload_i;
	if( eRT1CurrentState == RT1_W4Mscml_Response )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "TEST FAILED\n" );
	}

	switch( poPlayerInputEvent->eType )
	{
		case START_PLAY:

			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "START_PLAYSTART_PLAYSTART_PLAYSTART_PLAYSTART_PLAYSTART_PLAY.\n" );
			if( MS_ERR_NONE != Generate_DTMF_KEY_PRESSED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Generate_DTMF_KEY_PRESSED_For_Mscml_Play failed.\n" );
				return;
			}
			break;

		case STOP_PLAY:
			//check the second arguemnt, its of t_MsEvent type
			//it should not be of the type t_PlayerInputEvent
			if( MS_ERR_NONE != Generate_PLAY_ENDED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Generate_PLAY_ENDED_For_Mscml_Play failed.\n" );
				return;
			}
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
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
}

