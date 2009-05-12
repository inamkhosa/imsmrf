#ifndef _MS_TEAM_H_
#define _MS_TEAM_H_

#include "fw_api.h"
#include "list_util.h"
#include "ms_ret_code.h"
#include "call_leg_handler.h"
#include "ms_media.h"

e_MsRetCode MS_Team_Init( t_MsCtx* poMsCtx_i, t_AMPSDList** ppoTeamMateList_io );

e_MsRetCode MS_Team_Destroy( t_MsCtx* poMsCtx_i, t_AMPSDList** ppoTeamMateList_io );

e_MsRetCode MS_Team_AddTeamMate( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx1_i,  t_MsUserCtrlCtx* poUserCtrlCtx2_i );

e_MsRetCode MS_Team_DeleteTeamMate( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx1_i,  t_MsUserCtrlCtx* poUserCtrlCtx2_i );

#endif /* _MS_TEAM_H_ */

