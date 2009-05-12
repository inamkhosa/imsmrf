/*! @file ms_api.h
*
* @brief The file contains defines for media server API.
*
* @author Razi-ud-Din Chaudhry
*
*/

#ifndef _MS_API_H_
#define _MS_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ms.h"

e_MsRetCode MS_GetNumOfRTPPackets( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, e_MediaCompType eCompType_i, e_MediaCompConfig eConfig_i, unsigned int* punValue_o );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MS_API_H_ */

