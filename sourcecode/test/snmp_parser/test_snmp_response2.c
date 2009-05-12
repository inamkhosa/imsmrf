/******************************************************************************
** Written by Razi-ud-Din Chaudhry
**
** Copyright (C) 2008 AdvancedIMS
**
** All rights reserved.
**
*******************************************************************************
** FILE NAME        : test_snmp_response2.c
** DESCRIPTION      : The file contains test for snmp parser api.
** PROJECT NAME     : Media Server
** TYPE             : ANSI C
** CONTRIBUTOR(S)   : Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

#include "snmp_parser/snmp_parser_api.h"

/******************************************************************************
** Function Name:
**		main
**
** Parameters:
**		int nArgc_i
**			type: input
**			detail: This number indicates the no of command line
**				    arguments passed to this program.
**
**		char*  ppcArgv_i[]
**			type: input
**			detail: This pointer array contains the command line
**				    arguments passed to this program.
**
**
** Return:
**		An integer specifying the success or failure of this program.
**
**
** Additional Details:
**		This function calls functions defined in the snmp parser to
**		    prepare a response by setting the values of different
**		    attributes.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

int main( int nArgc_i, char* ppcArgv_i[] )
{
	printf( "Welcome to SNMP Parser\n" );
	printf( "**********************\n" );

	xmlDocPtr pxmlDoc = NULL;
	char* pcEncodedRes = NULL;
	int nSize = 0;

	// Get response template
	pxmlDoc = SP_GetResponse();
	if( NULL == pxmlDoc )
	{
		printf( "Error while getting xml response template\n" );
		return (-1);
	}
	else
	{
		xmlDocDump( stdout, pxmlDoc );
	}
	// Set opcode
	if( MS_ERR_NONE != SP_SetAttrAsString( pxmlDoc, 0, "OPCODE", "Get" ))
	{
		printf( "Error while adding opcode\n" );
		return (-1);
	}
	else
	{
		printf( "Opcode added Successfully\n" );
	}
	// Set oid
	if( MS_ERR_NONE != SP_SetAttrAsString( pxmlDoc, 0, "OID", "ipaddress" ))
	{
		printf( "Error while adding oid\n" );
		return (-1);
	}
	else
	{
		printf( "Oid added Successfully\n" );
	}
	// Set value
	if( MS_ERR_NONE != SP_SetAttrAsString( pxmlDoc, 0, "VALUE", "127.0.0.1" ))
	{
		printf( "Error while adding value\n" );
		return (-1);
	}
	else
	{
		printf( "Value added Successfully\n" );
	}
	// Set error code
	if( MS_ERR_NONE != SP_SetAttrAsUL( pxmlDoc, 0, "ERRORCODE", 200 ))
	{
		printf( "Error while adding errorcode\n" );
		return (-1);
	}
	else
	{
		printf( "Errorcode added Successfully\n" );
	}
	// Set error description
	if( MS_ERR_NONE != SP_SetAttrAsString( pxmlDoc, 0, "ERRORDESC", "OK" ))
	{
		printf( "Error while adding errordesc\n" );
		return (-1);
	}
	else
	{
		printf( "Errordesc added Successfully\n" );
	}
	// Set opcode
	if( MS_ERR_NONE != SP_SetAttrAsString( pxmlDoc, 2, "OPCODE", "Get" ))
	{
		printf( "Error while adding opcode\n" );
		return (-1);
	}
	else
	{
		printf( "Opcode added Successfully\n" );
	}
	// Set oid
	if( MS_ERR_NONE != SP_SetAttrAsString( pxmlDoc, 2, "OID", "username" ))
	{
		printf( "Error while adding oid\n" );
		return (-1);
	}
	else
	{
		printf( "Oid added Successfully\n" );
	}
	// Set value
	if( MS_ERR_NONE != SP_SetAttrAsString( pxmlDoc, 2, "VALUE", "ms" ))
	{
		printf( "Error while adding value\n" );
		return (-1);
	}
	else
	{
		printf( "Value added Successfully\n" );
	}
	// Set error code
	if( MS_ERR_NONE != SP_SetAttrAsUL( pxmlDoc, 2, "ERRORCODE", 200 ))
	{
		printf( "Error while adding errorcode\n" );
		return (-1);
	}
	else
	{
		printf( "Errorcode added Successfully\n" );
	}
	// Set error description
	if( MS_ERR_NONE != SP_SetAttrAsString( pxmlDoc, 2, "ERRORDESC", "OK" ))
	{
		printf( "Error while adding errordesc\n" );
		return (-1);
	}
	else
	{
		printf( "Errordesc added Successfully\n" );
	}
	// Encode message
	pcEncodedRes = SP_EncodeMessage( pxmlDoc, &nSize );

	printf( "Encoded Response Size: %d\nEncoded Response:\n%s\n", nSize, pcEncodedRes );

	xmlFree( pcEncodedRes );
	xmlFreeDoc( pxmlDoc );

	return (0);
}

