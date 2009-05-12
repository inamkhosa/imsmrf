/******************************************************************************
** Written by Razi-ud-Din Chaudhry
**
** Copyright (C) 2008 AdvancedIMS
**
** All rights reserved.
**
*******************************************************************************
** FILE NAME        : test_snmp_request_table1.c
** DESCRIPTION      : The file contains test for snmp parser api.
** PROJECT NAME     : Media Server
** TYPE             : ANSI C
** CONTRIBUTOR(S)   : Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

#include "snmp_parser/snmp_parser_api.h"

char pcRequest[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><REQUEST><VARIABLE><OPCODE>set</OPCODE><OID>localhosttable</OID><VALUE>127.0.0.1</VALUE><COLUMN>ipaddress</COLUMN><ROW>1</ROW></VARIABLE></REQUEST>";

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
**		    parse the given request and extract the values of different
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
	char* pcOpcode = NULL;
	char* pcOid = NULL;
	char* pcValue = NULL;
	char* pcColumn = NULL;
	unsigned long ulRow = -1;
	char* pcEncodedRes = NULL;
	int nSize = 0;

	// Parse request
	pxmlDoc = SP_ParseXML( pcRequest );
	if( NULL == pxmlDoc )
	{
		printf( "Error while parsing xml\n" );
		return (-1);
	}
	else
	{
		xmlDocDump( stdout, pxmlDoc );
	}
	// Get opcode
	if( MS_ERR_NONE != SP_GetVariableAttrAsString( pxmlDoc, 0, "OPCODE", &pcOpcode ))
	{
		printf( "Error while getting opcode\n" );
		return (-1);
	}
	else
	{
		printf( "Opcode : %s\n", pcOpcode );
		xmlFree( pcOpcode );
	}
	// Get oid
	if( MS_ERR_NONE != SP_GetVariableAttrAsString( pxmlDoc, 0, "OID", &pcOid ))
	{
		printf( "Error while getting oid\n" );
		return (-1);
	}
	else
	{
		printf( "Oid : %s\n", pcOid );
		xmlFree( pcOid );
	}
	// Get value
	if( MS_ERR_NONE != SP_GetVariableAttrAsString( pxmlDoc, 0, "VALUE", &pcValue ))
	{
		printf( "Error while getting value\n" );
		return (-1);
	}
	else
	{
		printf( "Value : %s\n", pcValue );
		xmlFree( pcValue );
	}
	// Get column
	if( MS_ERR_NONE != SP_GetVariableAttrAsString( pxmlDoc, 0, "COLUMN", &pcColumn ))
	{
		printf( "Error while getting column\n" );
		return (-1);
	}
	else
	{
		printf( "Column : %s\n", pcColumn );
		xmlFree( pcColumn );
	}
	// Get row
	if( MS_ERR_NONE != SP_GetVariableAttrAsUL( pxmlDoc, 0, "ROW", &ulRow ))
	{
		printf( "Error while getting row\n" );
		return (-1);
	}
	else
	{
		printf( "Row : %d\n", ulRow );
	}
	// Encode message
	pcEncodedRes = SP_EncodeMessage( pxmlDoc, &nSize );

	printf( "Encoded Response Size: %d\nEncoded Response:\n%s\n", nSize, pcEncodedRes );

	xmlFree( pcEncodedRes );
	xmlFreeDoc( pxmlDoc );

	return (0);
}

