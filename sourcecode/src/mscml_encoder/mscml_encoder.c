/*! @file mscml_encoder.c
*
* @ingroup MscmlEncoder
*
* @brief  The file contains template based implementations of functions for generating MSCML <response>.
*
*/

#include "mscml_encoder.h"

/*! MSCML Response Template */
char pcResponseTempl[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\
<MediaServerControl version=\"1.0\"><response request=\"\" code=\"\" text=\"\"></response></MediaServerControl>"
;

/*! MSCML Play Request Template */
char cpPlayReqTemplate[] = "<?xml version=\"1.0\"?>\
<MediaServerControl version=\"1.0\"><request><play ></play></request></MediaServerControl>"                                                     ;
/*! Constant integer variable for providing infinite time value in milliseconds equal to the maximum limit for integers. Used by @ref GetPromptAttrAsMilliSec and @ref GetRequestAttrAsMilliSec functions */
const int nInfiniteValue_c = INT_MAX;
/*! Constant integer variable for providing immediate time value in milliseconds having value equal to zero. Used by @ref GetPromptAttrAsMilliSec and @ref GetRequestAttrAsMilliSec functions */
const int nImmediateValue_c = 0;

/*! @fn xmlDocPtr GetMscmlResponse( void )
*
* @brief This function provides MSCML response template as xml document pointer
*
* @return MSCML response message template as xml document pointer xmlDocPtr
*
*/

xmlDocPtr GetMscmlResponse( void )
{
	xmlDocPtr poXmlDocPtr = NULL;

	poXmlDocPtr = xmlParseMemory( pcResponseTempl, sizeof (pcResponseTempl)-1 );
	if( poXmlDocPtr == NULL )
	{
		return (NULL);
	}
	return (poXmlDocPtr);
}

/*! @fn e_MsRetCode GetPromptAttrAsBool( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, AMPS_BOOL* pbAttrValue_o )
*
* @brief This function gets an attribute of prompt node as a boolean value.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML message.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] pbAttrValue_o is the pointer to variable for storing the obtained boolean value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode GetPromptAttrAsBool( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, AMPS_BOOL* pbAttrValue_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"play" ) ||
				    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playrecord" ) ||
				    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playcollect" ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				bNodePresent = AMPS_FALSE;

				for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
				{
					if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"prompt" ) )
					{
						bNodePresent = AMPS_TRUE;
						break;
					}
				}
				if( AMPS_TRUE == bNodePresent )
				{
					xmlChar* poPromptValue = NULL;

					poPromptValue = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
					if( NULL != poPromptValue )
					{
						if( 0 == xmlStrcasecmp( poPromptValue, (xmlChar*)"yes" ))
						{
							*pbAttrValue_o = AMPS_TRUE;
						}
						else if( 0 == xmlStrcasecmp( poPromptValue, (xmlChar*)"no" ))
						{
							*pbAttrValue_o = AMPS_FALSE;
						}
						xmlFree( poPromptValue );
					}
				}
				else
				{
					eRetCode = MS_ERR_INVALID_INPUT;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetRequestAttrAsString( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, char** ppcAttrValue_o )
*
* @brief This function gets an attribute from the request as a string.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] poReqName_i is the name of the request.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] ppcAttrValue_o is the pointer to variable for storing the obtained string value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
* @note The caller of this function must deallocate the memory for string value returned by using xmlFree() function.
*
*/

e_MsRetCode GetRequestAttrAsString( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, char** ppcAttrValue_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)poReqName_i ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				*ppcAttrValue_o = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetRequestAttrAsBool( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, AMPS_BOOL* pbAttrValue_o )
*
* @brief This function gets an attribute from the request as a boolean value.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] poReqName_i is the name of the request.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] pbAttrValue_o is the pointer to variable for storing the obtained boolean value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode GetRequestAttrAsBool( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, AMPS_BOOL* pbAttrValue_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)poReqName_i ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				xmlChar* poPromptValue = NULL;

				poPromptValue = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
				if( NULL != poPromptValue )
				{
					if( 0 == xmlStrcasecmp( poPromptValue, (xmlChar*)"yes" ))
					{
						*pbAttrValue_o = AMPS_TRUE;
					}
					else if( 0 == xmlStrcasecmp( poPromptValue, (xmlChar*)"no" ))
					{
						*pbAttrValue_o = AMPS_FALSE;
					}
					xmlFree( poPromptValue );
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetRequestAttrAsChar( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, char* pcAttrValue_o )
*
* @brief This function gets an attribute from the request as a character.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] poReqName_i is the name of the request.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] pcAttrValue_o is the pointer to variable for storing the obtained character value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode GetRequestAttrAsChar( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, char* pcAttrValue_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)poReqName_i ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				xmlChar* pcAttrValue = NULL;
				pcAttrValue = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
				if( NULL != pcAttrValue )
				{
					pcAttrValue_o[0] = *pcAttrValue;
					xmlFree( pcAttrValue );
				}
				else
				{
					eRetCode = MS_ERR_END_OF_LIST;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetRequestAttrAsInt( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, int* pnAttrVal_o )
*
* @brief This function gets an attribute from the request as an integer value.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] poReqName_i is the name of the request.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] pnAttrVal_o is the pointer to variable for storing the obtained integer value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode GetRequestAttrAsInt( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, int* pnAttrVal_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)poReqName_i ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				t_String oPromptValue = TSTRING_INITIALIZER;

				oPromptValue.pucData = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
				if( NULL != oPromptValue.pucData )
				{
					oPromptValue.unLength = strlen( oPromptValue.pucData );
					*pnAttrVal_o = TS_StringToInt( &oPromptValue, 10 );
					TS_Free( &oPromptValue );
				}
				else
				{
					eRetCode = MS_ERR_END_OF_LIST;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetRequestAttrAsMilliSec( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, int* pnAttrVal_o )
*
* @brief This function gets an attribute from the request as a integer time value in milliseconds.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] poReqName_i is the name of the request.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] pnAttrVal_o is the pointer to variable for storing the obtained integer value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
* @Note If the value of the attribute is in seconds, this function will convert that value into milliseconds, and then return the converted value. Similarly if the value for the given attribute is infinite or immediate, then the value for @ref nInfiniteValue_c and @ref nImmediateValue_c variables respectively will be used.
*
*/

e_MsRetCode GetRequestAttrAsMilliSec( xmlDocPtr poXmlDoc_i, char* poReqName_i, char* pcAttrName_i, int* pnAttrVal_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)poReqName_i ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				t_String oPromptValue = TSTRING_INITIALIZER;
				t_String oInfinite = {"infinite", sizeof ("infinite")-1};
				t_String oImmediate = {"immediate", sizeof ("immediate")-1};

				oPromptValue.pucData = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
				if( NULL != oPromptValue.pucData )
				{
					oPromptValue.unLength = strlen( oPromptValue.pucData );
					if( 0 == TS_CompareCase( &oPromptValue, &oInfinite ) )
					{
						*pnAttrVal_o = nInfiniteValue_c;
					}
					else if( 0 == TS_CompareCase( &oPromptValue, &oImmediate ) )
					{
						*pnAttrVal_o = nImmediateValue_c;
					}
					else
					{
						int nMultiplier = 1;
						t_String oMs = {"ms", sizeof ("ms")-1};
						t_String oSec = {"s", sizeof ("s")-1};
						if( 0 <  TS_CompareSubString( &oPromptValue, &oMs ) )
						{
							nMultiplier = 1;
						}
						else if( 0 <  TS_CompareSubString( &oPromptValue, &oSec ) )
						{
							nMultiplier = 1000;
						}
						*pnAttrVal_o = TS_StringToInt( &oPromptValue, 10 );
						*pnAttrVal_o *= nMultiplier;        // convert to millisec
					}
					TS_Free( &oPromptValue );
				}
				else
				{
					eRetCode = MS_ERR_END_OF_LIST;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetPatternAttrAsString( xmlDocPtr poXmlDoc_i, char* pcNodeName_i, int nIndex_i, char* pcAttrName_i, char** ppcAttrValue_o )
*
* @brief This function gets an attribute for the given pattern from the request as a string.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] pcNodeName_i is the name of the node containing the pattern.
*
* @param[in] nIndex_i is the index of the node containing the pattern.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] ppcAttrValue_o is the pointer to variable for storing the obtained string value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
* @note The caller of this function must deallocate the memory for string value returned by using xmlFree() function.
*
*/

e_MsRetCode GetPatternAttrAsString( xmlDocPtr poXmlDoc_i, char* pcNodeName_i, int nIndex_i, char* pcAttrName_i, char** ppcAttrValue_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playcollect" ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				bNodePresent = AMPS_FALSE;

				for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
				{
					if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"pattern" ) )
					{
						bNodePresent = AMPS_TRUE;
						break;
					}
				}
				if( AMPS_TRUE == bNodePresent )
				{
					int nCurIndex = 0;
					bNodePresent = AMPS_FALSE;

					for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
					{
						if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)pcNodeName_i ) )
						{
							nCurIndex++;
							if( nCurIndex == nIndex_i )
							{
								bNodePresent = AMPS_TRUE;
								break;
							}
						}
					}
					if( AMPS_TRUE == bNodePresent )
					{
						*ppcAttrValue_o = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
					}
					else
					{
						eRetCode = MS_ERR_END_OF_LIST;
					}
				}
				else
				{
					eRetCode = MS_ERR_INVALID_INPUT;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetPromptAttrAsInt( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int* pnAttrVal_o )
*
* @brief This function gets an attribute of prompt node as an integer value.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] pnAttrVal_o is the pointer to variable for storing the obtained integer value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode GetPromptAttrAsInt( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int* pnAttrVal_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				bNodePresent = AMPS_FALSE;
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"play" ) ||
				    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playrecord" ) ||
				    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playcollect" ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				bNodePresent = AMPS_FALSE;

				for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
				{
					if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"prompt" ) )
					{
						bNodePresent = AMPS_TRUE;
						break;
					}
				}
				if( AMPS_TRUE == bNodePresent )
				{
					t_String oPromptValue = TSTRING_INITIALIZER;
					t_String oInfinite = {"infinite", sizeof ("infinite")-1};
					t_String oImmediate = {"immediate", sizeof ("immediate")-1};

					bNodePresent = AMPS_TRUE;

					oPromptValue.pucData = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
					if( NULL != oPromptValue.pucData )
					{
						oPromptValue.unLength = strlen( oPromptValue.pucData );
						if( 0 == TS_CompareCase( &oPromptValue, &oInfinite ) )
						{
							*pnAttrVal_o = nInfiniteValue_c;
						}
						else if( 0 == TS_CompareCase( &oPromptValue, &oImmediate ) )
						{
							*pnAttrVal_o = nImmediateValue_c;
						}
						else
						{
							*pnAttrVal_o = TS_StringToInt( &oPromptValue, 10 );
						}
						TS_Free( &oPromptValue );
					}
				}
				else
				{
					eRetCode = MS_ERR_END_OF_LIST;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetPromptAttrAsMilliSec( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int* pnAttrVal_o )
*
* @brief This function gets an attribute of prompt node as an integer value in milliseconds.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] pnAttrVal_o is the pointer to variable for storing the obtained integer value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
* @Note If the value of the attribute is in seconds, this function will convert that value into milliseconds, and then return the converted value. Similarly if the value for the given attribute is infinite or immediate, then the values for @ref nInfiniteValue_c and @ref nImmediateValue_c variables respectively will be used.
*
*/

e_MsRetCode GetPromptAttrAsMilliSec( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int* pnAttrVal_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				bNodePresent = AMPS_FALSE;
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"play" ) ||
				    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playrecord" ) ||
				    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playcollect" ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				bNodePresent = AMPS_FALSE;

				for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
				{
					if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"prompt" ) )
					{
						bNodePresent = AMPS_TRUE;
						break;
					}
				}
				if( AMPS_TRUE == bNodePresent )
				{
					t_String oPromptValue = TSTRING_INITIALIZER;
					t_String oInfinite = {"infinite", sizeof ("infinite")-1};
					t_String oImmediate = {"immediate", sizeof ("immediate")-1};

					bNodePresent = AMPS_TRUE;

					oPromptValue.pucData = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
					if( NULL != oPromptValue.pucData )
					{
						oPromptValue.unLength = strlen( oPromptValue.pucData );
						if( 0 == TS_CompareCase( &oPromptValue, &oInfinite ) )
						{
							*pnAttrVal_o = nInfiniteValue_c;
						}
						else if( 0 == TS_CompareCase( &oPromptValue, &oImmediate ) )
						{
							*pnAttrVal_o = nImmediateValue_c;
						}
						else
						{
							int nMultiplier = 1;
							t_String oMs = {"ms", sizeof ("ms")-1};
							t_String oSec = {"s", sizeof ("s")-1};
							if( 0 <  TS_CompareSubString( &oPromptValue, &oMs ) )
							{
								nMultiplier = 1;
							}
							else if( 0 <  TS_CompareSubString( &oPromptValue, &oSec ) )
							{
								nMultiplier = 1000;
							}
							*pnAttrVal_o = TS_StringToInt( &oPromptValue, 10 );
							*pnAttrVal_o *= nMultiplier;        // convert to millisec
						}
						TS_Free( &oPromptValue );
					}
				}
				else
				{
					eRetCode = MS_ERR_END_OF_LIST;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode SetResponseAttrAsInt( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int nAttrVal_i )
*
* @brief This function sets an attribute of response message as an integer value.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML response message.
*
* @param[in] pcAttrName_i is the name of the attribute for setting its value.
*
* @param[in] nAttrVal_i is the integer value of the given attribute to be set.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode SetResponseAttrAsInt( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int nAttrVal_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"response" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			xmlChar poAttrValue[33] = "";

			TS_IntToString( poAttrValue, nAttrVal_i, 10 );
			if( NULL == xmlHasProp( poNode, pcAttrName_i ) )
			{
				xmlNewProp( poNode, pcAttrName_i, poAttrValue );
			}
			else
			{
				xmlSetProp( poNode, pcAttrName_i, poAttrValue );
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode SetResponseAttrAsString( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, char* pcAttrValue_i )
*
* @brief This function sets an attribute of response message as a string.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML response message.
*
* @param[in] pcAttrName_i is the name of the attribute for setting its value.
*
* @param[in] pcAttrValue_i is the sting value of the given attribute to be set.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode SetResponseAttrAsString( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, char* pcAttrValue_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"response" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;
			if( NULL == xmlHasProp( poNode, pcAttrName_i ) )
			{
				xmlNewProp( poNode, pcAttrName_i, (xmlChar*)pcAttrValue_i );
			}
			else
			{
				xmlSetProp( poNode, pcAttrName_i, (xmlChar*)pcAttrValue_i );
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetResponseAttrAsString( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, char** ppcAttrValue_o )
*
* @brief This function gets an attribute of response message as a string.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML response message.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] ppcAttrValue_o is the pointer to variable for storing the obtained sting value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
* @note The caller of this function must deallocate the memory for string value returned by using xmlFree() function.
*
*/

e_MsRetCode GetResponseAttrAsString( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, char** ppcAttrValue_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"response" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			*ppcAttrValue_o = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetResponseAttrAsInt( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int* pnAttrVal_o )
*
* @brief This function gets an attribute of response message as an integer value.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML response message.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] pnAttrVal_o is the pointer to variable for storing the obtained integer value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode GetResponseAttrAsInt( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int* pnAttrVal_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"response" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			t_String oPromptValue = TSTRING_INITIALIZER;

			oPromptValue.pucData = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
			if( NULL != oPromptValue.pucData )
			{
				oPromptValue.unLength = strlen( oPromptValue.pucData );
				*pnAttrVal_o = TS_StringToInt( &oPromptValue, 10 );
				TS_Free( &oPromptValue );
			}
			else
			{
				eRetCode = MS_ERR_END_OF_LIST;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode SetResponseErrorInfoAttrAsInt( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int nAttrVal_i )
*
* @brief This function sets an attribute of response.error_info node in the response message as an integer value. If the error_info node is not present, it will be created.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML response message.
*
* @param[in] pcAttrName_i is the name of the attribute for setting its value.
*
* @param[in] nAttrVal_i is the integer value of the given attribute to be set.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode SetResponseErrorInfoAttrAsInt( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, int nAttrVal_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"response" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			xmlNodePtr poReqNode = poNode;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, "error_info" ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_FALSE == bNodePresent )
			{
				poNode = xmlNewNode( NULL, "error_info" );
				xmlAddChild( poReqNode, poNode );
			}
			xmlChar poAttrValue[33] = "";

			TS_IntToString( poAttrValue, nAttrVal_i, 10 );
			if( NULL == xmlHasProp( poNode, pcAttrName_i ) )
			{
				xmlNewProp( poNode, (xmlChar*)pcAttrName_i, poAttrValue );
			}
			else
			{
				xmlSetProp( poNode, (xmlChar*)pcAttrName_i, poAttrValue );
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode SetResponseErrorInfoAttrAsString( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, char* pcAttrVal_i )
*
* @brief This function sets an attribute of response.error_info node in the response message as a string. If error_info node is not present, it will be created.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML response message.
*
* @param[in] pcAttrName_i is the name of the attribute for setting its value.
*
* @param[in] pcAttrVal_i is the string value of the given attribute to be set.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode SetResponseErrorInfoAttrAsString( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, char* pcAttrVal_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"response" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			xmlNodePtr poReqNode = poNode;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, "error_info" ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_FALSE == bNodePresent )
			{
				poNode = xmlNewNode( NULL, "error_info" );
				xmlAddChild( poReqNode, poNode );
			}
			if( NULL == xmlHasProp( poNode, pcAttrVal_i ) )
			{
				xmlNewProp( poNode, (xmlChar*)pcAttrName_i, pcAttrVal_i );
			}
			else
			{
				xmlSetProp( poNode, (xmlChar*)pcAttrName_i, pcAttrVal_i );
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetConfigureTeamAttrAsString( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, char** ppAttrValue_o )
*
* @brief This function gets an attribute of configure_team tag in the request message as a string.
*
* @param[in] poMscmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] pcAttrName_i is the name of the attribute for getting its value.
*
* @param[out] ppAttrValue_o is the pointer to the variable for storing the obtained string value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
* @note The caller of this function must deallocate the memory for string value returned by using xmlFree() function.
*
*/

e_MsRetCode GetConfigureTeamAttrAsString( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, char** ppAttrValue_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poMscmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"configure_leg" ))
				{
					break;
				}
			}
			// check if configure_leg found
			if( NULL != poNode )
			{
				for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
				{
					if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"configure_team" ))
					{
						*ppAttrValue_o = xmlGetProp( poNode, pcAttrName_i );
						if( *ppAttrValue_o == NULL )
						{
							eRetCode =  MS_ERR_MISSING_FIELD;
						}
						else
						{
							break;
						}
					}
				}
				if( NULL == poNode )
				{
					eRetCode = MS_ERR_INVALID_INPUT;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetTeamMateId( xmlDocPtr poMscmlDoc_i, unsigned int unTeamMateIndex_i, xmlChar** ppchTeamMateId_o )
*
* @brief This function gets the id of the teammate from the MSCML request message as a string.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] unTeamMateIndex_i is the index of the teammate for getting its id.
*
* @param[out] ppchTeamMateId_o is the pointer to the variable for storing the obtained id of the given teammate.
*
* @return e_MsRetCode the code for success or error
*
* @note The caller of this function must deallocate the memory for string value returned by using xmlFree() function.
*
*/

e_MsRetCode GetTeamMateId( xmlDocPtr poMscmlDoc_i, unsigned int unTeamMateIndex_i, xmlChar** ppchTeamMateId_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poMscmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"configure_leg" ))
				{
					break;
				}
			}
			// check if configure_leg found
			if( NULL != poNode )
			{
				for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
				{
					if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"configure_team" ))
					{
						break;
					}
				}
				// check if configure_team found
				if( NULL != poNode )
				{
					unsigned int unCurrIndex = 1;

					for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
					{
						if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"teammate" ))
						{
							if( unCurrIndex++ == unTeamMateIndex_i )
							{
								*ppchTeamMateId_o = xmlGetProp( poNode, (xmlChar*)"id" );
								if( *ppchTeamMateId_o == NULL )
								{
									eRetCode =  MS_ERR_END_OF_LIST;
								}
								else
								{
									break;
								}
							}
						}
					}
					if( NULL == poNode )
					{
						eRetCode = MS_ERR_END_OF_LIST;
					}
				}
				else
				{
					eRetCode = MS_ERR_INVALID_INPUT;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode SetTeamAttrAsString( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, char* pcAttrValue_i )
*
* @brief This function sets an attribute of the response.team node in MSCML message as a string. if the team node is not present, it will be created.
*
* @param[in] poMscmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] pcAttrName_i is the name of the attribute for setting its value.
*
* @param[in] pcAttrValue_i is the string value of the given attribute to be set.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode SetTeamAttrAsString( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, char* pcAttrValue_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode1 = NULL;  // node at level-1 in the tree

	poNode1 = xmlDocGetRootElement( poMscmlDoc_i );
	if( poNode1 != NULL )
	{
		// Search "response" node
		for( poNode1 = poNode1->xmlChildrenNode; poNode1 != NULL; poNode1 = poNode1->next )
		{
			if( 0 == xmlStrcasecmp( poNode1->name, (const xmlChar*)"response" ))
			{
				break;
			}
		}
		if( NULL != poNode1 )
		{
			xmlNodePtr poNode2 = NULL;  // node at level-2 in the tree

			for( poNode2 = poNode1->xmlChildrenNode; poNode2 != NULL; poNode2 = poNode2->next )
			{
				if( 0 == xmlStrcasecmp( poNode2->name, (const xmlChar*)"team" ))
				{
					break;
				}
			}
			if( NULL == poNode2 )
			{
				// add "team" node as child of "response" node, if it does not exist
				poNode2 = xmlNewNode( NULL, (xmlChar*)"team" );
				if( NULL == poNode2 )
				{
					eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
				}
				else
				{
					xmlAddChild( poNode1, poNode2 );
				}
			}
			xmlSetProp( poNode2, pcAttrName_i, pcAttrValue_i );
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode SetTeamAttrAsInt( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, int nAttrValue_i )
*
* @brief This function sets an attribute of the response.team node in MSCML message as an integer value. If the team node is not present, it will be created.
*
* @param[in] poMscmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] pcAttrName_i is the name of the attribute for setting its value.
*
* @param[in] nAttrValue_i is the integer value of the given attribute to be set.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode SetTeamAttrAsInt( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, int nAttrValue_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode1 = NULL;  // node at level-1 in the tree

	poNode1 = xmlDocGetRootElement( poMscmlDoc_i );
	if( poNode1 != NULL )
	{
		// Search "response" node
		for( poNode1 = poNode1->xmlChildrenNode; poNode1 != NULL; poNode1 = poNode1->next )
		{
			if( 0 == xmlStrcasecmp( poNode1->name, (const xmlChar*)"response" ))
			{
				break;
			}
		}
		if( NULL != poNode1 )
		{
			xmlNodePtr poNode2 = NULL;  // node at level-2 in the tree

			for( poNode2 = poNode1->xmlChildrenNode; poNode2 != NULL; poNode2 = poNode2->next )
			{
				if( 0 == xmlStrcasecmp( poNode2->name, (const xmlChar*)"team" ))
				{
					break;
				}
			}
			if( NULL == poNode2 )
			{
				// add "team" node as child of "response" node, if it does not exist
				poNode2 = xmlNewNode( NULL, (xmlChar*)"team" );
				if( NULL == poNode2 )
				{
					eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
				}
				else
				{
					xmlAddChild( poNode1, poNode2 );
				}
			}
			xmlChar pcAttrValue[33];

			TS_IntToString( pcAttrValue, nAttrValue_i, 10 );
			xmlSetProp( poNode2, pcAttrName_i, pcAttrValue );
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode AddTeamMateId( xmlDocPtr poMscmlDoc_i, xmlChar* poId_i )
*
* @brief This function adds a new 'response.team.teammate' node with the given id in MSCML response message.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML response message.
*
* @param[in] poId_i is the id of the new teammate.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode AddTeamMateId( xmlDocPtr poMscmlDoc_i, xmlChar* poId_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode1 = NULL;  // node at level-1 in the tree

	poNode1 = xmlDocGetRootElement( poMscmlDoc_i );
	if( poNode1 != NULL )
	{
		// Search "response" node
		for( poNode1 = poNode1->xmlChildrenNode; poNode1 != NULL; poNode1 = poNode1->next )
		{
			if( 0 == xmlStrcasecmp( poNode1->name, (const xmlChar*)"response" ))
			{
				break;
			}
		}
		if( NULL != poNode1 )
		{
			xmlNodePtr poNode2 = NULL;  // node at level-2 in the tree

			for( poNode1 = poNode1->xmlChildrenNode; poNode1 != NULL; poNode1 = poNode1->next )
			{
				if( 0 == xmlStrcasecmp( poNode1->name, (const xmlChar*)"team" ))
				{
					break;
				}
			}
			if( NULL != poNode1 )
			{
				poNode2 = xmlNewNode( NULL, (xmlChar*)"teammate" );
				if( NULL == poNode2 )
				{
					eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
				}
				else
				{
					xmlSetProp( poNode2, (xmlChar*)"id", poId_i );
					xmlAddChild( poNode1, poNode2 );
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode AppendBaseUrl( xmlDocPtr poMscmlDoc_i, char* pcSoundDir_i )
*
* @brief This function updates the \b prompt.audio.url by prepending sound directory and the base url (if given) in the given MSCML message.
*
* @param[in] poMscmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] pcSoundDir_i is the name of the sound directory to be added before base url.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode AppendBaseUrl( xmlDocPtr poMscmlDoc_i, char* pcSoundDir_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poMscmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"play" ) ||
				    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playrecord" ) ||
				    0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"playcollect" ) )
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				bNodePresent = AMPS_FALSE;

				for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
				{
					if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"prompt" ) )
					{
						bNodePresent = AMPS_TRUE;
						break;
					}
				}
				if( AMPS_TRUE == bNodePresent )
				{
					bNodePresent = AMPS_FALSE;

					xmlChar* poBaseUrl = NULL;

					poBaseUrl = xmlGetProp( poNode, (xmlChar*)"baseurl" );
					if( NULL == poBaseUrl )
					{
						poBaseUrl = xmlStrdup((xmlChar*)"" );
					}
					{
						for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
						{
							if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"audio" ) )
							{
								xmlChar* poUrl = NULL;

								poUrl = xmlGetProp( poNode, (xmlChar*)"url" );
								if( NULL != poUrl )
								{
									xmlChar* poAbsUrl = NULL;

									poAbsUrl = AMPS_Malloc( strlen( pcSoundDir_i ) + strlen( poBaseUrl ) + strlen( poUrl ) + 1 );

									ms_assert( NULL != poAbsUrl );

									strcpy( poAbsUrl, pcSoundDir_i );
									strcat( poAbsUrl, poBaseUrl );
									strcat( poAbsUrl, poUrl );

									xmlSetProp( poNode, "url", poAbsUrl );

									AMPS_Free( poAbsUrl );
									xmlFree( poUrl );
								}
							}
						}
						xmlFree( poBaseUrl );
					}
				}
				else
				{
					eRetCode = MS_ERR_INVALID_INPUT;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	POST_CONDITION
	{
	}

	return (eRetCode);
}

/*! @fn e_MsRetCode GetConfigureConfAttrAsInt( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, int* pnAttrVal_o )
*
* @brief This function gets an attribute of tbe request.configure_conference tag in the given MSCML request as an integer value.
*
* @param[in] poMscmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] pcAttrName_i is the name of the attribute whose value is to be obtained.
*
* @param[out] pnAttrVal_o is the pointer to the variable for storing the obtained integer value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode GetConfigureConfAttrAsInt( xmlDocPtr poMscmlDoc_i, char* pcAttrName_i, int* pnAttrVal_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poMscmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"configure_conference" ))
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				t_String oPromptValue = TSTRING_INITIALIZER;

				oPromptValue.pucData = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
				if( NULL != oPromptValue.pucData )
				{
					oPromptValue.unLength = strlen( oPromptValue.pucData );
					*pnAttrVal_o = TS_StringToInt( &oPromptValue, 10 );
					TS_Free( &oPromptValue );
				}
				else
				{
					eRetCode = MS_ERR_END_OF_LIST;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

/*! @fn e_MsRetCode GetConfigureConfAttrAsBool( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, AMPS_BOOL* pbAttrValue_o )
*
* @brief This function gets an attribute of tbe request.configure_conference tag in the given MSCML request as an boolean value.
*
* @param[in] poXmlDoc_i is the xml document pointer for the MSCML request message.
*
* @param[in] pcAttrName_i is the name of the attribute whose value is to be obtained.
*
* @param[out] pbAttrValue_o is the pointer to the variable for storing the obtained boolean value of the given attribute.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode GetConfigureConfAttrAsBool( xmlDocPtr poXmlDoc_i, char* pcAttrName_i, AMPS_BOOL* pbAttrValue_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	xmlNodePtr poNode = NULL;

	poNode = xmlDocGetRootElement( poXmlDoc_i );
	if( poNode != NULL )
	{
		// Search "request" node
		AMPS_BOOL bNodePresent = AMPS_FALSE;

		for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
		{
			if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"request" ))
			{
				bNodePresent = AMPS_TRUE;
				break;
			}
		}
		if( AMPS_TRUE == bNodePresent )
		{
			bNodePresent = AMPS_FALSE;

			for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
			{
				if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"configure_conference" ))
				{
					bNodePresent = AMPS_TRUE;
					break;
				}
			}
			if( AMPS_TRUE == bNodePresent )
			{
				xmlChar* poPromptValue = NULL;

				poPromptValue = xmlGetProp( poNode, (xmlChar*)pcAttrName_i );
				if( NULL != poPromptValue )
				{
					if( 0 == xmlStrcasecmp( poPromptValue, (xmlChar*)"yes" ))
					{
						*pbAttrValue_o = AMPS_TRUE;
					}
					else if( 0 == xmlStrcasecmp( poPromptValue, (xmlChar*)"no" ))
					{
						*pbAttrValue_o = AMPS_FALSE;
					}
					xmlFree( poPromptValue );
				}
				else
				{
					eRetCode = MS_ERR_END_OF_LIST;
				}
			}
			else
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
		else
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
	}
	return (eRetCode);
}

