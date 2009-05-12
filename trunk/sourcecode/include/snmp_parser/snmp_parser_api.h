/*! @file snmp_parser_api.h
*
* @ingroup SnmpParser
*
* @brief The file contains defines of functions for snmp parser API.
*
* @author Razi-ud-Din Chaudhry
*
*/

/*! @defgroup SnmpParser Snmp Parser Module
*
* @ingroup Snmp
*
* @brief This module consist of functionality for parsing messages from snmp agent
*
* @author Razi-ud-Din Chaudhry
*
* @{
*
*/

#ifndef _SNMP_PARSER_API_H_
#define _SNMP_PARSER_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <libxml/parser.h>
#include "ms_ret_code.h"

xmlDocPtr SP_ParseXML( char* pcXMLDoc_i );

xmlDocPtr SP_GetResponse( void );

xmlDocPtr SP_GetNotification( void );

char* SP_EncodeMessage( xmlDocPtr poXmlDoc_i, int* pnBytes_o );

e_MsRetCode SP_GetVariableAttrAsString( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, char** ppcAttribute_o );

e_MsRetCode SP_GetVariableAttrAsUL( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, unsigned long* pulAttribute_o );

e_MsRetCode SP_SetAttrAsString( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, char* pcAttribute_i );

e_MsRetCode SP_SetAttrAsUL( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, unsigned long ulAttribute_i );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /*_SNMP_PARSER_API_H_*/

/*! @} */ // End of SnmpParser Module

