#include "unit_tests.h"
// Check the State machine for the sequence MSCML_REQUEST , SM_STOP, SM_START and then MSCML_REQUESTs
//Unit Test State Machine States(UTSM_States)
enum e_SeqTest1States
{
	RT1_W4Mscml_Response,
};

enum e_SeqTest1States eST1CurrentState;

// Check for seq of inputs
char ST1_XFile[] = "<?xml version=\"1.0\"?>\
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
	t_MsEvent oPSMInputEvent;
	oPSMInputEvent.pvCtx = poPSMInputEvent->pvCtx;
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
	if( pvMSContext_i == NULL || poPSMOutputEvent->eType.oMSCMLResponseParams.xdpMSCMLResponse == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Failed: NULL Valued Argument.\n" );
		return;
	}
	xmlDocDump( stdout, poPSMOutputEvent->eType.oMSCMLResponseParams.xdpMSCMLResponse );
	printf( "\n\n\n" );

	switch( poPSMOutputEvent->ePSMOEventTypes )
	{
		case MSCML_RESPONSE:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " MSCML_RESPONSE\n:" );
			break;

		case SM_STOPPED:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "PLAY_ENDED\n:" );
			break;

		case NONE:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " NONE\n:" );
			break;

		case STOP_RECORDING:
			break;

		case START_RECORDING:
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
	//t_PlayerInputEvent structure for the Player module
	t_PlayerInputEvent* poPlayerInputEvent;
	poPlayerInputEvent = (t_PlayerInputEvent*) pvEventPayload_i;

	switch( poPlayerInputEvent->ePlayerIEType )
	{
		case START_PLAY:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "START_PLAY START_PLAY START_PLAY START_PLAY.\n" );
			//check the second arguemnt, its of t_MsEvent type
			//it should not be of the type t_PlayerInputEvent
			if( MS_ERR_NONE != Generate_PLAY_STARTED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
				return;
			}
			//check the second arguemnt, its of t_MsEvent type
			//it should not be of the type t_PlayerInputEvent
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
			oPSMInputEvent.ePlaySMInputEventType = MSCML_REQUEST;
			oPSMInputEvent.uEventParams.oMSCMLRequestParams.pxdMSCMLPlayRequestFile = docPtr;
			if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( pvMSContext_i, MSCML_OP_EVENT, &oPSMInputEvent, sizeof (t_MsEvent)))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
				return;
			}
			break;

		case STOP_PLAY:
			//check the second arguemnt, its of t_MsEvent type
			//it should not be of the type t_PlayerInputEvent
			//
			if( MS_ERR_NONE != Generate_PLAY_STOPED_For_Mscml_Play( pvMSContext_i, poPSMInputEvent ))
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
				return;
			}
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
	oPlaySMInputEvent.ePlaySMInputEventType = SM_STOP;
	oPlaySMInputEvent.pvCtx = i_poPlaySMIETP->poMscmlPlayContext;
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
	oPlaySMInputEvent.ePlaySMInputEventType = SM_START;
	oPlaySMInputEvent.pvCtx = i_poPlaySMIETP->poMscmlPlayContext;
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
	oPlaySMInputEvent.ePlaySMInputEventType = MSCML_REQUEST;
	oPlaySMInputEvent.pvCtx = i_poPlaySMIETP->poMscmlPlayContext;
	nMSEvtSysIEValue = FW_EvtSysSendIntEvt( pvAFWContext_i, MSCML_OP_EVENT, &oPlaySMInputEvent, sizeof (t_MsEvent));
	if( AMPS_SUCCESS != nMSEvtSysIEValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (MS_ERR_FRAMEWORK);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	return (MS_ERR_NONE);
}

