/******************************************************************************
** Written by Suliman Shah
**
** Copyright (C) 2008 AdvancedIMS
**
** All rights reserved.
**
*******************************************************************************
** FILE NAME            : fw_api.h
** DESCRIPTION          : This file contains wrapper functions
** PROJECT NAME         : MS
** TYPE                 : ANSI C
** CONTRIBUTOR(S)       : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
#ifndef _FW_API_H_
#define _FW_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"

#include "amps/AMPS_API.h"
#include "amps/AMPS_Defines.h"

#define FUNC_VER( func_name, ver ) (func_name ## _ ## ver)
#define DEFINE( func_name, ver ) FUNC_VER( func_name, ver )

#define FW_Init AMPS_Init
#define FW_Cleanup AMPS_Cleanup
#define FW_Main AMPS_Main
#define FW_EnableLogging AMPS_EnableLogging

#define FW_CreateTCPListenerObject AMPS_CreateTCPListenerObject
#define FW_CreateTCPListenerObjectBlocking AMPS_CreateTCPListenerObjectBlocking

#define FW_SetRecvBuffOffSetToNetHandle AMPS_SetRecvBuffOffSetToNetHandle
//In case of TCP use this function to set the status of message i.e if the application is waiting for 100 bytes packet and only 50 have been
//received u can return without processing the received buffer and setting the status of message to PENDING/AWAITING/COMPLETE
#define FW_SetRecvBuffStatusToNetHandle AMPS_SetRecvBuffStatusToNetHandle

#define FW_CreateTCPConnObject AMPS_CreateTCPConnObject
#define FW_CreateTCPConnObjectEx AMPS_CreateTCPConnObjectEx
#define FW_CreateTCPConnObjectBlocking AMPS_CreateTCPConnObjectBlocking

#define FW_DestroyTCPObject AMPS_DestroyTCPObject
#define FW_DestroyTCPObjectBlocking AMPS_DestroyTCPObjectBlocking

#define FW_CreateSecUDPObject AMPS_CreateSecUDPObject
#define FW_DestroySecUDPObject AMPS_DestroySecUDPObject

#define FW_CreateUDPObject AMPS_CreateUDPObject
#define FW_CreateUDPObjectBlocking AMPS_CreateUDPObjectBlocking

#define FW_DestroyUDPObject AMPS_DestroyUDPObject
#define FW_DestroyUDPObjectBlocking AMPS_DestroyUDPObjectBlocking

#define FW_IsAddressNatted AMPS_IsAddressNatted
#define FW_GetPublicIPAndPort AMPS_GetPublicIPAndPort

#define FW_SndBuffer AMPS_SndBuffer
#define FW_SndBufferList AMPS_SndBufferList

#define FW_CreateFileObject AMPS_CreateFileObject
#define FW_CreateFileObjectBlocking AMPS_CreateFileObjectBlocking

#define FW_DestroyFileObject AMPS_DestroyFileObject
#define FW_DestroyFileObjectBlocking AMPS_DestroyFileObjectBlocking

#define FW_FileWrite AMPS_FileWrite
#define FW_FileRead AMPS_FileRead

/*
#define FW_EncryptString AMPS_EncryptString
#define FW_DecryptString AMPS_DecryptString
*/

#define FW_SetAppServerCtxt AMPS_SetAppServerCtxt
#define FW_APP_HANDLE AMPS_APP_HANDLE

#define FW_GetUsrDataFromNetHandle AMPS_GetUsrDataFromNetHandle
#define FW_SetUsrDataToNetHandle AMPS_SetUsrDataToNetHandle

#define FW_GetConnTypeFromNetHandle AMPS_GetConnTypeFromNetHandle

#define FW_SetRemoteIPAddressOnNetHandle AMPS_SetRemoteIPAddressOnNetHandle
#define FW_SetRemotePortOnNetHandle AMPS_SetRemotePortOnNetHandle

#define FW_GetRemotePortFromNetHandle AMPS_GetRemotePortFromNetHandle
#define FW_GetRemoteIPAddressFromNetHandle AMPS_GetRemoteIPAddressFromNetHandle

#define FW_GetLocalPortFromNetHandle AMPS_GetLocalPortFromNetHandle
#define FW_GetLocalIPAddressFromNetHandle AMPS_GetLocalIPAddressFromNetHandle

//Timer APIs
#define FW_StartTimer AMPS_StartTimer
#define FW_DeleteTimer AMPS_DeleteTimer

//Cache APIs
#define FW_CACHE_HANDLE AMPS_CACHE_HANDLE
#define FW_CacheCleanup AMPS_CacheCleanup

#define FW_CacheAddEntry AMPS_CacheAddEntry
#define FW_CacheRemoveEntry AMPS_CacheRemoveEntry
#define FW_CacheUpdateEntry AMPS_CacheUpdateEntry
#define FW_CacheLookupEntry AMPS_CacheLookupEntry

//System APIs

#define FW_NetworkToHostShort AMPS_NetworkToHostShort
#define FW_NetworkToHostLong AMPS_NetworkToHostLong
#define FW_HostToNetworkShort AMPS_HostToNetworkShort
#define FW_HostToNetworkLong AMPS_HostToNetworkLong

#define FW_INetAtoN AMPS_INetAtoN
#define FW_INetNtoA AMPS_INetNtoA
#define FW_Rand AMPS_Rand
#define FW_StrnCaseCMP AMPS_StrnCaseCMP

#define FW_MODULE_HANDLE AMPS_MODULE_HANDLE
#define FW_CleanupLoadableModule AMPS_CleanupLoadableModule
#define FW_GetLoadableModuleContext AMPS_GetLoadableModuleContext

#define FW_GetHostIPAddress AMPS_GetHostIPAddress
#define FW_GetPeerName AMPS_GetPeerName
#define FW_GetHostByName AMPS_GetHostByName
#define FW_GetLocalIfaceAddress AMPS_GetLocalIfaceAddress

#define FW_GetATypeRecords AMPS_GetATypeRecords
#define FW_FreeATypeRecords AMPS_FreeATypeRecords
#define FW_GetSRVRecords AMPS_GetSRVRecords
#define FW_FreeSRVRecords AMPS_FreeSRVRecords
#define FW_GetNAPTRRecords AMPS_GetNAPTRRecords
#define FW_FreeNAPTRRecords AMPS_FreeNAPTRRecords

#define FW_GetCurrentTime AMPS_GetCurrentTime
#define FW_GetTimeDiffInMilliSec AMPS_GetTimeDiffInMilliSec
#define FW_GetCurrentTimeInMilliSec AMPS_GetCurrentTimeInMilliSec
#define FW_Sleep AMPS_Sleep
#define FW_SleepInMilliSeconds AMPS_SleepInMilliSeconds
#define FW_GetTimeDiffInSec AMPS_GetTimeDiffInSec
#define FW_GetFormattedTime AMPS_GetFormattedTime

//DBEngine APis
#define FW_DBE_HANDLE AMPS_DBE_HANDLE
#define FW_DBEngineCleanup AMPS_DBEngineCleanup
#define FW_DBEngineQuery AMPS_DBEngineQuery
#define FW_DBEngineFreeResults AMPS_DBEngineFreeResults

#define FW_DBES_HANDLE AMPS_DBES_HANDLE
#define FW_DBEngineStatementPrepare AMPS_DBEngineStatementPrepare
#define FW_DBEngineStatementParamCount AMPS_DBEngineStatementParamCount
#define FW_DBEngineCreateBindParameters AMPS_DBEngineCreateBindParameters
#define FW_DBEngineSetBindingParameter AMPS_DBEngineSetBindingParameter
#define FW_DBEngineBindParameters AMPS_DBEngineBindParameters
#define FW_DBEngineExecute AMPS_DBEngineExecute
#define FW_DBEngineStatementClose AMPS_DBEngineStatementClose

//EventSystem APIs
#define FW_EvtSysRegExtEvt AMPS_EvtSysRegExtEvt
#define FW_EvtSysCancelExtEvt AMPS_EvtSysCancelExtEvt
#define FW_EvtSysRegIntEvt AMPS_EvtSysRegIntEvt
#define FW_EvtSysCancelIntEvt AMPS_EvtSysCancelIntEvt
#define FW_EvtSysSendExtEvt AMPS_EvtSysSendExtEvt
#define FW_EvtSysSendIntEvt AMPS_EvtSysSendIntEvt
#define FW_EvtSysSetEvtHandleValidationCallback AMPS_EvtSysSetEvtHandleValidationCallback
#define FW_EvtSysConfigure AMPS_EvtSysConfigure

//Hash APIs
#define FW_HASH_HANDLE AMPS_HASH_HANDLE
#define FW_HashTableDelete AMPS_HashTableDelete
#define FW_HashTableDeleteAllEntries AMPS_HashTableDeleteAllEntries
#define FW_HashTableInsert AMPS_HashTableInsert
#define FW_HashTableRemoveByHandle AMPS_HashTableRemoveByHandle
#define FW_HashTableRemoveByKey AMPS_HashTableRemoveByKey
#define FW_HashTableSearch AMPS_HashTableSearch
#define FW_HashTableLookupByHandleAndHashKey AMPS_HashTableLookupByHandleAndHashKey
#define FW_HashTableLookupByHandle AMPS_HashTableLookupByHandle

//IOAgents APIs
#define FW_IOAGENTS_HANDLE AMPS_IOAGENTS_HANDLE
#define FW_IOAgentsCleanup AMPS_IOAgentsCleanup
#define FW_IOAgentsRegEvt AMPS_IOAgentsRegEvt
#define FW_IOAgentsCancelEvt AMPS_IOAgentsCancelEvt
#define FW_IOAgentsSendEvt AMPS_IOAgentsSendEvt

//Log API
#define FW_Log AMPS_Log

//MD5 APIs
#define FW_MD5_HANDLE AMPS_MD5_HANDLE
#define FW_MD5Cleanup AMPS_MD5Cleanup
#define FW_MD5Update AMPS_MD5Update
#define FW_MD5Final AMPS_MD5Final

//AES APIs
#define FW_AES_HANDLE AMPS_AES_HANDLE
#define FW_AESCleanup AMPS_AESCleanup
#define FW_AESSetKey AMPS_AESSetKey
#define FW_AESEncrypt AMPS_AESEncrypt
#define FW_AESDecrypt AMPS_AESDecrypt

// Memory Management APIs
#define FW_Malloc AMPS_Malloc
#define FW_Realloc AMPS_Realloc
#define FW_Free AMPS_Free

//Scheduler APIs
#define FW_SchedInit AMPS_SchedInit
#define FW_SchedCleanup AMPS_SchedCleanup

//LinkList APIs
#define FW_SListInsertSortedPrepend AMPS_SListInsertSortedPrepend
#define FW_SListInsertSortedAppend AMPS_SListInsertSortedAppend
#define FW_SListSortPrepend AMPS_SListSortPrepend
#define FW_SListSortAppend AMPS_SListSortAppend
#define FW_SListTraverse AMPS_SListTraverse
#define FW_SListCopy AMPS_SListCopy
#define FW_SListRemoveKey AMPS_SListRemoveKey
#define FW_SListFindData AMPS_SListFindData
#define FW_SListGetNextNode AMPS_SListGetNextNode
#define FW_SListGetNodeData AMPS_SListGetNodeData
#define FW_SListAppendGivenNode AMPS_SListAppendGivenNode
#define FW_SListPrependGivenNode AMPS_SListPrependGivenNode
#define FW_SListPrepend AMPS_SListPrepend
#define FW_SListAppend AMPS_SListAppend
#define FW_SListLast AMPS_SListLast
#define FW_SListSearch AMPS_SListSearch
#define FW_SListFind AMPS_SListFind
#define FW_SListRemove AMPS_SListRemove
#define FW_SListRemoveWithOutFree AMPS_SListRemoveWithOutFree
#define FW_SListFree AMPS_SListFree
#define FW_SListCount AMPS_SListCount
#define FW_SListConcat AMPS_SListConcat

#define FW_DListInit AMPS_DListInit
#define FW_DListConcat AMPS_DListConcat
#define FW_DListAppend AMPS_DListAppend
#define FW_DListPrepend AMPS_DListPrepend
#define FW_DListPrependGivenNode AMPS_DListPrependGivenNode
#define FW_DListAppendGivenNode AMPS_DListAppendGivenNode
#define FW_DListSearch AMPS_DListSearch
#define FW_DListFind AMPS_DListFind
#define FW_DListRemove AMPS_DListRemove
#define FW_DListSort AMPS_DListSort
#define FW_DListSwapNodesData AMPS_DListSwapNodesData
#define FW_DListRemoveFirstNode AMPS_DListRemoveFirstNode
#define FW_DListInsertAfter AMPS_DListInsertAfter
#define FW_DListInsertBefore AMPS_DListInsertBefore
#define FW_DListFree AMPS_DListFree
#define FW_DListFreeNodes AMPS_DListFreeNodes
#define FW_DListTraverse AMPS_DListTraverse
#define FW_DListRemoveWithOutFree AMPS_DListRemoveWithOutFree
#define FW_RemoveNthNode AMPS_RemoveNthNode

//Trace APIs
#define FW_Trace AMPS_Trace
#define FW_GetTraceID AMPS_GetTraceID
#define FW_SetTraceForTraceID AMPS_SetTraceForTraceID
#define FW_ClearTraceForTraceID AMPS_ClearTraceForTraceID

#define FW_SetTraceLevel AMPS_SetTraceLevel
#define FW_ClearTraceLevel AMPS_ClearTraceLevel
#define FW_SetTraceMode AMPS_SetTraceMode
#define FW_ClearTraceMode AMPS_ClearTraceMode

#define FW_SetTraceForAMPS AMPS_SetTraceForAMPS
#define FW_ClearTraceForAMPS AMPS_ClearTraceForAMPS
#define FW_SetTraceForCache AMPS_SetTraceForCache
#define FW_ClearTraceForCache AMPS_ClearTraceForCache
#define FW_SetTraceForSAPI AMPS_SetTraceForSAPI
#define FW_ClearTraceForSAPI AMPS_ClearTraceForSAPI
#define FW_SetTraceForTimer AMPS_SetTraceForTimer
#define FW_ClearTraceForTimer AMPS_ClearTraceForTimer
#define FW_SetTraceForDBEngine AMPS_SetTraceForDBEngine
#define FW_ClearTraceForDBEngine AMPS_ClearTraceForDBEngine
#define FW_SetTraceForMemoryManager AMPS_SetTraceForMemoryManager
#define FW_ClearTraceForMemoryManager AMPS_ClearTraceForMemoryManager
#define FW_SetTraceForEventSystem AMPS_SetTraceForEventSystem
#define FW_ClearTraceForEventSystem AMPS_ClearTraceForEventSystem
#define FW_SetTraceForIOAgent AMPS_SetTraceForIOAgent
#define FW_ClearTraceForIOAgent AMPS_ClearTraceForIOAgent
#define FW_SetTraceForHeap AMPS_SetTraceForHeap
#define FW_ClearTraceForHeap AMPS_ClearTraceForHeap
#define FW_SetTraceForLog AMPS_SetTraceForLog
#define FW_ClearTraceForLog AMPS_ClearTraceForLog
#define FW_SetTraceForHash AMPS_SetTraceForHash
#define FW_ClearTraceForHash AMPS_ClearTraceForHash
#define FW_SetTraceForMemoryManagerWithGC AMPS_SetTraceForMemoryManagerWithGC
#define FW_ClearTraceForMemoryManagerWithGC AMPS_ClearTraceForMemoryManagerWithGC
#define FW_SetTraceForSched AMPS_SetTraceForSched
#define FW_ClearTraceForSched AMPS_ClearTraceForSched
#define FW_SetTraceForMD5 AMPS_SetTraceForMD5
#define FW_ClearTraceForMD5 AMPS_ClearTraceForMD5

#ifdef __cplusplus
}
#endif

#endif //end of _FW_API_H_

