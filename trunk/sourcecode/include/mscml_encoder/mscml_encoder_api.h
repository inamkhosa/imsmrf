/*! @file mscml_encoder_api.h
*
* @ingroup MscmlEncoder
*
* @brief The file contains defines for MSCML encoder API.
*
*/

/*! @defgroup MscmlEncoder MSCML Encoder Module
*
* @brief This module consists of functionality for encoding and parsing MSCML messages.
*
* @{
*
*/

#ifndef _MSCML_ENCODER_API_H_
#define _MSCML_ENCODER_API_H_
#include <limits.h>

#include <libxml/xmlversion.h>
#include <libxml/parser.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/xmlschemas.h>
#include <libxml/xmlschemastypes.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "fw_api.h"
#include "ms_ret_code.h"
#include "ms_events.h"

extern const int nInfiniteValue_c;
extern const int nImmediateValue_c;

xmlDocPtr GetMscmlResponse( void );

e_MsRetCode GetPromptAttrAsBool( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, AMPS_BOOL* pbAttrValue_o );

e_MsRetCode GetRequestAttrAsString( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, char** ppcAttrValue_o );

e_MsRetCode GetRequestAttrAsBool( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, AMPS_BOOL* pbAttrValue_o );

e_MsRetCode GetRequestAttrAsChar( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, char* pcAttrValue_o );

e_MsRetCode GetRequestAttrAsInt( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, int* pnAttrVal_o );

e_MsRetCode GetRequestAttrAsMilliSec( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, int* pnAttrVal_o );

e_MsRetCode GetPatternAttrAsString( xmlDocPtr poXmlDoc_i, char* pcNodeName_i, int nIndex_i, char* pcAttrName_i, char** ppcAttrValue_o );

e_MsRetCode SetResponseAttrAsInt( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int nAttrVal_i );

e_MsRetCode SetResponseAttrAsString( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, char* pcAttrValue_i );

e_MsRetCode SetResponseErrorInfoAttrAsInt( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int nAttrVal_i );

e_MsRetCode SetResponseErrorInfoAttrAsString( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, char* pcAttrVal_i );

e_MsRetCode GetConfigureTeamAttrAsString( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, char** ppAttrValue_o );

e_MsRetCode GetTeamMateId( xmlDocPtr poMscmlDoc_i, unsigned int unTeamMateIndex_i, xmlChar** ppchTeamMateId_o );

e_MsRetCode SetTeamAttrAsString( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, char* pcAttrValue_i );

e_MsRetCode SetTeamAttrAsInt( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, int nAttrValue_i );

e_MsRetCode AddTeamMateId( xmlDocPtr poMscmlDoc_i, xmlChar* poId_i );

e_MsRetCode AppendBaseUrl( xmlDocPtr poMscmlDoc_i, char* pcSoundDir_i );

e_MsRetCode GetConfigureConfAttrAsInt( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, int* pnAttrVal_o );

e_MsRetCode GetConfigureConfAttrAsBool( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, AMPS_BOOL* pbAttrValue_o );

e_MsRetCode GetResponseAttrAsString( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, char** ppcAttrValue_o );

e_MsRetCode GetResponseAttrAsInt( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int* pnAttrVal_o );

#endif

/*! @} */ // End of MscmlEncoder Module

