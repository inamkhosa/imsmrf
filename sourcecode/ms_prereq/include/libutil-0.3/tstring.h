#ifndef _TSTRING_H_
#define _TSTRING_H_

#ifdef WIN32
#include "AMPS_API.h"
#else
#include "amps/AMPS_API.h"
#endif

typedef struct _t_String
{
	unsigned char* pucData;      //pointer to the message data
	unsigned int   unLength;       //size of buffer
} t_String;

#define TSTRING_INITIALIZER {NULL, 0}

int TS_IntToString( unsigned char* puchBuffer_i, int nValue_i, int nBase_i );

int TS_StringToInt( t_String* poString_i, int nBase_i );

int TS_CopyDuplicate( t_String* poDstString_i, t_String* poSrcString_i );

void TS_Free( t_String* poString_i );

void TS_Move( t_String* poSrc_i, t_String* poDest_i );

int TS_Set( char* pcSrc_i, t_String* poDest_i );

int TS_SetN( char* pcSrc_i, unsigned int unSrcLength, t_String* poDest_i );

t_String* TS_Alloc( void );

int TS_AllocBuffer( t_String* poString_i, unsigned int unSize_i );

t_String* TS_Duplicate( t_String* poString_i );

int TS_Compare( t_String* poString1_i, t_String* poString2_i );

// Case insensitive
int TS_CompareCase( t_String* poString1_i, t_String* poString2_i );

int TS_Print( int nTraceId_i, int nTraceLevel_i, char* pchPrefix_i, t_String* poString_i );

int TS_PrintN( int nTraceId_i, int nTraceLevel_i, char* pchPrefix_i, unsigned char* pucData_i, unsigned int unLength_i );

int TS_GenerateRandomString( t_String* poStr_i );

unsigned int TS_GetOffsetOfChar( t_String* poString_i, unsigned int unStartOffset_i, unsigned char ucSeachChar_i );

int TS_CopySubString( t_String* poSrcString_i, t_String* poDstString_i, unsigned int unStartOffset_i, unsigned int unEndOffset_i );

int TS_AsciiDigitToNumber( char cDigit_i );

char TS_NumToAsciiDigit( int nNum_i );

char* TS_GetStringZ( t_String* poString_i );

int TS_CompareSubString( t_String* poString_i, t_String* poSubString_i );

int TS_Assign( char* pcSrc_i, t_String* poDest_i );

int TS_AssignN( char* pcSrc_i, unsigned int unSrcLength, t_String* poDest_i );

unsigned long TS_StringToUL( t_String* poString_i, int nBase_i );

#endif /* _TSTRING_H_ */

