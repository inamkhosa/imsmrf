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
#include "unit_tests.h"
struct _TestAPPIntEventDummy1
{
	int nVar1;
};
int g_nMscmlPlayTraceID = 0;
int g_nTestAPPTraceID = 0;
int g_nUnitTestTraceID = 0;
void* g_pvAFWContext = NULL;
void* g_pvUTContext = NULL;
xmlDocPtr gpxdPlayReqFile = NULL;
typedef struct _TestAPPIntEventDummy1 t_TestAPPIntEventDummy1;
int main( int argc, char** argv )
{
	void* pvAMPSContext = NULL;
	void* pvMSContext = NULL;
	void* pvMSSchInitHandle = NULL;

	//int nCounter = 0;
	printf( "Main: Entering.\n" );
	pvAMPSContext = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );
	if( NULL == pvAMPSContext )
	{
		printf( "FW_Init: FW_Init is failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	g_pvAFWContext = pvAMPSContext;
	pvMSContext = pvAMPSContext;
	g_pvUTContext = pvAMPSContext;
	FW_SetTraceMode( g_pvUTContext, AMPS_TRACE_MODE_BOTH );
	FW_SetTraceLevel( g_pvUTContext, 31 );
	FW_EnableLogging( g_pvUTContext, "testapp_log.txt" );
	FW_SetTraceForIOAgent( g_pvUTContext );
	g_nTestAPPTraceID = FW_GetTraceID( g_pvUTContext );
	g_nMscmlPlayTraceID = g_nTestAPPTraceID;
	g_nUnitTestTraceID = g_nTestAPPTraceID;
	FW_SetTraceForTraceID( g_pvUTContext, g_nTestAPPTraceID );
	FW_EvtSysSetEvtHandleValidationCallback( g_pvUTContext, TestAPP_EventHandleValidationCallback );

	pvMSSchInitHandle = FW_SchedInit( g_pvUTContext, MSCMLPLAY_MAX_INT_EVENTS, MSCMLPLAY_MAX_EXT_EVENTS, NULL, NULL, NULL, NULL );
	if( NULL == pvMSSchInitHandle )
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_SchedInit failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	void* pvCtx;

	MOP_CreateSM( MOT_PLAY, g_nMscmlPlayTraceID, g_pvUTContext, "", &pvCtx );

	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Mscml play state machine context %p.\n", pvCtx );

	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Frmework Context is:  %p.\n", g_pvUTContext );
	if( MS_ERR_FRAMEWORK == All_Events_Registerar( g_pvUTContext, pvCtx ))   //, NULL))
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	t_MsEvent oPSMInputEvent;
	oPSMInputEvent.eType = MSCML_REQUEST;
	//oPSMInputEvent.poPlayCtx.eCurrentState = 0;
	void* pvMscmlPlayContext = NULL;
	oPSMInputEvent.pvCtx = pvCtx;
	if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( g_pvUTContext, INIT_TESTS, &oPSMInputEvent, sizeof (t_MsEvent)))
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
	}
	printf( "Main: FW_Main called.\n" );
	FW_Main( g_pvUTContext );
	printf( "Main: Leaving.\n" );

	AMPS_SchedCleanup( g_pvUTContext, pvMSSchInitHandle );

	AMPS_Cleanup( pvAMPSContext );

	return (AMPS_SUCCESS);
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
int TestAPP_EventHandleValidationCallback( void* r_pvContext, int r_nEventID, void* r_pvEventPayload )
{
	return (AMPS_SUCCESS);
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
// First argument is Media Server context and the second is AMPS context
e_MsRetCode All_Events_Registerar( void* pvFWContext_i, void* pvCtx_i ) //, void* pvParseXMLContext_i)
{
//    int nMSAllEVTRegValue;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	//t_MsEvent* poEvent = pvMSEvent_i;
	//t_MscmlOpCtx* poCtx = (t_MscmlPlayContext*)poEvent->pvCtx;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Mscml play state machine context %p.\n", pvCtx_i );
	eRetCode = MOP_RegisterEvent( pvFWContext_i, pvCtx_i ); // , NULL);
	if( MS_ERR_FRAMEWORK == eRetCode )   // remove NULL argument
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Player_Events_Registerar failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	// For Unit Testing FROM HERE::::::::::::::::::::::::::::::::::::::::::::::::
	eRetCode = PlayUnitTest_Events_Registerar( pvCtx_i ); // , NULL);
	if( MS_ERR_FRAMEWORK == eRetCode )   // remove NULL argument
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "PlayUnitTest_Events_Registerar failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	//TILL HERE:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
	return (eRetCode);
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
e_MsRetCode PlayUnitTest_Events_Registerar( void* pvCtx_i )
{
	int nMSSysRegIEValue;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Mscml play state machine context %p.\n", pvCtx_i );
	// Register INIT_TESTS Event
	nMSSysRegIEValue = AMPS_EvtSysRegIntEvt( g_pvAFWContext, INIT_TESTS, PlayUnitTests_INIT_TESTS_Handler, pvCtx_i );
	if( AMPS_SUCCESS != nMSSysRegIEValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	// Register MSCML_PLAY_RES Event
	eRetCode = UT_Test_Mscml_Play_Events_Registerar( pvCtx_i ); //, NULL);
	if( MS_ERR_FRAMEWORK == eRetCode )   // remove NULL argument
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Test_Mscml_Play_Events_Registerar failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	// Register PLAYER_REQ Event
	eRetCode = UT_Player_Events_Registerar( pvCtx_i ); //, NULL);
	if( MS_ERR_FRAMEWORK == eRetCode )   // remove NULL argument
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Player_Events_Registerar failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
	return (eRetCode);
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
e_MsRetCode UT_Test_Mscml_Play_Events_Registerar( void* pvCtx_i ) //, void* pvParseXMLPContext_i)
{
	int nMSSysRegIERes;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Mscml play state machine context %p.\n", pvCtx_i );
	if( pvCtx_i == NULL )
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, " pvMSContext_i is NULL.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
		return (eRetCode);
	}
	//t_MsEvent* poPSMInputEvent;
	//poPSMInputEvent = (t_MsEvent*) pvEventPayload_i;

	//poPSMInputEvent->pvCtx = pvCtx_i;

	nMSSysRegIERes = AMPS_EvtSysRegIntEvt( g_pvAFWContext, MSCML_PLAY_RES, UT_Test_Mscml_Play_MSCML_PLAY_RES_Handler, pvCtx_i );
	if( AMPS_SUCCESS != nMSSysRegIERes )
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, " AMPS_EvtSysRegIntEvt for MSCML_PLAY_RES failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
	return (eRetCode);
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
e_MsRetCode UT_Player_Events_Registerar( void* pvCtx ) //, void* pvParseXMLPContext_i)
{
	int nMSSysRegIEValue;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	//t_MsEvent* poPSMInputEvent;
	//poPSMInputEvent = (t_MsEvent*) pvEventPayload_i;

	//poPSMInputEvent->pvCtx = pvCtx;

	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Mscml play state machine context %p.\n", pvCtx );
	if( pvCtx == NULL )
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "Failed: NULL Valued Argument.\n" );
		return (MS_ERR_FRAMEWORK);
	}
	nMSSysRegIEValue = AMPS_EvtSysRegIntEvt( g_pvAFWContext, PLAYER_REQ, UT_Player_PLAYER_REQ_Handler, pvCtx );
	if( AMPS_SUCCESS != nMSSysRegIEValue )
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, " failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
	return (eRetCode);
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
e_MsRetCode Generate_PLAY_STARTED_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	t_MsEvent oPlaySMInputEvent;
	int nMSEvtSysIEValue;
	oPlaySMInputEvent.eType = PLAY_ENDED;
	oPlaySMInputEvent.pvCtx = i_poPlaySMIETP->pvCtx;
	nMSEvtSysIEValue = FW_EvtSysSendIntEvt( pvAFWContext_i, MSCML_OP_EVENT, &oPlaySMInputEvent, sizeof (t_MsEvent));
	if( AMPS_SUCCESS != nMSEvtSysIEValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (MS_ERR_FRAMEWORK);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
	return (MS_ERR_NONE);
}

/******************************************************************************
** Function Name:
**          Generate_PLAY_STOPED_For_Mscml_Play
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
**      This function generates PLAY_STOPEDs event for mscml_play
**
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
int Generate_PLAY_STOPED_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP )
{
	int nMSEvtSysIEValue;
	t_MsEvent oPlaySMInputEvent;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	oPlaySMInputEvent.eType = PLAY_ENDED;
	oPlaySMInputEvent.pvCtx = i_poPlaySMIETP->pvCtx;
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	nMSEvtSysIEValue = FW_EvtSysSendIntEvt( pvAFWContext_i, MSCML_OP_EVENT, &oPlaySMInputEvent, sizeof (t_MsEvent));
	if( AMPS_SUCCESS != nMSEvtSysIEValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (MS_ERR_FRAMEWORK);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
	return (MS_ERR_NONE);
}

/******************************************************************************
** Function Name:
**          Generate_PLAY_STOPED_For_Mscml_Play
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
**      This function generates PLAY_STOPEDs event for mscml_play
**
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
int UT_MSCMLPLAY_PlayerTimerExpired( void* pvMSContext_i, void* pvTimerData_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MsEvent* poPSMInputEvent = (t_MsEvent*) pvTimerData_i;
	int nMSEvtSysIEValue;
	t_MsEvent oPSMInputEvent;
	oPSMInputEvent.pvCtx = poPSMInputEvent->pvCtx;
	oPSMInputEvent.eType = PLAY_ENDED;
	nMSEvtSysIEValue = FW_EvtSysSendIntEvt( pvMSContext_i, MSCML_OP_EVENT, &oPSMInputEvent, sizeof (t_MsEvent));
	if( AMPS_SUCCESS != nMSEvtSysIEValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
		return (MS_ERR_FRAMEWORK);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
	return (MS_ERR_NONE);
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
void PlayUnitTests_INIT_TESTS_Handler( void* pvMSContext_i, void* pvParseXMLContext_i, void* pvEventHandle_i, void* pvEventPayload_i )
{
	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );
	t_MsEvent* poPSMInputEvent = pvEventPayload_i;
	//poPSMInputEvent = (t_MsEvent*) pvEventPayload_i;

	//poPSMInputEvent->pvCtx = pvEventPayload_i;
	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Mscml play state machine context %p.\n", poPSMInputEvent->pvCtx );

	//Execute a Test with which it is compiled
	execute_test( poPSMInputEvent->pvCtx );
	MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
}

/******************************************************************************
** Function Name:
**          Generate_PLAY_STOPED_For_Mscml_Play
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
**      This function generates PLAY_STOPEDs event for mscml_play
**
**
*******************************************************************************
** Created By:      : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
e_MsRetCode Generate_PLAY_ENDED_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* poPlayRecSMIETP_i )
{
	int nMSEvtSysIEValue;
	t_MsEvent oPlayRecSMInputEvent;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	oPlayRecSMInputEvent.eType = PLAY_ENDED;
	oPlayRecSMInputEvent.pvCtx = poPlayRecSMIETP_i->pvCtx;
	nMSEvtSysIEValue = FW_EvtSysSendIntEvt( pvAFWContext_i, MSCML_OP_EVENT, &oPlayRecSMInputEvent, sizeof (t_MsEvent));
	if( AMPS_SUCCESS != nMSEvtSysIEValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
		return (MS_ERR_FRAMEWORK);
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
	return (MS_ERR_NONE);
}

e_MsRetCode Play_UnitTestCleanUp( t_MsEvent* poPSMOutputEvent )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Entering.\n" );

	free( poPSMOutputEvent->uEventParams.oStartPlayParams.cpFileName );
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "free( poPSMOutputEvent->uEventParams.oStartPlayParams.cpFileName );.\n" );
	//xmlFreeDoc( poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse );
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "xmlFreeDoc( poPSMOutputEvent->uEventParams.oMSCMLResponseParams.xdpMSCMLResponse );.\n" );
	MOP_DestroySM( poPSMOutputEvent->pvCtx );
	printf( "Context freed:\n" );
	FW_Cleanup( g_pvUTContext );
	//xmlCleanupParser();

	//MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving.\n" );
}

