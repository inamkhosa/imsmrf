/*! @file mscml_play_record.h
*
* @ingroup MscmlPlayRecord
*
* @brief This file consist of all those functions that are used by playrecord state machine
* The <playrecord> request directs the media server to convert and possibly to transcode the RTP payloads it receives and store them to the specified URL using the requested content codec(s) and file format. This request proceeds in two phases; prompt and record.
*/

/**
*  @defgroup MscmlPlayRecord Mscml PlayRecord State Machine Module
*
*  @ingroup MSCML
*
*  This module consist of functionality of the playrecord state machine
*
*  The following is the state machine diagram of MSCML Play Record (RFC-5022)
*
*  @image html playrecord.jpg "MSCML Play Record State Machine"
*
*  @image latex playrecord.pdf "MSCML Play Record State Machine"
*
* @{
*
*/

#ifndef _MSCML_PLAY_RECORD_H_
#define _MSCML_PLAY_RECORD_H_

#include "mscml_sm.h"
/**
* A global functional pointer for play state machine
* externing f_OnEntry pf_PlaySmOnEntry[MOS_STATES]
*/
typedef e_MsRetCode (*f_PlayRecordSm)( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_Start_MSCML_REQUEST( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_PlayingSequence_START_PLAY_ERROR( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_PlayingSequence_PLAY_ENDED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_PlayingBeep_PLAY_ENDED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecordingPhase_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecMediaNotStarted_SOUND_DETECTED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecMediaNotStarted_START_RECORDING_ERROR( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecMediaNotStarted_RECORDING_SILENCE_TIMER_EXPIRED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecMediaNotStarted_STOP_RECORDING_ERROR( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecordingPhase_RECORDING_DURATION_TIMER_EXPIRED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecMediaNotStarted_RECORDING_STOPPED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecMediaStarted_SILENCE_DETECTED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecMediaStarted_SOUND_DETECTED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecMediaStarted_START_RECORDING_ERROR( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecMediaStarted_STOP_RECORDING_ERROR( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecMediaStarted_RECORDING_SILENCE_TIMER_EXPIRED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_RecMediaStarted_RECORDING_STOPPED( t_MsEvent* poEvent_i );

// Main State Machine
void PRSM_MSCML_PLAY_COLLECT_EVENT_Handler( AMPS_HANDLE hFw_i, void* pvContext_i, void* pvEventHandle_i, void* pvEvent_i );

//OnEntery and OnExit methods
e_MsRetCode OnEntry_PRSM_Start( t_MscmlOpCtx* poCtx );

e_MsRetCode OnExit_PRSM_Start( t_MscmlOpCtx* poCtx );

e_MsRetCode OnEntry_PRSM_RecMediaNotStarted( t_MscmlOpCtx* poCtx );

e_MsRetCode OnExit_PRSM_RecMediaNotStarted( t_MscmlOpCtx* poCtx );

e_MsRetCode OnEntry_PRSM_RecMediaStarted( t_MscmlOpCtx* poCtx );

e_MsRetCode OnExit_PRSM_RecMediaStarted( t_MscmlOpCtx* poCtx );

e_MsRetCode OnEntry_PRSM_PlayingBeep( t_MscmlOpCtx* poCtx );

e_MsRetCode OnExit_PRSM_PlayingBeep( t_MscmlOpCtx* poCtx );

e_MsRetCode OnEntry_PRSM_InterSeqDelay( t_MscmlOpCtx* poCtx );

e_MsRetCode OnExit_PRSM_InterSeqDelay( t_MscmlOpCtx* poCtx );

e_MsRetCode OnEntry_PRSM_PlayingSequence( t_MscmlOpCtx* poCtx );

e_MsRetCode OnExit_PRSM_PlayingSequence( t_MscmlOpCtx* poCtx );

e_MsRetCode PRSM_PlayPhase_DTMF_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_PlayPhase_ESCAPE_KEY_PRESSED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_PlayPhase_PLAY_DURATION_TIMER_EXPIRED( t_MsEvent* poEvent_i );

e_MsRetCode PRSM_CalculateRecordLength( t_MscmlOpCtx* poCtx_i );

e_MsRetCode PRSM_ChangeStateTo_BeepOrMediaNotStarted( t_MscmlOpCtx* poCtx_i );

e_MsRetCode PRSM_CheckFor_RecStopMaskDigit( t_MscmlOpCtx* poCtx_i, char cPressedKey_i );

e_MsRetCode PRSM_PlayingBeep_START_PLAY_ERROR( t_MsEvent* poEvent_i );

/** @} */ // end of MscmlPlayRecord
#endif

