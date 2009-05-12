/*! @file mscml_sm_api.h
*
*@brief This file provides API functions for applications to use its functionality
*
*/

#ifndef _MSCML_SM_API_H_
#define _MSCML_SM_API_H_

#include "ms_ret_code.h"
/*! @struct e_MscmlOpType These are the events for mscml play, mscml play record and mscml play collect state machines. */
typedef enum
{
	MOT_PLAY,       /*!< Mscml Play event */
	MOT_PLAYCOLLECT,    /*!< Mscml PlayCollect event */
	MOT_PLAYRECORD      /*!< Mscml PlayRecord event */
} //! Enum variable.
/*! State machine events. */ e_MscmlOpType;

e_MsRetCode MOP_CreateSM( e_MscmlOpType eType_i, int nTraceId_i, void* pvFw_i, char* pcSoundDir_i, void** ppvMscmlOpCtx_o );

e_MsRetCode MOP_DestroySM( void* pvMscmlContext_i );

e_MsRetCode MOP_RegisterEvent( void* pvFw_i, void* pvCtx_i );

e_MsRetCode MOP_UnregisterEvent( void* pvFw_i );

#endif /* _MSCML_SM_API_H_ */

