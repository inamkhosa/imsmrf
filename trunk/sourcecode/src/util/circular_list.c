#include "util/circular_list.h"

t_CirList* cir_list_append( t_CirList* poStartNode_i, void* pvData_i )
{
	t_CirList* poNode = NULL;

	poNode = cir_list_alloc();
	if( NULL != poNode )
	{
		poNode->pvData = pvData_i;
		if( NULL == poStartNode_i )
		{
			poNode->poNext = poNode->poPrev = poNode;
		}
		else
		{
			poNode = cir_list_insert_after( poStartNode_i->poPrev, poNode );
		}
	}
	return (poNode);
}

t_CirList* cir_list_remove( t_CirList* poStartNode_i, void* pvData_i )
{
	t_CirList* poNode = NULL;
	AMPS_BOOL bNodeFound = AMPS_FALSE;
	if( NULL != poStartNode_i )
	{
		poNode = cir_list_first( poStartNode_i );

		do
		{
			if( poNode->pvData == pvData_i )
			{
				bNodeFound = AMPS_TRUE;
				break;
			}
			poNode = cir_list_next( poNode );
		}
		while( poNode != cir_list_first( poStartNode_i ));
		if( AMPS_TRUE == bNodeFound )
		{
			poStartNode_i = cir_list_remove_link( poStartNode_i, poNode );

			cir_list_free_1( poNode );
		}
	}
	return (poStartNode_i);
}

t_CirList* cir_list_remove_link( t_CirList* poStartNode_i, t_CirList* poNode_i )
{
	poNode_i->poPrev->poNext = poNode_i->poNext;
	poNode_i->poNext->poPrev = poNode_i->poPrev;
	if( poNode_i == poStartNode_i )
	{
		poStartNode_i = poNode_i->poNext;
	}
	return (poStartNode_i);
}

t_CirList* cir_list_delete( t_CirList* poStartNode_i, void* pvData_i, fnCompareFunc pfnCompare_i, fnNodeFunc pfnDelete_i, void* pvUserData_i )
{
	t_CirList* poNode = NULL;
	AMPS_BOOL bNodeFound = AMPS_FALSE;
	if( NULL != poStartNode_i )
	{
		poNode = cir_list_first( poStartNode_i );

		do
		{
			if( pfnCompare_i( poNode->pvData, pvData_i ) == 0 )
			{
				bNodeFound = AMPS_TRUE;
				break;
			}
			poNode = cir_list_next( poNode );
		}
		while( poNode != cir_list_first( poStartNode_i ));
		if( AMPS_TRUE == bNodeFound )
		{
			poNode->poPrev->poNext = poNode->poNext;
			poNode->poNext->poPrev = poNode->poPrev;
			if( poNode == poStartNode_i )
			{
				poStartNode_i = poNode->poNext;
			}
			if( NULL != pfnDelete_i )
			{
				pfnDelete_i( poNode->pvData, pvUserData_i );
			}
			cir_list_free_1( poNode );
		}
	}
	return (poStartNode_i);
}

t_CirList* cir_list_insert_after( t_CirList* poStartNode_i, t_CirList* poNodeToInsert_i )
{
	poNodeToInsert_i->poNext = poStartNode_i->poNext;
	poNodeToInsert_i->poPrev = poStartNode_i;

	poStartNode_i->poNext->poPrev = poNodeToInsert_i;
	poStartNode_i->poNext = poNodeToInsert_i;
}

void cir_list_free( t_CirList* poStartNode_i )
{
	t_CirList* poNode = NULL;
	t_CirList* poNextNode = NULL;
	t_CirList* poPrevNode = NULL;
	if( NULL != poStartNode_i )
	{
		poNode = poStartNode_i;
		poPrevNode = poStartNode_i->poPrev;

		do
		{
			poNextNode = cir_list_next( poNode );

			cir_list_free_1( poNode );

			poNode = poNextNode;
		}
		while( poNode != poPrevNode );

		// remove the last node
		cir_list_free_1( poNode );
	}
}

void cir_list_free_custom( t_CirList* poStartNode_i, fnNodeFunc pfOperation_i, void* pvUserData_i )
{
	t_CirList* poNode = NULL;
	t_CirList* poNextNode = NULL;
	t_CirList* poPrevNode = NULL;
	if( NULL != poStartNode_i )
	{
		poNode = poStartNode_i;
		poPrevNode = poStartNode_i->poPrev;

		do
		{
			poNextNode = cir_list_next( poNode );

			pfOperation_i( poNode->pvData, pvUserData_i );

			cir_list_free_1( poNode );

			poNode = poNextNode;
		}
		while( poNode != poPrevNode );

		pfOperation_i( poNode->pvData, pvUserData_i );
		// remove the last node
		cir_list_free_1( poNode );
	}
}

unsigned int cir_list_length( t_CirList* poStartNode_i )
{
	t_CirList* poNode = NULL;
	unsigned int unLength = 0;
	if( NULL != poStartNode_i )
	{
		poNode = cir_list_first( poStartNode_i );

		do
		{
			unLength++;

			poNode = cir_list_next( poNode );
		}
		while( poNode != cir_list_first( poStartNode_i ));
	}
	else
	{
		unLength = 0;
	}
	return (unLength);
}

void cir_list_iterate( t_CirList* poStartNode_i, t_CirList* poEndNode_i, fnNodeFunc pfOperation_i, void* pvUserData_i )
{
	t_CirList* poNode = NULL;
	unsigned int unLength = 0;
	if( NULL != poStartNode_i )
	{
		poNode = cir_list_first( poStartNode_i );

		while( poNode != poEndNode_i )
		{
			pfOperation_i( poNode->pvData, pvUserData_i );

			poNode = cir_list_next( poNode );
		}
	}
}

void cir_list_foreach( t_CirList* poStartNode_i, fnNodeFunc pfOperation_i, void* pvUserData_i )
{
	t_CirList* poNode = NULL;
	unsigned int unLength = 0;
	if( NULL != poStartNode_i )
	{
		poNode = cir_list_first( poStartNode_i );

		do
		{
			pfOperation_i( poNode->pvData, pvUserData_i );

			poNode = cir_list_next( poNode );
		}
		while( poNode != cir_list_first( poStartNode_i ));
	}
}

int cir_list_equal( void* pvData1_i, void* pvData2_i )
{
	return (!(pvData1_i == pvData2_i));
}

t_CirList* cir_list_find_custom( t_CirList* poStartNode_i, void* pvData_i, fnCompareFunc pfnCompare_i )
{
	t_CirList* poNode = NULL;
	AMPS_BOOL bNodeFound = AMPS_FALSE;

	poNode = cir_list_first( poStartNode_i );

	do
	{
		if( pfnCompare_i( poNode->pvData, pvData_i ) == 0 )
		{
			bNodeFound = AMPS_TRUE;
			break;
		}
		poNode = cir_list_next( poNode );
	}
	while( poNode != cir_list_first( poStartNode_i ));
	if( AMPS_FALSE == bNodeFound )
	{
		poNode = NULL;
	}
	return (poNode);
}

t_CirList* cir_list_insert_sorted( t_CirList* poStartNode_i, void* pvData_i, fnCompareFunc pfnCompare_i )
{
	t_CirList* poNode = NULL;
	if( NULL == poStartNode_i )
	{
		poStartNode_i = cir_list_append( poStartNode_i, pvData_i );
	}
	else
	{
		AMPS_BOOL bNodeFound = AMPS_FALSE;
		poNode = cir_list_first( poStartNode_i );

		do
		{
			if( pfnCompare_i( poNode->pvData, pvData_i ) > 0 )
			{
				poNode = cir_list_append( poNode, pvData_i );
				bNodeFound = AMPS_TRUE;
				break;
			}
			poNode = cir_list_next( poNode );
		}
		while( poNode != cir_list_first( poStartNode_i ));
		if( AMPS_TRUE == bNodeFound )
		{
			poStartNode_i = poNode;
		}
		else
		{
			poStartNode_i = cir_list_append( poNode, pvData_i );
		}
	}
	return (poStartNode_i);
}

t_CirList* cir_list_first( t_CirList* poStartNode_i )
{
	return (poStartNode_i);
}

t_CirList* cir_list_last( t_CirList* poStartNode_i )
{
	return (poStartNode_i->poPrev);
}

t_CirList* cir_list_next( t_CirList* poStartNode_i )
{
	return (poStartNode_i->poNext);
}

t_CirList* cir_list_previous( t_CirList* poStartNode_i )
{
	return (poStartNode_i->poPrev);
}

void cir_list_free_1( t_CirList* poNode_i )
{
	FW_Free( poNode_i );
}

t_CirList* cir_list_alloc( void )
{
	return (FW_Malloc( sizeof (t_CirList)));
}

