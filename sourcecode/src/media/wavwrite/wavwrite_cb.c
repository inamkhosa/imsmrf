#include "wavwrite_priv.h"

e_MsRetCode WavWriteAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_WavWrite* poWavWrite = hMediaCompAppCtx_i;

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poWavWrite, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_UDP_Packet == poDataPkt->eType ||
		                  DPT_GEN_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_WAVWRITE_MAGIC_NUM == poWavWrite->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Packet info = %p(%d)\n", poDataPkt->oGenPkt.pucData, poDataPkt->oGenPkt.nLength );

		int nTotalBytesWritten = 0;
		if( NULL != poWavWrite->poFile )
		{
			MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Packet time = %d\n", poDataPkt->nPktGenTime );

			nTotalBytesWritten = sf_write_raw( poWavWrite->poFile, poDataPkt->oGenPkt.pucData, poDataPkt->oGenPkt.nLength );
			if( 0 > nTotalBytesWritten )
			{
				eRetCode = MS_ERR_FRAMEWORK;
			}
			else
			{
				poWavWrite->nByteWritten += nTotalBytesWritten;
			}
		}
		// Delete packet
		(poDataPkt->pfnDataPktDelete)( poDataPkt );
		FW_Free( poDataPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode WavWriteAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_WavWrite* poWavWrite = hMediaCompAppCtx_i;

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poWavWrite, poWavWrite->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

