/*! @file ms_api.c
*
* @brief The file contains implementation of functions in media server API.
*
* @author Razi-ud-Din Chaudhry
*
*/

#include "ms_api.h"

/*! @fn e_MsRetCode MS_GetNumOfRTPPackets( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, e_MediaCompType eCompType_i, e_MediaCompConfig eConfig_i, unsigned int* punValue_o )
*
* @brief This function calculates the number of RTP packets received or sent by the media server.
*
* @param[in] poMsCtx_i is the pointer to the context of the media server.
*
* @param[in] hMediaChain_i is the handle of the media chain to be used for calculation of number of RTP packets.
*
* @param[in] eCompType_i is the type of the media component.
*
* @param[in] eConfig_i is the configuration of the media component.
*
* @param[out] punValue_o is the pointer to the memory ocation for storing the number of RTP packets.
*
* @return e_MsRetCode the code for success or error
*
* @author Razi-ud-Din Chaudhry
*
*/

e_MsRetCode MS_GetNumOfRTPPackets( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, e_MediaCompType eCompType_i, e_MediaCompConfig eConfig_i, unsigned int* punValue_o )
{
	e_MsRetCode eRet_Code = MS_ERR_NONE;
	int nTraceId = poMsCtx_i->nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRet_Code = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != punValue_o, eRet_Code = MS_ERR_PRE_COND_FAILED );
	}

	{
		unsigned int unValue;
		h_MediaComp hMediaComp;
		eRet_Code = Media_ChainGetCompOfType( hMediaChain_i, eCompType_i, &hMediaComp );
		if( MS_ERR_NONE == eRet_Code )
		{
			eRet_Code = Media_CompGetParam( hMediaComp, eConfig_i, (void*) &unValue );
			if( MS_ERR_NONE == eRet_Code )
			{
				*punValue_o = unValue;
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRet_Code ));
	return (eRet_Code);
}

