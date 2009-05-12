/*! @file media_priv.h
*
* @ingroup Media
*
* @brief The file contains additional defines for media API.
*
*/

#ifndef _MEDIA_PRIV_H_
#define _MEDIA_PRIV_H_

#include "media/media.h"
#include "ms_debug.h"

#include "list_util.h"

/*! Media chain magic number */
#define MCT_CHAIN_MAGIC_NUM 0x12345678

/*! @struct t_MediaChain
*
* The details about media chain
*
*/

typedef struct _t_MediaChain
{
	t_MediaComp          oComp;
	t_AMPSDList*         poCompList;
	int                  nMagicNum;
	f_MediaChainSignalCb pfMediaChainSignalCb;
	h_MediaCompAppCtx    hMediaCompAppCtx1;
	h_MediaCompAppCtx    hMediaCompAppCtx2;
} //! Struct variable.
/*! Media chain details structure */ t_MediaChain;

/*! @struct t_MediaCompApi
*
* The functions for media component API
*
*/

typedef struct _t_MediaCompApi
{
	f_MediaCompeInit      fCompInit;
	f_MediaCompSetAppData fCompSetAppData;
	f_MediaCompDestroy    fCompDestroy;
	f_MediaCompSetStatus  fCompSetStatus;
	f_AppDataCb           fAppDataCb;
	f_AppSignalCb         fAppSignalCb;
	f_MediaCompSetParam   fCompSetParam;
	f_MediaCompGetParam   fCompGetParam;
} //! Struct variable.
/*! Media component API functions structure */ t_MediaCompApi;

/*! A global function pointer
* extern t_MediaCompApi ppfn_CompApi[MCT_MAX]
*/
extern t_MediaCompApi ppfn_CompApi[MCT_MAX];

void Media_LListMediaCompDestroyCb( void* pvListNodeData_i, void* pvArg_i );

void Media_LListMediaCompSetStatusStartCb( void* pvListNodeData_i, void* pvArg_i );

void Media_LListMediaCompSetStatusStopCb( void* pvListNodeData_i, void* pvArg_i );

#endif /* _MEDIA_PRIV_H_ */

