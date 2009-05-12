/*! @file call_leg.h
*
* @ingroup CallLeg
*
* @brief The file contains additional function prototypes and the definition of other required structures for call leg API.
*
*/

#ifndef _CALL_LEG_H_
#define _CALL_LEG_H_

#include "call_leg/call_leg_api.h"
#include "tstring.h"
#include "dnsresolver/DNSResolver_Core.h"
#include "util/sip_util.h"

/*! Define for DNS resolver lookup event */
#define SS_EVENT_3263_LOOKUP_REQ    2       // Must be the same as defined in SS_Events.h for DNS Resolver

/*! @struct _t_CallLeg
*
* The details about call leg
*
*/

struct _t_CallLeg
{
	AMPS_BOOL       bInitialized;   /*!< Is call leg initialized */
	SIP_CONN_HANDLE hSipCnx;    /*!< Handle to sip connection */
	e_CallLegState  eState; /*!< Call leg state */
	t_CallLegCb     oCallLegCb; /*!< Call leg callbacks */
	h_CallLegApp    hAppCtx;    /*!< Handle to the call leg application context */
	t_AMPSDList*    poPendingEventList; /*!< Pending event list of type @ref t_CallLegEventData */
	t_SIPConnObject oLocalRxCnx;    /*!< Local sip connection */
} //! Struct variable.
/*! Call leg structure */;

/*! @struct t_CallLegStack
*
* The details about call leg stack
*
*/

typedef struct _t_CallLegStack
{
	void*        pvFrameworkCtx;    /*!< Framework context */
	SIP_HANDLE   hSipStack; /*!< Handle to sip stack */
	int          nTraceId;  /*!< Trace id for call leg stack traces */
	f_NewCallLeg pfnNewCallLegCb;   /*!< Function pointer to a new call leg */
	h_CallLegApp hAppCtx;   /*!< Handle to the call leg application context */
} //! Struct variable.
/*! Call leg stack structure */ t_CallLegStack;

/*! @struct t_CallLegDnsData
*
* The details about dns data of call leg
*
*/

typedef struct _t_CallLegDnsData
{
	t_CallLegStack* poCLStack;  /*!< Call leg Stack */
	t_CallLeg*      poCallLeg;  /*!< Call leg */
	SIP_MSG_HANDLE  hSipMsg;    /*! Handle to the sip message for the call leg */
	e_CallLegState  eStatetoSet;    /*!< Call leg state. In case the msg is sent successfully t_CallLeg->eState should be set with this value */
	int             bRequestCompleted;  /*!< Is the current request completed or not */
} //! Struct variable.
/*! Call leg dns data structure */ t_CallLegDnsData;

/*! @struct t_CallLegEventData
*
* The details about event data of call leg
*
*/

typedef struct _t_CallLegEventData
{
	e_CallLegEvent     eEvent;  /*!< Event for call leg */
	h_CallLegEventData hEvData; /*!< Data for the event */
} //! Struct variable.
/*! Call leg event data structure */ t_CallLegEventData;

SIP_CONN_HANDLE cl_create_sip_handle( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i );

e_MsRetCode cl_queue_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, e_CallLegEvent eEvent_i, h_CallLegEventData hEventData_i );

e_MsRetCode cl_process_pending_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i );

/*! Prototype for call leg state machine */
typedef e_MsRetCode (*f_CallLegSm)( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, h_CallLegEventData hEventData_i );

/*! A global function pointer for call leg state machine */
extern f_CallLegSm ppf_CallLegSm[CLS_Max][CLE_Max];

e_MsRetCode cl_send_request( t_CallLegStack* poClStack_i, t_CallLeg* poCallLeg_i, SIP_MSG_HANDLE hSipMsg_i, e_CallLegState  eNextState_i );

#endif /* _CALL_LEG_H_ */

