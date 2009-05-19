#ifndef _LIST_UTIL_H_
#define _LIST_UTIL_H_

#ifdef WIN32
#include "AMPS_API.h"
#else
#include "amps/AMPS_API.h"
#endif

typedef void* (*DListDuplicate)( void* pvData );

void* DListGetNthNode( t_AMPSDList* poDList_i, int nNodeLocation_i );

int DListCopy( t_AMPSDList* pSrcList_i, t_AMPSDList* pDestList_i, DListDuplicate pfnDup_i );

int DListRemoveFromData( t_AMPSDList* poDList_i, void* pvData_i, AMPS_LListRemoveLinkDataCallback pfRemovDataCb_i );

#endif /* _LIST_UTIL_H_ */

