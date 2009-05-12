#include "util/circular_list.h"

void PrintNode( void* pvData, void* pvUserData_i )
{
	printf( "%d\t", (int)pvData );
}

int main( void )
{
	int count = 0;
	t_CirList* poList1 = NULL;

	void* pvAMPSContext = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );

	for( count = 0; count < 10; count++ )
	{
		printf( "Appending %d\n", count );
		poList1 = cir_list_append( poList1, (void*)count );
		printf( "length of list = %d\n", cir_list_length( poList1 ));
		cir_list_foreach( poList1, PrintNode, NULL );
		printf( "\n" );
	}
	printf( "Removing some elements \n\n" );

	for( count = 0; count < 10; count += 2 )
	{
		printf( "Removing %d\n", count );
		poList1 = cir_list_remove( poList1, (void*)count );
		printf( "length of list = %d\n", cir_list_length( poList1 ));
		cir_list_foreach( poList1, PrintNode, NULL );
		printf( "\n" );
	}
	cir_list_free( poList1 );

	FW_Main( pvAMPSContext );

	FW_Cleanup( pvAMPSContext );

	return (0);
}

