/*! @file ms_config_api.c
*
* @ingroup MsConfigApi
*
* @brief The file contains implementation of functions for media server configuration API.
*
* @author Razi-ud-Din Chaudhry
*
*/

#include "ms_config_api.h"

/*! @fn e_MsRetCode MSC_GetItemAsTString( xmlDocPtr pxmlDoc_i, char* pcNodeName_i, char* pcAttribute_i, t_String* poDestString_o )
*
* @brief This function gets the value of a media server configuration item as TString.
*
* @param[in] pxmlDoc_i is the xml documnent pointer to get value from.
*
* @param[in] pcNodeName_i is the name of the node which contains the requested attribute.
*
* @param[in] pcAttribute_i is the name of the attribute whose value is to be obtained.
*
* @param[out] poDestString_o is the pointer to the t_String variable in which the obtained value is to be stored.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode MSC_GetItemAsTString( xmlDocPtr pxmlDoc_i, char* pcNodeName_i, char* pcAttribute_i, t_String* poDestString_o )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;

	PRE_CONDITION
	{
		ms_assert_return( NULL != pxmlDoc_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		char* pcValue = NULL;
		xmlNodePtr poNode = NULL;
		poNode = xmlDocGetRootElement( pxmlDoc_i );
		if( poNode != NULL )
		{
			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*) pcNodeName_i ))
				{
					pcValue = xmlGetProp( poNode, (const xmlChar*) pcAttribute_i );
					if( NULL != pcValue )
					{
						TS_Set( pcValue, poDestString_o );
					}
					else
					{
						eRet_Code = MS_ERR_NOT_FOUND;
					}
					break;
				}
			}
			if( poNode == NULL )
			{
				eRet_Code = MS_ERR_END_OF_LIST;
			}
		}
		else
		{
			eRet_Code = MS_ERR_INVALID_HANDLE;
		}
	}

	POST_CONDITION
	{
	}

	return (eRet_Code);
}

/*! @fn e_MsRetCode MSC_GetItemAsUL( xmlDocPtr pxmlDoc_i, char* pcNodeName_i, char* pcAttribute_i, unsigned long* pulDestValue_o )
*
* @brief This function gets the value of a media server configuration item as unsigned long.
*
* @param[in] pxmlDoc_i is the xml documnent pointer to get value from.
*
* @param[in] pcNodeName_i is the name of the node which contains the requested attribute.
*
* @param[in] pcAttribute_i is the name of the attribute whose value is to be obtained.
*
* @param[out] pulDestValue_o is the pointer to the unsigned long variable in which the obtained value is to be stored.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode MSC_GetItemAsUL( xmlDocPtr pxmlDoc_i, char* pcNodeName_i, char* pcAttribute_i, unsigned long* pulDestValue_o )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;

	PRE_CONDITION
	{
		ms_assert_return( NULL != pxmlDoc_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		char* pcValue = NULL;
		xmlNodePtr poNode = NULL;
		poNode = xmlDocGetRootElement( pxmlDoc_i );
		if( poNode != NULL )
		{
			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*) pcNodeName_i ))
				{
					pcValue = xmlGetProp( poNode, (const xmlChar*) pcAttribute_i );
					if( NULL != pcValue )
					{
						char* pcTemp = NULL;
						*pulDestValue_o = strtoul( pcValue, &pcTemp, 10 );
					}
					else
					{
						eRet_Code = MS_ERR_NOT_FOUND;
					}
					break;
				}
			}
			if( poNode == NULL )
			{
				eRet_Code = MS_ERR_END_OF_LIST;
			}
		}
		else
		{
			eRet_Code = MS_ERR_INVALID_HANDLE;
		}
	}

	POST_CONDITION
	{
	}

	return (eRet_Code);
}

/*! @fn e_MsRetCode MSC_SetItemAsTString( xmlDocPtr pxmlDoc_i, char* pcNodeName_i, char* pcAttribute_i, t_String* poSrcString_i )
*
* @brief This function sets the value of a media server configuration item as TString.
*
* @param[in] pxmlDoc_i is the xml documnent pointer to set value in.
*
* @param[in] pcNodeName_i is the name of the node which contains the requested attribute.
*
* @param[in] pcAttribute_i is the name of the attribute whose value is to be set.
*
* @param[in] poSrcString_i is the pointer to the t_String variable which contains the value to be set.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode MSC_SetItemAsTString( xmlDocPtr pxmlDoc_i, char* pcNodeName_i, char* pcAttribute_i, t_String* poSrcString_i )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;

	PRE_CONDITION
	{
		ms_assert_return( NULL != pxmlDoc_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		char* pcString = NULL;
		xmlNodePtr poNode = NULL;
		poNode = xmlDocGetRootElement( pxmlDoc_i );
		if( poNode != NULL )
		{
			pcString = TS_GetStringZ( poSrcString_i );

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*) pcNodeName_i ))
				{
					if( NULL == xmlHasProp( poNode, pcAttribute_i ) )
					{
						xmlNewProp( poNode, (xmlChar*)pcAttribute_i, pcString );
					}
					else
					{
						xmlSetProp( poNode, (xmlChar*) pcAttribute_i, (xmlChar*) pcString );
					}
					break;
				}
			}
			if( poNode == NULL )
			{
				eRet_Code = MS_ERR_END_OF_LIST;
			}
			FW_Free( pcString );
		}
		else
		{
			eRet_Code = MS_ERR_INVALID_HANDLE;
		}
	}

	POST_CONDITION
	{
	}

	return (eRet_Code);
}

/*! @fn e_MsRetCode MSC_SetItemAsUL( xmlDocPtr pxmlDoc_i, char* pcNodeName_i, char* pcAttribute_i, unsigned long ulSrcValue_i )
*
* @brief This function sets the value of a media server configuration item as unsigned long.
*
* @param[in] pxmlDoc_i is the xml documnent pointer to get value in.
*
* @param[in] pcNodeName_i is the name of the node which contains the requested attribute.
*
* @param[in] pcAttribute_i is the name of the attribute whose value is to be set.
*
* @param[out] ulSrcValue_i is the unsigned long variable containing the value to be set.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode MSC_SetItemAsUL( xmlDocPtr pxmlDoc_i, char* pcNodeName_i, char* pcAttribute_i, unsigned long ulSrcValue_i )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;

	PRE_CONDITION
	{
		ms_assert_return( NULL != pxmlDoc_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		xmlNodePtr poNode = NULL;
		poNode = xmlDocGetRootElement( pxmlDoc_i );
		if( poNode != NULL )
		{
			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*) pcNodeName_i ))
				{
					unsigned char pcValue[33];
					if( 0 == TS_IntToString( pcValue, (int) ulSrcValue_i, 10 ))
					{
						eRet_Code = MS_ERR_FRAMEWORK;
					}
					else
					{
						if( NULL == xmlHasProp( poNode, pcAttribute_i ) )
						{
							xmlNewProp( poNode, (xmlChar*)pcAttribute_i, (xmlChar*) pcValue );
						}
						else
						{
							xmlSetProp( poNode, (xmlChar*)pcAttribute_i, (xmlChar*) pcValue );
						}
					}
					break;
				}
			}
			if( poNode == NULL )
			{
				eRet_Code = MS_ERR_END_OF_LIST;
			}
		}
		else
		{
			eRet_Code = MS_ERR_INVALID_HANDLE;
		}
	}

	POST_CONDITION
	{
	}

	return (eRet_Code);
}

