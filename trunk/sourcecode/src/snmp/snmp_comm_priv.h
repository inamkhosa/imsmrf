/*! @file snmp_comm_priv.h
*
* @ingroup SnmpComm
*
* @brief The file contains defines of additional functions for communication with snmp agent.
*
* @author Razi-ud-Din Chaudhry
*
*/

#ifndef _SNMP_COMM_PRIV_H_
#define _SNMP_COMM_PRIV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ms_debug.h"
#include "ms_config_api.h"
#include "sipstack/SIP_Defines.h"
#include "snmp/snmp_comm.h"
#include "media/media.h"

/*! This number is used as a magic number for snmp */
#define SNMP_MAGIC_NUM 0x78563412

/*! @struct t_SnmpRecv
*
* This structure contains the data required for communication on udp with snmp agent.
*
* @author Razi-ud-Din Chaudhry
*
*/

typedef struct _t_SnmpRecv
{
	t_SnmpCommConfig oSnmpCommConfig;   /*!< Snmp Communication Configuration */
	int              nMagicNum; /*!< Magic Number */
	AMPS_NET_HANDLE  hUdpNetHandle; /*!< Amps Net Handle */
} //! Struct variable.
/*! snmp communication private data. */ t_SnmpRecv;

int SNMP_UdpRecvDataCallback( AMPS_HANDLE hAMPS_HANDLE_i, AMPS_APP_HANDLE hAMPS_APP_HANDLE_i, AMPS_NET_HANDLE hAMPS_NET_HANDLE_i, unsigned char* pucBuff_i, int nBuffLength_i, int nPort_i, char* pcIPAddress_i );

// void* SNMP_MallocCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, int r_nSize );

// void SNMP_FreeCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, void* r_pvData );

e_PerformanceCtrs SNMP_GetPerformanceCtrIndx( t_SnmpRecv* poSnmpRecv_i, char* pcAttribute_i );

e_FaultCtrs SNMP_GetFaultCtrIndx( t_SnmpRecv* poSnmpRecv_i, char* pcAttribute_i );

e_MsRetCode SNMP_GetNumberOfIncomingRTPPackets( t_SnmpRecv* poSnmpRecv_i, t_MsCtx* poMsCtx_i, unsigned long* pulValue_o );

e_MsRetCode SNMP_GetNumberOfOutgoingRTPPackets( t_SnmpRecv* poSnmpRecv_i, t_MsCtx* poMsCtx_i, unsigned long* pulValue_o );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /*_SNMP_COMM_PRIV_H_*/

