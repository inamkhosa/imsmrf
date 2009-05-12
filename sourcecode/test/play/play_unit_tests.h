/*
//Unit Test State Machine States(UTSM_States)
enum RepeatTest1States{
UTSM_W4Start_Play,
UTSM_W4Mscml_Response,
UTSM_W4Stop_Play,
UTSM_W4Start_Timer,
UTSM_W4SM_Stopped
};
*/
#ifndef _PLAY_UNIT_TESTS_H_
#define _PLAY_UNIT_TESTS_H_
//#define e_MsRetCode   enum e_PlayRetCode
// This one register all the three events and call the following two functions
e_MsRetCode PlayUnitTest_Events_Registerar( void* pvAFWContext_i );

//Two Dummy Event Registerars for MSCML_PLAY_RES and PLAYER_REQ respectively
e_MsRetCode UT_Test_Mscml_Play_Events_Registerar( void* pvMSContext_i );

e_MsRetCode UT_Player_Events_Registerar( void* pvAFWContext_i );

// PLAY UNIT TESTS's EVENT HANDLERS
void PlayUnitTests_INIT_TESTS_Handler( void* pvMSContext_i, void* pvParseXMLContext_i, void* pvEventHandle_i, void* pvEventPayload_i );

void UT_Test_Mscml_Play_MSCML_PLAY_RES_Handler( void* pvMSContext_i, void* pvParseXMLPContext_i, void* pvEventHandle_i, void* pvEventPayload_i );

void UT_Player_PLAYER_REQ_Handler( void* pvMSContext_i, void* pvParseXMLContext_i, void* pvEventHandle_i, void* pvEventPayload_i );

int UT_MSCMLPLAY_PlayerTimerExpired( void* pvMSContext_i, void* pvTimerData_i );

int play_repeat_test1( void* pvMSContext_i );

int play_repeat_test2( void* pvMSContext_i );

int play_repeat_test3( void* pvMSContext_i );

int play_delay_test1( void* pvMSContext_i );

int play_duration_test1( void* pvMSContext_i );

int play_Stoponerror_test1( void* pvMSContext_i );

int play_seq_test1( void* pvMSContext_i );

e_MsRetCode Generate_PLAY_STARTED_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP );

int Generate_PLAY_STOPED_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP );

#endif

