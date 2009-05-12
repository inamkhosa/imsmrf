/*! @file dreg_api.cpp
*
* @ingroup Dreg
*
* @brief The file contains implementation of functions for digit regular expression PEG parser API.
*
* @author Razi-ud-Din Chaudhry
*
*/

#include "dreg_priv.h"

/*! @fn e_MsRetCode re_create( hDRegEx* phDRegEx_o, t_DRegExConfig* poDRegExConfig_i )
*
* @brief This function initializes is used to parse the given digit regular expression and initialize the given handle for this purpose.
*
* @param[out] phDRegEx_o is the pointer to the handle for matching the user inputs with the given digit regular expression.
*
* @param[in] poSnmpCommConfig_i is the pointer variable containing the configuration parameters required for initializing digit regular expression.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode re_create( hDRegEx* phDRegEx_o, t_DRegExConfig* poDRegExConfig_i )
{
	TRACE( poDRegExConfig_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_INVALID_INPUT;

	PRE_CONDITION
	{
		ms_assert_return( NULL != phDRegEx_o, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDRegExConfig_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_DRegEx* poDRegEx = NULL;
		poDRegEx = new t_DRegEx;

		char* pcExpression = poDRegExConfig_i->pcExpression;
		strcpy( poDRegEx->pcName, poDRegExConfig_i->pcName );
		poDRegEx->nDRegTraceID = poDRegExConfig_i->nDRegTraceID;
		poDRegEx->poDRegExpr = NULL;

		typedef And <
		t_DReg_Expr,
		Char < '\0' >
		> AndRule;
		bool bResult = AndRule::Matches( pcExpression, poDRegEx );
		if( bResult == true )
		{
			TRACE( poDRegExConfig_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Parsing Successful \n" );
			*(phDRegEx_o) = (hDRegEx*)poDRegEx;
			eRet_Code = MS_ERR_NONE;
		}
		else
		{
			TRACE( poDRegExConfig_i->nDRegTraceID, AMPS_TRACE_LEVEL_ERROR, "Parsing Failed \n" );
		}
/*#ifdef MS_DEBUG
eRet_Code = Print_Parser_Result( poDRegEx );
#endif*/
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegExConfig_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/*! @fn e_MsRetCode re_destroy( hDRegEx* phDRegEx_io )
*
* @brief This function destroys the given digit regular expression.
*
* @param[in,out] phDRegEx_io is the pointer to the handle of the digit regular expression.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode re_destroy( hDRegEx* phDRegEx_io )
{
	t_DRegEx* poDRegEx = (t_DRegEx*)(*phDRegEx_io);

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_NONE;

	PRE_CONDITION
	{
		ms_assert_return( NULL != phDRegEx_io, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = poDRegEx->Destroy( poDRegEx );
		*phDRegEx_io = NULL;
	}

	POST_CONDITION
	{
	}

	TRACE( poDRegEx->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	delete poDRegEx;
	return (eRet_Code);
}

/*! @fn e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i )
*
* @brief This function matches a digit with the given digit regular expression.
*
* @param[in] hDRegEx_i is the handle for matching the user inputs with the given digit regular expression.
*
* @param[in] cDigit_i contains the character to be matched with the given digit regular expression.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i )
{
	TRACE(((t_DRegEx*)hDRegEx_i)->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_NOTMATCHED;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRet_Code = ((t_DRegEx*)hDRegEx_i)->re_match( hDRegEx_i, cDigit_i );
	}

	POST_CONDITION
	{
	}

	TRACE(((t_DRegEx*)hDRegEx_i)->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/*! @fn e_MsRetCode re_get_current_match( hDRegEx hDRegEx_i, char* pcMatch_o, unsigned int* punNumDigits_io )
*
* @brief This function gets the characters that have been matched with the given digit regular expression.
*
* @param[in] hDRegEx_i is the handle for the given digit regular expression.
*
* @param[out] pcMatch_o is the buffer for returning the matched characters.
*
* @param[in,out] punNumDigits_io contains the maximum length of the buffer to store the matched characters as input and returns the actual number of digits stored.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode re_get_current_match( hDRegEx hDRegEx_i, char* pcMatch_o, unsigned int* punNumDigits_io )
{
	TRACE(((t_DRegEx*)hDRegEx_i)->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_INSUFFICIENT_MEMORY;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		strncpy( pcMatch_o, ((t_DRegEx*)hDRegEx_i)->poDRegExpr->pcMatchBuffer, *punNumDigits_io );
		*punNumDigits_io = (unsigned int)strlen( pcMatch_o );
		eRet_Code = MS_ERR_NONE;
	}

	POST_CONDITION
	{
	}

	TRACE(((t_DRegEx*)hDRegEx_i)->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

/*! @fn e_MsRetCode re_get_name( hDRegEx hDRegEx_i, char* pcName_o, unsigned int* punNumDigits_io )
*
* @brief This function gets the name of the given digit regular expression.
*
* @param[in] hDRegEx_i is the handle for the given digit regular expression.
*
* @param[out] pcName_o is the buffer for returning the name of the given digit regular expression.
*
* @param[in,out] punNumDigits_io contains the maximum length of the buffer to store the name of the digit regular expression as input and returns the actual length of the stored string for the name.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode re_get_name( hDRegEx hDRegEx_i, char* pcName_o, unsigned int* punNumDigits_io )
{
	TRACE(((t_DRegEx*)hDRegEx_i)->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	e_MsRetCode eRet_Code = MS_ERR_INSUFFICIENT_MEMORY;

	PRE_CONDITION
	{
		ms_assert_return( NULL != hDRegEx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		strncpy( pcName_o, ((t_DRegEx*)hDRegEx_i)->pcName, *punNumDigits_io );
		*punNumDigits_io = (unsigned int)strlen( pcName_o );
		eRet_Code = MS_ERR_NONE;
	}

	POST_CONDITION
	{
	}

	TRACE(((t_DRegEx*)hDRegEx_i)->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

