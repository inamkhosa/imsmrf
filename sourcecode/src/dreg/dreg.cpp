/*! @file dreg.cpp
*
* @ingroup Dreg
*
* @brief The file contains implementation of additional functions for digit regular expression PEG parser API.
*
* @author Razi-ud-Din Chaudhry
*
*/

#include "dreg_priv.h"

/******************************************************************************
** Function Name:
**		t_DRegX::_t_DRegX
**
**
** Additional Details:
**		Constructor for the structure t_DRegX
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegX::_t_DRegX()
{
	eDRNodeType = DNT_X;
}

/******************************************************************************
** Function Name:
**		t_DRegX::~_t_DRegX
**
**
** Additional Details:
**		Destructor for the structure t_DRegX
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegX::~_t_DRegX()
{
}

/******************************************************************************
** Function Name:
**		t_DRegX::re_match
**
** Parameters:
**		hDRegEx hDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**		char cDigit_i
**			type: input
**			detail: The character to be matched.
**
**		eIncNode eInc_Node_o
**			type: output
**			detail: The pointer value indicating whether the current
**				    node in the list is to be updated or not.
**
**
** Return:
**		An value indicating the success or failure in matching the input
**		    character with the character x.
**
**
** Additional Details:
**		This function matches the input character with the character x.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegX::re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o )
{
	t_DRegEx* poDRegEx = (t_DRegEx*)hDRegEx_i;

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	*eInc_Node_o = INC_Yes;
	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( cDigit_i >= '0' && cDigit_i <= '9' )
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_x successful \n" );
			char cTemp[2] = {cDigit_i};
			strcat( poDRegEx->poDRegExpr->pcMatchBuffer, cTemp );
			eRet_Code = MS_ERR_MATCHED;
		}
		else
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_x failed \n" );
			eRet_Code = MS_ERR_MATCHFAILURE;
		}
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegX::Destroy
**
** Parameters:
**		t_DRegEx* poDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**
** Additional Details:
**		Destructor for the structure t_DRegX
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegX::Destroy( t_DRegEx* poDRegEx_i )
{
	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = MS_ERR_NONE;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegStar::_t_DRegStar
**
**
** Additional Details:
**		Constructor for the structure t_DRegStar
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegStar::_t_DRegStar()
{
	eDRNodeType = DNT_Star;
}

/******************************************************************************
** Function Name:
**		t_DRegStar::~_t_DRegStar
**
**
** Additional Details:
**		Destructor for the structure t_DRegStar
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegStar::~_t_DRegStar()
{
}

/******************************************************************************
** Function Name:
**		t_DRegStar::re_match
**
** Parameters:
**		hDRegEx hDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**		char cDigit_i
**			type: input
**			detail: The character to be matched.
**
**		eIncNode eInc_Node_o
**			type: output
**			detail: The pointer value indicating whether the current
**				    node in the list is to be updated or not.
**
**
** Return:
**		An value indicating the success or failure in matching the input
**		    character with the character star.
**
**
** Additional Details:
**		This function matches the input character with the character
**		    star.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegStar::re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o )
{
	t_DRegEx* poDRegEx = (t_DRegEx*)hDRegEx_i;

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	*eInc_Node_o = INC_Yes;
	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( cDigit_i == '*' )
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_star successful \n" );
			char cTemp[2] = {cDigit_i};
			strcat( poDRegEx->poDRegExpr->pcMatchBuffer, cTemp );
			eRet_Code = MS_ERR_MATCHED;
		}
		else
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_star failed \n" );
			eRet_Code = MS_ERR_MATCHFAILURE;
		}
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegStar::Destroy
**
** Parameters:
**		t_DRegEx* poDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**
** Additional Details:
**		Destructor for the structure t_DRegStar
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegStar::Destroy( t_DRegEx* poDRegEx_i )
{
	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = MS_ERR_NONE;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegHash::_t_DRegHash
**
**
** Additional Details:
**		Constructor for the structure t_DRegHash
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegHash::_t_DRegHash()
{
	eDRNodeType = DNT_Hash;
}

/******************************************************************************
** Function Name:
**		t_DRegHash::~_t_DRegHash
**
**
** Additional Details:
**		Destructor for the structure t_DRegHash
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegHash::~_t_DRegHash()
{
}

/******************************************************************************
** Function Name:
**		t_DRegHash::re_match
**
** Parameters:
**		hDRegEx hDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**		char cDigit_i
**			type: input
**			detail: The character to be matched.
**
**		eIncNode eInc_Node_o
**			type: output
**			detail: The pointer value indicating whether the current
**				    node in the list is to be updated or not.
**
**
** Return:
**		An value indicating the success or failure in matching the input
**		    character with the character hash.
**
**
** Additional Details:
**		This function matches the input character with the character
**		    hash.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegHash::re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o )
{
	t_DRegEx* poDRegEx = (t_DRegEx*)hDRegEx_i;

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	*eInc_Node_o = INC_Yes;
	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( cDigit_i == '#' )
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_hash successful \n" );
			char cTemp[2] = {cDigit_i};
			strcat( poDRegEx->poDRegExpr->pcMatchBuffer, cTemp );
			eRet_Code = MS_ERR_MATCHED;
		}
		else
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_hash failed \n" );
			eRet_Code = MS_ERR_MATCHFAILURE;
		}
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegHash::Destroy
**
** Parameters:
**		t_DRegEx* poDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**
** Additional Details:
**		Destructor for the structure t_DRegHash
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegHash::Destroy( t_DRegEx* poDRegEx_i )
{
	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = MS_ERR_NONE;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegDot::_t_DRegDot
**
**
** Additional Details:
**		Constructor for the structure t_DRegDot
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegDot::_t_DRegDot()
{
	eDRNodeType = DNT_Dot;
}

/******************************************************************************
** Function Name:
**		t_DRegDot::~_t_DRegDot
**
**
** Additional Details:
**		Destructor for the structure t_DRegDot
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegDot::~_t_DRegDot()
{
}

/******************************************************************************
** Function Name:
**		t_DRegDot::re_match
**
** Parameters:
**		hDRegEx hDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**		char cDigit_i
**			type: input
**			detail: The character to be matched.
**
**		eIncNode eInc_Node_o
**			type: output
**			detail: The pointer value indicating whether the current
**				    node in the list is to be updated or not.
**
**
** Return:
**		An value indicating the success or failure in matching the input
**		    character with the character dot.
**
**
** Additional Details:
**		This function matches the input character with the character
**		    dot.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegDot::re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o )
{
	t_DRegEx* poDRegEx = (t_DRegEx*)hDRegEx_i;

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	*eInc_Node_o = INC_Yes;
	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		if((cDigit_i >= '0' && cDigit_i <= '9')
		   || (cDigit_i >= 'a' && cDigit_i <= 'd')
		   || (cDigit_i >= 'A' && cDigit_i <= 'D')
		   || cDigit_i == '#'
		   || cDigit_i == '*' )
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_dot successful \n" );
			char cTemp[2] = {cDigit_i};
			strcat( poDRegEx->poDRegExpr->pcMatchBuffer, cTemp );
			eRet_Code = MS_ERR_MATCHED;
		}
		else
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_dot failed \n" );
			eRet_Code = MS_ERR_MATCHFAILURE;
		}
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegDot::Destroy
**
** Parameters:
**		t_DRegEx* poDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**
** Additional Details:
**		Destructor for the structure t_DRegDot
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegDot::Destroy( t_DRegEx* poDRegEx_i )
{
	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = MS_ERR_NONE;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegDigit::_t_DRegDigit
**
**
** Additional Details:
**		Constructor for the structure t_DRegDigit
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegDigit::_t_DRegDigit() : cValue( -1 )
{
	eDRNodeType = DNT_Digit;
}

/******************************************************************************
** Function Name:
**		t_DRegDigit::~_t_DRegDigit
**
**
** Additional Details:
**		Destructor for the structure t_DRegDigit
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegDigit::~_t_DRegDigit()
{
}

/******************************************************************************
** Function Name:
**		t_DRegDigit::re_match
**
** Parameters:
**		hDRegEx hDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**		char cDigit_i
**			type: input
**			detail: The character to be matched.
**
**		eIncNode eInc_Node_o
**			type: output
**			detail: The pointer value indicating whether the current
**				    node in the list is to be updated or not.
**
**
** Return:
**		An value indicating the success or failure in matching the input
**		    character with the stored digits.
**
**
** Additional Details:
**		This function matches the input character with the stored digits.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegDigit::re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o )
{
	t_DRegEx* poDRegEx = (t_DRegEx*)hDRegEx_i;

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	*eInc_Node_o = INC_Yes;
	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( cDigit_i == cValue )
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_digit successful \n" );
			char cTemp[2] = {cDigit_i};
			strcat( poDRegEx->poDRegExpr->pcMatchBuffer, cTemp );
			eRet_Code = MS_ERR_MATCHED;
		}
		else
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_digit failed \n" );
			eRet_Code = MS_ERR_MATCHFAILURE;
		}
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegDigit::Destroy
**
** Parameters:
**		t_DRegEx* poDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**
** Additional Details:
**		Destructor for the structure t_DRegDigit
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegDigit::Destroy( t_DRegEx* poDRegEx_i )
{
	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = MS_ERR_NONE;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegRange::_t_DRegRange
**
**
** Additional Details:
**		Constructor for the structure t_DRegRange
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegRange::_t_DRegRange() : cMinValue( -1 ), cMaxValue( -1 )
{
	eDRNodeType = DNT_Range;
}

/******************************************************************************
** Function Name:
**		t_DRegRange::~_t_DRegRange
**
**
** Additional Details:
**		Destructor for the structure t_DRegRange
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegRange::~_t_DRegRange()
{
}

/******************************************************************************
** Function Name:
**		t_DRegRange::re_match
**
** Parameters:
**		hDRegEx hDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**		char cDigit_i
**			type: input
**			detail: The character to be matched.
**
**		eIncNode eInc_Node_o
**			type: output
**			detail: The pointer value indicating whether the current
**				    node in the list is to be updated or not.
**
**
** Return:
**		An value indicating the success or failure in matching the input
**		    character with the range.
**
**
** Additional Details:
**		This function matches the input character with the range.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegRange::re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o )
{
	t_DRegEx* poDRegEx = (t_DRegEx*)hDRegEx_i;

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	*eInc_Node_o = INC_Yes;
	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( cDigit_i >= cMinValue && cDigit_i <= cMaxValue )
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_range successful \n" );
			char cTemp[2] = {cDigit_i};
			strcat( poDRegEx->poDRegExpr->pcMatchBuffer, cTemp );
			eRet_Code = MS_ERR_MATCHED;
		}
		else
		{
			TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_range failed \n" );
			eRet_Code = MS_ERR_MATCHFAILURE;
		}
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegRange::Destroy
**
** Parameters:
**		t_DRegEx* poDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**
** Additional Details:
**		Destructor for the structure t_DRegRange
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegRange::Destroy( t_DRegEx* poDRegEx_i )
{
	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = MS_ERR_NONE;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegLong::_t_DRegLong
**
**
** Additional Details:
**		Constructor for the structure t_DRegLong
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegLongExpr::_t_DRegLongExpr()
{
	eDRNodeType = DNT_LongExpr;
	poNode = NULL;
}

/******************************************************************************
** Function Name:
**		t_DRegLong::~_t_DRegLong
**
**
** Additional Details:
**		Destructor for the structure t_DRegLong
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegLongExpr::~_t_DRegLongExpr()
{
}

/******************************************************************************
** Function Name:
**		t_DRegLongExpr::re_match
**
** Parameters:
**		hDRegEx hDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**		char cDigit_i
**			type: input
**			detail: The character to be matched.
**
**		eIncNode eInc_Node_o
**			type: output
**			detail: The pointer value indicating whether the current
**				    node in the list is to be updated or not.
**
**
** Return:
**		An value indicating the success or failure in matching the input
**		    character with the long expression.
**
**
** Additional Details:
**		This function matches the input character with the long expression.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegLongExpr::re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o )
{
	t_DRegEx* poDRegEx = (t_DRegEx*)hDRegEx_i;

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	*eInc_Node_o = INC_Yes;
	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = MS_ERR_MATCHED;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegLongExpr::Destroy
**
** Parameters:
**		t_DRegEx* poDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**
** Additional Details:
**		Destructor for the structure t_DRegLongExpr
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegLongExpr::Destroy( t_DRegEx* poDRegEx_i )
{
	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = poNode->Destroy( poDRegEx_i );
		delete poNode;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegRangeExpr::_t_DRegRangeExpr
**
**
** Additional Details:
**		Constructor for the structure t_DRegRangeExpr
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegRangeExpr::_t_DRegRangeExpr()
{
	eDRNodeType = DNT_RangeExpr;
	poNodeList = NULL;
}

/******************************************************************************
** Function Name:
**		t_DRegRangeExpr::~_t_DRegRangeExpr
**
**
** Additional Details:
**		Destructor for the structure t_DRegRangeExpr
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegRangeExpr::~_t_DRegRangeExpr()
{
}

/******************************************************************************
** Function Name:
**		t_DRegRangeExpr::re_match
**
** Parameters:
**		hDRegEx hDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**		char cDigit_i
**			type: input
**			detail: The character to be matched.
**
**		eIncNode eInc_Node_o
**			type: output
**			detail: The pointer value indicating whether the current
**				    node in the list is to be updated or not.
**
**
** Return:
**		An value indicating the success or failure in matching the input
**		    character with the range expression.
**
**
** Additional Details:
**		This function matches the input character with the range
**		    expression.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegRangeExpr::re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o )
{
	t_DRegEx* poDRegEx = (t_DRegEx*)hDRegEx_i;

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	*eInc_Node_o = INC_Yes;
	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		poDRegEx->poDRegExpr->poCurrentNode = g_slist_nth( poNodeList, 0 );

		while( poDRegEx->poDRegExpr->poCurrentNode != NULL )
		{
			eRet_Code = ((t_DRegNode*)(poDRegEx->poDRegExpr->poCurrentNode->data))->re_match( hDRegEx_i, cDigit_i, eInc_Node_o );
			if( eRet_Code == MS_ERR_MATCHED )
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Digit or Range Matched \n" );
				poDRegEx->poDRegExpr->poCurrentNode = NULL;
				break;
			}
			else
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Digit or Range does not Matched \n" );
			}
			poDRegEx->poDRegExpr->poCurrentNode = poDRegEx->poDRegExpr->poCurrentNode->next;
		}
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegRangeExpr::Destroy
**
** Parameters:
**		t_DRegEx* poDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**
** Additional Details:
**		Destructor for the structure t_DRegRangeExpr
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegRangeExpr::Destroy( t_DRegEx* poDRegEx_i )
{
	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );
	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_List* poCurrentNode = poDRegEx_i->poDRegExpr->poCurrentNode;
		poDRegEx_i->poDRegExpr->poCurrentNode = g_slist_nth( poNodeList, 0 );

		while( poDRegEx_i->poDRegExpr->poCurrentNode != NULL )
		{
			t_DRegNode* poDRegNode = ((t_DRegNode*)(poDRegEx_i->poDRegExpr->poCurrentNode->data));
			eRet_Code = poDRegNode->Destroy( poDRegEx_i );

			poDRegEx_i->poDRegExpr->poCurrentNode = poDRegEx_i->poDRegExpr->poCurrentNode->next;
			delete poDRegNode;
		}
		g_slist_free( poDRegEx_i->poDRegExpr->poCurrentNode );
		poCurrentNode = poDRegEx_i->poDRegExpr->poCurrentNode = poCurrentNode;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegRepetition::_t_DRegRepetition
**
**
** Additional Details:
**		Constructor for the structure t_DRegRepetition
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegRepetitionExpr::_t_DRegRepetitionExpr() : nCurrentRepCount( 0 ), nMinRep( -1 ), nMaxRep( -1 )
{
	eDRNodeType = DNT_RepetitionExpr;
	poNode = NULL;
	pcRepBuffer[0] = '\0';
}

/******************************************************************************
** Function Name:
**		t_DRegRepetition::~_t_DRegRepetition
**
**
** Additional Details:
**		Destructor for the structure t_DRegRepetition
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegRepetitionExpr::~_t_DRegRepetitionExpr()
{
}

/******************************************************************************
** Function Name:
**		t_DRegRepetitionExpr::re_match
**
** Parameters:
**		hDRegEx hDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**		char cDigit_i
**			type: input
**			detail: The character to be matched.
**
**		eIncNode eInc_Node_o
**			type: output
**			detail: The pointer value indicating whether the current
**				    node in the list is to be updated or not.
**
**
** Return:
**		An value indicating the success or failure in matching the input
**		    character with the repetition expression.
**
**
** Additional Details:
**		This function matches the input character with the repetition
**		    expression.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegRepetitionExpr::re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o )
{
	t_DRegEx* poDRegEx = (t_DRegEx*)hDRegEx_i;

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	*eInc_Node_o = INC_Yes;
	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = poNode->re_match( hDRegEx_i, cDigit_i, eInc_Node_o );
		if( eRet_Code == MS_ERR_MATCHED )
		{
			eRet_Code = MS_ERR_NOTMATCHED;
			nCurrentRepCount++;
			if( nMinRep == nMaxRep )            //{m}
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "{m}\n" );
				if( nCurrentRepCount == nMinRep )
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount == nMinRep\n" );
					int nLength = strlen( poDRegEx->poDRegExpr->pcMatchBuffer );
					poDRegEx->poDRegExpr->pcMatchBuffer[nLength-1] = '\0';
					strcat( poDRegEx->poDRegExpr->pcMatchBuffer, pcRepBuffer );
					pcRepBuffer[0] = '\0';
					char cTemp[2] = {cDigit_i};
					strcat( poDRegEx->poDRegExpr->pcMatchBuffer, cTemp );
					eRet_Code = MS_ERR_MATCHED;
					*eInc_Node_o = INC_Yes;
				}
				else
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount != nMinRep\n" );
					int nLength = strlen( poDRegEx->poDRegExpr->pcMatchBuffer );
					poDRegEx->poDRegExpr->pcMatchBuffer[nLength-1] = '\0';
					char cTemp[2] = {cDigit_i};
					strcat( pcRepBuffer, cTemp );
					eRet_Code = MS_ERR_MATCHED;
					*eInc_Node_o = INC_No;
				}
			}
			else if( nMinRep != -1 && nMaxRep != -1 )   //{m,n}
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "{m,n}\n" );
				if( nCurrentRepCount < nMinRep )
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount < nMinRep\n" );
					int nLength = strlen( poDRegEx->poDRegExpr->pcMatchBuffer );
					poDRegEx->poDRegExpr->pcMatchBuffer[nLength-1] = '\0';
					char cTemp[2] = {cDigit_i};
					strcat( pcRepBuffer, cTemp );
					eRet_Code = MS_ERR_NOTMATCHED;
					*eInc_Node_o = INC_No;
				}
				else if( nCurrentRepCount >= nMinRep && nCurrentRepCount < nMaxRep )
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount >= nMinRep && nCurrentRepCount < nMaxRep\n" );
					int nLength = strlen( poDRegEx->poDRegExpr->pcMatchBuffer );
					poDRegEx->poDRegExpr->pcMatchBuffer[nLength-1] = '\0';
					strcat( poDRegEx->poDRegExpr->pcMatchBuffer, pcRepBuffer );
					pcRepBuffer[0] = '\0';
					char cTemp[2] = {cDigit_i};
					strcat( poDRegEx->poDRegExpr->pcMatchBuffer, cTemp );
					eRet_Code = MS_ERR_MATCHED;
					*eInc_Node_o = INC_No;
				}
				else if( nCurrentRepCount == nMaxRep )
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount == nMaxRep\n" );
					int nLength = strlen( poDRegEx->poDRegExpr->pcMatchBuffer );
					poDRegEx->poDRegExpr->pcMatchBuffer[nLength-1] = '\0';
					strcat( poDRegEx->poDRegExpr->pcMatchBuffer, pcRepBuffer );
					pcRepBuffer[0] = '\0';
					char cTemp[2] = {cDigit_i};
					strcat( poDRegEx->poDRegExpr->pcMatchBuffer, cTemp );
					eRet_Code = MS_ERR_MATCHED;
					*eInc_Node_o = INC_Yes;
				}
				else
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount > nMaxRep\n" );
				}
			}
			else if( nMinRep != -1 )            //{m,}
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "{m,}\n" );
				if( nCurrentRepCount < nMinRep )
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount < nMinRep\n" );
					int nLength = strlen( poDRegEx->poDRegExpr->pcMatchBuffer );
					poDRegEx->poDRegExpr->pcMatchBuffer[nLength-1] = '\0';
					char cTemp[2] = {cDigit_i};
					strcat( pcRepBuffer, cTemp );
					eRet_Code = MS_ERR_NOTMATCHED;
					*eInc_Node_o = INC_No;
				}
				else
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount >= nMinRep\n" );
					int nLength = strlen( poDRegEx->poDRegExpr->pcMatchBuffer );
					poDRegEx->poDRegExpr->pcMatchBuffer[nLength-1] = '\0';
					strcat( poDRegEx->poDRegExpr->pcMatchBuffer, pcRepBuffer );
					pcRepBuffer[0] = '\0';
					char cTemp[2] = {cDigit_i};
					strcat( poDRegEx->poDRegExpr->pcMatchBuffer, cTemp );
					eRet_Code = MS_ERR_MATCHED;
					*eInc_Node_o = INC_No;
				}
			}
			else if( nMaxRep != -1 )            //{,n}
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "{,n}\n" );
				int nLength = strlen( poDRegEx->poDRegExpr->pcMatchBuffer );
				poDRegEx->poDRegExpr->pcMatchBuffer[nLength-1] = '\0';
				char cTemp[2] = {cDigit_i};
				strcat( poDRegEx->poDRegExpr->pcMatchBuffer, cTemp );
				eRet_Code = MS_ERR_MATCHED;
				if( nCurrentRepCount < nMaxRep )
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount < nMaxRep\n" );
					*eInc_Node_o = INC_No;
				}
				else
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount >= nMaxRep\n" );
					*eInc_Node_o = INC_Yes;
				}
			}
			else
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Other Conditions\n" );
			}
		}
		else
		{
			if( nMinRep == nMaxRep )            //{m}
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "{m}\n" );
				eRet_Code = MS_ERR_MATCHFAILURE;
				*eInc_Node_o = INC_No;
			}
			else if( nMinRep != -1 && nMaxRep != -1 )   //{m,n}
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "{m,n}\n" );
				if( nCurrentRepCount < nMinRep )
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount < nMinRep\n" );
					eRet_Code = MS_ERR_MATCHFAILURE;
					*eInc_Node_o = INC_No;
				}
				else if( nCurrentRepCount >= nMinRep && nCurrentRepCount < nMaxRep )
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount >= nMinRep && nCurrentRepCount < nMaxRep\n" );
					eRet_Code = MS_ERR_NOTMATCHED;
					*eInc_Node_o = INC_Immediate;
				}
				else
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount >= nMaxRep\n" );
				}
			}
			else if( nMinRep != -1 )            //{m,}
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "{m,}\n" );
				if( nCurrentRepCount < nMinRep )
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount < nMinRep\n" );
					eRet_Code = MS_ERR_MATCHFAILURE;
					*eInc_Node_o = INC_No;
				}
				else
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount >= nMinRep\n" );
					eRet_Code = MS_ERR_MATCHED;
					*eInc_Node_o = INC_Immediate;
				}
			}
			else if( nMaxRep != -1 )            //{,n}
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "{,n}\n" );
				if( nCurrentRepCount <= nMaxRep )
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount <= nMaxRep\n" );
					eRet_Code = MS_ERR_MATCHED;
					*eInc_Node_o = INC_Immediate;
				}
				else
				{
					TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "nCurrentRepCount >= nMaxRep\n" );
				}
			}
			else
			{
				TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Other Conditions\n" );
			}
		}
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegRepetitionExpr::Destroy
**
** Parameters:
**		t_DRegEx* poDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**
** Additional Details:
**		Destructor for the structure t_DRegRepetitionExpr
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegRepetitionExpr::Destroy( t_DRegEx* poDRegEx_i )
{
	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );
	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( poNode != NULL )
		{
			TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "poNode != NULL \n" );
			eRet_Code = poNode->Destroy( poDRegEx_i );
		}
		else
		{
			TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "poNode == NULL \n" );
			eRet_Code = MS_ERR_NONE;
		}
		delete poNode;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegExpr::_t_DRegExpr
**
**
** Additional Details:
**		Constructor for the structure t_DRegExpr
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegExpr::_t_DRegExpr()
{
	pcMatchBuffer[0] = '\0';
	poNodeList = NULL;
	poCurrentNode = NULL;
}

/******************************************************************************
** Function Name:
**		t_DRegExpr::re_match
**
** Parameters:
**		hDRegEx hDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**		char cDigit_i
**			type: input
**			detail: The character to be matched.
**
**
** Return:
**		An value indicating the success or failure in matching the input
**		    character with the regular expression.
**
**
** Additional Details:
**		This function matches the input character with the regular
**		    expression.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegExpr::re_match( hDRegEx hDRegEx_i, char cDigit_i )
{
	t_DRegEx* poDRegEx_i = (t_DRegEx*)hDRegEx_i;

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		poDRegEx_i->poDRegExpr->poCurrentNode = poDRegEx_i->poDRegExpr->poNodeList;
		eIncNode eInc_Node = INC_No;

		do
		{
			eRet_Code = ((t_DRegNode*)(poDRegEx_i->poDRegExpr->poCurrentNode->data))->re_match( hDRegEx_i, cDigit_i, &eInc_Node );
			if( eRet_Code == MS_ERR_MATCHED || eInc_Node == INC_Immediate )
			{
				if( poDRegEx_i->poDRegExpr->poNodeList->next != NULL && (eInc_Node == INC_Yes || eInc_Node == INC_Immediate))
				{
					TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Next Node Available \n" );
					poDRegEx_i->poDRegExpr->poNodeList = poDRegEx_i->poDRegExpr->poNodeList->next;
				}
				else if( eInc_Node == INC_Yes )
				{
					TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Match Completed \n" );
					eRet_Code = MS_ERR_MATCHCOMPLETE;
				}
				else
				{
					TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Match Not Completed \n" );
				}
			}
			else
			{
				TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "re_match_dreg_expr failed \n" );
			}
		}
		while( eInc_Node == INC_Immediate );
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegExpr::Destroy
**
** Parameters:
**		t_DRegEx* poDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**
** Additional Details:
**		Destructor for the structure t_DRegExpr
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegExpr::Destroy( t_DRegEx* poDRegEx_i )
{
	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );
	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Digit Regular Expression \n" );
		poCurrentNode = g_slist_nth( poNodeList, 0 );

		while( poCurrentNode != NULL )
		{
			eRet_Code = ((t_DRegNode*)(poCurrentNode->data))->Destroy( poDRegEx_i );

			t_List* poTempNode = poCurrentNode;

			poCurrentNode = poCurrentNode->next;
			delete ((t_DRegNode*)(poTempNode->data));
		}
		g_slist_free( poCurrentNode );
		poCurrentNode = NULL;

		poNodeList = NULL;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegEx::_t_DRegEx
**
**
** Additional Details:
**		Constructor for the structure t_DRegEx
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

t_DRegEx::_t_DRegEx() : nDRegTraceID( 0 ), nLastNum( -1 ), nMinNum( -1 ), nMaxNum( -1 ), nDigitCount( 0 )
{
	pcName[0] = '\0';
	poDRegExpr = NULL;
	poCurrentList = NULL;
	poCurrentContextNode = NULL;
}

/******************************************************************************
** Function Name:
**		t_DRegEx::re_match
**
** Parameters:
**		hDRegEx hDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**		char cDigit_i
**			type: input
**			detail: The character to be matched.
**
**
** Return:
**		An value indicating the success or failure in matching the input
**		    character with the regular expression.
**
**
** Additional Details:
**		This function matches the input character with the regular
**		    expression.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegEx::re_match( hDRegEx hDRegEx_i, char cDigit_i )
{
	t_DRegEx* poDRegEx_i = (t_DRegEx*)hDRegEx_i;

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = poDRegExpr->re_match( hDRegEx_i, cDigit_i );
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/******************************************************************************
** Function Name:
**		t_DRegEx::Destroy
**
** Parameters:
**		t_DRegEx* poDRegEx_i
**			type: input
**			detail: This pointer serves as a context for the function.
**
**
** Additional Details:
**		Destructor for the structure t_DRegEx
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

e_MsRetCode t_DRegEx::Destroy( t_DRegEx* poDRegEx_i )
{
	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );
	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = poDRegExpr->Destroy( poDRegEx_i );
		g_slist_free( poCurrentList );
		poCurrentContextNode = NULL;
		delete poDRegExpr;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

