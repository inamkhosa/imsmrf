/*! @file call_leg_sm.h
*
* @ingroup CallLeg
*
* @brief The file contains defines for call leg state machine.
*
*/

#ifndef _CALL_LEG_SM_H_
#define _CALL_LEG_SM_H_

#include "call_leg.h"

e_MsRetCode Start__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Start__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Start__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Start__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Start__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Start__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Start__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_app_ack__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_app_ack__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_app_ack__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_app_ack__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_app_ack__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_app_ack__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_app_ack__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_app_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_app_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_app_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_app_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_app_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_app_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_invite_app_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_app_response__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_app_response__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_app_response__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_app_response__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_app_response__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_app_response__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_non_invite_app_response__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_ack__Send_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_ack__Send_non_invite_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_ack__Send_ack( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_ack__Send_provisional_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_ack__Send_final_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_ack__Incoming_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

e_MsRetCode Waiting_for_ack__Incoming_response( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

extern f_CallLegSm ppf_CallLegSm[CLS_Max][CLE_Max];

#endif /* _CALL_LEG_SM_H_ */

