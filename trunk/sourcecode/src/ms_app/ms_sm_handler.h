#ifndef _MS_SM_HANDLER_H_
#define _MS_SM_HANDLER_H_

#include "fw_api.h"
#include "ms_events.h"
#include "call_leg_handler.h"
#include "ms_util.h"
#include "mscml_sm/mscml_sm_api.h"

void MS_SM_Handler( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i );

void MS_SM_HandleMscmlResponse( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i );

void MS_SM_HandleSmStopped( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i );

void MS_SM_HandleStartPlay( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i );

void MS_SM_HandleStopPlay( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i );

void MS_SM_HandleStartRecording( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i );

void MS_SM_HandleStopRecording( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i );

void MS_SM_ClearQuarantineBuffer( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i );

void MS_SM_GetQuarantineEvent( AMPS_HANDLE hFramework_i, AMPS_APP_HANDLE hMsCtx_i, void* pvEventHandle_i, void* pvMsEvent_i );

#endif  /* _MS_SM_HANDLER_H_ */

