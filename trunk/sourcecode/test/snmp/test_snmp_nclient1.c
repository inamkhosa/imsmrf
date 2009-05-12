/******************************************************************************
** Written by Razi-ud-Din Chaudhry
**
** Copyright (C) 2008 AdvancedIMS
**
** All rights reserved.
**
*******************************************************************************
** FILE NAME        : test_snmp_nclient1.c
** DESCRIPTION      : The file contains test for snmp communication.
** PROJECT NAME     : Media Server
** TYPE             : ANSI C
** CONTRIBUTOR(S)   : Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXBUFLEN 400

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
**		This function calls functions defined in the snmp communication
**		    api and checks the response obtained.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

int main( int nArgc_i, char* ppcArgv_i[] )
{
	char pcServerIp[32];
	int nServerPort;
	if( nArgc_i != 3 )
	{
		printf( "Usage: %s Server_Ip Server_Port\n", ppcArgv_i[0] );
		return (-1);
	}
	else
	{
		strcpy( pcServerIp, ppcArgv_i[1] );
		nServerPort = atoi( ppcArgv_i[2] );
	}
	int udpsockfd;
	struct sockaddr_in oAddress;
	int nLen, nResult;
	char buff[MAXBUFLEN];

	// Create socket
	udpsockfd = socket( PF_INET, SOCK_DGRAM, 0 );
	if( udpsockfd == -1 )
	{
		perror( "socket" );
		exit( 1 );
	}
	else
	{
		printf( "Socket created successfully\n" );
	}
	oAddress.sin_family = AF_INET;
	oAddress.sin_port = htons( nServerPort );
	oAddress.sin_addr.s_addr = inet_addr( pcServerIp );
	nLen = sizeof (oAddress);
	// bind socket
	if( bind( udpsockfd, ( struct sockaddr* ) &oAddress, nLen ) == -1 )
	{
		perror( "bind" );
		exit( 1 );
	}
	else
	{
		printf( "Socket binding successful\n" );
	}
	// Receive response
	nResult = recvfrom( udpsockfd, buff, sizeof (buff), 0, ( struct sockaddr* ) &oAddress, &nLen );
	if( nResult == -1 )
	{
		perror( "read" );
		exit( 1 );
	}
	else
	{
		buff[nResult] = '\0';
		printf( "Size of received response: %d bytes\n", nResult );
		printf( "Received response: %s\n", buff );
	}
	// Close socket
	close( udpsockfd );

	return (0);
}

