#include "ms_team.h"

e_MsRetCode MS_Team_Init( t_MsCtx* poMsCtx_i, t_AMPSDList** ppoTeamMateList_io )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != ppoTeamMateList_io, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		FW_DListInit( ppoTeamMateList_io );
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_Team_Destroy( t_MsCtx* poMsCtx_i, t_AMPSDList** ppoTeamMateList_io )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != ppoTeamMateList_io, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		FW_DListFree( ppoTeamMateList_io, NULL );
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

// int MS_DeleteTeamMate(poMsCtx_i, poMsConfCtx_i, t_String* poTeamMateId_i )
// {
//                  eRetCode = MS_GetUserCtxFromId( poMsCtx_i, poMsConfCtx_i, poTeamMateId_i, &poUserCtrlCtx2 );
//                  if( MS_ERR_NONE == eRetCode )
//                  {
//                      if( 0 == xmlStrcasecmp( pchAction, (const xmlChar*)"add" ))
//                      {
//                          eRetCode = MS_Team_AddTeamMate( poMsCtx_i, poMsConfCtx_i, poUserCtrlCtx1,  poUserCtrlCtx2 );
//                      }
//                      else if( 0 == xmlStrcasecmp( pchAction, (const xmlChar*)"delete" ))
//                      {
//                          eRetCode = MS_Team_DeleteTeamMate( poMsCtx_i, poMsConfCtx_i, poUserCtrlCtx1,  poUserCtrlCtx2 );
//                      }
//                  }
//                  else
//                  {
//                      TS_Print( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_WARNING, "No user control context found agaist team-mate id (skipped) = ", &oTeamMateId );
//                  }
//                  TS_Free( &oTeamMateId );
//
//  MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );
//
//  return (eRetCode);
// }

e_MsRetCode MS_Team_AddTeamMate( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx1_i,  t_MsUserCtrlCtx* poUserCtrlCtx2_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsConfCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUserCtrlCtx1_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUserCtrlCtx2_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		FW_DListAppend( poUserCtrlCtx1_i->poTeamMateList, poUserCtrlCtx2_i );
		FW_DListAppend( poUserCtrlCtx2_i->poTeamMateList, poUserCtrlCtx1_i );

		// ASSUMPTION: a team will only contain two members

//      eRetCode = MS_Media_AddInMixer( poMsCtx_i, poMsConfCtx_i, poUserCtrlCtx1_i );
//      ms_assert( MS_ERR_NONE == eRetCode );
//
//      eRetCode = MS_Media_AddInMixer( poMsCtx_i, poMsConfCtx_i, poUserCtrlCtx2_i );
//      ms_assert( MS_ERR_NONE == eRetCode );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_Team_DeleteTeamMate( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx1_i,  t_MsUserCtrlCtx* poUserCtrlCtx2_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMsConfCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUserCtrlCtx1_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUserCtrlCtx2_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		DListRemoveFromData( poUserCtrlCtx1_i->poTeamMateList, poUserCtrlCtx2_i, NULL );
		DListRemoveFromData( poUserCtrlCtx2_i->poTeamMateList, poUserCtrlCtx1_i, NULL );

		ms_assert( poUserCtrlCtx1_i->poTeamMateList->uchCount == poUserCtrlCtx2_i->poTeamMateList->uchCount );
		if( poUserCtrlCtx1_i->poTeamMateList->uchCount == 0 )
		{
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

