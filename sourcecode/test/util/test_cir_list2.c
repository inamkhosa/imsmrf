#include "util/circular_list.h"

void PrintNode( void* pvData, void* pvUserData_i )
{
	printf( "%s\t", (char*)pvData );
}

int string_compare( void* pvData1_i, void* pvData2_i )
{
	printf( "Comparing %s and %s\n", (char*)pvData1_i, (char*)pvData2_i );
	return (strcmp((char*)pvData1_i, (char*)pvData2_i ));
}

void string_free( void* pvData_i, void* pvUserData_i )
{
	FW_Free( pvData_i );
}

int main( void )
{
	int count = 0;
	t_CirList* poList1 = NULL;
	char pcData[100] = "";

	void* pvAMPSContext = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );

	for( count = 0; count < 10; count++ )
	{
		sprintf( pcData, "%c-%d", 'a'+10-count, count );
		printf( "Appending %s\n", pcData );
		poList1 = cir_list_insert_sorted( poList1, (void*)strdup( pcData ), string_compare );
		printf( "length of list = %d\n", cir_list_length( poList1 ));
		//cir_list_foreach(poList1, PrintNode, NULL);
		cir_list_iterate( poList1, poList1, PrintNode, NULL );
		printf( "\n" );
	}
	printf( "Removing some elements \n\n" );

	for( count = 0; count < 10; count += 2 )
	{
		sprintf( pcData, "%c-%d", 'a'+10-count, count );
		printf( "Removing %s\n", pcData );
		poList1 = cir_list_delete( poList1, (void*)pcData, string_compare, string_free, NULL );
		printf( "length of list = %d\n", cir_list_length( poList1 ));
		cir_list_foreach( poList1, PrintNode, NULL );
		printf( "\n" );
	}
	cir_list_free_custom( poList1, string_free, NULL );

	FW_Main( pvAMPSContext );

	FW_Cleanup( pvAMPSContext );

	return (0);
}

