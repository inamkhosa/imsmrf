/*! @file mscml_play_collect.h
*
* @ingroup MscmlPlayCollect
*
* @brief This file consist of all those functions that are used by playcollect state machine
*
* The client issues a <playcollect> request to play an announcement (optional) and collect digits. The <playcollect> request is executed in two phases, prompt and collect. If the client specifies prompt content to be played, using the <prompt> element or prompturl attribute, the media server plays the content before starting the collection phase. If no prompt content is specified, the collect phase begins immediately.(RFC5022)
*/

/**
*  @defgroup MscmlPlayCollect Mscml PlayCollect State Machine Module
*
*  @ingroup MSCML
*
*  This module consist of functionality of the playcollect state machine
*
*  The following is the state machine diagram of MSCML Play Collect (RFC-5022)
*
*  @image html playcollect.jpg "MSCML Play Collect State Machine"
*
*  @image latex playcollect.pdf "MSCML Play Collect State Machine"
*
*  @{
*
*/

#ifndef _MSCML_PLAY_COLLECT_H_
#define _MSCML_PLAY_COLLECT_H_

#include "mscml_sm.h"

// Main State Machine
void PCSM_MSCML_PLAY_COLLECT_EVENT_Handler( AMPS_HANDLE hFw_i, void* pvContext_i, void* pvEventHandle_i, void* pvEvent_i );

//Play Collect functions

e_MsRetCode PCSM_Start_MSCML_REQUEST( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_PlayingSequence_START_PLAY_ERROR( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_PlayingSequence_PLAY_ENDED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_PlayingSequence_FF_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_PlayingSequence_RW_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_PlayingSequence_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_PlayingPhase_PLAY_DURATION_TIMER_EXPIRED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_InterSeqDelay_FF_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_InterSeqDelay_RW_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_InterSeqDelay_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_AnyState_RETURN_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_AnyState_ESCAPE_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_CollectingDigits_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_CollectingDigits_COLLECTION_TIMER_EXPIRED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_DigitsCollected_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PCSM_DigitsCollected_COLLECTION_TIMER_EXPIRED( t_MsEvent* poEvent_i );

e_MsRetCode OnEntry_PCSM_Start( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnExit_PCSM_Start( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnEntry_PCSM_PlayingSequence( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnExit_PCSM_PlayingSequence( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnEntry_PCSM_InterSeqDelay( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnExit_PCSM_InterSeqDelay( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnEntry_PCSM_CollectingDigits( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnExit_PCSM_CollectingDigits( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnEntry_PCSM_DigitsCollected( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnExit_PCSM_DigitsCollected( t_MscmlOpCtx* poCtx_i );

e_MsRetCode PCSM_ProcessDigit( t_MscmlOpCtx* poCtx_i, char cDigit_i, t_MsEvent* poEvent_i );

e_MsRetCode CreateRegExList( t_MscmlOpCtx* poCtx_i );

/** @} */ // end of MscmlPlayCollect
#endif

