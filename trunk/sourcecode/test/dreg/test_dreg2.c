/******************************************************************************
** Written by Razi-ud-Din Chaudhry
**
** Copyright (C) 2008 AdvancedIMS
**
** All rights reserved.
**
*******************************************************************************
** FILE NAME        : test_dreg2.c
** DESCRIPTION      : This file contains test for digit regular expression
**			    PEG parser API
** PROJECT NAME     : Media Server
** TYPE             : ANSI C
** CONTRIBUTOR(S)   : Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

#include "dreg/dreg.h"
#include "ms_debug.h"
#include "fw_api.h"
#include "stdio.h"

typedef enum
{
	APP_INTERNAL_EVENT_MAX = 1
} e_RTP_InteranlEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX = 1
} e_RTP_ExternalEvent;

/******************************************************************************
** Function Name:
**		Main
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
**		This function takes the command line arguments from the user
**		    and parses them using PEG.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

int main( int nArgc_i, char* ppcArgv_i[] )
{
	if( nArgc_i > 1 )        // Command Line Arguments Found
	{
		printf( "Error : Command Line Arguments Found\n" );
		printf( "\tAccepted Syntax for Execution : ./test_dreg1\n" );
		return (-1);
	}
	else
	{
		printf( "Welcome to PEG Parser API Test Program\n" );
		printf( "**************************************\n" );
	}
	void* pvAMPSContext = NULL;
	hDRegEx hDRegEx1;
	t_DRegExConfig oDRegExConfig;
	oDRegExConfig.unLongMatchDurationMs = 0;

	pvAMPSContext = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );
	if( NULL == pvAMPSContext )
	{
		printf( "FW_Init: AMPS_Init is failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	FW_SetTraceMode( pvAMPSContext, AMPS_TRACE_MODE_BOTH );
	FW_SetTraceLevel( pvAMPSContext, 31 );
	FW_EnableLogging( pvAMPSContext, "logfile.txt" );

	FW_EvtSysSetEvtHandleValidationCallback( pvAMPSContext, NULL );
	if( NULL == FW_SchedInit( pvAMPSContext, APP_INTERNAL_EVENT_MAX, APP_EXTERNAL_EVENT_MAX, NULL, NULL, NULL, NULL ))
	{
		printf( "FW_SchedInit failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	oDRegExConfig.nDRegTraceID = FW_GetTraceID( pvAMPSContext );
	FW_SetTraceForTraceID( pvAMPSContext, oDRegExConfig.nDRegTraceID );

	int bExprChk = 1;

	while( bExprChk == 1 )
	{
		char cDummy = '\0';
		printf( "Enter a Regular Expression :" );
		scanf( "%s", oDRegExConfig.pcExpression );
		scanf( "%c", &cDummy );
		printf( "Enter a Name for This Expression :" );
		scanf( "%s", oDRegExConfig.pcName );
		scanf( "%c", &cDummy );

		e_MsRetCode eRet_Code = re_create( &hDRegEx1, &oDRegExConfig );
		if( eRet_Code == MS_ERR_INVALID_INPUT )   // Parsing Failed
		{
			printf( "Result = Failure:\n" );
			printf( "\t The given argument is not according to PEG rules\n" );
			return (-1);
		}
		else                    // Parser Completed Successfully
		{
			printf( "Result = Success:\n" );
			printf( "\t The given argument is according to PEG rules\n" );
		}
		char cDigit = '\0';
		cDummy = '\0';
		printf( "Enter a Character to Match (Enter 'q' or 'Q' to Exit) :" );
		scanf( "%c", &cDigit );
		scanf( "%c", &cDummy );

		while( cDigit != 'q' && cDigit != 'Q' )
		{
			printf( "Matching character : %c\n", cDigit );
			eRet_Code = re_match( hDRegEx1, cDigit );
			printf( "Match Result: %s\n", get_ms_ret_code_string( eRet_Code ));

			char pcMatch[20] = "\0";
			unsigned int punNumDigits = 20;
			eRet_Code = re_get_current_match( hDRegEx1, pcMatch, &punNumDigits );
			if( eRet_Code != MS_ERR_NONE )
			{
				printf( "Error in Getting Matched Regular Expression\n" );
			}
			else
			{
				printf( "Matched Expression : %s\n", pcMatch );
			}
			printf( "Enter a Character to Match (Enter 'q' or 'Q' to Exit) :" );
			scanf( "%c", &cDigit );
			scanf( "%c", &cDummy );
		}
		char pcName[20];
		unsigned int punNum = 20;
		eRet_Code = re_get_name( hDRegEx1, pcName, &punNum );
		if( eRet_Code != MS_ERR_NONE )
		{
			printf( "Error in Getting Name of Regular Expression\n" );
		}
		else
		{
			printf( "Name of Regular Expression : %s\n", pcName );
		}
		eRet_Code = re_destroy( &hDRegEx1 );
		if( eRet_Code != MS_ERR_NONE )
		{
			printf( "Error in Destroying Regular Expression\n" );
		}
		else
		{
			printf( "Regular Expression Destroyed Successfully\n" );
		}
		cDigit = '\0';
		cDummy = '\0';
		printf( "Do you want to check another regular expression (y/n):" );
		scanf( "%c", &cDigit );
		scanf( "%c", &cDummy );
		if( cDigit == 'y' || cDigit == 'Y' )
		{
			bExprChk = 1;
		}
		else
		{
			bExprChk = 0;
		}
	}
	return (0);
}

