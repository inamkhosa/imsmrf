#include "filewrite_priv.h"

e_MsRetCode FileWriteAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_FileWrite* poFileWrite = hMediaCompAppCtx_i;

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poFileWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_UDP_Packet == poDataPkt->eType ||
		                  DPT_GEN_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_FILEWRITE_MAGIC_NUM == poFileWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nRetVal = AMPS_SUCCESS;
		int nTotalBytesWritten = 0;
		if( NULL != poFileWrite->hFile )
		{
			nRetVal = FW_FileWrite( poFileWrite->oComp.pvFrameworkCtx, poFileWrite->hFile, poDataPkt->oGenPkt.pucData, poDataPkt->oGenPkt.nLength, &nTotalBytesWritten );
			if( AMPS_SUCCESS != nRetVal )
			{
				eRetCode = MS_ERR_FRAMEWORK;
			}
			else
			{
				poFileWrite->nByteWritten += nTotalBytesWritten;
			}
		}
		// Delete packet
		(poDataPkt->pfnDataPktDelete)( poDataPkt );
		FW_Free( poDataPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode FileWriteAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_FileWrite* poFileWrite = hMediaCompAppCtx_i;

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poFileWrite, poFileWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

