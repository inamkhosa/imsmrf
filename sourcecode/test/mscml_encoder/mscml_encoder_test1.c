#include "unit_tests.h"

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
	int size = 0;
	xmlDocPtr pxdXmlDocPtr = NULL;
	//xmlDocPtr pxdXmlPlayDocPtr = NULL;
	char* pcEncodedRes = NULL;

	printf( "Main: Entering.\n" );
	pvAMPSContext = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );
	if( NULL == pvAMPSContext )
	{
		printf( "FW_Init: AMPS_Init is failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	g_pvAFWContext = pvAMPSContext;
	pvMSContext = pvAMPSContext;
	g_pvUTContext = pvAMPSContext;
	AMPS_SetTraceMode( g_pvUTContext, AMPS_TRACE_MODE_BOTH );
	AMPS_SetTraceLevel( g_pvUTContext, 31 );
	AMPS_EnableLogging( g_pvUTContext, "testapp_log.txt" );
	AMPS_SetTraceForIOAgent( g_pvUTContext );
	g_nTestAPPTraceID = AMPS_GetTraceID( g_pvUTContext );
	g_nMscmlPlayTraceID = g_nTestAPPTraceID;
	g_nUnitTestTraceID = g_nTestAPPTraceID;
	AMPS_SetTraceForTraceID( g_pvUTContext, g_nTestAPPTraceID );
	AMPS_EvtSysSetEvtHandleValidationCallback( g_pvUTContext, TestAPP_EventHandleValidationCallback );
	if( NULL == AMPS_SchedInit( g_pvUTContext, 8, 10, NULL, NULL, NULL, NULL ))
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "AMPS_SchedInit failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
/*	if( MS_ERR_NONE != SetMscmlEncoderTraceId( g_nTestAPPTraceID ))
{
MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "SetMscmlEncoderTraceId failed.\n" );
return (AMPS_ERROR_FAILURE);
}*/
	pxdXmlDocPtr = GetMscmlResponse();
	xmlDocDump( stdout, pxdXmlDocPtr );
	if( NULL == pxdXmlDocPtr )
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "SetMscmlEncoderTraceId failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
/*	if( MS_ERR_NONE != SetMscmlEncoderTraceId( g_nTestAPPTraceID ))
{
MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "SetMscmlEncoderTraceId failed.\n" );
return (AMPS_ERROR_FAILURE);
}*/
	//xmlDocDump( stdout, pxdXmlDocPtr);
/*	if( MS_ERR_NONE != SetResponseId( pxdXmlDocPtr, (xmlChar*)"123" ))
{
MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "SetResponseId failed.\n" );
return (AMPS_ERROR_FAILURE);
}*/
/*	if( MS_ERR_NONE != SetResponseRequest( pxdXmlDocPtr, (xmlChar*)"play" ))
{
MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "SetResponseName failed.\n" );
return (AMPS_ERROR_FAILURE);
}
if( MS_ERR_NONE != SetResponseCode( pxdXmlDocPtr, 200 ))*/
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "SetResponseCode failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	if( MS_ERR_NONE != SetResponseText( pxdXmlDocPtr, (xmlChar*)"hello" ))
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "SetResponseText failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	if( MS_ERR_NONE != SetResponseReason( pxdXmlDocPtr, "OK" ))
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, "SetResponseReason failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	if( MS_ERR_NONE !=  SetResponsePlayDuration( pxdXmlDocPtr, "23" ))
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, " SetResponsePlayDuration failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	if( MS_ERR_NONE !=  SetResponsePlayOffset( pxdXmlDocPtr, "33" ))
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, " SetResponsePlayOffset failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	pcEncodedRes = EncodeResponse( pxdXmlDocPtr, &size );
	if( pcEncodedRes ==  NULL )
	{
		MS_TRACE( g_nTestAPPTraceID, AMPS_TRACE_LEVEL_ERROR, " EncodeResponse failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	printf( "EncodeResponse(pxdXmlDocPtr, &size): %d\n%s\n", size, pcEncodedRes );

	AMPS_Main( g_pvUTContext );
	printf( "Main: Leaving.\n" );
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

