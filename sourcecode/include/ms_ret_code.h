/******************************************************************************
** Written by Suliman Shah
**
** Copyright (C) 2008 AdvancedIMS
**
** All rights reserved.
**
*******************************************************************************
** FILE NAME            : ms_ret_code.h
** DESCRIPTION          : This file contains error code for media server
** PROJECT NAME         : MS
** TYPE                 : ANSI C
** CONTRIBUTOR(S)       : Suliman Shah
*******************************************************************************
*******************************************************************************
*/
#ifndef _MS_RET_CODE_H_
#define _MS_RET_CODE_H_

typedef enum
{
	MS_ERR_NONE,
	MS_ERR_INSUFFICIENT_MEMORY,
	MS_ERR_PRE_COND_FAILED,
	MS_ERR_POST_COND_FAILED,
	MS_ERR_FRAMEWORK,
	MS_ERR_INVALID_HANDLE,
	MS_ERR_INVALID_INPUT,
	MS_ERR_NOT_INITIALIZED,
	MS_ERR_INTERNAL,
	MS_ERR_INVALID_PACKET,
	MS_ERR_INVALID_COMP_STATUS,
	MS_ERR_MATCHED,
	MS_ERR_NOTMATCHED,
	MS_ERR_MATCHCOMPLETE,
	MS_ERR_MATCHFAILURE,
	MS_ERR_SIP_STACK,
	MS_ERR_MISSING_FIELD,
	MS_ERR_DECODE,
	MS_ERR_ENCODE,
	MS_ERR_INVALID_OPERATION,
	MS_ERR_NOT_IMPLEMENTED,
	MS_ERR_END_OF_LIST,
	MS_ERR_NOT_FOUND,
	MS_ERR_MAX
} e_MsRetCode;

#endif /* _MS_RET_CODE_H_ */

