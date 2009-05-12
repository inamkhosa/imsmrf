#ifndef _CIRCULAR_LIST_H_
#define _CIRCULAR_LIST_H_

#include "fw_api.h"

typedef struct _t_CirList t_CirList;

struct _t_CirList
{
	void*      pvData;
	t_CirList* poNext;
	t_CirList* poPrev;
};

typedef void (*fnNodeFunc)( void* pvData_i, void* pvUserData_i );
typedef int (*fnCompareFunc)( void* pvData1_i, void* pvData2_i );

t_CirList* cir_list_append( t_CirList* poStartNode_i, void* pvData );

// t_CirList* cir_list_prepend(t_CirList* poStartNode_i, void* pvData);
t_CirList* cir_list_insert_after( t_CirList* poStartNode_i, t_CirList* poNodeToInsert_i );

t_CirList* cir_list_delete( t_CirList* poStartNode_i, void* pvData_i, fnCompareFunc pfnCompare_i, fnNodeFunc pfnDelete_i, void* pvUserData_i );

t_CirList* cir_list_remove( t_CirList* poStartNode_i, void* pvData );

t_CirList* cir_list_remove_link( t_CirList* poStartNode_i, t_CirList* poLinkNode_i );

void cir_list_free( t_CirList* poStartNode_i );

void cir_list_free_custom( t_CirList* poStartNode_i, fnNodeFunc pfOperation_i, void* pvUserData_i );

unsigned int cir_list_length( t_CirList* poStartNode_i );

void cir_list_foreach( t_CirList* poStartNode_i, fnNodeFunc pfOperation_i, void* pvUserData_i );

t_CirList* cir_list_find_custom( t_CirList* poStartNode_i, void* pvData_i, fnCompareFunc pfnCompare_i );

int cir_list_equal( void* pvData1_i, void* pvData2_i );

void cir_list_iterate( t_CirList* poStartNode_i, t_CirList* poEndNode_i, fnNodeFunc pfOperation_i, void* pvUserData_i );

t_CirList* cir_list_first( t_CirList* poStartNode_i );

t_CirList* cir_list_last( t_CirList* poStartNode_i );

t_CirList* cir_list_next( t_CirList* poStartNode_i );

t_CirList* cir_list_previous( t_CirList* poStartNode_i );

void cir_list_free_1( t_CirList* poNode_i );

t_CirList* cir_list_alloc( void );

#endif /* _CIRCULAR_LIST_H_ */

