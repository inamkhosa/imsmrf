#include "ms_mm.h"

/**************
FUNCTION NAME		: MS_Malloc
DESCRIPTION		:
FUNCTION PARAMETERS
hMm_i		: Handle of Memory Manager
unSize_i	: Number of bytes
RETURN TYPE
void*       : Pointer to allocated buffer or NULL in case when memory could not be allocated
**************/
void* MS_Malloc( void* hMm_i, unsigned int unSize_i )
{
	return (malloc( unSize_i ));
}

/**************
FUNCTION NAME		: MS_Free
DESCRIPTION		:
FUNCTION PARAMETERS
hMm_i		: Handle of Memory Manager
pvBuffer_i	: Pointer to the buffer to deallocate
RETURN TYPE
void        :
**************/
void MS_Free( void* hMm_i, void* pvBuffer_i )
{
	free( pvBuffer_i );
}

