#ifndef _UNIT_TESTS_H_
#define _UNIT_TESTS_H_
#include "mscml_sm/mscml_sm_api.h"
#include "fw_api.h"
#include "ms_events.h"
#include "ms_ret_code.h"
#include "test_mscml_play.h"
#include "play_unit_tests.h"

#define MSCMLPLAY_DURATION_TIMER_VALUE    5000 //569 millisec

extern int g_nMscmlPlayTraceID;
extern int g_nTestAPPTraceID;
extern int g_nUnitTestTraceID;
extern void* g_pvAFWContext;
extern void* g_pvUTContext;
extern xmlDocPtr gpxdPlayReqFile;
int execute_test( void* pvMSContext_i );

e_MsRetCode Play_UnitTestCleanUp( t_MsEvent* poPSMOutputEvent );

e_MsRetCode UT_Generate_START_START_PLAY_ERROR_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP );

e_MsRetCode UT_Generate_SM_STOP_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP );

e_MsRetCode UT_Generate_SM_START_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP );

e_MsRetCode UT_Generate_SM_STOP_For_Mscml_Play( void* pvAFWContext_i, t_MsEvent* i_poPlaySMIETP );

#endif

