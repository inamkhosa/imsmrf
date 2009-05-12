#ifndef _MS_DEBUG_H_
#define _MS_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

#include "ms_ret_code.h"

#ifdef MS_DEBUG
 #define ms_assert_return( cond, retval )      \
	{                       \
		if( !(cond))             \
		{                   \
			(retval);           \
			goto post_condition;        \
		}                   \
	}
#else
 #define ms_assert_return( cond, retval )  (assert((cond)))
#endif

#ifdef MS_DEBUG
 #define ms_assert( cond )             \
	{                       \
		if( !(cond))             \
		{                   \
			goto post_condition;        \
		}                   \
	}
#else
 #define ms_assert( cond )     (assert((cond)))
#endif

#ifdef MS_DEBUG
 #define POST_CONDITION post_condition :
 #define PRE_CONDITION pre_condition :
#else
 #define POST_CONDITION
 #define PRE_CONDITION
#endif

char* get_ms_ret_code_string( e_MsRetCode eRetCode_i );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MS_DEBUG_H_ */

