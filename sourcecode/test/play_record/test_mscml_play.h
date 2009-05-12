/******************************************************************************
** Written by Suliman Shah
**
** Copyright (C) 2008 AdvancedIMS
**
** All rights reserved.
**
*******************************************************************************
** FILE NAME            : test_play.h
** DESCRIPTION          : The file contains inputs & ouputs for the PLAY STATE MACHINE.
** PROJECT NAME         : MS
** TYPE                 : ANSI C
** CONTRIBUTOR(S)       : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
#ifndef _TEST_MSCML_PLAY_H_
#define _TEST_MSCML_PLAY_H_
//#include "ms_defines.h"
#define LOG_FILE_PATH               "ms_logs.txt"
#define TRACE_LOG_FILE_PATH         "trace_ms_logs.txt"
#define MSCMLPLAY_MAX_INT_EVENTS    22
#define MSCMLPLAY_MAX_EXT_EVENTS    25
enum
{
	TEST_APP_EVENT_INT_DUMMY_1 = 2,
	/*TEST_APP_EVENT_INT_DUMMY_2,
	TEST_APP_EVENT_INT_DUMMY_3,
	TEST_APP_EVENT_INT_DUMMY_4,
	TEST_APP_EVENT_INT_DUMMY_5,*/
	TEST_APP_EVENT_INT_MAX
} e_TestAPPInternalEvents;
enum
{
	TEST_APP_EVENT_EXT_DUMMY_1 = 2,
	/*TEST_APP_EVENT_EXT_DUMMY_2,
	TEST_APP_EVENT_EXT_DUMMY_3,
	TEST_APP_EVENT_EXT_DUMMY_4,
	TEST_APP_EVENT_EXT_DUMMY_5,*/
	TEST_APP_EVENT_EXT_MAX
} e_TestAPPExternalEvents;
int TestAPP_EventHandleValidationCallback( void* r_pvContext, int r_nEventID, void* r_pvEventPayload );

void TestAPP_HandleExtDummy1( void* r_pvAMPSContext, void* r_pvTestAPPContext, void* r_pvEventHandle, void* r_pvEventPayload );

int TestAPP_RegisterEvents( void* r_pvAFWContext, void* r_pvTestAPPContext );

void Test_Mscml_Play_MSCML_PLAY_RES_Handler( void* pvMSContext_i, void* pvParseXMLPContext_i, void* pvEventHandle_i, void* pvEventPayload_i );

e_MsRetCode Test_Mscml_Play_Events_Registerar( void* pvAFWContext_i );

void Initialize_Context( void );

//This function register all( MSCML_PLAY_EVENT, MSCML_PLAY_RES, PLAYER_REQ)
e_MsRetCode All_Events_Registerar( void* pvAFWContext_i, void* pvAMPSContext_i ); //, void* pvParseXMLContext_i);

char* MSCMLPlay_XMLFileReader( void* pvAFWContext_i, void* pvParseXMLPContext_i, char* pcFilename_i );

#endif

