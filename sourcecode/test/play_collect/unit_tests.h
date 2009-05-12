#ifndef _UNIT_TESTS_H_
#define _UNIT_TESTS_H_
#include "mscml_sm/mscml_sm_api.h"
#include "fw_api.h"
#include "ms_events.h"
#include "test_mscml_play.h"
#define MSCMLPLAY_DURATION_TIMER_VALUE    5000 //569 millisec
#define MSCMLPLAY_DELAY_TIMER_VALUE       500
#define MSCMLPLAY_PLAYER_TIMER_VALUE      500

//#include "play_unit_tests.h"
extern int g_nMscmlPlayTraceID;
extern int g_nTestAPPTraceID;
extern int g_nUnitTestTraceID;
extern void* g_pvAFWContext;
extern void* g_pvUTContext;
extern xmlDocPtr gpxdPlayReqFile;
int execute_test( void* pvMSContext_i );

e_MsRetCode UT_Generate_START_ERROR_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP );

e_MsRetCode UT_Generate_SM_STOP_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP );

e_MsRetCode UT_Generate_SM_START_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP );

e_MsRetCode UT_Generate_SM_STOP_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP );

e_MsRetCode PlayUnitTest_Events_Registerar( void* pvAFWContext_i );

e_MsRetCode PlayUnitTest_Events_Registerar( void* pvAFWContext_i );

//Two Dummy Event Registerars for MSCML_PLAY_RES and PLAYER_REQ respectively
e_MsRetCode UT_Test_Mscml_Play_Events_Registerar( void* pvMSContext_i );

e_MsRetCode UT_Player_Events_Registerar( void* pvAFWContext_i );

// PLAY UNIT TESTS's EVENT HANDLERS
void PlayUnitTests_INIT_TESTS_Handler( void* pvMSContext_i, void* pvParseXMLContext_i, void* pvEventHandle_i, void* pvEventPayload_i );

void UT_Test_Mscml_Play_MSCML_PLAY_RES_Handler( void* pvMSContext_i, void* pvParseXMLPContext_i, void* pvEventHandle_i, void* pvEventPayload_i );

void UT_Player_PLAYER_REQ_Handler( void* pvMSContext_i, void* pvParseXMLContext_i, void* pvEventHandle_i, void* pvEventPayload_i );

int UT_MSCMLPLAY_PlayerTimerExpired( void* pvMSContext_i, void* pvTimerData_i );

e_MsRetCode PlayCollect_UnitTestCleanUp( t_MsEvent* poPSMOutputEvent );

//e_MsRetCode Generate_PLAY_STARTED_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP );

#endif

