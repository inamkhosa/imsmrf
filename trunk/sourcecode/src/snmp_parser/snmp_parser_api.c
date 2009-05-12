/*! @file snmp_parser_api.c
*
* @ingroup SnmpParser
*
* @brief The file contains implementations for snmp parser API.
*
* @author Razi-ud-Din Chaudhry
*
*/

#include "snmp_priv.h"
#include "snmp_parser/snmp_parser_api.h"

/*! @fn xmlDocPtr SP_ParseXML( char* pcXMLDoc_i )
*
* @brief This function gives the xml document pointer of the snmp message.
*
* @param[in] pcXMLDoc_i is the pointer variable containing the snmp message to be parsed.
*
* @return The parsed snmp message as xml document pointer xmlDocPtr.
*
* @author Razi-ud-Din Chaudhry
*
*/

xmlDocPtr SP_ParseXML( char* pcXMLDoc_i )
{
	xmlDocPtr poXmlDocPtr = NULL;

	poXmlDocPtr = xmlParseMemory( pcXMLDoc_i, strlen( pcXMLDoc_i ) );

	return (poXmlDocPtr);
}

/*! @fn xmlDocPtr SP_GetResponse( void )
*
* @brief This function gives the xml document pointer of the snmp response template.
*
* @return The response message template as xml document pointer xmlDocPtr.
*
* @author Razi-ud-Din Chaudhry
*
*/

xmlDocPtr SP_GetResponse( void )
{
	xmlDocPtr poXmlDocPtr = NULL;

	char pcResponseTempl[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><RESPONSE/>";

	poXmlDocPtr = xmlParseMemory( pcResponseTempl, strlen( pcResponseTempl ) );

	return (poXmlDocPtr);
}

/*! @fn xmlDocPtr SP_GetNotification( void )
*
* @brief This function gives the xml document pointer of the snmp notification template.
*
* @return The notification message template as xml document pointer xmlDocPtr.
*
* @author Razi-ud-Din Chaudhry
*
*/

xmlDocPtr SP_GetNotification( void )
{
	xmlDocPtr poXmlDocPtr = NULL;

	char pcNotificationTempl[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><NOTIFICATION/>";

	poXmlDocPtr = xmlParseMemory( pcNotificationTempl, strlen( pcNotificationTempl ) );

	return (poXmlDocPtr);
}

/*! @fn char* SP_EncodeMessage( xmlDocPtr poXmlDoc_i, int* pnBytes_o )
*
* @brief This function encodes the snmp message.
*
* @param[in] poXmlDoc_i is the pointer variable containing the snmp message to be encoded.
*
* @param[out] pnBytes_o is the pointer variable containing the number of bytes encoded.
*
* @return The encoded snmp message as a character string.
*
* @author Razi-ud-Din Chaudhry
*
*/

char* SP_EncodeMessage( xmlDocPtr poXmlDoc_i, int* pnBytes_o )
{
	xmlChar* pxmlcEncodedRes;

	xmlDocDumpMemory( poXmlDoc_i, &pxmlcEncodedRes, pnBytes_o );

	return ((char*)pxmlcEncodedRes);
}

/*! @fn e_MsRetCode SP_GetVariableAttrAsString( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, char** ppcAttribute_o )
*
* @brief This function provides the value of the given attribute as string.
*
* @param[in] poXmlDoc_i is the xml document pointer of the snmp message.
*
* @param[in] nVarIndx_i is the index of the variable whose attribute is to be obtained.
*
* @param[in] pcField_i is the attribute whose value is to be obtained.
*
* @param[out] ppcAttribute_o is the pointer to the obtained value of the attribute.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode SP_GetVariableAttrAsString( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, char** ppcAttribute_o )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;

	xmlNodePtr pXMLNodeCur = NULL;
	xmlNodePtr pxnNodeCur = NULL;
	xmlChar* pxmlcAttribute = NULL;

	pXMLNodeCur = xmlDocGetRootElement( poXmlDoc_i );
	if( pXMLNodeCur == NULL ) // Check missing root element
	{
		eRet_Code = MS_ERR_FRAMEWORK;
	}
	else if( 0 != xmlStrcasecmp( pXMLNodeCur->name, (xmlChar*) "REQUEST" ))  // Check missing request node
	{
		eRet_Code = MS_ERR_INVALID_INPUT;
	}
	else    // Request node found
	{
		pxnNodeCur = SP_XMLGetNthChild( pXMLNodeCur, nVarIndx_i, (xmlChar*) "VARIABLE" );
		if( NULL == pxnNodeCur )    // Check missing child node of request node
		{
			eRet_Code = MS_ERR_INVALID_INPUT;
		}
		else    // Variable node found
		{
			for( pXMLNodeCur = pxnNodeCur->xmlChildrenNode; NULL != pXMLNodeCur; pXMLNodeCur = pXMLNodeCur->next )
			{
				if( 0 == xmlStrcasecmp( pXMLNodeCur->name, (xmlChar*) pcField_i ))  // check matching field name
				{
					break;
				}
				else
				{
					// Current field has not matched
				}
			}
			if( NULL == pXMLNodeCur )   // No field matched
			{
				eRet_Code = MS_ERR_END_OF_LIST;
			}
			else    // Matching field found
			{
				*ppcAttribute_o = (char*) xmlNodeGetContent( pXMLNodeCur );
				eRet_Code = MS_ERR_NONE;
			}
		}
	}
	return (eRet_Code);
}

/*! @fn e_MsRetCode SP_GetVariableAttrAsUL( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, unsigned long* pulAttribute_o )
*
* @brief This function provides the value of the given attribute as unsigned long.
*
* @param[in] poXmlDoc_i is the xml document pointer of the snmp message.
*
* @param[in] nVarIndx_i is the index of the variable whose attribute is to be obtained.
*
* @param[in] pcField_i is the attribute whose value is to be obtained.
*
* @param[out] pulAttribute_o is the pointer to the obtained value of the attribute.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode SP_GetVariableAttrAsUL( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, unsigned long* pulAttribute_o )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;
	xmlNodePtr pXMLNodeCur = NULL;
	xmlNodePtr pxnNodeCur = NULL;
	char* pcValue = NULL;

	pXMLNodeCur = xmlDocGetRootElement( poXmlDoc_i );
	if( pXMLNodeCur == NULL )     // Check missing root node
	{
		eRet_Code = MS_ERR_FRAMEWORK;
	}
	else if( 0 != xmlStrcasecmp( pXMLNodeCur->name, (xmlChar*) "REQUEST" ))  // Check missing request node
	{
		eRet_Code = MS_ERR_INVALID_INPUT;
	}
	else
	{
		pxnNodeCur = SP_XMLGetNthChild( pXMLNodeCur, nVarIndx_i, (xmlChar*) "VARIABLE" );
		if( NULL == pxnNodeCur )    // Check missing child nodes of request node
		{
			eRet_Code = MS_ERR_INVALID_INPUT;
		}
		else    // Variable node found
		{
			for( pXMLNodeCur = pxnNodeCur->xmlChildrenNode; NULL != pXMLNodeCur; pXMLNodeCur = pXMLNodeCur->next )
			{
				if( 0 == xmlStrcasecmp( pXMLNodeCur->name, (xmlChar*) pcField_i ))  // Check matching field name
				{
					break;
				}
				else
				{
					// Current field has not matched
				}
			}
			if( NULL == pXMLNodeCur )   // No field matched
			{
				eRet_Code = MS_ERR_END_OF_LIST;
			}
			else    // Matching field found
			{
				pcValue = xmlNodeGetContent( pXMLNodeCur );
				if( NULL != pcValue )
				{
					errno = 0;
					char* pcTemp = NULL;
					long lValue = strtol( pcValue, &pcTemp, 10 );
					if((errno == ERANGE && (lValue == LONG_MAX || lValue == LONG_MIN)) || (errno != 0 && lValue == 0) || lValue < 0 )
					{
						eRet_Code = MS_ERR_NOT_IMPLEMENTED;
					}
					else
					{
						*pulAttribute_o = lValue;
						eRet_Code = MS_ERR_NONE;
					}
					xmlFree( pcValue );
				}
				else
				{
					eRet_Code = MS_ERR_END_OF_LIST;
				}
			}
		}
	}
	return (eRet_Code);
}

/*! @fn e_MsRetCode SP_SetAttrAsString( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, char* pcAttribute_i )
*
* @brief This function sets the value of the given attribute as string.
*
* @param[in] poXmlDoc_i is the xml document pointer of the snmp message.
*
* @param[in] nVarIndx_i is the index of the variable whose attribute value is to be set.
*
* @param[in] pcField_i is the attribute whose value is to be set.
*
* @param[in] pcAttribute_i is the value of the attribute to be set.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode SP_SetAttrAsString( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, char* pcAttribute_i )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;

	xmlNodePtr pXMLNodeCur = NULL;
	xmlNodePtr pxnNodeCur = NULL;

	pXMLNodeCur = xmlDocGetRootElement( poXmlDoc_i );
	if( pXMLNodeCur == NULL ) // Check missing root node
	{
		eRet_Code = MS_ERR_FRAMEWORK;
	}
	else    // Root node found
	{
		if( 0 != xmlStrcasecmp( pXMLNodeCur->name, (const xmlChar*)"RESPONSE" ) && 0 != xmlStrcasecmp( pXMLNodeCur->name, (const xmlChar*)"NOTIFICATION" ))    // Check missing both response node and notification node
		{
			eRet_Code = MS_ERR_FRAMEWORK;
		}
		else    // Either response node of notification node found
		{
			if( 0 == xmlStrcasecmp( pXMLNodeCur->name, (xmlChar*) "RESPONSE" ))  // Check response node
			{
				pxnNodeCur = SP_XMLGetNthChild( pXMLNodeCur, nVarIndx_i, (xmlChar*) "VARIABLE" );
			}
			else    // Notification node found
			{
				pxnNodeCur = SP_XMLGetNthChild( pXMLNodeCur, nVarIndx_i, (xmlChar*) "TRAP" );
			}
			if( NULL == pxnNodeCur )    // Check Missing Child Node
			{
				if( 0 == xmlStrcasecmp( pXMLNodeCur->name, (xmlChar*) "RESPONSE" ))  // Check response node
				{
					// Create variable node
					pxnNodeCur = xmlNewChild( pXMLNodeCur, NULL, (xmlChar*)"VARIABLE", NULL );
				}
				else    // notification node found
				{
					// Create trap node
					pxnNodeCur = xmlNewChild( pXMLNodeCur, NULL, (xmlChar*)"TRAP", NULL );
				}
				if( NULL == pxnNodeCur )
				{
					eRet_Code = MS_ERR_FRAMEWORK;
				}
				else
				{
					eRet_Code = MS_ERR_NONE;
				}
			}
			else
			{
				eRet_Code = MS_ERR_NONE;
			}
			if( eRet_Code == MS_ERR_NONE )
			{
				// Add new attribute
				pXMLNodeCur = xmlNewChild( pxnNodeCur, NULL, (xmlChar*) pcField_i, NULL );
				if( NULL == pXMLNodeCur )
				{
					eRet_Code = MS_ERR_FRAMEWORK;
				}
				else
				{
					xmlNodeAddContent( pXMLNodeCur, (xmlChar*) pcAttribute_i );
				}
			}
			else
			{
				// Error while setting variable attribute
			}
		}
	}
	return (eRet_Code);
}

/*! @fn e_MsRetCode SP_SetAttrAsUL( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, unsigned long ulAttribute_i )
*
* @brief This function sets the value of the given attribute as unsigned long.
*
* @param[in] poXmlDoc_i is the xml document pointer of the snmp message.
*
* @param[in] nVarIndx_i is the index of the variable whose attribute value is to be set.
*
* @param[in] pcField_i is the attribute whose value is to be set.
*
* @param[in] ulAttribute_i is the value of the attribute to be set.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode SP_SetAttrAsUL( xmlDocPtr poXmlDoc_i, int nVarIndx_i, char* pcField_i, unsigned long ulAttribute_i )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;

	xmlNodePtr pXMLNodeCur = NULL;
	xmlNodePtr pxnNodeCur = NULL;

	pXMLNodeCur = xmlDocGetRootElement( poXmlDoc_i );
	if( pXMLNodeCur == NULL ) // Check missing root node
	{
		eRet_Code = MS_ERR_FRAMEWORK;
	}
	else    // Root node found
	{
		if( 0 != xmlStrcasecmp( pXMLNodeCur->name, (const xmlChar*)"RESPONSE" ) && 0 != xmlStrcasecmp( pXMLNodeCur->name, (const xmlChar*)"NOTIFICATION" ))   // Check missing both response node and notification node
		{
			eRet_Code = MS_ERR_FRAMEWORK;
		}
		else    // Either response node or notification node found
		{
			if( 0 == xmlStrcasecmp( pXMLNodeCur->name, (xmlChar*) "RESPONSE" ))  // Check response node
			{
				pxnNodeCur = SP_XMLGetNthChild( pXMLNodeCur, nVarIndx_i, (xmlChar*) "VARIABLE" );
			}
			else    // Notification node found
			{
				pxnNodeCur = SP_XMLGetNthChild( pXMLNodeCur, nVarIndx_i, (xmlChar*) "TRAP" );
			}
			if( NULL == pxnNodeCur )    // Check missing child node
			{
				if( 0 == xmlStrcasecmp( pXMLNodeCur->name, (xmlChar*) "RESPONSE" ))  // Check response node
				{
					// Create variable node
					pxnNodeCur = xmlNewChild( pXMLNodeCur, NULL, (xmlChar*)"VARIABLE", NULL );
				}
				else    // notification node found
				{
					// Create trap node
					pxnNodeCur = xmlNewChild( pXMLNodeCur, NULL, (xmlChar*)"TRAP", NULL );
				}
				if( NULL == pxnNodeCur )
				{
					eRet_Code = MS_ERR_FRAMEWORK;
				}
				else
				{
					eRet_Code = MS_ERR_NONE;
				}
			}
			else
			{
				eRet_Code = MS_ERR_NONE;
			}
			if( eRet_Code == MS_ERR_NONE )
			{
				// Add new attribute
				pXMLNodeCur = xmlNewChild( pxnNodeCur, NULL, (xmlChar*) pcField_i, NULL );
				if( NULL == pXMLNodeCur )
				{
					eRet_Code = MS_ERR_FRAMEWORK;
				}
				else
				{
					unsigned char pcAttribute[33];
					if( 0 == TS_IntToString( pcAttribute, (int) ulAttribute_i, 10 ))
					{
						eRet_Code = MS_ERR_FRAMEWORK;
					}
					else
					{
						xmlNodeAddContent( pXMLNodeCur, (xmlChar*) pcAttribute );
					}
				}
			}
			else
			{
				// Error while setting variable attribute
			}
		}
	}
	return (eRet_Code);
}

