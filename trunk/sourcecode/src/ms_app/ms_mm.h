#ifndef _MS_MM_H_
#define _MS_MM_H_

#include "fw_api.h"

void* MS_Malloc( void* hMm_i, unsigned int unSize_i );

void MS_Free( void* hMm_i, void* pvBuffer_i );

#endif /* _MS_MM_H_ */

