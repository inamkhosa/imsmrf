/*! @file snmp_comm.h
*
* @ingroup SnmpComm
*
* @brief The file contains defines of structures & functions for communication with snmp agent.
*
* @author Razi-ud-Din Chaudhry
*
*/

/*! @defgroup SnmpComm Snmp Communication Module
*
* @ingroup Snmp
*
* @brief This module consist of functionality for communication with snmp agent
*
* @author Razi-ud-Din Chaudhry
*
* @{
*
*/

#ifndef _SNMP_COMM_H_
#define _SNMP_COMM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ms.h"
#include "ms_ret_code.h"
#include "tstring.h"
#include "snmp_parser/snmp_parser_api.h"

/*! This number is used for maximum size of oid string */
#define SNMP_MAX_OID_STRING_LENGTH      30

/*! This number is used for maximum size of value string */
#define SNMP_MAX_VALUE_STRING_LENGTH        30

/*! This number is used for severity level string */
#define SNMP_MAX_SEVERITY_STRING_LENGTH     10

/*! This number is used for error description string */
#define SNMP_MAX_ERRORDESC_STRING_LENGTH    50

/*! @struct t_SnmpCommConfig
*
* The configuration for communication with snmp agent
*
* @author Razi-ud-Din Chaudhry
*
*/

typedef struct _t_SnmpCommConfig
{
	int      nTraceId;  /*!< Trace Id */
	char     pcLocalIp[AMPS_MAX_IP_STRING_LENGTH];  /**< Ip address of local host */
	int      nLocalPort;    /*!< Port of local host */
	char     pcRemoteIp[AMPS_MAX_IP_STRING_LENGTH]; /**< Ip address of remote host */
	int      nRemotePort;   /*!< Port of remote host */
	t_MsCtx* poMsCtx;   /**< Media server context */
	void*    pvFrameworkCtx;    /*!< Amps context */
} //! Struct variable.
/*! snmp communication configuration structure. */ t_SnmpCommConfig;

/*! @struct t_SnmpTrapMsg
*
* The attributes of a trap tag required for sending snmp notifications.
*
* @author Razi-ud-Din Chaudhry
*
*/

typedef struct _t_SnmpTrapMsg
{
	char pcOid[SNMP_MAX_OID_STRING_LENGTH]; /*!< Oid */
	char pcValue[SNMP_MAX_VALUE_STRING_LENGTH]; /*!< Value */
	char pcSeverity[SNMP_MAX_SEVERITY_STRING_LENGTH];   /*!< Severity */
	int  nErrorCode;    /*!< Error code */
	char pcErrorDesc[SNMP_MAX_ERRORDESC_STRING_LENGTH]; /*!< Error description */
} //! Struct variable.
/*! Trap message elements structure. */ t_SnmpTrapMsg;

e_MsRetCode _SNMP_Init( t_SnmpCommConfig* poSnmpCommConfig_i, h_SnmpComm* phSnmpComm_o );

e_MsRetCode _SNMP_SendNotification( h_SnmpComm hSnmpComm_i, t_AMPSDList* poSnmpTrapMsg_i );

e_MsRetCode _SNMP_Destroy( h_SnmpComm hSnmpComm_i );

e_MsRetCode _SNMP_CheckFault( h_SnmpComm hSnmpComm_i, e_FaultCtrs eFaultCtr_i, unsigned long nCurrentValue_i, unsigned long nMaxValue_i );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /*_SNMP_COMM_H_*/

/*! @} */ // End of SnmpComm Module

