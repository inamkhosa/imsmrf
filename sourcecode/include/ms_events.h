/*! @file ms_events.h
*
*@brief This file contains input output events of the state machines
*
*/

#ifndef     _MS_EVENTS_H_
#define     _MS_EVENTS_H_

#include <libxml/xmlversion.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlschemas.h>
#include <libxml/xmlschemastypes.h>
#include "fw_api.h"

#define MSCML_OP_EVENT                  3
#define PLAYER_REQ                      4
#define MSCML_PLAY_RES                  5
#define INIT_TESTS                      6
#define SS_EVENT_3263_LOOKUP_RES        9       // Must be the same as defined in SS_Events.h for DNS Resolver
#define TOTAL_MSCML_EVENTS      10

/*! @enum e_MsEvent
*@brief This is list of all the events the Media Sever is able to handle
*/
typedef enum _e_MsEvent
{
	// MSCML related events
	MSCML_RESPONSE              = 0,    /*!< Mscml response event of the state machines */
	MSCML_REQUEST               = 1,    /*!< Mscml request event of the state machines */
	// State-machine related events
	SM_STOP                 = 3,    /*!< Stop state machine */
	SM_STOPPED              = 4,    /*!< Indicates that state machine has stopped */
	// Play related events
	START_PLAY              = 5,    /*!< Start playing */
	STOP_PLAY               = 6,    /*!< Stop playing */
	START_PLAY_ERROR            = 7,    /*!< Error in starting file play */
	PLAY_ENDED              = 9,    /*!< File playing has ended */
	// Key related events
	DTMF_KEY_PRESSED            = 11,   /*!< DTMF key is pressed */
	RETURN_KEY_PRESSED          = 12,   /*!< Return key is pressed */
	ESCAPE_KEY_PRESSED          = 13,   /*!< Escape key is pressed */
	FF_KEY_PRESSED              = 14,   /*!< Forward key is pressed */
	RW_KEY_PRESSED              = 15,   /*!< Rewind key is pressed */
	// Sound related events
	SILENCE_DETECTED            = 16,   /*!< Silence is detected (after sound) in audio */
	SOUND_DETECTED              = 17,   /*!< Sound is detected (after silence) in audio */
	// Recording related events
	STOP_RECORDING              = 18,   /*!< Stop recording media */
	START_RECORDING             = 19,   /*!< Start recording media */
	START_RECORDING_ERROR           = 20,   /*!< Error in starting recording */
	STOP_RECORDING_ERROR            = 21,   /*!< Error in stopping recording */
	RECORDING_STOPPED           = 22,   /*!< Recording has stopped */
	// Timer related events
	RECORDING_DURATION_TIMER_EXPIRED    = 23,   /*!< Recording duration timer expired */
	RECORDING_SILENCE_TIMER_EXPIRED     = 24,   /*!< Recording silence timer expired */
	PLAY_DURATION_TIMER_EXPIRED     = 25,   /*!< Time of total duration of play has ended */
	INTER_SEQ_TIMER_EXPIRED         = 26,   /*!< Time of inter-sequence delay has ended */
	COLLECTION_TIMER_EXPIRED        = 27,   /*!< One of digit collection timers has ended */
	//Quarantine buffer related events
	GET_QUARANTINE              = 29,   /*!< Get quarantine buffer */
	CLEAR_QUARANTINE            = 30,   /*!< Clear quarantine buffer */
	MS_EVENT_MAX                    = 31    /*!< Maxmum number of events */
} //! Enum variable.
/*! Media server events. */ e_MsEvent;

typedef struct _t_StartPlayParams
{
	char* cpFileName;
	int   nPlayOffset;  //time stamp
} t_StartPlayParams;

typedef struct _t_StopPlayParams
{
} t_StopPlayParams;

typedef struct _t_PlayStartedParams
{
	int nStartTime;  //time stamp
} t_PlayStartedParams;

typedef struct _t_PlayStoppedParams
{
	int nStopTime;  //time stamp
} t_PlayStoppedParams;

typedef struct _t_TimerExpiredParams
{
	int nTimerId;
} t_TimerExpiredParams;

typedef struct _t_SMStopParams
{
} t_SMStopParams;

typedef struct _t_PlayErrorParams
{
	int nErrorCode;
} t_PlayErrorParams;

typedef struct _t_StopErrorParams
{
	int nErrorCode;
} t_StopErrorParams;

typedef struct _t_MSCMLRequestParams
{
	xmlDocPtr pxdMSCMLPlayRequestFile;
} t_MSCMLRequestParams;

typedef struct _t_MSCMLResponseParams
{
	xmlDocPtr xdpMSCMLResponse;
} t_MSCMLResponseParams;

typedef struct _t_SMStoppedParams
{
	xmlDocPtr xdpMSCMLResponse;
} t_SMStoppedParams;

typedef struct _t_DtmfKeyPressedParams
{
	char      cDtmfKeyVal;
	AMPS_BOOL bQuarantined;
} t_DtmfKeyPressedParams;

typedef struct _t_EscapeKeyPressedParams
{
} t_EscapeKeyPressedParams;

typedef struct _t_SilenceDetectedParams
{
} t_SilenceDetectedParams;

typedef struct _t_SoundDetectedParams
{
} t_SoundDetectedParams;

typedef struct _t_StartRecordingParams
{
	char* cpRecURL;
} t_StartRecordingParams;

typedef struct _t_StartRecordingErrorParams
{
	int nErrorCode;
} t_StartRecordingErrorParams;

typedef struct _t_StopRecordingErrorParams
{
	int nErrorCode;
} t_StopRecordingErrorParams;

typedef struct _t_RecordingDurationTimerExpParams
{
	int nTimeInMs;
} t_RecordingDurationTimerExpParams;

typedef struct _t_RecordingSilenceTimerExpParams
{
	int nTimeInMs;
} t_RecordingSilenceTimerExpParams;

typedef struct _t_RecordingStoppedParams
{
	int nReclength;
} t_RecordingStoppedParams;

typedef struct _t_PlayEndedParams
{
} t_PlayEndedParams;

typedef struct _t_SmStopParams
{
} t_SmStopParams;

typedef struct _t_PlayDurTimerExpParams
{
	int nTimeInMs;
} t_PlayDurTimerExpParams;

typedef struct _t_InterSeqTimerExpParams
{
	int nTimeInMs;
} t_InterSeqTimerExpParams;

typedef struct _t_ReturnKeyPressedParams
{
	char cReturnKeyVal;
} t_ReturnKeyPressedParams;

typedef struct _t_RwKeyPressedParams
{
	char cRwKeyVal;
} t_RwKeyPressedParams;

typedef struct _t_FwKeyPressedParams
{
	char cFwKeyVal;
} t_FwKeyPressedParams;

typedef struct _t_CollectionTimerExpParams
{
	int nTimeInMs;
} t_CollectionTimerExpParams;

typedef struct _t_MsEvent
{
	e_MsEvent eType;
	void*     pvCtx;
	void*     pvAppCtx;
	union
	{
		t_StartPlayParams oStartPlayParams;
		t_StopPlayParams oStopPlayParams;
		t_PlayStartedParams oPlayStartedParams;
		t_PlayStoppedParams oPlayStoppedParams;
		t_TimerExpiredParams oTimerExpiredParams;
		t_SMStopParams oSMStopParams;
		t_PlayErrorParams oPlayErrorParams;
		t_StopErrorParams oStopErrorParams;
		t_MSCMLRequestParams oMSCMLRequestParams;
		t_MSCMLResponseParams oMSCMLResponseParams;
		t_SMStoppedParams oSMStoppedParams;
		t_DtmfKeyPressedParams oDtmfKeyPressedParams;
		t_EscapeKeyPressedParams oEscapeKeyPressedParams;
		t_SilenceDetectedParams oSilenceDetectedParams;
		t_SoundDetectedParams oSoundDetectedParams;

		t_StartRecordingParams oStartRecParams;
		t_StartRecordingErrorParams oStartRecErrorParams;
		t_StopRecordingErrorParams oStopRecErrorParams;

		t_RecordingDurationTimerExpParams oRecDTimerExpParams;
		t_RecordingSilenceTimerExpParams oRecSTimerExpParams;

		t_RecordingStoppedParams oRecStoppedParams;

		t_PlayEndedParams oPlayEndedParams;

		t_SmStopParams oSmStopParams;
		t_PlayDurTimerExpParams oPlayDurTimerExpParams;

		t_InterSeqTimerExpParams oInterSeqTimerExpParams;

		t_ReturnKeyPressedParams oReturnKeyPressedParams;
		t_RwKeyPressedParams oRwKeyPressedParams;
		t_FwKeyPressedParams oFwKeyPressedParams;
		t_CollectionTimerExpParams oCollectionTimerExpParams;
	} uEventParams;
} t_MsEvent;

#endif

