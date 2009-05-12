/*! @file mscml_play.h
*
* @ingroup MscmlPlay
*
*@brief This file consist of all those functions that are used by the play state machine
*The client issues a <play> request to play an announcement without interruption and with no digit collection. One use, for example, is to announce the name of a new participant to the entire conference. (RFC5022)
*/

/**
*  @defgroup MscmlPlay Mscml Play State Machine Module
*
*  @ingroup MSCML
*
*  This module consist of functionality of the play state machine
*
*  The following is the state machine diagram of MSCML Play (RFC-5022)
*
*  @image html play.jpg "MSCML Play State Machine"
*
*  @image latex play.pdf "MSCML Play State Machine"
*
*  @{
*/

#ifndef _MSCML_PLAY_H_
#define _MSCML_PLAY_H_

#include "mscml_sm.h"

e_MsRetCode PSM_Start_MSCML_REQUEST( t_MsEvent* poEvent_i );

e_MsRetCode PSM_PlayingSequence_START_PLAY_ERROR( t_MsEvent* poEvent_i );

e_MsRetCode PSM_PlayingSequence_PLAY_ENDED( t_MsEvent* poEvent_i );

e_MsRetCode PSM_PlayingSequence_PLAY_DURATION_TIMER_EXPIRED( t_MsEvent* poEvent_i );

e_MsRetCode PSM_InterSeqDelay_PLAY_DURATION_TIMER_EXPIRED( t_MsEvent* poEvent_i );

e_MsRetCode PSM_InterSeqDelay_INTER_SEQ_TIMER_EXPIRED( t_MsEvent* poEvent_i );

//OnEntery and OnExit methods
e_MsRetCode OnEntry_PSM_Start( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnExit_PSM_Start( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnEntry_PSM_InterSeqDelay( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnExit_PSM_InterSeqDelay( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnEntry_PSM_PlayingSequence( t_MscmlOpCtx* poCtx_i );

e_MsRetCode OnExit_PSM_PlayingSequence( t_MscmlOpCtx* poCtx_i );

/** @} */ // end of MscmlPlay

#endif /* _MSCML_PLAY_H_ */

