/*! @file dreg_priv.h
*
* @ingroup Dreg
*
* @brief The file contains additional defines for digit regular expression PEG parser API.
*
* @author Razi-ud-Din Chaudhry
*
*/

#ifndef _DREG_PRIV_H_
#define _DREG_PRIV_H_

#include <glib.h>

#include "peg_templ.h"
#include "dreg/dreg.h"

using namespace peg_templ;

typedef enum
{
	DNT_Digit,
	DNT_RangeExpr,
	DNT_X,
	DNT_Dot,
	DNT_Star,
	DNT_Hash,
	DNT_LongExpr,
	DNT_RepetitionExpr,
	DNT_Range
} e_DRegNodeType;

typedef enum
{
	INC_No,
	INC_Yes,
	INC_Immediate
} eIncNode;

typedef GSList t_List;

typedef struct _t_DRegEx t_DRegEx;

/******************************************************************************
** Structure Name:
**		t_DRegNode
**
** Additional Details:
**		This structure contains regular node type
**		    expression.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegNode
{
	e_DRegNodeType eDRNodeType;

	virtual ~_t_DRegNode()
	{
	}

	virtual e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o ) = 0;

	virtual e_MsRetCode Destroy( t_DRegEx* poDRegEx_i ) = 0;
} t_DRegNode;

/******************************************************************************
** Structure Name:
**		t_DRegX
**
** Additional Details:
**		This structure indicates X.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegX : public t_DRegNode
{
	_t_DRegX();

	~_t_DRegX();

	e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o );

	e_MsRetCode Destroy( t_DRegEx* poDRegEx_i );
} t_DRegX;

/******************************************************************************
** Structure Name:
**		t_DRegStar
**
** Additional Details:
**		This structure indicates star character.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegStar : public t_DRegNode
{
	_t_DRegStar();

	~_t_DRegStar();

	e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o );

	e_MsRetCode Destroy( t_DRegEx* poDRegEx_i );
} t_DRegStar;

/******************************************************************************
** Structure Name:
**		t_DRegHash
**
** Additional Details:
**		This structure indicates hash character.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegHash : public t_DRegNode
{
	_t_DRegHash();

	~_t_DRegHash();

	e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o );

	e_MsRetCode Destroy( t_DRegEx* poDRegEx_i );
} t_DRegHash;

/******************************************************************************
** Structure Name:
**		t_DRegDot
**
** Additional Details:
**		This structure indicates dot character.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegDot : public t_DRegNode
{
	_t_DRegDot();

	~_t_DRegDot();

	e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o );

	e_MsRetCode Destroy( t_DRegEx* poDRegEx_i );
} t_DRegDot;

/******************************************************************************
** Structure Name:
**		t_DRegDigit
**
** Additional Details:
**		This structure contains the value of the digit received.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegDigit : public t_DRegNode
{
	char cValue;

	_t_DRegDigit();

	~_t_DRegDigit();

	e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o );

	e_MsRetCode Destroy( t_DRegEx* poDRegEx_i );
} t_DRegDigit;

/******************************************************************************
** Structure Name:
**		t_DRegRange
**
** Additional Details:
**		This structure contains range values.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegRange : public t_DRegNode
{
	char cMinValue;
	char cMaxValue;

	_t_DRegRange();

	~_t_DRegRange();

	e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o );

	e_MsRetCode Destroy( t_DRegEx* poDRegEx_i );
} t_DRegRange;

/******************************************************************************
** Structure Name:
**		t_DRegLongExpr
**
** Additional Details:
**		This structure contains the node to be detected for long duration
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegLongExpr : public t_DRegNode
{
	t_DRegNode* poNode;

	_t_DRegLongExpr();

	~_t_DRegLongExpr();

	e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o );

	e_MsRetCode Destroy( t_DRegEx* poDRegEx_i );
} t_DRegLongExpr;

/******************************************************************************
** Structure Name:
**		t_DRegRangeExpr
**
** Additional Details:
**		This structure contains the list of nodes in the range
**		    expression.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegRangeExpr : public t_DRegNode
{
	t_List* poNodeList;

	_t_DRegRangeExpr();

	~_t_DRegRangeExpr();

	e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o );

	e_MsRetCode Destroy( t_DRegEx* poDRegEx_i );
} t_DRegRangeExpr;

/******************************************************************************
** Structure Name:
**		t_DRegRepetitionExpr
**
** Additional Details:
**		This structure contains the node to be repeated
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegRepetitionExpr : public t_DRegNode
{
	t_DRegNode* poNode;
	int         nCurrentRepCount;
	int         nMinRep;
	int         nMaxRep;
	char        pcRepBuffer[MAX_DREG_EX_MATCH_BUFFER_LENGTH];

	_t_DRegRepetitionExpr();

	~_t_DRegRepetitionExpr();

	e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i, eIncNode* eInc_Node_o );

	e_MsRetCode Destroy( t_DRegEx* poDRegEx_i );
} t_DRegRepetitionExpr;

/******************************************************************************
** Structure Name:
**		t_DRegExpr
**
** Additional Details:
**		This structure contains the list of nodes in the regular
**		    expression.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegExpr
{
	t_List* poNodeList;
	char    pcMatchBuffer[MAX_DREG_EX_MATCH_BUFFER_LENGTH];
	t_List* poCurrentNode;

	_t_DRegExpr();

	e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i );

	e_MsRetCode Destroy( t_DRegEx* poDRegEx_i );
} t_DRegExpr;

/******************************************************************************
** Structure Name:
**		t_DRegEx
**
** Additional Details:
**		This structure serves as a context to pass on values to
**		    the Match function of different structures.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DRegEx
{
	char        pcName[MAX_DREG_EX_NAME_LENGTH];    //Name of the Expression
	int         nDRegTraceID;   //Trace ID
	t_DRegExpr* poDRegExpr;     //Pointer to Regular Expression
	t_List*     poCurrentList;  //Current List Pointer to be used during creation of regular expression
	t_DRegNode* poCurrentContextNode;  //Current Node Pointer
	int         nLastNum;       //Number through number structure
	int         nMinNum;        //Number stored as minimum limit
	int         nMaxNum;        //Number stored as maximum limit
	int         nDigitCount;    //Number of Digits

	_t_DRegEx();

	e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i );

	e_MsRetCode Destroy( t_DRegEx* poDRegEx_i );
} t_DRegEx;

/******************************************************************************
** Structure Name:
**		t_X
**
** Additional Details:
**		This structure is used to match the input character with the
**		    x character.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_X
{
	typedef Char < 'x' > XRule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			t_DRegX* poDRegNode = NULL;
			poDRegNode = new t_DRegX;

			bResult = XRule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				if( pC_i->poCurrentContextNode == NULL )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "X Matches & Current Node NULL \n" );
					pC_i->poCurrentList = g_slist_append( pC_i->poCurrentList, (void*)poDRegNode );
				}
				else if( pC_i->poCurrentContextNode->eDRNodeType == DNT_LongExpr )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "X Matches as Long Expression \n" );
					((t_DRegLongExpr*)pC_i->poCurrentContextNode)->poNode = poDRegNode;
				}
				else if( pC_i->poCurrentContextNode->eDRNodeType == DNT_RepetitionExpr )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "X Matches as Repetition Expression \n" );
					((t_DRegRepetitionExpr*)pC_i->poCurrentContextNode)->poNode = poDRegNode;
				}
				else
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "X Matches Other Conditions \n" );
					poDRegNode->Destroy( pC_i );
					delete poDRegNode;
				}
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "X does not match \n" );
				poDRegNode->Destroy( pC_i );
				delete poDRegNode;
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_X;

/******************************************************************************
** Structure Name:
**		t_Star
**
** Additional Details:
**		This structure is used to match the input character with the
**		    star character.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Star
{
	typedef Char < '*' > StarRule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			t_DRegStar* poDRegNode = NULL;
			poDRegNode = new t_DRegStar;

			bResult = StarRule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				if( pC_i->poCurrentContextNode == NULL )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Star Matches & Current Node Null \n" );
					pC_i->poCurrentList = g_slist_append( pC_i->poCurrentList, (void*)poDRegNode );
				}
				else if( pC_i->poCurrentContextNode->eDRNodeType == DNT_LongExpr )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Star Matches as Long Expression \n" );
					((t_DRegLongExpr*)pC_i->poCurrentContextNode)->poNode = poDRegNode;
				}
				else if( pC_i->poCurrentContextNode->eDRNodeType == DNT_RepetitionExpr )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Star Matches as Repetition Expression \n" );
					((t_DRegRepetitionExpr*)pC_i->poCurrentContextNode)->poNode = poDRegNode;
				}
				else
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Star Matches Other Conditions \n" );
				}
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Star does not Match \n" );
				delete poDRegNode;
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Star;

/******************************************************************************
** Structure Name:
**		t_Hash
**
** Additional Details:
**		This structure is used to match the input character with the
**		    hash character.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Hash
{
	typedef Char < '#' > HashRule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			t_DRegHash* poDRegNode = NULL;
			poDRegNode = new t_DRegHash;

			bResult = HashRule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				if( pC_i->poCurrentContextNode == NULL )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Hash Matches & Current Node Null \n" );
					pC_i->poCurrentList = g_slist_append( pC_i->poCurrentList, (void*)poDRegNode );
				}
				else if( pC_i->poCurrentContextNode->eDRNodeType == DNT_LongExpr )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Hash Matches as Long Expression \n" );
					((t_DRegLongExpr*)pC_i->poCurrentContextNode)->poNode = poDRegNode;
				}
				else if( pC_i->poCurrentContextNode->eDRNodeType == DNT_RepetitionExpr )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Hash Matches as Repetition Expression \n" );
					((t_DRegRepetitionExpr*)pC_i->poCurrentContextNode)->poNode = poDRegNode;
				}
				else
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Hash Matches Other Conditions \n" );
				}
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Hash does not Match \n" );
				delete poDRegNode;
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Hash;

/******************************************************************************
** Structure Name:
**		t_Dot
**
** Additional Details:
**		This structure is used to match the input character with the
**		    dot character.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Dot
{
	typedef Char < '.' > DotRule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			t_DRegDot* poDRegNode = NULL;
			poDRegNode = new t_DRegDot;

			bResult = DotRule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				if( pC_i->poCurrentContextNode == NULL )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Dot Matches & Current Node Null\n" );
					pC_i->poCurrentList = g_slist_append( pC_i->poCurrentList, (void*)poDRegNode );
				}
				else if( pC_i->poCurrentContextNode->eDRNodeType == DNT_LongExpr )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Dot Matches as Long Expression \n" );
					((t_DRegLongExpr*)pC_i->poCurrentContextNode)->poNode = poDRegNode;
				}
				else if( pC_i->poCurrentContextNode->eDRNodeType == DNT_RepetitionExpr )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Dot Matches as Repetition Expression \n" );
					((t_DRegRepetitionExpr*)pC_i->poCurrentContextNode)->poNode = poDRegNode;
				}
				else
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Dot Matches Other Conditions \n" );
				}
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Dot does not Match \n" );
				delete poDRegNode;
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Dot;

/******************************************************************************
** Structure Name:
**		t_Extra
**
** Additional Details:
**		This structure is used to match the input character with the
**		    extra characters.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Extra
{
	typedef In < 'a', 'd', 'A', 'D' > ExtraRule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			t_DRegDigit* poDRegNode = NULL;
			poDRegNode = new t_DRegDigit;

			bool bResult = ExtraRule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				poDRegNode->cValue = p_io[-1];
				if( pC_i->poCurrentContextNode == NULL )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Extra Matches & Current Node Null \n" );
					pC_i->poCurrentList = g_slist_append( pC_i->poCurrentList, (void*)poDRegNode );
					pC_i->poCurrentContextNode = NULL;
				}
				else
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Extra Matches Other Conditions \n" );
					delete poDRegNode;
				}
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Extra does not Match \n" );
				delete poDRegNode;
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Extra;

/******************************************************************************
** Structure Name:
**		t_Digit
**
** Additional Details:
**		This structure is used to match the input character with the
**		    numerical digit.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Digit
{
	typedef In < '0', '9' > DigitRule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			t_DRegDigit* poDRegNode = NULL;
			poDRegNode = new t_DRegDigit;

			bResult = DigitRule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				poDRegNode->cValue = p_io[-1];
				if( pC_i->poCurrentContextNode == NULL )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Digit Matches & Current Node Null \n" );
					pC_i->poCurrentList = g_slist_append( pC_i->poCurrentList, (void*)poDRegNode );
					pC_i->poCurrentContextNode = NULL;
				}
				else if( pC_i->poCurrentContextNode->eDRNodeType == DNT_LongExpr )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Digit Matches as Long Expression \n" );
					((t_DRegLongExpr*)pC_i->poCurrentContextNode)->poNode = poDRegNode;
				}
				else if( pC_i->poCurrentContextNode->eDRNodeType == DNT_RepetitionExpr )
				{
					if( ((t_DRegRepetitionExpr*)pC_i->poCurrentContextNode)->poNode == NULL )
					{
						MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Digit Matches as Repetition Expression with Null Repetition Node \n" );
						((t_DRegRepetitionExpr*)pC_i->poCurrentContextNode)->poNode = poDRegNode;
					}
					else
					{
						MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Digit Matches as Repetition Expression with no Null Repetition Node \n" );
						delete poDRegNode;
					}
				}
				else
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Digit Matches Other Conditions \n" );
					delete poDRegNode;
				}
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Digit does not Match \n" );
				delete poDRegNode;
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Digit;

/******************************************************************************
** Structure Name:
**		t_Number
**
** Additional Details:
**		This structure is used to match the input characters with the
**		    numerical digits and if matched, combines the individual
**		    digits to a number.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Number
{
	typedef PlusRepeat<t_Digit> PlusRepeatDigitRule;
	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			It pStart = p_io;
			bResult = PlusRepeatDigitRule::Matches( p_io, pC_i );
			pC_i->nDigitCount = p_io - pStart;
			if( bResult == true )
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Number Matches \n" );
				pC_i->nLastNum = 0;

				while( pC_i->nDigitCount > 0 )
				{
					pC_i->nLastNum *= 10;
					pC_i->nLastNum += *(p_io - pC_i->nDigitCount) - '0';
					pC_i->nDigitCount--;
				}
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Number does not Match \n" );
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Number;

/******************************************************************************
** Structure Name:
**		t_MinNum
**
** Additional Details:
**		This structure is used to match the input characters with the
**		    numerical digits and store the number as minimum limit.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_MinNum
{
	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			bResult = t_Number::Matches( p_io, pC_i );
			if( bResult == true )
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Minimum Number Matches \n" );
				pC_i->nMinNum = pC_i->nLastNum;
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Minimum Number does not Match \n" );
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_MinNum;

/******************************************************************************
** Structure Name:
**		t_MaxNum
**
** Additional Details:
**		This structure is used to match the input characters with the
**		    numerical digits and store the number as maximum limit.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_MaxNum
{
	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			bResult = t_Number::Matches( p_io, pC_i );
			if( bResult == true )
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Maximum Number Matches \n" );
				pC_i->nMaxNum = pC_i->nLastNum;
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Maximum Number does not Match \n" );
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_MaxNum;

/******************************************************************************
** Structure Name:
**		t_Comma
**
** Additional Details:
**		This structure is used to match the input character with the
**		    comma character.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Comma
{
	typedef Char < ',' > CommaRule;
	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			bResult = CommaRule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Comma Matches \n" );
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Comma does not Match \n" );
				pC_i->nMaxNum = pC_i->nMinNum;
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Comma;

/******************************************************************************
** Structure Name:
**		t_Repetition
**
** Additional Details:
**		This structure is used to match the input characters with the
**		    repetition rule.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Repetition
{
	typedef And <
	Char < '{' >,
	Option<t_MinNum>,
	Option<t_Comma>,
	Option<t_MaxNum>,
	Char < '}' >
	> Repetition_Rule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			pC_i->nMinNum = -1;
			pC_i->nMaxNum = -1;
			pC_i->nDigitCount = 0;

			pC_i->poCurrentContextNode->eDRNodeType = DNT_X;
			bResult = Repetition_Rule::Matches( p_io, pC_i );
			pC_i->poCurrentContextNode->eDRNodeType = DNT_RepetitionExpr;
			if( bResult == true )
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Repetition Matches \n" );
				((t_DRegRepetitionExpr*)pC_i->poCurrentContextNode)->nMinRep = pC_i->nMinNum;
				((t_DRegRepetitionExpr*)pC_i->poCurrentContextNode)->nMaxRep = pC_i->nMaxNum;
				((t_DRegRepetitionExpr*)pC_i->poCurrentContextNode)->nCurrentRepCount = 0;
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Repetition does not Match \n" );
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Repetition;

/******************************************************************************
** Structure Name:
**		t_Range
**
** Additional Details:
**		This structure is used to match the input characters with the
**		    range.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Range
{
	typedef Or <
	And <
	t_Digit,
	Char < '-' >,
	t_Digit
	>,
	And <
	t_Extra,
	Char < '-' >,
	t_Extra
	>
	> Range_Rule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			t_DRegRange* poDRegNode = NULL;
			poDRegNode = new t_DRegRange;

			pC_i->poCurrentContextNode = poDRegNode;

			bResult = Range_Rule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				if( pC_i->poCurrentContextNode->eDRNodeType == DNT_Range )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Range Matches with Current Node \n" );
					poDRegNode->cMinValue = p_io[-3];
					poDRegNode->cMaxValue = p_io[-1];
					pC_i->poCurrentList = g_slist_append( pC_i->poCurrentList, (void*)poDRegNode );
				}
				else
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Range Matches Other Conditions \n" );
					poDRegNode->Destroy( pC_i );
					delete poDRegNode;
				}
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Range does not Match \n" );
				poDRegNode->Destroy( pC_i );
				delete poDRegNode;
			}
			pC_i->poCurrentContextNode = NULL;
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Range;

/******************************************************************************
** Structure Name:
**		t_Range_Value
**
** Additional Details:
**		This structure is used to match the input characters with the
**		    range value.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Range_Value
{
	typedef Or<
	           t_Range,
	           t_Digit,
	           t_Extra,
	           t_Star,
	           t_Hash
	> Range_Value_Rule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			bResult = Range_Value_Rule::Matches( p_io, pC_i );
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Range_Value;

/******************************************************************************
** Structure Name:
**		t_Long_Expr
**
** Additional Details:
**		This structure is used to match the input characters with the
**		    long expression.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Long_Expr
{
	typedef And <
	Char < 'L' >,
	Or<
	   t_X,
	   t_Star,
	   t_Hash,
	   t_Dot,
	   t_Digit
	>
	> Long_Expr_Rule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			t_DRegLongExpr* poDRegNode = NULL;
			poDRegNode = new t_DRegLongExpr;

			pC_i->poCurrentContextNode = poDRegNode;

			bResult = Long_Expr_Rule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Long Expression Mathces \n" );
				pC_i->poCurrentList = g_slist_append( pC_i->poCurrentList, (void*)poDRegNode );
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Long Expression does not Match \n" );
				delete poDRegNode;
			}
			pC_i->poCurrentContextNode = NULL;
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Long_Expr;

/******************************************************************************
** Structure Name:
**		t_Range_Expr
**
** Additional Details:
**		This structure is used to match the input characters with the
**		    range expression.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Range_Expr
{
	typedef And <
	Char < '[' >,
	PlusRepeat<t_Range_Value>,
	Char < ']' >
	> Range_Expr_Rule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			t_DRegNode* poCurrentNode = pC_i->poCurrentContextNode;
			t_List* poCurrentList = pC_i->poCurrentList;
			t_DRegRangeExpr* poDRegNode = NULL;
			poDRegNode = new t_DRegRangeExpr;
			poDRegNode->poNodeList = NULL;
			pC_i->poCurrentList = poDRegNode->poNodeList;
			pC_i->poCurrentContextNode = NULL;

			bResult = Range_Expr_Rule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				poDRegNode->poNodeList = pC_i->poCurrentList;
				if( poCurrentNode == NULL )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Range Expression Matches & Current Node Null \n" );
					poCurrentList = g_slist_append( poCurrentList, (void*)poDRegNode );
				}
				else if( poCurrentNode->eDRNodeType == DNT_RepetitionExpr )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Range Expression Matches as Repetition Expression \n" );
					((t_DRegRepetitionExpr*)poCurrentNode)->poNode = poDRegNode;
				}
				else
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Range Expression Matches Other Conditions \n" );
					poDRegNode->Destroy( pC_i );
					delete poDRegNode;
				}
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Range Expression does not Match \n" );
				poDRegNode->Destroy( pC_i );
				delete poDRegNode;
			}
			pC_i->poCurrentList = poCurrentList;
			pC_i->poCurrentContextNode = poCurrentNode;
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Range_Expr;

/******************************************************************************
** Structure Name:
**		t_Repetition_Expr
**
** Additional Details:
**		This structure is used to match the input characters with the
**		    repetition expression.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_Repetition_Expr
{
	typedef And<
	            Or<
	               t_Range_Expr,
	               t_X,
	               t_Star,
	               t_Hash,
	               t_Dot,
	               t_Digit
	            >,
	            t_Repetition
	> Repetition_Expr_Rule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			t_DRegRepetitionExpr* poDRegNode = NULL;
			poDRegNode = new t_DRegRepetitionExpr;

			pC_i->poCurrentContextNode = poDRegNode;

			bResult = Repetition_Expr_Rule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Repetition Expression Matches \n" );
				pC_i->poCurrentList = g_slist_append( pC_i->poCurrentList, (void*)poDRegNode );
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Repetition Expression does not Match \n" );
				poDRegNode->Destroy( pC_i );
				delete poDRegNode;
			}
			pC_i->poCurrentContextNode = NULL;
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_Repetition_Expr;

/******************************************************************************
** Structure Name:
**		t_DReg_Expr
**
** Additional Details:
**		This structure is used to match the input characters with the
**		    digit regular expression PEG grammer.
**
*******************************************************************************
** Created By:	: Razi-ud-Din Chaudhry
*******************************************************************************
*******************************************************************************
*/

typedef struct _t_DReg_Expr
{
	typedef PlusRepeat<
	                   Or<
	                      t_Repetition_Expr,
	                      t_Range_Expr,
	                      t_Long_Expr,
	                      t_X,
	                      t_Star,
	                      t_Hash,
	                      t_Dot,
	                      t_Digit
	                   >
	> DReg_Expr_Rule;

	template<typename It, typename Context>
	static bool Matches( It&p_io, Context* pC_i )
	{
		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

		bool bResult = false;

		PRE_CONDITION
		{
			ms_assert_return( NULL != p_io, bResult = false );
			ms_assert_return( NULL != pC_i, bResult = false );
		}

		{
			MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Given Expression : %s \n", p_io );
			pC_i->poDRegExpr = NULL;
			pC_i->poDRegExpr = new t_DRegExpr;
			pC_i->poCurrentList = pC_i->poDRegExpr->poNodeList;
			pC_i->poCurrentContextNode = NULL;

			bResult = DReg_Expr_Rule::Matches( p_io, pC_i );
			if( bResult == true )
			{
				pC_i->poDRegExpr->poNodeList = pC_i->poCurrentList;
				t_List* pList = g_slist_nth( pC_i->poDRegExpr->poNodeList, 0 );
				pC_i->poCurrentContextNode = NULL;
				if( pList != NULL )
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Valid pList \n" );
					pC_i->poDRegExpr->poCurrentNode = pList;
				}
				else
				{
					MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Invalid pList \n" );
					pC_i->Destroy( pC_i );
				}
			}
			else
			{
				MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Invalid Expression \n" );
				pC_i->Destroy( pC_i );
			}
		}

		POST_CONDITION
		{
		}

		MS_TRACE( pC_i->nDRegTraceID, AMPS_TRACE_LEVEL_INFO, "Leaving with error code : %d\n", bResult );
		return (bResult);
	}
} t_DReg_Expr;

// #ifdef MS_DEBUG
// e_MsRetCode Print_Parser_Result( t_DRegEx* poDRegEx_i );
// #endif

#endif /* _DREG_PRIV_H */

