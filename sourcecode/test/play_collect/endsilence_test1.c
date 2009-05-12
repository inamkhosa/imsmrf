#include "unit_tests.h"
// Test For endsilencetimer attribute
//Unit Test State Machine States(UTSM_States)
enum e_EndSilenceTest1States
{
	DTMFKT1_W4Start_Recording,
	DTMFKT1_W4Stop_Recording,
	DTMFKT1_W4Mscml_Response
};
enum e_EndSilenceTest1States eRT1CurrentState;
char RT1_XFile[] = "<?xml version=\"1.0\"?>\
        <MediaServerControl version=\"1.0\">\
        <request>\
        <playrecord id=\"5556123\"\
        recurl=\"file:////nfs.example.com/rec/name.wav\"\
        recencoding=\"msgsm\"\
        initsilence=\"5000\" endsilence=\"3000\" duration=\"30000\"\
        barge=\"yes\"\
        beep=\"yes\"\
        mode=\"overwrite\"\
        cleardigits=\"no\"\
        escapekey=\"*\"\
        recstopmask=\"0123456789#*\">\
        <prompt stoponerror=\"yes\"\
        baseurl=\"file:////var/mediaserver/prompts/\"\
        locale=\"en_US\" offset=\"50\" gain=\"0\" rate=\"0\"\
        delay=\"500\" duration=\"infinite\" repeat=\"2\">\
        <audio url=\"num_dialed.raw\" encoding=\"ulaw\"/>\
        <audio url=\"num_dialed.raw\" encoding=\"ulaw\"/>\
        <variable type=\"dig\" subtype=\"ndn\" value=\"3014170700\"/>\
        </prompt>\
        </playrecord>\
        </request>\
        </MediaServerControl>"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     ;
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
	eRT1CurrentState = DTMFKT1_W4Start_Recording;
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
	if( eRT1CurrentState == DTMFKT1_W4Mscml_Response && poPSMOutputEvent->eType == MSCML_RESPONSE )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "SUCCESS: END SILENCE TIMER UNIT TEST MSCML_RESPONSE\n:" );
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
			MOP_DestroySM( poPSMOutputEvent );
			break;

		case STOP_PLAY:
			break;

		case START_PLAY:
			break;

		case STOP_RECORDING:
			break;

		case START_RECORDING:
			break;
			/*case SM_STOPPED:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "PLAY_ENDED\n:" );
			break;

			case NONE:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " NONE\n:" );
			break;

			case START_TIMER:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "START_TIMER\n:" );
			break;
			*/
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

	switch( poPlayerInputEvent->eType )
	{
		case START_PLAY:

			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "START_PLAYSTART_PLAYSTART_PLAYSTART_PLAYSTART_PLAYSTART_PLAY.\n" );
			//it should not be of the type t_PlayerInputEvent
			if( MS_ERR_NONE != Generate_PLAY_ENDED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))   //, PLAY_ENDED))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Generate_PLAY_ENDED_For_Mscml_Play failed.\n" );
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
			if( eRT1CurrentState == DTMFKT1_W4Stop_Recording )
			{
				eRT1CurrentState = DTMFKT1_W4Mscml_Response;
			}
			else
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "TEST FAILED.\n" );
				exit( 0 );
			}
			if( MS_ERR_NONE != Generate_RECORDING_STOPPED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Generate_RECORDING_STOPPED_For_Mscml_Play failed.\n" );
				return;
			}
			break;

		case START_RECORDING:
			if( eRT1CurrentState == DTMFKT1_W4Start_Recording )
			{
				eRT1CurrentState = DTMFKT1_W4Stop_Recording;
			}
			else
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "TEST FAILED.\n" );
				exit( 0 );
			}
			if( MS_ERR_NONE != Generate_SOUND_DETECTED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Generate_RECORDING_STOPPED_For_Mscml_Play failed.\n" );
				return;
			}
			if( MS_ERR_NONE != Generate_SILENCE_DETECTED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Generate_RECORDING_STOPPED_For_Mscml_Play failed.\n" );
				return;
			}
			break;
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
}

