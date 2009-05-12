#include "play/mscml_play_io.h"
int g_nMscmlPlayTraceID = 0;
static void* g_pvIOAgentsContext = NULL;
static void* g_pvIOAgentsReadObject = NULL;
static void* g_pvAFWContext = NULL;
xmlDocPtr gpxdPlayReqFile = NULL;
/******************************************************************************
** Function Name:
**			CalculateStringLength
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
int main( int argc, char* argv[] )
{
	void* pvMSContext = NULL;
	void* pvMSSchedInitValue;
	int nAllEvtRegValue;
	char* pcXMLFileReaderBuf;
	e_MsRetCode eAllEvtRegRetVal;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Main: Entering.\n" );
	pvMSContext = (void*)MS_Init( MS_TRUE, TRAC_LOG_FILE_PATH );
	if( NULL == pvMSContext )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "MS_Init: MS_Init is failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	//make it local
	gpvCtx = (t_MscmlPlayContext*) MS_Malloc( sizeof (t_MscmlPlayContext));
	if( NULL == gpvCtx )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "AMPS_Malloc failed for gpvCtx.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	Initialize_Context( pvMSContext );
	gpoMscmlPlayResponse = (t_MscmlPlayResponse*) MS_Malloc( sizeof (t_MscmlPlayResponse));
	if( NULL == gpoMscmlPlayResponse )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "AMPS_Malloc failed for gpoMscmlPlayResponse.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	//test1_play_create();
	g_pvAFWContext = pvMSContext;
	MS_SetTraceMode( pvMSContext, TRACE_MODE_DISPLAY );
	MS_SetTraceLevel( pvMSContext, SET_TRACE_LEVEL );
	MS_EnableLogging( pvMSContext, LOG_FILE_PATH );
	MS_SetTraceForIOAgent( pvMSContext );
	g_nMscmlPlayTraceID = MS_GetTraceID( pvMSContext );
	MS_SetTraceForTraceID( pvMSContext, g_nMscmlPlayTraceID );
	MS_EvtSysSetEvtHandleValidationCallback( pvMSContext, TestMscmlPlay_EventHandleValidationCallback );
	pvMSSchedInitValue = (void*)MS_SchedInit( pvMSContext, XMLPARSER_MAX_INT_EVENTS, XMLPARSER_MAX_EXT_EVENTS, NULL, NULL, NULL, NULL );
	if( NULL == pvMSSchedInitValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "MS_SchedInit failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	//Read MSCML PLAY REQUEST FILE
	pcXMLFileReaderBuf = MSCMLPlay_XMLFileReader( pvMSContext, NULL, NULL );
	if( NULL == pcXMLFileReaderBuf )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "MSCMLPlay_XMLFileReader Failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	//Call the Registerars of All Events
	eAllEvtRegRetVal = All_Events_Registerar( pvMSContext );
	if( MS_ERR_FRAMEWORK != eAllEvtRegRetVal )   //, NULL))
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "All_Events_Registerar failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	t_MsEvent poPSMInputEvent;
	if( AMPS_SUCCESS != FW_EvtSysSendIntEvt( pvMSContext, INIT_TESTS, &poPSMInputEvent, sizeof (t_MsEvent)))
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "FW_EvtSysSendIntEvt failed.\n" );
	}
	MS_Main( pvMSContext );
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Main: Leaving.\n" );
	return (0);
}

/******************************************************************************
** Function Name:
**			CalculateStringLength
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
e_MsRetCode All_Events_Registerar( void* pvMSContext_i ) //, void* pvParseXMLContext_i)
{
	int nMSAllEVTRegValue;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	/* pre-condition */
	{ //---------------------------------------------------------------------
		ms_assert_return( NULL != pvMSContext_i, MS_ERR_PRE_COND_FAILED );
	} //---------------------------------------------------------------------
	  /* - */
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	eRetCode = Mscml_Play_Events_Registerar( pvMSContext_i ); // , NULL);
	if( MS_ERR_FRAMEWORK != eRetCode )   // remove NULL argument
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Player_Events_Registerar failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	eRetCode = PlayUnitTest_Events_Registerar( pvMSContext_i ); // , NULL);
	if( MS_ERR_FRAMEWORK != eRetCode )   // remove NULL argument
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "PlayUnitTest_Events_Registerar failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	return (eRetCode);
}

/******************************************************************************
** Function Name:
**			CalculateStringLength
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
int TestMscmlPlay_EventHandleValidationCallback( void* i_pvContext, int i_nEventID, void* pvEventPayload_i )
{
	return (AMPS_SUCCESS);
}

/******************************************************************************
** Function Name:
**			CalculateStringLength
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
e_MsRetCode Test_Mscml_Play_Events_Registerar( void* pvMSContext_i )
{
	int nMSSysRegIERes;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	/* pre-condition */
	{ //---------------------------------------------------------------------
		ms_assert_return( NULL != pvMSContext_i, MS_ERR_PRE_COND_FAILED );
	} //---------------------------------------------------------------------
	  /* - */
	nMSSysRegIERes = AMPS_EvtSysRegIntEvt( pvMSContext_i, MSCML_PLAY_RES, Test_Mscml_Play_MSCML_PLAY_RES_Handler, NULL );
	if( AMPS_SUCCESS != nMSSysRegIERes )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " AMPS_EvtSysRegIntEvt for MSCML_PLAY_RES failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	/* post-condition */
	{ //--------------------------------------------------------------------------------
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert_return( nMSSysRegIERes != AMPS_SUCCESS, MS_ERR_POST_COND_FAILED );
		}
	} //--------------------------------------------------------------------------------
	  /* - */
	return (eRetCode);
}

/******************************************************************************
** Function Name:
**			CalculateStringLength
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
void Test_Mscml_Play_MSCML_PLAY_RES_Handler( void* pvMSContext_i, void* pvParseXMLPContext_i, void* pvEventHandle_i, void* pvEventPayload_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n\n\n" );
	t_MsEvent* poPSMOutputEvent;
	poPSMOutputEvent = (t_MsEvent*) pvEventPayload_i;
	/* pre-condition */
	{ //---------------------------------------------------------------------
		ms_assert( NULL != pvMSContext_i, MS_ERR_PRE_COND_FAILED );
		ms_assert( NULL != pvEventPayload_i, MS_ERR_PRE_COND_FAILED );
		ms_assert( NULL != poPSMOutputEvent->eType.oMSCMLResponseParams.xdpMSCMLResponse, MS_ERR_PRE_COND_FAILED );
	} //---------------------------------------------------------------------
	  /* - */
	xmlDocDump( stdout, poPSMOutputEvent->eType.oMSCMLResponseParams.xdpMSCMLResponse );

	switch( poPSMOutputEvent->ePSMOEventTypes )
	{
		case MSCML_RESPONSE:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " MSCML_RESPONSE\n:" );
			break;

		case SM_STOPPED:
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "PLAY_ENDED\n:" );
			break;
	}
	printf( "\n\n\n" );
	// NO Post Condition
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
}

/******************************************************************************
** Function Name:
**			CalculateStringLength
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
char* MSCMLPlay_XMLFileReader( void* pvMSContext_i, void* pvParseXMLPContext_i, char* cpFilename_i )
{
	FILE* pFile;
	long lSize;
	char* pcBuffer;
	size_t result;
	xmlDocPtr docPtr;
	xmlNodePtr cur;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	pFile = fopen( "example.xml", "rb" );
	if( pFile == NULL )
	{
		fputs( "File error", stderr );
		exit( 1 );
	}
	// obtain file size:
	fseek( pFile, 0, SEEK_END );
	lSize = ftell( pFile );
	rewind( pFile );
	// allocate memory to contain the whole file:
	pcBuffer = (char*) malloc( sizeof (char)*lSize );
	if( pcBuffer == NULL )
	{
		fputs( "Memory error", stderr );
		exit( 2 );
	}
	// copy the file into the buffer:
	result = fread( pcBuffer, 1, lSize, pFile );
	if( result != lSize )
	{
		fputs( "Reading error", stderr );
		exit( 3 );
	}
	//Parse the XML File
	docPtr = xmlParseMemory( pcBuffer, strlen( pcBuffer ) );
	if( docPtr == NULL )
	{
		fprintf( stderr, "xmlParseMemory Cannot Parse the XML File\n" );
		return (NULL);
	}
	gpxdPlayReqFile = docPtr;
	//xmlDocDump(stdout,docPtr);
	cur = xmlDocGetRootElement( docPtr );
	//Populate the attribute to the xml file into Sturcture
	// MscmlPlay_PopulateAttributeValues (pvMSContext_i, pvParseXMLPContext_i,docPtr, cur) ;
	fclose( pFile );
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	return (pcBuffer);
	//free (buffer);
}

/******************************************************************************
** Function Name:
**			CalculateStringLength
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
//This function populates (prompt delay duration repeat etc) Attributes to MscmlPlayAttribute sturcture
e_MsRetCode MscmlPlay_PopulateAttributeValues( void* pvMSContext_i, void* pvParseXMLPContext_i, xmlDocPtr pxdDoc_i, xmlNodePtr pxnCur_i )
{
	xmlChar* uri;
	int nTotalNoOfFilesInSeq = 0;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	/* pre-condition */
	{ //------------------------------------------------------------------------
		ms_assert( NULL != pvMSContext_i, MS_ERR_PRE_COND_FAILED );
		ms_assert( NULL != pxdDoc_i, MS_ERR_PRE_COND_FAILED );
		ms_assert( NULL != pxnCur_i,             MS_ERR_PRE_COND_FAILED );
	} //-------------------------------------------------------------------------
	  /* - */
	pxnCur_i = pxnCur_i->xmlChildrenNode;
	if( pxnCur_i == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "xmlChildrenNode Failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
		return (eRetCode);
	}

	while( pxnCur_i != NULL )
	{
		if((!xmlStrcmp( pxnCur_i->name, (const xmlChar*)"prompt" )))
		{
			uri = xmlGetProp( pxnCur_i, (xmlChar*)"stoponerror" );
			if( uri == NULL )
			{
				eRetCode = MS_ERR_FRAMEWORK;
			}
			if( !strcmp( uri, "yes" ))
			{
				//gpoMscmlPlayAttributes->nStoponerror = 1;
				//
				gpvCtx->nStoponerrorAttr = 1;
			}
			else if( !strcmp( uri, "no" ))
			{
				//gpoMscmlPlayAttributes->nStoponerror = 0;
				//
				gpvCtx->nStoponerrorAttr = 0;
			}
			uri = xmlGetProp( pxnCur_i, (xmlChar*)"delay" );
			if( uri == NULL )
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "xmlGetProp Failed.\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			//gpoMscmlPlayAttributes->nDelay = atoi(uri);
			//
			gpvCtx->nDelayAttr = atoi( uri );
			uri = xmlGetProp( pxnCur_i, (xmlChar*)"duration" );
			if( uri == NULL )
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "xmlGetProp Failed.\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			if( !strcmp( uri, "infinite" ))
			{
				//gpoMscmlPlayAttributes->nDuration = -1;
				//
				gpvCtx->nDurationAttr = -1;
			}
			else if( !strcmp( uri, "immediate" ))
			{
				gpvCtx->nDurationAttr = 0;
			}
			else
			{
				//gpoMscmlPlayAttributes->nDuration = atoi(uri);
				//
				gpvCtx->nDurationAttr = atoi( uri );
			}
			uri = xmlGetProp( pxnCur_i, (xmlChar*)"repeat" );
			if( uri == NULL )
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "xmlGetProp Failed.\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			//keep it in context
			gpvCtx->nRepeat = atoi( uri );
			//
			gpvCtx->nRepeatAttr = atoi( uri );
			//keep it in attributes struct
			//gpoMscmlPlayAttributes->nRepeat = atoi(uri);
			uri = xmlGetProp( pxnCur_i, (xmlChar*)"offset" );
			if( uri == NULL )
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "xmlGetProp Failed.\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			//gpoMscmlPlayAttributes->nOffset = atoi(uri);
			//
			gpvCtx->nOffsetAttr = atoi( uri );
			xmlFree( uri );
		}
		if((!xmlStrcmp( pxnCur_i->name, (const xmlChar*)"audio" )))
		{
			nTotalNoOfFilesInSeq = nTotalNoOfFilesInSeq + 1;
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Audio URL: %s::: %d\n", xmlGetProp( pxnCur_i, (xmlChar*)"url" ), nTotalNoOfFilesInSeq );
			//gpoMscmlPlayAttributes->nTotalNoOfFilesInSeq = nTotalNoOfFilesInSeq;
			//
			gpvCtx->nTotalNoOfFilesInSeqAttr = nTotalNoOfFilesInSeq;
		}
		if((!xmlStrcmp( pxnCur_i->name, (const xmlChar*)"play" )))
		{
			uri = xmlGetProp( pxnCur_i, (xmlChar*)"id" );
			if( uri == NULL )
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "xmlGetProp Failed.\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Play ID: %s\n", uri );
			gpoMscmlPlayResponse->pcResponseId = (char*)uri;
			gpoMscmlPlayResponse->pcRequestType = "play";
		}
		if((!xmlStrcmp( pxnCur_i->name, (const xmlChar*)"stop" )))
		{
			uri = xmlGetProp( pxnCur_i, (xmlChar*)"id" );
			if( uri == NULL )
			{
				MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "xmlGetProp Failed.\n" );
				eRetCode = MS_ERR_FRAMEWORK;
			}
			MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Play ID: %s\n", uri );
			gpoMscmlPlayResponse->pcResponseId = (char*)uri;
			gpoMscmlPlayResponse->pcRequestType = "stop";
		}
		//Getting into Internal Nodes
		if( xmlGetLastChild( pxnCur_i ) == NULL )
		{
			pxnCur_i = pxnCur_i->next;
		}
		else
		{
			pxnCur_i = pxnCur_i->xmlChildrenNode;
		}
	}
	//Populate the attribute into structure
	/* post-condition */
	{ //--------------------------------------------------------------------------------
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert_return( gpoMscmlPlayResponse != NULL, MS_ERR_POST_COND_FAILED );
			ms_assert_return( gpoMscmlPlayResponse != NULL, MS_ERR_POST_COND_FAILED );
		}
	} //--------------------------------------------------------------------------------
	  /* - */
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	return (eRetCode);
}

e_MsRetCode Initialize_Context( void* pvMSContext_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	/* pre-condition */
	{ //------------------------------------------------------------------------
		ms_assert( NULL != pvMSContext_i, MS_ERR_PRE_COND_FAILED );
	} //-------------------------------------------------------------------------
	  /* - */
	gpvCtx->eCurrentState = -1;
	gpvCtx->nPlayDurationStartTime = 0;
	gpvCtx->nPlayOffsetStartTime = 0;
	gpvCtx->nRepeat = 0; //nRepeat value changes with time
	gpvCtx->nLeftNoOfAudioFilesInSeq = 0; // its value also changes with time
	//New attributes
	gpvCtx->nDelayAttr = 0;
	gpvCtx->nDurationAttr = 0;
	gpvCtx->nOffsetAttr = 0;
	gpvCtx->nRepeatAttr = 0;
	gpvCtx->nStoponerrorAttr = 0;
	gpvCtx->nTotalNoOfFilesInSeqAttr = 0;
	gpvCtx->pvDelayTimerID = NULL;
	gpvCtx->pvDurationTimerID = NULL;
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
}

