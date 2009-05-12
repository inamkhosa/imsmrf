/*! @file dreg.h
*
* @ingroup Dreg
*
* @brief The file contains defines for digit regular expression PEG parser API.
*
* @author Razi-ud-Din Chaudhry
*
*/

/*! @page MsDreg Digit Regular Expression
*
* @brief This page contains information related to digit regular expression
*
* Digit regular expression is used to collect the dtmf keys pressed by the user
* according to a certain format. This format for the regular expression has been
* defined in Appendix A of RFC-5022. @ref Dreg has been developed to serve
* this purpose.
*
* Parsing Expression Grammar (PEG) is used to parse the regular expression. PEG
* is a standard notation that can be used to parse a string that has defined format.
* Once the syntax of the input is specified in the PEG notation, standard
* parser-generators are available. These generators take the PEG notation as input
* and generates code of a basic parser. More code is added to generate the
* data-structures that are required.
*
* Further information about PEG can be found on the following link:
*
* http://en.wikipedia.org/wiki/Parsing_expression_grammar
*
* @section DregNotation Digit Regular Expression PEG Notation:
*
* \b dreg_expr: (repetition_expr / range_expr / long_expr / x / star / hash / dot / digit )+
*
* \b repetition_expr: ( range_expr  / x / star / hash / dot / digit ) repetition
*
* \b range_expr: '[' range_value+ ']'
*
* \b long_expr: long ( x / star / hash / dot / digit )
*
* \b range_value: range / digit / extra / star / hash
*
* \b range: (digit '-' digit) / (extra '-' extra)
*
* \b repetition: '{' minnum? ','? maxnum? '}'
*
* \b minnum: num
*
* \b maxnum: num
*
* \b digit: '0' / '1' / '2' / '3' / '4' / '5' / '6' / '7' / '8' / '9'
*
* \b extra: 'a' / 'b' / 'c' / 'd' / 'A' / 'B' / 'C' / 'D'
*
* \b num: digit+
*
* \b x: 'x'
*
* \b star: '*'
*
* \b hash: '#'
*
* \b dot: '.'
*
* \b long: 'L'
*
* @author Razi-ud-Din Chaudry
*
*/

/*! @defgroup Dreg Digit Regular Expression Module
*
* @brief This module consist of functionality for parsing different digit regular expressions using the concepts of parsing expression grammer (PEG).
*
* For more information about Digit Regular Expression and Parsing Expression Grammer, see @ref FaqDregQ1
*
* @author Razi-ud-Din Chaudhry
*
* @{
*
*/

#ifndef _DREG_H_
#define _DREG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ms_ret_code.h"

#include "ms_debug.h"
#include "fw_api.h"

/*! This number is used for maximum size of the name assigned to the given digit regular expression */
#define MAX_DREG_EX_NAME_LENGTH 20

/*! This number is used for maximum size of the digit regular expression */
#define MAX_DREG_EX_EXPRESSION_LENGTH 30

/*! This number is used for maximum size of the digit regular expression matching buffer */
#define MAX_DREG_EX_MATCH_BUFFER_LENGTH 30

/*! Handle for digit regular expression created */
typedef void* hDRegEx;

/*! @struct t_DRegExConfig
*
* The configuration of the digit regular expression received from the client.
*
* @author Razi-ud-Din Chaudhry
*
*/

typedef struct _t_DRegExConfig
{
	char         pcName[MAX_DREG_EX_NAME_LENGTH]; /*!< Name of the digit regular expression */
	char         pcExpression[MAX_DREG_EX_EXPRESSION_LENGTH]; /*!< Digit regular expression */
	unsigned int unLongMatchDurationMs; /*!< Long digit duration in milliseconds */
	int          nDRegTraceID; /*!< Trace id for media server traces */
} //! Struct variable.
/*! Digit regular expression configuration structure. */ t_DRegExConfig;

e_MsRetCode re_create( hDRegEx* phDRegEx_o, t_DRegExConfig* poDRegExConfig_i );

e_MsRetCode re_destroy( hDRegEx* phDRegEx_io );

e_MsRetCode re_match( hDRegEx hDRegEx_i, char cDigit_i );

e_MsRetCode re_get_current_match( hDRegEx hDRegEx_i, char* pcMatch_o, unsigned int* punNumDigits_io );

e_MsRetCode re_get_name( hDRegEx hDRegEx_i, char* pcName_o, unsigned int* punNumDigits_io );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _DREG_H_ */

/*! @} */ // End of Dreg Module

