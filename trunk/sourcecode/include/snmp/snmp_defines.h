/*! @file snmp_defines.h
*
* @ingroup Snmp
*
* @brief The file contains defines for snmp.
*
* @author Razi-ud-Din Chaudhry
*
*/

/*! @defgroup Snmp Snmp Module
*
* @brief This module consist of functionality for interaction of media server with snmp agent
*
* @author Razi-ud-Din Chaudhry
*
* @{
*
*/

#ifndef _SNMP_DEFINES_H_
#define _SNMP_DEFINES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*! @def SNMP_Init( poSnmpCommConfig_i, phSnmpComm_o )
*
* @brief Macro to initialize communcation with the snmp agent.
*
* If communication with snmp agent is enabled, then this macro calls @ref _SNMP_Init function.
* Otherwise it simply returns MS_ERR_NONE error code.
*
* @author Razi-ud-Din Chaudhry
*
*/

/*! @def SNMP_SendNotification( hSnmpComm_i, poSnmpTrapMsg_i )
*
* @brief Macro to send notification to the snmp agent.
*
* If communication with snmp agent is enabled, then this macro calls @ref _SNMP_SendNotification function.
* Otherwise it simply returns MS_ERR_NONE error code.
*
* @author Razi-ud-Din Chaudhry
*
*/

/*! @def SNMP_Destroy( hSnmpComm_i )
*
* @brief Macro to destroy/close communication with the snmp agent.
*
* If communication with snmp agent is enabled, then this macro calls @ref _SNMP_Destroy function.
* Otherwise it simply returns MS_ERR_NONE error code.
*
* @author Razi-ud-Din Chaudhry
*
*/

/*! @def SNMP_CheckFault( hSnmpComm_i, eFaultCtr_i, nCurrentValue_i, nMaxValue_i )
*
* @brief Macro to check whether a fault has occurred or not.
*
* If communication with snmp agent is enabled, then this macro calls @ref _SNMP_CheckFault function.
* Otherwise it simply returns MS_ERR_NONE error code.
*
* @author Razi-ud-Din Chaudhry
*
*/

/*! @def SNMP_IncrementPerformanceCtr( poMsCtx_i, ePerformanceCtr_i )
*
* @brief Macro to increment the performance counter indicated by \a ePerformaceCtr_i.
*
* If communcation with snmp agent is enabled, then this macro performs its functions.
* Otherwise it simply returns back to the calling function.
*
* @author Razi-ud-Din Chaudhry
*
*/

/*! @def SNMP_AddInPerformanceCtr( poMsCtx_i, ePerformanceCtr_i, ulValue_i )
*
* @brief Macro to add the value indicated by \a ulValue_i in the performance counter indicated by \a ePerformaceCtr_i.
*
* If communcation with snmp agent is enabled, then this macro performs its functions.
* Otherwise it simply returns back to the calling function.
*
* @author Razi-ud-Din Chaudhry
*
*/

#ifdef ENABLE_SNMP
 #define SNMP_Init _SNMP_Init
 #define SNMP_SendNotification _SNMP_SendNotification
 #define SNMP_Destroy _SNMP_Destroy
 #define SNMP_CheckFault _SNMP_CheckFault
 #define SNMP_IncrementPerformanceCtr( poMsCtx_i, ePerformanceCtr_i ) ((poMsCtx_i)->pulPerformanceCtrs[(ePerformanceCtr_i)] += 1)
 #define SNMP_AddInPerformanceCtr( poMsCtx_i, ePerformanceCtr_i, ulValue_i ) ((poMsCtx_i)->pulPerformanceCtrs[(ePerformanceCtr_i)] += (ulValue_i))
#else
 #define SNMP_Init( poSnmpCommConfig_i, phSnmpComm_o ) (MS_ERR_NONE)
 #define SNMP_SendNotification( hSnmpComm_i, poSnmpTrapMsg_i ) (MS_ERR_NONE)
 #define SNMP_Destroy( hSnmpComm_i ) (MS_ERR_NONE)
 #define SNMP_CheckFault( hSnmpComm_i, eFaultCtr_i, nCurrentValue_i, nMaxValue_i ) (MS_ERR_NONE)
 #define SNMP_IncrementPerformanceCtr( poMsCtx_i, ePerformanceCtr_i )
 #define SNMP_AddInPerformanceCtr( poMsCtx_i, ePerformanceCtr_i, ulValue_i )
#endif

/*! Handle for communication with the snnmp agent */
typedef void* h_SnmpComm;

/*! @enum e_PerformanceCtrs
*
* This is the set of defines for media server performance counters
*
* @author Razi-ud-Din Chaudhry
*
*/

typedef enum
{
    PC_InviteAttemptConf = 0,   /*!< Invite attempted in conference counters */
    PC_InviteFailedConf = 1,    /*!< Invite failed in conference counters */
    PC_InviteSuccessConf = 2,   /*!< Invite succeeded in conference counters */
    PC_InviteAnsweredConf = 3,  /*!< Invite answered in conference counters */

    PC_InfoAttemptConf = 4, /*!< Info attempted in conference counters */
    PC_InfoFailedConf = 5,  /*!< Info failed in conference counters */
    PC_InfoSuccessConf = 6, /*!< Info succeeded in conference counters */
    PC_InfoAnsweredConf = 7,    /*!< Info answered in conference counters */

    PC_CancelAttemptConf = 8,   /*!< Cancel attempted in conference counters */
    PC_CancelFailedConf = 9,    /*!< Cancel failed in conference counters */
    PC_CancelSuccessConf = 10,  /*!< Cancel succeeded in conference counters */
    PC_CancelAnsweredConf = 11, /*!< Cancel answered in conference counters */

    PC_ByeAttemptConf = 12, /*!< Bye attempted in conference counters */
    PC_ByeFailedConf = 13,  /*!< Bye failed in conference counters */
    PC_ByeSuccessConf = 14, /*!< Bye succeeded in conference counters */
    PC_ByeAnsweredConf = 15,    /*!< Bye answered in conference counters */

    PC_InviteAttemptNonConf = 16,   /*!< Invite attempted in nonconference counters */
    PC_InviteFailedNonConf = 17,    /*!< Invite failed in nonconference counters */
    PC_InviteSuccessNonConf = 18,   /*!< Invite succeeded in nonconference counters */
    PC_InviteAnsweredNonConf = 19,  /*!< Invite answered in nonconference counters */

    PC_InfoAttemptNonConf = 20, /*!< Info attempted in nonconference counters */
    PC_InfoFailedNonConf = 21,  /*!< Info failed in nonconference counters */
    PC_InfoSuccessNonConf = 22, /*!< Info succeeded in nonconference counters */
    PC_InfoAnsweredNonConf = 23,    /*!< Info answered in nonconference counters */

    PC_CancelAttemptNonConf = 24,   /*!< Cancel attempted in nonconference counters */
    PC_CancelFailedNonConf = 25,    /*!< Cancel failed in nonconference counters */
    PC_CancelSuccessNonConf = 26,   /*!< Cancel succeeded in nonconference counters */
    PC_CancelAnsweredNonConf = 27,  /*!< Cancel answered in nonconference counters */

    PC_ByeAttemptNonConf = 28,  /*!< Bye attempted in nonconference counters */
    PC_ByeFailedNonConf = 29,   /*!< Bye failed in nonconference counters */
    PC_ByeSuccessNonConf = 30,  /*!< Bye succeeded in nonconference counters */
    PC_ByeAnsweredNonConf = 31, /*!< Bye answered in nonconference counters */

    PC_IncomingRTPPackets = 32, /*!< RTP packets received */
    PC_OutgoingRTPPackets = 33, /*!< RTp packets sent */

    PC_ConfigureConfAttempt = 34,   /*!< Configure conference request attempted */
    PC_ConfigureConfFailed = 35,    /*!< Configure conference request failed */
    PC_ConfigureConfSuccess = 36,   /*!< Configure conference request succeeded */
    PC_ConfigureConfAnswered = 37,  /*!< Configure conference request answered */

    PC_ConfigureLegAttempt = 38,    /*!< Configure leg request attempted */
    PC_ConfigureLegFailed = 39, /*!< Configure leg request failed */
    PC_ConfigureLegSuccess = 40,    /*!< Configure leg request succeeded */
    PC_ConfigureLegAnswered = 41,   /*!< Configure leg request answered */

    PC_ConfigureTeamAttempt = 42,   /*!< Configure team request attempted */
    PC_ConfigureTeamFailed = 43,    /*!< Configure team request failed */
    PC_ConfigureTeamSuccess = 44,   /*!< Configure team request succeeded */
    PC_ConfigureTeamAnswered = 45,  /*!< Configure team request answered */

    PC_PlayAttempt = 46,    /*!< Play request attempted */
    PC_PlayFailed = 47, /*!< Play request failed */
    PC_PlaySuccess = 48,    /*!< Play request succeeded */
    PC_PlayAnswered = 49,   /*!< Play request answered */

    PC_PlayCollectAttempt = 50, /*!< Playcollect request attempted */
    PC_PlayCollectFailed = 51,  /*!< Playcollect request failed */
    PC_PlayCollectSuccess = 52, /*!< Playcollect request succeeded */
    PC_PlayCollectAnswered = 53,    /*!< Playcollect request answered */

    PC_PlayRecordAttempt = 54,  /*!< Playrecord request attempted */
    PC_PlayRecordFailed = 55,   /*!< Playrecord request failed */
    PC_PlayRecordSuccess = 56,  /*!< Playrecord request succeeded */
    PC_PlayRecordAnswered = 57  /*!< Playrecord request answered */
    ,
    PC_StopAttempt = 58,    /*!< Stop request attempted */
    PC_StopFailed = 59, /*!< Stop request failed */
    PC_StopSuccess = 60,    /*!< Stop request succeeded */
    PC_StopAnswered = 61,   /*!< Stop request answered */

    PC_MaxCtrValue = 62 /*!< Maximum performance limit */
} //! Enum variable.
/*! All defines for performance counters */ e_PerformanceCtrs;

/*! @enum e_FaultCtrs
*
* This is the set of defines for media server fault counters
*
* @author Razi-ud-Din Chaudhry
*
*/

typedef enum
{
    FC_InvitesReceived = 0, /*!< Invite request received */
    FC_InfoReceived = 1,    /*!< Info request received */

    FC_MaxCtrValue = 2  /*!< Maximum fault limit */
} //! Enum variable.
/*! All defines for fault Counters */ e_FaultCtrs;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /*_SNMP_DEFINES_H_*/

/*! @} */ // End of Snmp Module

