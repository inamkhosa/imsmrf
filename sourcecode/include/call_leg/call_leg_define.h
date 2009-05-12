/*! @file call_leg_define.h
*
* @ingroup CallLeg
*
* @brief The file contains defines for call leg API.
*
*/

/*! @defgroup CallLeg Call Leg Module
*
* @brief This module consists of functionality to handle different calls for media server.
*
*  The following is the state machine diagram of call leg
*
*  @image html call_leg.jpg "Call Leg State Machine"
*
*  @image latex call_leg.pdf "Call Leg State Machine"
*
* @{
*
*/

#ifndef _CALL_LEG_DEFINE_H_
#define _CALL_LEG_DEFINE_H_

#include "fw_api.h"

#include "ms_ret_code.h"
#include "ms_debug.h"

#include "sdp_api.h"
#include "sipstack/SIP_API.h"

/*! Creates a typename t_CallLeg for _t_CallLeg */
typedef struct _t_CallLeg t_CallLeg;

/*! @enum e_CallLegState
*
* These are the different states of call leg
*
*/

typedef enum
{
	CLS_Start               = 0,    /*!< Start State */
	CLS_Waiting_for_invite_response     = 1,    /*!< Waiting state for response of invite request from remote side */
	CLS_Waiting_for_app_ack         = 2,    /*!< Waiting state for acknowledgement from application */
	CLS_Waiting_for_non_invite_response = 3,    /*!< Waiting state for response of non-invite request from remote side */
	CLS_Waiting_for_invite_app_response = 4,    /*!< Waiting state for response of invite from application */
	CLS_Waiting_for_non_invite_app_response = 5,    /*!< Waiting state for response of non-invite request from application */
	CLS_Waiting_for_ack         = 6,    /*!< Waiting state for acknowledgement from remote side */
	CLS_Max                 = 7 /*!< Maximum state */
} //! Enum variable.
/*! All defines for different states of call leg */ e_CallLegState;

/*! @enum e_CallLegEvent
*
* These are the different events of call leg
*
*/

typedef enum
{
	CLE_Send_invite_request     = 0,    /*!< Sending invite request event */
	CLE_Send_non_invite_request = 1,    /*!< Sending non-invite request event */
	CLE_Send_ack            = 2,    /*!< Acknowledgement event */
	CLE_Send_provisional_response   = 3,    /*!< sending invite request event */
	CLE_Send_final_response     = 4,    /*!< Sending invite request event */
	CLE_Incoming_request        = 5,    /*!< Incoming request event */
	CLE_Incoming_response       = 6,    /*!< Incoming response event */
	CLE_Max             = 7 /*!< Maximum call leg event */
} //! Enum variable.
/*! All defines for different events of call leg */ e_CallLegEvent;

/*! Handle for event data for call leg */
typedef void*       h_CallLegEventData;
/*! Handle for call leg */
typedef void*       h_CallLeg;
/*! Handle for application of call leg */
typedef void*       h_CallLegApp;
/*! Handle for call leg stack */
typedef void*       h_CallLegStack;

#endif /* _CALL_LEG_DEFINE_H_ */

/*! @} */ // End of CallLeg Module

