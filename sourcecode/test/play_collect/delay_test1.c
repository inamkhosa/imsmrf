// For Delay >0
#include "unit_tests.h"
//Unit Test State Machine States(UTSM_States)
enum e_SMDelayTest1States
{
	DET1_W4Start_Play,
	DET1_W4Mscml_Response,
};
int nDelayStart;
int nDelayStop;
int nStarPlayCount;
char ppc_PlayRetCode[][100] = {
	"MS_ERR_NONE",
	"PLAY_ERR_INSUFFICIANT_MEMORY",
	"MS_ERR_INVALID_HANDLE",
	"MS_ERR_PRE_COND_FAILED",
	"MS_ERR_POST_COND_FAILED"
};

enum e_SMDelayTest1States eSMDET1CurrentState;
char DET1_XFile[] = "<?xml version=\"1.0\"?>\
        <MediaServerControl version=\"1.0\">\
        <request>\
        <playcollect id=\"5556123\" maxdigits=\"6\"\
        firstdigittimer=\"1000\"\
        interdigittimer=\"500\" extradigittimer=\"1000\"\
        interdigitcriticaltimer=\"1000\" returnkey=\"#\" escapekey=\"*\"\
        cleardigits=\"no\" barge=\"yes\" maskdigits=\"no\">\
        <prompt stoponerror=\"yes\"\
        baseurl=\"/home/suliman_shah/mediaserver/trunk/qa/sipp/non-conference/play/\"\
        offset=\"50\" delay=\"500\" duration=\"infinite\" repeat=\"2\">\
            <audio url=\"file.wav\"/>\
        </prompt>\
        </playcollect>\
        </request>\
        </MediaServerControl>"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  ;

int execute_test( void* pvMSContext_i )
{
	xmlDocPtr docPtr;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	docPtr = xmlParseMemory( DET1_XFile, strlen( DET1_XFile ) );
	if( docPtr == NULL )
	{
		fprintf( stderr, "xmlParseMemory Cannot Parse the XML File\n" );
		return (0);
	}
	//xmlDocDump(stdout,docPtr);
	t_MsEvent* poPSMInputEvent = (t_MsEvent*)pvMSContext_i;
	t_MsEvent oPSMInputEvent;
	oPSMInputEvent.pvCtx = pvMSContext_i; //poPSMInputEvent->pvCtx;
	oPSMInputEvent.eType = MSCML_REQUEST;
	//void* pvMSCMLContext;
	//MOP_CreateSM( MOT_PLAYCOLLECT, g_nMscmlPlayTraceID, g_pvUTContext, &pvMSCMLContext );
	//oPSMInputEvent.pvCtx = pvMSCMLContext;
	oPSMInputEvent.uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile = docPtr;
	eSMDET1CurrentState = DET1_W4Start_Play;
	nStarPlayCount = 0;
	if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( g_pvUTContext, MSCML_OP_EVENT, &oPSMInputEvent, sizeof (t_MsEvent)))
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (0);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	return (1);
}

// MSCML_PLAY_RES
void UT_Test_Mscml_Play_MSCML_PLAY_RES_Handler( void* pvMSContext_i, void* pvParseXMLPContext_i, void* pvEventHandle_i, void* pvEventPayload_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n\n\n" );
	t_MsEvent* poPSMOutputEvent;
	poPSMOutputEvent = (t_MsEvent*) pvEventPayload_i;
	//Testing Lines*~*~*~*~*~**~*~*~**~*~*~*~*~**~*~*~*~**~*~*~*~*~*~**~*~*~*~*~**~*~*~**~*~*
	nDelayStop = FW_GetCurrentTimeInMilliSec( pvMSContext_i );
	if( eSMDET1CurrentState == DET1_W4Mscml_Response && poPSMOutputEvent->eType == MSCML_RESPONSE )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "DELAY UNIT TEST1 SUCCESSFUL %d\n", nDelayStop-nDelayStart );
	}
	else
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "DELAY UNIT TEST1 FAILED " );
	}
	//*~*~*~*~*~**~*~*~**~*~*~*~*~**~*~*~*~**~*~*~*~*~*~**~*~*~*~*~**~*~*~**~*~**~*~*~*~*~*~*~*
	xmlDocDump( stdout, poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse );
	printf( "\n\n\n" );

	switch( poPSMOutputEvent->eType )
	{
		case MSCML_RESPONSE:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " MSCML_RESPONSE\n:" );
			//Memory CleanUp
			PlayCollect_UnitTestCleanUp( poPSMOutputEvent );
			break;

		case START_PLAY:
			break;

		case STOP_PLAY:
			break;

		case STOP_RECORDING:
			break;

		case START_RECORDING:
			break;

		case GET_QUARANTINE:

			break;

		case CLEAR_QUARANTINE:

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
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
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
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	t_MsEvent* poPSMInputEvent;
	poPSMInputEvent = (t_MsEvent*) pvEventPayload_i;

	t_MsEvent* poPlayerInputEvent;
	poPlayerInputEvent = (t_MsEvent*) pvEventPayload_i;

	switch( poPlayerInputEvent->eType )
	{
		case START_PLAY:
			free( poPlayerInputEvent->uEventParams.oStartPlayParams.cpFileName );
			if( nStarPlayCount == 0 )
			{
				nDelayStart = FW_GetCurrentTimeInMilliSec( pvMSContext_i );
				eSMDET1CurrentState = DET1_W4Mscml_Response;
			}
			if( nStarPlayCount == 1 )
			{
				nDelayStop = FW_GetCurrentTimeInMilliSec( pvMSContext_i );
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "UNIT TEST DLEAY VALUE IS : %d\n", nDelayStop-nDelayStart );
				//exit(0);
			}
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "START_PLAY START_PLAY START_PLAY START_PLAY.\n" );
			nStarPlayCount = nStarPlayCount + 1;
			// dummy PLAY_ENDED afer no time
			//check the second arguemnt, its of t_MsEvent type
			//it should not be of the type t_PlayerInputEvent
			if( MS_ERR_NONE != Generate_PLAY_ENDED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))   //, PLAY_ENDED))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Generate_PLAY_STOPED_For_Mscml_Play failed.\n" );
				return;
			}
			break;

		case STOP_PLAY:
			//check the second arguemnt, its of t_MsEvent type
			//it should not be of the type t_PlayerInputEvent
			if( MS_ERR_NONE != Generate_PLAY_ENDED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Generate_PLAY_STOPED_For_Mscml_Play failed.\n" );
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

