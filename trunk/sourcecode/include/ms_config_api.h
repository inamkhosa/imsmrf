/*! @file ms_config_api.h
*
* @ingroup MsConfigApi
*
* @brief The file contains defines for media server configuration API.
*
* @author Razi-ud-Din Chaudhry
*
*/

/*! @defgroup MsConfigApi Media Server Configuration API Module
*
* @brief This module consist of functionality for media server configuration.
*
* @author Razi-ud-Din Chaudhry
*
* @{
*
*/

#ifndef _MS_CONFIG_API_H_
#define _MS_CONFIG_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <libxml/parser.h>
#include "tstring.h"
#include "ms_ret_code.h"
#include "ms_debug.h"
#include "fw_api.h"

e_MsRetCode MSC_GetItemAsTString( xmlDocPtr pxmlDoc_i, char* pcNodeName, char* pcAttribute, t_String* poDestString_o );

e_MsRetCode MSC_GetItemAsUL( xmlDocPtr pxmlDoc_i, char* pcNodeName, char* pcAttribute, unsigned long* pulDestValue_o );

e_MsRetCode MSC_SetItemAsTString( xmlDocPtr pxmlDoc_i, char* pcNodeName, char* pcAttribute, t_String* poSrcString_i );

e_MsRetCode MSC_SetItemAsUL( xmlDocPtr pxmlDoc_i, char* pcNodeName, char* pcAttribute, unsigned long ulSrcValue_i );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*_MS_CONFIG_API_H_ */

/*! @} */ // End of MsConfigApi Module

