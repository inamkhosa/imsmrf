/*! \file sample.h
\brief This file consist of the whole examples of the all of the constructs
*/

/*! This is the set of states of all the state machines(Mscml play, playrecord and playcollect). */
typedef enum _e_MscmlOpState
{
	MOS_Start,  /*!< Start state of play phase */
	MOS_PlayingSequence, /*!< PlayingSequence state of play phase */
	MOS_InterSeqDelay, /*!< InterSeqDelay state of play phase */
	MOS_CollectingDigits, /*!< CollectingDigits state of playcollect */
	MOS_DigitsCollected, /*!< DigitsCollected state of playcollect */
	MOS_PlayingBeep, /*!< PlayingBeep state of playrecord */
	MOS_RecMediaNotStarted, /*!< RecMediaNotStarted state of playrecord */
	MOS_RecMediaStarted, /*!< RecMediaStarted state of playrecord */
	MOS_STATES /*!< maximum states */
} //! Enum variable.
/*! All the states of the state machines. */ e_MscmlOpState;

/*!
*  This number is used for context validation
*/
#define MSCML_OP_MAGIC_NUM 0x59595959

/*! Creates a type name t_MscmlOpCtx for _t_MscmlOpCtx */
typedef struct _t_MscmlOpCtx t_MscmlOpCtx;

/*! \struct t_MscmlPlayInfo
* The context of mscml play state machine
*/
typedef struct _t_MscmlPlayInfo
{
	int           nOffsetMs;          /**< Offset of file to be played in Milliseconds */
	int           nPlayDurationStartTime; /**< Start time of playing, for duration calculations */
	int           nPlayOffsetStartTime;   /**< Start time of the current sequence, for offset calculation */
	int           nRepeatCount;       /**< Number of times sequence is repeated */
	xmlNodePtr    poCurAudioNode;          /**< Current audio node in prompt to be played */
	AMPS_TIMER_ID pvDelayTimerID;    /**< Delay Timer ID */
	AMPS_TIMER_ID pvDurationTimerID; /**< Duration timer id */
} t_MscmlPlayInfo //! Struct variable.
/*! mscml play context structure. */;

/*! \struct t_MscmlPlayCollectInfo
* The context of mscml playcollect state machine
*/
typedef struct _t_MscmlPlayCollectInfo
{
	AMPS_TIMER_ID pvCollectionTimerID; /**< Collection timer ID */
	t_AMPSDList*  poDRegExHandleList;  /**< The list of regular expression in request */
	t_AMPSDList*  poPressedDigitList; /**< The pressed dtmf digits */
	int           nInterSeqStartTime; /**< Note the start of intersequence time */
	hDRegEx       hMatchedExp;       /**< Handle of the matched regular expression */
} //! Struct variable.
/*! mscml playcollect context structure. */ t_MscmlPlayCollectInfo;

/*! \union _u_SmInfo
* Consists of mscml playrecord and mscml playcollect contexts
*/
typedef union _u_SmInfo
{
	t_MscmlPlayCollectInfo oPlayCollectInfo; /**< Context of PlayCollect state machine */
	t_MscmlPlayRecordInfo  oPlayRecordInfo; /**< Context of PlayRecord state machine */
} //! Variable of type union.
/*! Either mscml playrecord or playcollect is used. */ u_SmInfo;

e_MsRetCode CreateRegExList( t_MscmlOpCtx* poCtx_i );

e_MsRetCode DeleteRegExList( t_MscmlOpCtx* poCtx_i );

/** @defgroup Module1 The First Module
*  This is the first group
*  @{
*/

/** @brief class C1 in Module 1 */
class C1 {};

/** @brief class C2 in Module 1 */
class C2 {};

/** function in Module 1 */
void func()
{
}

/** @} */ // end of Module1

/**
*  @defgroup Module2 The Second Module
*  This is the second group
*/

/** @defgroup Module3 The Third Module
*  This is the third group
*/

/** @defgroup Module4 The Fourth Module
*  @ingroup Module3
*  Module 4 is a subModule of Module 3
*/

/**
*  @ingroup Module2
*  @brief class C3 in Module 2
*/
class C3 {};

/** @ingroup Module2
*  @brief class C4 in Module 2
*/
class C4 {};

/** @ingroup Module3
*  @brief class C5 in @link Module3 the third Module@endlink.
*/
class C5 {};

/** @ingroup Module1 Module2 Module3 Module4
*  namespace N1 is in four Modules
*  @sa @link Module1 The first Module@endlink, Module2, Module3, Module4
*
*  Also see @ref mypage2
*/
namespace N1 {};

/** @file
*  @ingroup Module3
*  @brief this file in Module 3
*/

/** @defgroup module5 The Fifth Group
*  This is the fifth module
*  @{
*/

/** @page mypage1 This is a section in module 5
*  Text of the first section
*/

/** @page mypage2 This is another section in module 5
*  Text of the second section
*/

/** @} */ // end of module5

/** @addtogroup Module1
*
*  More documentation for the first group.
*  @{
*/

/** another function in Module 1 */
void func2()
{
}

/** yet another function in Module 1 */
void func3()
{
}

/** @} */ // end of Module1

