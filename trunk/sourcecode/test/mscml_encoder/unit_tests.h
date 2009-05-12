#ifndef _UNIT_TESTS_H_
#define _UNIT_TESTS_H_

#include "fw_api.h"
#include "ms_events.h"
#include "ms_ret_code.h"
#include "mscml_encoder/mscml_encoder_api.h"

#define MSCMLPLAY_DURATION_TIMER_VALUE    5000 //569 millisec

int TestAPP_EventHandleValidationCallback( void* r_pvContext, int r_nEventID, void* r_pvEventPayload );

#endif

