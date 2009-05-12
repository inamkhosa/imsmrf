/******************************************************************************
** Written by Razi-ud-Din Chaudhry
**
** Copyright (C) 2008 AdvancedIMS
**
** All rights reserved.
**
*******************************************************************************
** FILE NAME        : test_snmp_get1.c
** DESCRIPTION      : The file contains test for snmp communication.
** PROJECT NAME     : Media Server
** TYPE             : ANSI C
** CONTRIBUTOR(S)   : Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXBUFLEN 300
const int NO_OF_REPETITION = 10000;

char pcGetRequestStart[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><REQUEST><VARIABLE><OPCODE>get</OPCODE><OID>";
char pcGetRequestEnd[] = "</OID><VALUE>NULL</VALUE></VARIABLE></REQUEST>";

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
	char pcOid[30];
	if( nArgc_i != 4 )
	{
		printf( "Usage: %s Server_Ip Server_Port Oid\n", ppcArgv_i[0] );
		return (-1);
	}
	else
	{
		strcpy( pcServerIp, ppcArgv_i[1] );
		nServerPort = atoi( ppcArgv_i[2] );
		strcpy( pcOid, ppcArgv_i[3] );
	}
	char pcRequest[MAXBUFLEN];
	strcpy( pcRequest, pcGetRequestStart );
	strcat( pcRequest, pcOid );
	strcat( pcRequest, pcGetRequestEnd );

	int udpsockfd;
	struct sockaddr_in oAddress;
	int nLen, nResult;
	char buff[MAXBUFLEN];
	struct timeval oTimeVal1, oTimeVal2, oTimeVal3;
	float fAvgTime;
	int nIndx = 0;

	// Create socket
	udpsockfd = socket( PF_INET, SOCK_DGRAM, 0 );
	if( udpsockfd == -1 )
	{
		perror( "socket" );
		exit( 1 );
	}
	oAddress.sin_family = AF_INET;
	oAddress.sin_port = htons( nServerPort );
	oAddress.sin_addr.s_addr = inet_addr( pcServerIp );
	nLen = sizeof (oAddress);
	if( -1 == gettimeofday( &oTimeVal1, NULL ))
	{
		perror( "time" );
		exit( 1 );
	}

	for( nIndx = 0; nIndx < NO_OF_REPETITION; nIndx++ )
	{
		// Send request
		nResult = sendto( udpsockfd, pcRequest, strlen( pcRequest ), 0, ( struct sockaddr* ) &oAddress, nLen );
		if( nResult == -1 )
		{
			perror( "write" );
			exit( 1 );
		}
		nLen = sizeof (&oAddress);

		// Receive response
		nResult = recvfrom( udpsockfd, buff, sizeof (buff), 0, ( struct sockaddr* ) &oAddress, &nLen );
		if( nResult == -1 )
		{
			perror( "read" );
			exit( 1 );
		}
	}
	if( -1 == gettimeofday( &oTimeVal2, NULL ))
	{
		perror( "time" );
		exit( 1 );
	}
	timersub( &oTimeVal2, &oTimeVal1, &oTimeVal3 );

	printf( "No of GET requests sent: %d\n", NO_OF_REPETITION );
	printf( "Start time = %d.%d seconds\n", oTimeVal1.tv_sec, oTimeVal1.tv_usec );
	printf( "End time = %d.%d seconds\n", oTimeVal2.tv_sec, oTimeVal2.tv_usec );
	printf( "Time Difference = %d.%d seconds\n", oTimeVal3.tv_sec, oTimeVal3.tv_usec );
	fAvgTime = ((oTimeVal3.tv_sec * 1000000) + oTimeVal3.tv_usec) / 1000;
	printf( "Average time per reqeust = %f milliseconds\n", fAvgTime / NO_OF_REPETITION );

	// Close socket
	close( udpsockfd );

	return (0);
}

