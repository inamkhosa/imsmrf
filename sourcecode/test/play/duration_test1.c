// For Duration >0
#include "unit_tests.h"
#include "mscml_encoder/mscml_encoder_api.h"
//Unit Test State Machine States(UTSM_States)
enum e_SMDurationTest1States
{
	DT1_W4Start_Play,
	DT1_W4Mscml_Response,
};
enum e_SMDurationTest1States eSMDT1CurrentState;
char DT1_XFile[] = "<?xml version=\"1.0\"?>\
<MediaServerControl version=\"1.0\">\
    <request>\
        <play id=\"332985001\">\
            <prompt stoponerror=\"yes\"\
                baseurl=\"/home/suliman_shah/mediaserver/trunk/qa/sipp/non-conference/play/\"\
                locale=\"en_US\" offset=\"0\" gain=\"0\" rate=\"0\"\
                delay=\"0\" duration=\"30\" repeat=\"infinite\">\
                <audio url=\"file1.wav\" encoding=\"ulaw\"/>\
                <variable type=\"dig\" subtype=\"ndn\" value=\"3014170700\"/>\
            </prompt>\
        </play>\
    </request>\
</MediaServerControl>"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       ;
int nDurationStart;
int nDurationStop;
// int g_nMscmlPlayTraceID  =  0;
// static int g_nTestAPPTraceID = 0;
// static int g_nUnitTestTraceID = 0;
// static void* g_pvAFWContext = NULL;
// static void* g_pvUTContext = NULL;
// xmlDocPtr   gpxdPlayReqFile=NULL;

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
struct _TestAPPIntEventDummy1
{
	int nVar1;
};
typedef struct _TestAPPIntEventDummy1 t_TestAPPIntEventDummy1;
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

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering: %p\n", pvMSContext_i );
	docPtr = xmlParseMemory( DT1_XFile, strlen( DT1_XFile ) );
	if( docPtr == NULL )
	{
		fprintf( stderr, "xmlParseMemory Cannot Parse the XML File\n" );
		return (0);
	}
	t_MsEvent* poPSMInputEvent; //= (t_MsEvent*)pvMSContext_i;
	t_MsEvent oPSMInputEvent;
	oPSMInputEvent.pvCtx = pvMSContext_i; //poPSMInputEvent->pvCtx;
	oPSMInputEvent.eType = MSCML_REQUEST;
	//void* pvMSCMLContext;
	//Mscml_Play_CreateContext( &pvMSCMLContext, g_nMscmlPlayTraceID, g_pvUTContext );
	//oPSMInputEvent.pvCtx = pvMSCMLContext;
	oPSMInputEvent.uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile = docPtr;
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
void UT_Test_Mscml_Play_MSCML_PLAY_RES_Handler( void* pvMSContext_i, void* pvParseXMLPContext_i, void* pvEventHandle_i, void* pvEventPayload_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n\n\n" );
	t_MsEvent* poPSMOutputEvent;
	poPSMOutputEvent = (t_MsEvent*) pvEventPayload_i;
	nDurationStop = FW_GetCurrentTimeInMilliSec( pvMSContext_i );
	if( eSMDT1CurrentState == DT1_W4Mscml_Response && poPSMOutputEvent->eType == MSCML_RESPONSE )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "DURATION UNIT TEST1 SUCCESSFUL %d\n", nDurationStop-nDurationStart );
	}
	else
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Duration UNIT TEST1 FAILED " );
	}
	xmlDocDump( stdout, poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse );
	printf( "\n\n\n" );

	/*char* pcDuration_o;
	GetPlayRecordRequestDuration(poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse, pcDuration_o );
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "pcDuration_o: %s,atoi(pcDuration_o): %d",pcDuration_o,atoi(pcDuration_o));
	exit(0);
	*/
	switch( poPSMOutputEvent->eType )
	{
		case MSCML_RESPONSE:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, " MSCML_RESPONSE\n:" );
			//Memory CleanUp

			Play_UnitTestCleanUp( poPSMOutputEvent );

			exit( 0 );
			break;

		case SM_STOPPED:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "PLAY_ENDED\n:" );
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
	//t_MsEvent structure for the Player module
	t_MsEvent* poPlayerInputEvent;
	poPlayerInputEvent = (t_MsEvent*) pvEventPayload_i;

	switch( poPlayerInputEvent->eType )
	{
		case START_PLAY:

			xmlFree( poPlayerInputEvent->uEventParams.oStartPlayParams.cpFileName );

			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "START_PLAY START_PLAY START_PLAY START_PLAY.\n" );
			//check the second arguemnt, its of t_MsEvent type
			//it should not be of the type t_MsEvent
			if( MS_ERR_NONE != Generate_PLAY_STARTED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
				return;
			}
			if( NULL == (void*)FW_StartTimer( pvMSContext_i, MSCMLPLAY_DURATION_TIMER_VALUE, UT_MSCMLPLAY_PlayerTimerExpired, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_StartTimer failed.\n" );
				return;
			}
			break;

		case STOP_PLAY:
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
	// Duration Test 1
	nDurationStart = FW_GetCurrentTimeInMilliSec( pvMSContext_i );
	if( poPlayerInputEvent->eType == START_PLAY )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "DURATION UNIT TEST1 START_PLAY\n" );
		eSMDT1CurrentState = DT1_W4Mscml_Response;
	}
	else
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "DURATION UNIT TEST1 FAILED\n" );
		//exit(0);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
}

