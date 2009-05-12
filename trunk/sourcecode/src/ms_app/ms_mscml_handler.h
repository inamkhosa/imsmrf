#ifndef _MS_MSCML_HANDLER_H_
#define _MS_MSCML_HANDLER_H_

#include <libxml/parser.h>

#include "call_leg_handler.h"
#include "ms_util.h"
#include "mscml_encoder/mscml_encoder_api.h"
#include "mscml_sm/mscml_sm_api.h"

e_MsRetCode MS_HandleConfigureConference( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, xmlDocPtr poMsmclDoc_i );

e_MsRetCode MS_HandleConfigureLeg( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i );

e_MsRetCode MS_HandleConfigureTeam( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i, xmlDocPtr poMscmlResp_io );

e_MsRetCode MS_HandlePlay( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i );

e_MsRetCode MS_HandlePlayRecord( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i );

e_MsRetCode MS_HandlePlayCollect( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i );

e_MsRetCode MS_HandleStop( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i );

e_MsRetCode MS_HandleMscmlMessage( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i,  SIP_MSG_HANDLE hSipMsg_i );

e_MsRetCode MS_SM_SendMscmlRequest( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMsmclDoc_i );

e_MsRetCode MS_SM_SendSmStop( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i );

e_MsRetCode MS_SendMscmlInInfo( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poMsUserCtrlCtx_i, xmlDocPtr poMscmlMsg_i );

#endif /* _MS_MSCML_HANDLER_H_ */

