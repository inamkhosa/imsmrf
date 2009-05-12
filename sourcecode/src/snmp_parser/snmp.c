/*! @file snmp.c
*
* @ingroup SnmpParser
*
* @brief The file contains implementations for additional functions of snmp parser.
*
* @author Razi-ud-Din Chaudhry
*
*/

#include "snmp_priv.h"

/*! @fn xmlNodePtr SP_XMLGetNthChild( xmlNodePtr pXMLParentNode_i, int nChildIndx_i, xmlChar* pxmlChildValue_i )
*
* @brief This function gets nth xml child node for the given parent node.
*
* @param[in] pXMLParentNode_i is the xml node pointer of the parent node whose child is to be created.
*
* @param[in] nChildIndx_i is the index of the xml child node.
*
* @param[in] pxmlcValue_i is the name of the requested child node.
*
* @return An xml node pointer (xmlNodePtr) of the child node at the given index or null if no child exists at that node.
*
* @author Razi-ud-Din Chaudhry
*
*/

xmlNodePtr SP_XMLGetNthChild( xmlNodePtr pXMLParentNode_i, int nChildIndx_i, xmlChar* pxmlChildValue_i )
{
	xmlNodePtr pxmlChildNode = NULL;
	int nCurrentIndx = 1;

	switch( nChildIndx_i )
	{
		case 0:     // first node

			for( pxmlChildNode = pXMLParentNode_i->xmlChildrenNode; NULL != pxmlChildNode; pxmlChildNode = pxmlChildNode->next )
			{
				if( 0 == xmlStrcasecmp( pxmlChildNode->name, pxmlChildValue_i ))
				{
					break;
				}
			}
			break;

		case -1:    // last node

			for( pxmlChildNode = pXMLParentNode_i->last; NULL != pxmlChildNode; pxmlChildNode = pxmlChildNode->prev )
			{
				if( 0 == xmlStrcasecmp( pxmlChildNode->name, pxmlChildValue_i ))
				{
					break;
				}
			}
			break;

		default:    // node at location (1-based)

			for( pxmlChildNode = pXMLParentNode_i->xmlChildrenNode; NULL != pxmlChildNode; pxmlChildNode = pxmlChildNode->next )
			{
				if( 0 == xmlStrcasecmp( pxmlChildNode->name, pxmlChildValue_i ))
				{
					if( nCurrentIndx == nChildIndx_i )
					{
						break;
					}
					else
					{
						nCurrentIndx++;
					}
				}
			}
			break;
	}

	return (pxmlChildNode);
}

