/*! @file media_api.c
*
* @ingroup Media
*
* @brief The file contains implementation of functions for media API.
*
*/

#include "media_priv.h"

/*! @fn e_MsRetCode Media_CompCreate( e_MediaCompType eType_i, h_CompConfig hCompConfig_i, h_MediaComp* phMediaComp_o )
*
* @brief This function creates a media component.
*
* @param[in] eType_i is the type of the media component to be created.
*
* @param[in] hCompConfig_i is the handle for the media component configuration.
*
* @param[out] phMediaComp_o is the pointer to the handle of the media component created.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_CompCreate( e_MediaCompType eType_i, h_CompConfig hCompConfig_i, h_MediaComp* phMediaComp_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaCompConfig* poCompConfig = (t_MediaCompConfig*)hCompConfig_i;

	MS_TRACE( poCompConfig->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != phMediaComp_o, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCompConfig_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_MAX > eType_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		ms_assert( NULL != ppfn_CompApi[eType_i].fCompInit );
		eRetCode = (ppfn_CompApi[eType_i].fCompInit)( hCompConfig_i, phMediaComp_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert_return( NULL != *phMediaComp_o, eRetCode = MS_ERR_POST_COND_FAILED );
		}
	}

	MS_TRACE( poCompConfig->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_CompDestroy( h_MediaComp hMediaComp_i )
*
* @brief This function destroys a media component.
*
* @param[in] hMediaComp_i is the handle of the media component to be destroyed.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_CompDestroy( h_MediaComp hMediaComp_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaComp* poMediaComp = (t_MediaComp*)hMediaComp_i;

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		ms_assert( NULL != ppfn_CompApi[poMediaComp->eType].fCompDestroy );
		eRetCode = (ppfn_CompApi[poMediaComp->eType].fCompDestroy)( hMediaComp_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_CompSetAppData( h_MediaComp hMediaComp_i, h_MediaCompAppData hCompAppData_i )
*
* @brief This function sets application data for the media component.
*
* @param[in] hMediaComp_i is the handle of the media component.
*
* @param[in] hCompAppData_i is the handle of the application data for the media component.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_CompSetAppData( h_MediaComp hMediaComp_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaComp* poMediaComp = (t_MediaComp*)hMediaComp_i;

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hCompAppData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		ms_assert( NULL != ppfn_CompApi[poMediaComp->eType].fCompSetAppData );
		eRetCode = (ppfn_CompApi[poMediaComp->eType].fCompSetAppData)( hMediaComp_i, hCompAppData_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_CompSetStatus( h_MediaComp hMediaComp_i, e_MediaCompStatus eStatus )
*
* @brief This function sets the status of the media component.
*
* @param[in] hMediaComp_i is the handle of the media component.
*
* @param[in] eStatus is the status to be set for the media component.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_CompSetStatus( h_MediaComp hMediaComp_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaComp* poMediaComp = (t_MediaComp*)hMediaComp_i;

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		ms_assert( NULL != ppfn_CompApi[poMediaComp->eType].fCompSetStatus );
		eRetCode = (ppfn_CompApi[poMediaComp->eType].fCompSetStatus)( hMediaComp_i, eStatus );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_CompSetParam( h_MediaComp hMediaComp_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
*
* @brief This function sets the parameters of the media component.
*
* @param[in] hMediaComp_i is the handle of the media component.
*
* @param[in] eConfig_i is the media component configuration item for which data is to be set.
*
* @param[in] pvConfigData_i is the pointer to the data for the media component configuration item.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_CompSetParam( h_MediaComp hMediaComp_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaComp* poMediaComp = (t_MediaComp*)hMediaComp_i;

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		ms_assert( NULL != ppfn_CompApi[poMediaComp->eType].fCompSetParam );
		eRetCode = (ppfn_CompApi[poMediaComp->eType].fCompSetParam)( hMediaComp_i, eConfig_i, pvConfigData_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_CompGetParam( h_MediaComp hMediaComp_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
*
* @brief This function gets the parameters of the media component.
*
* @param[in] hMediaComp_i is the handle of the media component.
*
* @param[in] eConfig_i is the media component configuration item for which data is to be obtained.
*
* @param[in] pvConfigData_i is the pointer to the data obtained for the media component configuration item.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_CompGetParam( h_MediaComp hMediaComp_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaComp* poMediaComp = (t_MediaComp*)hMediaComp_i;

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		ms_assert( NULL != ppfn_CompApi[poMediaComp->eType].fCompGetParam );
		eRetCode = (ppfn_CompApi[poMediaComp->eType].fCompGetParam)( hMediaComp_i, eConfig_i, pvConfigData_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_ChainCreate( h_CompConfig hCompConfig_i, h_MediaChain* phMediaChain_o )
*
* @brief This function creates a media chain.
*
* @param[in] hCompConfig_i is the handle of the media component configuration.
*
* @param[out] phMediaChain_o is the pointer to the handle of the media chain created.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainCreate( h_CompConfig hCompConfig_i, h_MediaChain* phMediaChain_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaCompConfig* poCompConfig = (t_MediaCompConfig*)hCompConfig_i;

	MS_TRACE( poCompConfig->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hCompConfig_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phMediaChain_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MediaChain* poMediaChain = NULL;

		poMediaChain = FW_Malloc( sizeof (t_MediaChain));
		if( NULL != poMediaChain )
		{
			// Initialize t_MediaChain
			poMediaChain->oComp.eType = MCT_CHAIN;
			poMediaChain->oComp.eGroup = MCG_OTHER;
			poMediaChain->oComp.nTraceId = poCompConfig->nTraceId;
			poMediaChain->oComp.pvFrameworkCtx = poCompConfig->pvFrameworkCtx;

			poMediaChain->poCompList = FW_DListInit( &poMediaChain->poCompList );
			poMediaChain->nMagicNum = MCT_CHAIN_MAGIC_NUM;
			if( NULL == poMediaChain->poCompList )
			{
				eRetCode = MS_ERR_FRAMEWORK;

				FW_Free( poMediaChain );
				poMediaChain = NULL;
			}
			*phMediaChain_o = (h_MediaChain)poMediaChain;
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poCompConfig->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_ChainAddLink( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp1_i, h_MediaComp hMediaComp2_i )
*
* @brief This function adds a link between media components in the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain.
*
* @param[in] hMediaComp1_i is the handle of the first media component whose link is to be created.
*
* @param[in] hMediaComp2_i is the handle of the second media component whose link is to be created.
*
* @return e_MsRetCode the code for success or error
*
*/

// e_MsRetCode Media_ChainAddLink( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp1_i, h_MediaComp hMediaComp2_i )
// {
//  e_MsRetCode eRetCode = MS_ERR_NONE;
//  t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;
//  t_MediaComp* poMediaComp1 = (t_MediaComp*)hMediaComp1_i;
//  t_MediaComp* poMediaComp2 = (t_MediaComp*)hMediaComp2_i;
//
//  MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );
//
//  //PRE_CONDITION
//  {
//      ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
//      ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//      ms_assert_return( NULL != hMediaComp1_i, eRetCode = MS_ERR_PRE_COND_FAILED );
//      ms_assert_return( NULL != hMediaComp2_i, eRetCode = MS_ERR_PRE_COND_FAILED );
//  }
//
//  {
//      t_MediaCompAppData oAppData;
//
//      oAppData.hAppDataCtx = hMediaComp2_i;
//      oAppData.hAppSignalCtx = poMediaChain;
//      oAppData.pfAppDataCb = ppfn_CompApi[poMediaComp2->eType].fAppDataCb;
//      oAppData.pfAppSignalCb = MediaChain_AppSignalCb;
//
//      eRetCode = Media_CompSetAppData( hMediaComp1_i, &oAppData );
//      if( MS_ERR_NONE == eRetCode )
//      {
//          // First component is only added when its a SOURCE component
//          if( poMediaComp1->eGroup == MCG_SOURCE )
//          {
//              FW_DListAppend( poMediaChain->poCompList, hMediaComp1_i );
//
//              MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Adding component of type = %d\n", poMediaComp1->eType );
//          }
//          FW_DListAppend( poMediaChain->poCompList, hMediaComp2_i );
//
//          MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Adding component of type = %d\n", poMediaComp2->eType );
//      }
//  }
//
//  POST_CONDITION
//  {
//  }
//
//  MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));
//
//  return (eRetCode);
// }

/*! @fn e_MsRetCode Media_ChainAddInChain( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i )
*
* @brief This function adds a media component in the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain.
*
* @param[in] hMediaComp_i is the handle of the media component to be added.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainAddInChain( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;
	t_MediaComp* poMediaComp = (t_MediaComp*)hMediaComp_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		FW_DListAppend( poMediaChain->poCompList, poMediaComp );

		MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Adding component of type = %d\n", poMediaComp->eType );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_ChainRemoveFromChain( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i )
*
* @brief This function removes a media component from the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain.
*
* @param[in] hMediaComp_i is the handle of the media component to be removed.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainRemoveFromChain( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;
	t_MediaComp* poMediaComp = (t_MediaComp*)hMediaComp_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Removing component of type = %d\n", poMediaComp->eType );

		DListRemoveFromData( poMediaChain->poCompList, poMediaComp, NULL );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_ChainSetLink( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp1_i, h_MediaComp hMediaComp2_i )
*
* @brief This function sets a link between media components in the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain.
*
* @param[in] hMediaComp1_i is the handle of the first media component whose link is to be set.
*
* @param[in] hMediaComp2_i is the handle of the second media component whose link is to be set.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainSetLink( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp1_i, h_MediaComp hMediaComp2_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;
	t_MediaComp* poMediaComp2 = (t_MediaComp*)hMediaComp2_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaComp1_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaComp2_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MediaCompAppData oAppData;

		Media_ChainFillAppData( hMediaChain_i, hMediaComp2_i, &oAppData );

		eRetCode = Media_CompSetAppData( hMediaComp1_i, &oAppData );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_ChainFillAppData( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i, t_MediaCompAppData* poAppData_i )
*
* @brief This function fills the appication data for the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain.
*
* @param[in] hMediaComp1_i is the handle of the media component for which application data is to be filled.
*
* @param[in] poAppData_i is the pointer to the application data to be filled.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainFillAppData( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i, t_MediaCompAppData* poAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;
	t_MediaComp* poMediaComp = (t_MediaComp*)hMediaComp_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMediaComp, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poAppData_i->hAppDataCtx = hMediaComp_i;
		poAppData_i->hAppSignalCtx = poMediaChain;
		poAppData_i->pfAppDataCb = ppfn_CompApi[poMediaComp->eType].fAppDataCb;
		poAppData_i->pfAppSignalCb = MediaChain_AppSignalCb;
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_ChainSetAppCb( h_MediaChain hMediaChain_i, f_MediaChainSignalCb pfMediaChainSignalCb_i, h_MediaCompAppCtx hMediaCompAppCtx1_i, h_MediaCompAppCtx hMediaCompAppCtx2_i )
*
* @brief This function callback functions for the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain.
*
* @param[in] pfMediaChainSignalCb_i is the pointer for the signal callback function.
*
* @param[in] hMediaCompAppCtx1_i is the handle of the first application context for the media component.
*
* @param[in] hMediaCompAppCtx1_i is the handle of the second application context for the media component.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainSetAppCb( h_MediaChain hMediaChain_i, f_MediaChainSignalCb pfMediaChainSignalCb_i, h_MediaCompAppCtx hMediaCompAppCtx1_i, h_MediaCompAppCtx hMediaCompAppCtx2_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		poMediaChain->pfMediaChainSignalCb = pfMediaChainSignalCb_i;
		poMediaChain->hMediaCompAppCtx1 = hMediaCompAppCtx1_i;
		poMediaChain->hMediaCompAppCtx2 = hMediaCompAppCtx2_i;
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_ChainDestroy( h_MediaChain hMediaChain_i )
*
* @brief This function destroys the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain to be destroyed.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainDestroy( h_MediaChain hMediaChain_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		FW_DListTraverse( poMediaChain->poCompList, Media_LListMediaCompDestroyCb, poMediaChain );

		FW_DListFree( &poMediaChain->poCompList, NULL );
		// Free Chain structure
		FW_Free( poMediaChain );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn void Media_LListMediaCompDestroyCb( void* pvListNodeData_i, void* pvArg_i )
*
* @brief This callback function destroys list of media components.
*
* @param[in] pvListNodeData_i is the data node for a media component to be destroyed.
*
* @param[in] pvArg_i is the pointer to the additional data required for destroying media component.
*
*/

void Media_LListMediaCompDestroyCb( void* pvListNodeData_i, void* pvArg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)pvArg_i;
	t_MediaComp* poMediaComp = (t_MediaComp*)pvListNodeData_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != pvArg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvListNodeData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_CompDestroy( poMediaComp );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));
}

/*! @fn e_MsRetCode Media_ChainSetStatus( h_MediaChain hMediaChain_i, e_MediaCompStatus eStatus_i )
*
* @brief This function sets the status of the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain.
*
* @param[in] eStatus_i is the new status of the media chain.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainSetStatus( h_MediaChain hMediaChain_i, e_MediaCompStatus eStatus_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		// Set status of all components
		switch( eStatus_i )
		{
			case MCS_START:
			{
				AMPS_DListTraverse( poMediaChain->poCompList, Media_LListMediaCompSetStatusStartCb, poMediaChain );

				break;
			}

			case MCS_STOP:
			{
				AMPS_DListTraverse( poMediaChain->poCompList, Media_LListMediaCompSetStatusStopCb, poMediaChain );

				break;
			}

			case MCS_MAX:
			default:
			{
				MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus_i );
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn void Media_LListMediaCompSetStatusStartCb( void* pvListNodeData_i, void* pvArg_i )
*
* @brief This callback function sets the status of the media component to start state.
*
* @param[in] pvListNodeData_i is the pointer to the media component whose status is to be changed to start state.
*
* @param[in] pvArg_i is the pointer to the additional data required for this callback function.
*
*/

void Media_LListMediaCompSetStatusStartCb( void* pvListNodeData_i, void* pvArg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)pvArg_i;
	t_MediaComp* poMediaComp = (t_MediaComp*)pvListNodeData_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != pvArg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvListNodeData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_CompSetStatus( poMediaComp, MCS_START );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));
}

/*! @fn void Media_LListMediaCompSetStatusStopCb( void* pvListNodeData_i, void* pvArg_i )
*
* @brief This callback function sets the status of the media component to stop state.
*
* @param[in] pvListNodeData_i is the pointer to the media component whose status is to be changed to stop state.
*
* @param[in] pvArg_i is the pointer to the additional data required for this callback function.
*
*/

void Media_LListMediaCompSetStatusStopCb( void* pvListNodeData_i, void* pvArg_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)pvArg_i;
	t_MediaComp* poMediaComp = (t_MediaComp*)pvListNodeData_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != pvArg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvListNodeData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_CompSetStatus( poMediaComp, MCS_STOP );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));
}

/*! @fn e_MsRetCode Media_ChainGetCompAtOffset( h_MediaChain hMediaChain_i, int nOffset_i, h_MediaComp* phMediaComp_o )
*
* @brief This function gets media component at the given offset in the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain.
*
* @param[in] nOffset_i is the offset value in the media chain for accessing media component.
*
* @param[in] phMediaComp_o is the pointer to the handle of the media component at the given offset.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainGetCompAtOffset( h_MediaChain hMediaChain_i, int nOffset_i, h_MediaComp* phMediaComp_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phMediaComp_o, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( nOffset_i >= -1, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_AMPSSList* poListNode = NULL;
		poListNode = DListGetNthNode( poMediaChain->poCompList, nOffset_i );
		if( NULL == poListNode )
		{
			eRetCode = MS_ERR_INVALID_INPUT;
		}
		else
		{
			*phMediaComp_o = (h_MediaComp)poListNode->pvData;
		}
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( *phMediaComp_o != NULL );
		}
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_ChainGetCompOfType( h_MediaChain hMediaChain_i, e_MediaCompType eCompType_i, h_MediaComp* phMediaComp_o )
*
* @brief This function gets the media component of the given type in the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain.
*
* @param[in] eCompType_i is the type of the media component to be obtained.
*
* @param[in] phMediaComp_o is the pointer to the handle of the media component obtained.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainGetCompOfType( h_MediaChain hMediaChain_i, e_MediaCompType eCompType_i, h_MediaComp* phMediaComp_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phMediaComp_o, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_AMPSSList* poListNode = NULL;
		t_MediaComp* poComp = NULL;

		for( poListNode = poMediaChain->poCompList->poAMPSSListHead; NULL != poListNode; poListNode = poListNode->poAMPSSListNext )
		{
			poComp = (t_MediaComp*)poListNode->pvData;
			if( poComp->eType == eCompType_i )
			{
				break;
			}
			else
			{
				poComp = NULL;
			}
		}
		if( NULL == poComp )
		{
			eRetCode = MS_ERR_END_OF_LIST;
		}
		else
		{
			*phMediaComp_o = (h_MediaComp)poComp;
		}
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( *phMediaComp_o != NULL );
		}
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_ChainGetNextComp( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i, h_MediaComp* phMediaComp_o )
*
* @brief This function gets the next media component in the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain.
*
* @param[in] hMediaComp_i is the handle of the current media component.
*
* @param[in] phMediaComp_o is the pointer to the handle of the next media component obtained.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainGetNextComp( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i, h_MediaComp* phMediaComp_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phMediaComp_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_AMPSSList* poListNode = NULL;
		t_MediaComp* poComp = NULL;

		for( poListNode = poMediaChain->poCompList->poAMPSSListHead; NULL != poListNode; poListNode = poListNode->poAMPSSListNext )
		{
			poComp = (t_MediaComp*)poListNode->pvData;
			if( poComp == hMediaComp_i )
			{
				// goto the next component
				poListNode = poListNode->poAMPSSListNext;
				break;
			}
		}
		if( NULL == poListNode )
		{
			eRetCode = MS_ERR_END_OF_LIST;
		}
		else
		{
			*phMediaComp_o = (h_MediaComp)poListNode->pvData;
		}
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( *phMediaComp_o != NULL );
		}
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_ChainGetPrevComp( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i, h_MediaComp* phMediaComp_o )
*
* @brief This function gets the previous media component in the media chain.
*
* @param[in] hMediaChain_i is the handle of the media chain.
*
* @param[in] hMediaComp_i is the handle of the current media component.
*
* @param[in] phMediaComp_o is the pointer to the handle of the previous media component obtained.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_ChainGetPrevComp( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i, h_MediaComp* phMediaComp_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaChain_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_CHAIN_MAGIC_NUM == poMediaChain->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phMediaComp_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_AMPSSList* poListNode = NULL;
		h_MediaComp hPrevComp = NULL;

		for( poListNode = poMediaChain->poCompList->poAMPSSListHead; NULL != poListNode; poListNode = poListNode->poAMPSSListNext )
		{
			if( poListNode->pvData == hMediaComp_i )
			{
				break;
			}
			hPrevComp = poListNode->pvData;
		}
		if( NULL == hPrevComp )
		{
			eRetCode = MS_ERR_END_OF_LIST;
		}
		else
		{
			*phMediaComp_o = hPrevComp;
		}
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( *phMediaComp_o != NULL );
		}
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn e_MsRetCode Media_CompGetType( h_MediaComp hMediaComp_i, e_MediaCompType* peType_o )
*
* @brief This function gets the type of the given media component.
*
* @param[in] hMediaComp_i is the handle of the media component.
*
* @param[out] peType_o is the pointer to the type of the given media component.
*
* @return e_MsRetCode the code for success or error
*
*/

e_MsRetCode Media_CompGetType( h_MediaComp hMediaComp_i, e_MediaCompType* peType_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaComp* poMediaComp = (t_MediaComp*)hMediaComp_i;

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		*peType_o = poMediaComp->eType;
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaComp->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

