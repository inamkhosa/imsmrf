#include "play/mscml_play_io.h"
#include "test_mscml_play.h"

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
e_MsRetCode PlayUnitTest_Events_Registerar( void* pvAFWContext_i )
{
	int nMSSysRegIEValue;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	/* pre-condition */
	{ //---------------------------------------------------------------------
		ms_assert_return( NULL != pvAFWContext_i, MS_ERR_PRE_COND_FAILED );
	} //---------------------------------------------------------------------
	  /* - */
	if( pvAFWContext_i == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Failed: NULL Valued Argument.\n" );
		return (MS_ERR_FRAMEWORK);
	}
	// Register INIT_TESTS Event
	nMSSysRegIEValue = AMPS_EvtSysRegIntEvt( pvAFWContext_i, INIT_TESTS, PlayUnitTests_INIT_TESTS_Handler, NULL );
	if( AMPS_SUCCESS != nMSSysRegIEValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	// Register MSCML_PLAY_RES Event
	eRetCode = UT_Test_Mscml_Play_Events_Registerar( pvAFWContext_i ); //, NULL);
	if( MS_ERR_FRAMEWORK != eRetCode )   // remove NULL argument
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Test_Mscml_Play_Events_Registerar failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	// Register PLAYER_REQ Event
	eRetCode = UT_Player_Events_Registerar( pvAFWContext_i ); //, NULL);
	if( MS_ERR_FRAMEWORK != eRetCode )   // remove NULL argument
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Player_Events_Registerar failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	/* post-condition */
	{ //--------------------------------------------------------------------------------
	} //--------------------------------------------------------------------------------
	  /* - */
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
void PlayUnitTests_INIT_TESTS_Handler( void* pvMSContext_i, void* pvParseXMLContext_i, void* pvEventHandle_i, void* pvEventPayload_i )
{
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	t_MsEvent* poPSMInputEvent;
	poPSMInputEvent = (t_MsEvent*) pvEventPayload_i;
	/* pre-condition */
	{ //---------------------------------------------------------------------
		ms_assert( NULL != pvMSContext_i, MS_ERR_PRE_COND_FAILED );
		ms_assert( NULL != poPSMInputEvent, MS_ERR_PRE_COND_FAILED );
	} //---------------------------------------------------------------------
	  /* - */
	if( pvMSContext_i == NULL || poPSMInputEvent == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Failed: NULL Valued Argument.\n" );
		return;
	}
	// play_repeat_test1(pvMSContext_i);
	//play_repeat_test2(pvMSContext_i);
	//play_repeat_test3(pvMSContext_i);
	play_duration_test1( pvMSContext_i );
	//play_delay_test1(pvMSContext_i);
	//play_Stoponerror_test1(pvMSContext_i);
	//play_seq_test1(pvMSContext_i);
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
e_MsRetCode UT_Player_Events_Registerar( void* pvAFWContext_i ) //, void* pvParseXMLPContext_i)
{
	int nMSSysRegIEValue;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	/* pre-condition */
	{ //---------------------------------------------------------------------
		ms_assert_return( NULL != pvAFWContext_i, MS_ERR_PRE_COND_FAILED );
	} //---------------------------------------------------------------------
	  /* - */
	if( pvAFWContext_i == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Failed: NULL Valued Argument.\n" );
		return (MS_ERR_FRAMEWORK);
	}
	nMSSysRegIEValue = AMPS_EvtSysRegIntEvt( pvAFWContext_i, PLAYER_REQ, UT_Player_PLAYER_REQ_Handler, NULL );
	if( AMPS_SUCCESS != nMSSysRegIEValue )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " failed.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
	}
	/* post-condition */
	{ //--------------------------------------------------------------------------------
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert_return( eRetCode != MS_ERR_NONE, MS_ERR_POST_COND_FAILED );
		}
	} //--------------------------------------------------------------------------------
	  /* - */
	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Leaving.\n" );
	return (eRetCode);
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
e_MsRetCode UT_Test_Mscml_Play_Events_Registerar( void* pvMSContext_i ) //, void* pvParseXMLPContext_i)
{
	int nMSSysRegIERes;
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, "Entering.\n" );
	/* pre-condition */
	{ //---------------------------------------------------------------------
		ms_assert_return( NULL != pvMSContext_i, MS_ERR_PRE_COND_FAILED );
	} //---------------------------------------------------------------------
	  /* - */
	if( pvMSContext_i == NULL )
	{
		MS_TRACE( g_nMscmlPlayTraceID, AMPS_TRACE_LEVEL_ERROR, " pvMSContext_i is NULL.\n" );
		eRetCode = MS_ERR_FRAMEWORK;
		return (eRetCode);
	}
	nMSSysRegIERes = AMPS_EvtSysRegIntEvt( pvMSContext_i, MSCML_PLAY_RES, UT_Test_Mscml_Play_MSCML_PLAY_RES_Handler, NULL );
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

