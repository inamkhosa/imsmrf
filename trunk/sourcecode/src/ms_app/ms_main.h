#ifndef _MS_MAIN_H_
#define _MS_MAIN_H_

#include "ms_ret_code.h"
#include "call_leg_handler.h"

e_MsRetCode MS_SetLocalSdp( t_MsCtx* poMsCtx_i, h_Sdp hLocalSdp_i );

e_MsRetCode MS_SetSipStack( t_MsCtx* poMsCtx_i, SIP_HANDLE hSipStack_i );

e_MsRetCode MS_InitializeMsCtx( void* pvFwCtx, int nTraceId, t_MsCtx** ppMsCtx_o );

e_MsRetCode MS_DestroyMsCtx( void* pvFwCtx, t_MsCtx* poMsCtx_i );

int MS_EventHandleValidationCallback( void* r_pvContext, int r_nEventID, void* r_pvEventPayload );

int RemoveIfaceDataCb( void** ppvData_io );

#endif /* _MS_MAIN_H_ */

