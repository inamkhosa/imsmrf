/*! @file mscml_sm.h
@brief This file consist of all the data structures, datatypes and functions that are needed to use the state machines
*/
/*!
* @ref MSCML serve for this purpose and is further subdivided in the following submodules:
*
* 1) @ref MscmlPlay
*
* 2) @ref MscmlPlayCollect
*
* 3) @ref MscmlPlayRecord
*
*
*/

/*! @page SMachines MSCML State Machines
*
* @brief This page contains information related to the mscml state machines
*
*We have implemented three kinds of state machines. They are
*
*@section DregNotation 1) MSCML Play
* The client issues a <play> request to play an announcement without interruption and with no digit collection. One use, for example, is to announce the name of a new participant to the entire conference. The <play> request has the attributes described in the list below.
*@section DregNotation 2) MSCML PlayCollect
*
*@section DregNotation 3) MSCML PlayRecord
*
* Further information on state machine follow this link:
*
* http://community.roxen.com/developers/idocs/rfc/rfc5022.html
*
*
* @author Suliman Shah
*
*/

/*! @defgroup MSCML Mscml State Machine Module
*
* @brief This module consists of all the functionality of the state machines
*
* @author Suliman Shah
*
* @{
*
*/

#ifndef _MSCML_SM_H_
#define _MSCML_SM_H_

#include "tstring.h"
#include "sipstack/SIP_API.h"

#include "ms_debug.h"
#include "ms_events.h"
#include "mscml_encoder/mscml_encoder_api.h"
#include "dreg/dreg.h"

#include "mscml_sm/mscml_sm_api.h"
#include "sndfile.h"
/*!
*  This number is used for context validation
*/
#define MSCML_OP_MAGIC_NUM 0x59595959
/*!
*  minimum number of milliseconds for timers
*/

#define MIN_TIMER_MS 20     // minimum value of timer supported in Framework

/*! @enum e_MscmlOpState
*This is the set of states of all the state machines(Mscml play, playrecord and playcollect).
*/
typedef enum _e_MscmlOpState
{
	MOS_Start,              /*!< Start state of play phase */
	MOS_PlayingSequence,    /*!< PlayingSequence state of play phase */
	MOS_InterSeqDelay,      /*!< InterSeqDelay state of play phase */
	MOS_CollectingDigits,   /*!< CollectingDigits state of playcollect */
	MOS_DigitsCollected,    /*!< DigitsCollected state of playcollect */
	MOS_PlayingBeep,        /*!< PlayingBeep state of playrecord */
	MOS_RecMediaNotStarted, /*!< RecMediaNotStarted state of playrecord */
	MOS_RecMediaStarted,    /*!< RecMediaStarted state of playrecord */
	MOS_STATES              /*!< maximum states */
} //! Enum variable.
/*! All the states of the state machines. */ e_MscmlOpState;

/*! Creates a type name t_MscmlOpCtx for _t_MscmlOpCtx */
typedef struct _t_MscmlOpCtx t_MscmlOpCtx;

/*! Creates a type name for ( t_MsEvent* poEvent_i */
typedef e_MsRetCode (*f_MscmlOpSm)( t_MsEvent* poEvent_i );

/*! Creates a type name for e_MsRetCode (*f_OnEntry)( t_MscmlOpCtx* poCtx_i ) */
typedef e_MsRetCode (*f_OnEntry)( t_MscmlOpCtx* poCtx_i );

/*! Creates a type name for e_MsRetCode (*f_OnExit)( t_MscmlOpCtx* poCtx_i ) */
typedef e_MsRetCode (*f_OnExit)( t_MscmlOpCtx* poCtx_i );

typedef e_MsRetCode (*f_FreeEvent)(t_MsEvent* poEvent_i );

/*! \struct t_MscmlPlayInfo
* The context of mscml play state machine
*/
typedef struct _t_MscmlPlayInfo
{
	int           nOffsetMs;                /**< Offset of file to be played in Milliseconds */
	int           nPlayDurationStartTime;   /**< Start time of playing, for duration calculations */
	int           nPlayOffsetStartTime;     /**< Start time of the current sequence, for offset calculation */
	int           nRepeatCount;             /**< Number of times sequence is repeated */
	xmlNodePtr    poCurAudioNode;           /**< Current audio node in prompt to be played */
	AMPS_TIMER_ID pvDelayTimerID;           /**< Delay Timer ID */
	AMPS_TIMER_ID pvDurationTimerID;        /**< Duration timer id */
} //! Struct variable.
/*! mscml play context structure. */ t_MscmlPlayInfo;

/*! \struct t_MscmlPlayCollectInfo
* The context of mscml playcollect state machine
*/
typedef struct _t_MscmlPlayCollectInfo
{
	AMPS_TIMER_ID pvCollectionTimerID;  /**< Collection timer ID */
	t_AMPSDList*  poDRegExHandleList;   /**< The list of regular expression in request */
	t_AMPSDList*  poPressedDigitList;   /**< The pressed dtmf digits */
	int           nInterSeqStartTime;   /**< Note the start of intersequence time */
	hDRegEx       hMatchedExp;          /**< Handle of the matched regular expression */
} //! Struct variable.
/*! mscml playcollect context structure. */ t_MscmlPlayCollectInfo;

/*! \struct t_MscmlPlayRecordInfo
* The context of mscml playcollect state machine
*/
typedef struct _t_MscmlPlayRecordInfo
{
	AMPS_TIMER_ID pvSilenceTimerID;     /**< Silence timer ID */
	AMPS_TIMER_ID pvRecordTimerID;      /**< Recording timer iD */
	char          cPressedDigit;        /**< The dtmf digit that is pressed during recoding phase */
	int           nRecordStartTime;     /**< Note the start of recording time */
} //! Struct variable.
/*! mscml playRecord context structure. */ t_MscmlPlayRecordInfo;

/*! \struct t_MscmlResponseInfo
* The consist of attributes need for mscml response
*/
typedef struct _t_MscmlResponseInfo
{
	int   nResponseCode;        /**< Mscml response error code */
	char* pcResponseId;         /**< Responsed id is same as the request id */
	char* pcReason;         /**< Reason corresponding to the error code */
	int   nPlayDuration;        /**< The duration an audio sequence is player for */
	int   nPlayOffset;      /**< Play offset of an audio sequence */
	int   nRecLength;       /**< Record lenght is the size(Bytes) of the recoded file */
	int   nRecDuration;         /**< The duration is of the recorded file in milliseconds */
} //! Struct variable.
/*! mscml response structure. */ t_MscmlResponseInfo;

/*! \union u_SmInfo
* Consists of mscml playrecord and mscml playcollect contexts
*/
typedef union _u_SmInfo
{
	t_MscmlPlayCollectInfo oPlayCollectInfo;    /**< Context of PlayCollect state machine */
	t_MscmlPlayRecordInfo  oPlayRecordInfo;     /**< Context of PlayRecord state machine */
} //! Variable of type union.
/*! Either mscml playrecord or playcollect is used. */ u_SmInfo;

/*! \struct t_MscmlOpCtx
* State machine context
*/
struct _t_MscmlOpCtx
{
	int           nMagicNum;          /**<  Magic number of Play state machine */
	int           nTraceId;           /**<  Trace Id for messages */
	void*         pvFwCtx;            /**<  Frame work Context */
	void*         pvAppCtx;           /**<  Context of application of the state-machine */
	e_MscmlOpType eType;              /**<  Event type */
	char*         pcSoundDir;         /**<  The sound directory of audio files */

	void*      pvMscmlSm;             /**<  State machine context(playrecord or playcollect) */
	f_OnEntry* pfMscmlOnEntry;        /**<  OnEntry functional pointer */
	f_OnExit*  pfMscmlOnExit;         /**<  OnExit functional pointer */

	xmlDocPtr poMscmlReq;             /**< Request that is processed */

	e_MscmlOpState eCurrentState;     /**< Current state of state-machine */

	t_MscmlResponseInfo oRespInfo;    /**< mscml response structure */

	t_MscmlPlayInfo oPlayInfo;        /**< mscml play context struct */
	u_SmInfo        uSmInfo;          /**< mscml playrecord of mscml playcollect context */
};

e_MsRetCode MOP_Dummy( t_MsEvent* poEvent_i );

e_MsRetCode MOP_OnEntryOnExit_Dummy( t_MscmlOpCtx* poCtx );

e_MsRetCode MOP_ChangeState(  t_MscmlOpCtx* poCtx_i, e_MscmlOpState eState );

e_MsRetCode MOP_Generate_START_PLAY( t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_Generate_START_BEEP( t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_Generate_STOP_PLAY( t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_Generate_MSCML_RESPONSE( t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_Generate_STOP_INTER_SEQ_TIMER(  t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_Generate_STOP_PLAY_DURATION_TIMER(  t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_Generate_STOP_COLLECTION_TIMER(  t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_Generate_GET_QUARANTINE( t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_Generate_CLEAR_QUARANTINE( t_MscmlOpCtx* poCtx_i );

int MOP_DurationTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i );

int MOP_DelayTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i );

int MOP_CollectionTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i );

e_MsRetCode MOP_PlayNextFile( t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_AnyState_SM_STOP( t_MsEvent* poEvent_i );

e_MsRetCode MOP_CalculatePlayFileAndOffset(  t_MscmlOpCtx* poCtx_i, int nOffset_i );

xmlDocPtr MOP_GenerateMscmlResponse( t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_Generate_START_COLLECTION_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i );

e_MsRetCode MOP_Generate_START_PLAY_DURATION_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i );

e_MsRetCode MOP_Generate_START_PLAY_DELAY_TIMER(  t_MscmlOpCtx* poCtx_i, int nTimer_i );

e_MsRetCode MOP_PrintContext( t_MscmlOpCtx* poCtx_i );

void MOP_EVENT_Handler( AMPS_HANDLE hFw_i, void* pvContext_i, void* pvEventHandle_i, void* pvEvent_i );

//play record
e_MsRetCode MOP_Generate_START_RECORDING( t_MscmlOpCtx* poCtx_i  );

e_MsRetCode MOP_Generate_STOP_RECORDING_SILENCE_TIMER( t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_Generate_STOP_RECORDING( t_MscmlOpCtx* poCtx_i );

e_MsRetCode MOP_Generate_STOP_RECORDING_DURATION_TIMER( t_MscmlOpCtx* poCtx_i );

int MOP_RecSilenceTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i );

int MOP_RecDurationTimerExpired( AMPS_HANDLE hFw_i, void* pvTimerData_i );

e_MsRetCode CalcualateFileDuration( t_MscmlOpCtx* poCtx_i );

/**
* A global functional pointer
* externing f_MscmlOpSm ppf_PlaySm[MOS_STATES][MS_EVENT_MAX]
*/
extern f_MscmlOpSm ppf_PlaySm[MOS_STATES][MS_EVENT_MAX];
/**
* A global functional pointer for play state machine
* externing f_OnEntry pf_PlaySmOnEntry[MOS_STATES]
*/
extern f_OnEntry pf_PlaySmOnEntry[MOS_STATES];
/**
* A global functional pointer for OnExit function of play state machine
* externing f_OnExit pf_PlaySmOnExit[MOS_STATES]
*/
extern f_OnExit pf_PlaySmOnExit[MOS_STATES];
/**
* A global functional pointer for playcollect state machine
* externing f_MscmlOpSm ppf_PlayCollectSm[MOS_STATES][MS_EVENT_MAX]
*/
extern f_MscmlOpSm ppf_PlayCollectSm[MOS_STATES][MS_EVENT_MAX];
/**
* A global functional pointer for OnEtry of playcollect state machine
* externing f_OnEntry pf_PlayCollectSmOnEntry[MOS_STATES]
*/
extern f_OnEntry pf_PlayCollectSmOnEntry[MOS_STATES];
/**
* A global functional pointer for OnExit of playcollect state machine
* externing f_OnExit pf_PlayCollectSmOnExit[MOS_STATES]
*/

extern f_OnExit pf_PlayCollectSmOnExit[MOS_STATES];
/**
* A global functional pointer for OnExit of playRecord state machine
* externing f_MscmlOpSm ppf_PlayRecordSm[MOS_STATES][MS_EVENT_MAX]
*/

extern f_MscmlOpSm ppf_PlayRecordSm[MOS_STATES][MS_EVENT_MAX];
/**
* A global functional pointer for OnEntry of playRecord state machine
* externing f_OnEntry pf_PlayRecordSmOnEntry[MOS_STATES]
*/

extern f_OnEntry pf_PlayRecordSmOnEntry[MOS_STATES];
/**
* A global functional pointer for OnExit of playRecord state machine
* externing f_OnExit pf_PlayRecordSmOnExit[MOS_STATES]
*/

extern f_OnExit pf_PlayRecordSmOnExit[MOS_STATES];

int RemoveDRegNodeCb( void** ppvData_io );


e_MsRetCode MOP_FreeEvent(t_MsEvent* poEvent_i );
e_MsRetCode MOP_FreeEvent_NONE(t_MsEvent* poEvent_i );
e_MsRetCode MOP_FreeEvent_MSCML_RESPONSE(t_MsEvent* poEvent_i );
e_MsRetCode MOP_FreeEvent_MSCML_REQUEST(t_MsEvent* poEvent_i );
e_MsRetCode MOP_FreeEvent_SM_STOPPED(t_MsEvent* poEvent_i );
e_MsRetCode MOP_FreeEvent_START_PLAY(t_MsEvent* poEvent_i );
e_MsRetCode MOP_FreeEvent_START_RECORDING(t_MsEvent* poEvent_i );

#endif

