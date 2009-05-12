#include "unit_tests.h"
// Check the State machine for the sequence MSCML_REQUEST , SM_STOP, SM_START and then MSCML_REQUESTs
//Unit Test State Machine States(UTSM_States)
enum e_SeqTest1States
{
	RT1_W4Mscml_Response,
};
// Check for seq of inputs
char ST1_XFile[] = "<?xml version=\"1.0\"?>\
<MediaServerControl version=\"1.0\">\
    <request>\
        <play id=\"332985001\">\
            <prompt stoponerror=\"yes\"\
                baseurl=\"/home/suliman_shah/mediaserver/trunk/qa/sipp/non-conference/play/\"\
                locale=\"en_US\" offset=\"0\" gain=\"0\" rate=\"0\"\
                delay=\"100\" duration=\"10000\" repeat=\"2\">\
                <audio url=\"file1.wav\" encoding=\"ulaw\"/>\
		<audio url=\"file2.wav\" encoding=\"ulaw\"/>\
                <variable type=\"dig\" subtype=\"ndn\" value=\"3014170700\"/>\
	    </prompt>\
        </play>\
    </request>\
</MediaServerControl>"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              ;
enum e_SeqTest1States eST1CurrentState;
int execute_test( void* pvMSContext_i )
{
	xmlDocPtr docPtr;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	docPtr = xmlParseMemory( ST1_XFile, strlen( ST1_XFile ) );
	if( docPtr == NULL )
	{
		fprintf( stderr, "xmlParseMemory Cannot Parse the XML File\n" );
		return (0);
	}
	//xmlDocDump(stdout,docPtr);
	t_MsEvent* poPSMInputEvent; //= (t_MsEvent*)pvMSContext_i;
	t_MsEvent oPSMInputEvent;
	oPSMInputEvent.pvCtx = pvMSContext_i; //poPSMInputEvent->pvCtx;
	oPSMInputEvent.eType = MSCML_REQUEST;

	oPSMInputEvent.uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile = docPtr;
	eST1CurrentState = RT1_W4Mscml_Response;
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
**			UT_Test_Mscml_Play_Events_Registerar
**
** Parameters:
**		  char* pcMyString_i
**			 type: input
**			 detail : this is a C style NULL terminated char array whose
**				    length is to be calculated
**
**		  BOOL  bLogEnable_i
**			  type: input
**			  detail: This is a flag indicating whether logging should
**				    be done inside this function or not
**
**
** Return:
**		An integer specifying the size in characters of pcMyString_i
**
**
** Additional Details:
**		Any additional details about the function may be added here
**		if Required
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
	t_MsEvent* poPSMOutputEvent;
	poPSMOutputEvent = (t_MsEvent*) pvEventPayload_i;
	if( pvMSContext_i == NULL || poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Failed: NULL Valued Argument.\n" );
		return;
	}
	xmlDocDump( stdout, poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse );
	printf( "\n\n\n" );

	switch( poPSMOutputEvent->eType )
	{
		case MSCML_RESPONSE:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " MSCML_RESPONSE\n:" );
			exit( 0 );
			break;

		case SM_STOPPED:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "PLAY_ENDED\n:" );
			break;

			/*case SM_START:
			{ break;
			}*/

		case SM_STOP:
		{ break;
		}

		case MSCML_REQUEST:
		{ break;
		}

		case START_PLAY:
		{ break;
		}

		case STOP_PLAY:
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

		case STOP_RECORDING:
		{ break;
		}

		case START_RECORDING:
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
	// NO Post Condition
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
}

/******************************************************************************
** Function Name:
**			UT_Test_Mscml_Play_Events_Registerar
**
** Parameters:
**		  char* pcMyString_i
**			 type: input
**			 detail : this is a C style NULL terminated char array whose
**				    length is to be calculated
**
**		  BOOL  bLogEnable_i
**			  type: input
**			  detail: This is a flag indicating whether logging should
**				    be done inside this function or not
**
**
** Return:
**		An integer specifying the size in characters of pcMyString_i
**
**
** Additional Details:
**		Any additional details about the function may be added here
**		if Required
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
	xmlDocPtr docPtr;
	t_MsEvent* poPSMInputEvent;
	poPSMInputEvent = (t_MsEvent*) pvEventPayload_i;
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	docPtr = xmlParseMemory( ST1_XFile, strlen( ST1_XFile ) );
	if( docPtr == NULL )
	{
		fprintf( stderr, "xmlParseMemory Cannot Parse the XML File\n" );
		return;
	}
	if( pvMSContext_i == NULL || poPSMInputEvent == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Failed: NULL Valued Argument.\n" );
		return;
	}
	//t_MsEvent structure for the Player module
	t_MsEvent* poPlayerInputEvent;
	poPlayerInputEvent = (t_MsEvent*) pvEventPayload_i;

	switch( poPlayerInputEvent->eType )
	{
		case START_PLAY:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "START_PLAY START_PLAY START_PLAY START_PLAY.\n" );
			//check the second arguemnt, its of t_MsEvent type
			//it should not be of the type t_MsEvent
			if( MS_ERR_NONE != Generate_PLAY_STARTED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
				return;
			}
			//check the second arguemnt, its of t_MsEvent type
			//it should not be of the type t_MsEvent
			if( MS_ERR_NONE != Generate_PLAY_STOPED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))   //, PLAY_ENDED))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
				return;
			}
			if( MS_ERR_NONE != UT_Generate_SM_STOP_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))   //, PLAY_ENDED))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
				return;
			}
			if( MS_ERR_NONE != UT_Generate_SM_START_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))   //, PLAY_ENDED))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
				return;
			}
			// Generate MSCML_REQUEST Again
			t_MsEvent oPSMInputEvent;
			oPSMInputEvent.eType = MSCML_REQUEST;
			oPSMInputEvent.uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile = docPtr;
			if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( pvMSContext_i, MSCML_OP_EVENT, &oPSMInputEvent, sizeof (t_MsEvent)))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
				return;
			}
			break;

		case STOP_PLAY:
			//check the second arguemnt, its of t_MsEvent type
			//it should not be of the type t_MsEvent
			//
			if( MS_ERR_NONE != Generate_PLAY_STOPED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
				return;
			}
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

		case MSCML_RESPONSE:
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

		case STOP_RECORDING:
		{ break;
		}

		case START_RECORDING:
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
**          Generate_PLAY_STARTED_For_Mscml_Play
**
** Parameters:
**
**        void* pvMSContext_i
**           type: input
**           detail : Modified MS's Context
**
**        t_MsEvent *poPSMIETP_i
**            type: input
**            Its a Play State Machine Input Event type structure
**
**
**
** Return:
**      SUCCESS or Failure Error Code
**
**
** Additional Details:
**      This function generates PLAY_STARTED event for mscml_play
**
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
e_MsRetCode UT_Generate_SM_STOP_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	t_MsEvent oPlaySMInputEvent;
	int nMSEvtSysIEValue;
	oPlaySMInputEvent.eType = SM_STOP;
	oPlaySMInputEvent.pvCtx = i_poPlaySMIETP->pvCtx;
	nMSEvtSysIEValue = FW_EvtSysSendIntEvt( pvAFWContext_i, MSCML_OP_EVENT, &oPlaySMInputEvent, sizeof (t_MsEvent));
	if( AMPS_SUCCESS != nMSEvtSysIEValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (MS_ERR_FRAMEWORK);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	return (MS_ERR_NONE);
}

/******************************************************************************
** Function Name:
**          Generate_PLAY_STARTED_For_Mscml_Play
**
** Parameters:
**
**        void* pvMSContext_i
**           type: input
**           detail : Modified MS's Context
**
**        t_MsEvent *poPSMIETP_i
**            type: input
**            Its a Play State Machine Input Event type structure
**
**
**
** Return:
**      SUCCESS or Failure Error Code
**
**
** Additional Details:
**      This function generates PLAY_STARTED event for mscml_play
**
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
e_MsRetCode UT_Generate_SM_START_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	t_MsEvent oPlaySMInputEvent;
	int nMSEvtSysIEValue;
	oPlaySMInputEvent.eType = SM_STOP; //SM_START;
	oPlaySMInputEvent.pvCtx = i_poPlaySMIETP->pvCtx;
	nMSEvtSysIEValue = FW_EvtSysSendIntEvt( pvAFWContext_i, MSCML_OP_EVENT, &oPlaySMInputEvent, sizeof (t_MsEvent));
	if( AMPS_SUCCESS != nMSEvtSysIEValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (MS_ERR_FRAMEWORK);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	return (MS_ERR_NONE);
}

/******************************************************************************
** Function Name:
**          Generate_PLAY_STARTED_For_Mscml_Play
**
** Parameters:
**
**        void* pvMSContext_i
**           type: input
**           detail : Modified MS's Context
**
**        t_MsEvent *poPSMIETP_i
**            type: input
**            Its a Play State Machine Input Event type structure
**
**
**
** Return:
**      SUCCESS or Failure Error Code
**
**
** Additional Details:
**      This function generates PLAY_STARTED event for mscml_play
**
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
e_MsRetCode Generate_MSCML_REQUEST_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	t_MsEvent oPlaySMInputEvent;
	int nMSEvtSysIEValue;
	oPlaySMInputEvent.eType = MSCML_REQUEST;
	oPlaySMInputEvent.pvCtx = i_poPlaySMIETP->pvCtx;
	nMSEvtSysIEValue = FW_EvtSysSendIntEvt( pvAFWContext_i, MSCML_OP_EVENT, &oPlaySMInputEvent, sizeof (t_MsEvent));
	if( AMPS_SUCCESS != nMSEvtSysIEValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (MS_ERR_FRAMEWORK);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	return (MS_ERR_NONE);
}

