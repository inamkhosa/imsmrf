/*! @file snmp_priv.h
*
* @ingroup SnmpParser
*
* @brief The file contains defines for additional functions of snmp parser.
*
* @author Razi-ud-Din Chaudhry
*
*/

#ifndef _SNMP_PRIV_H_
#define _SNMP_PRIV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <libxml/parser.h>
#include "limits.h"
#include "tstring.h"

xmlNodePtr SP_XMLGetNthChild( xmlNodePtr pXMLParentNode_i, int nChildIndx_i, xmlChar* pxmlChildValue_i );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /*_SNMP_PRIV_H_*/

