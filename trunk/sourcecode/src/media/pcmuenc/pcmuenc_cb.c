#include "pcmuenc_priv.h"

e_MsRetCode PcmuEncAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_PcmuEnc* poPcmuEnc = hMediaCompAppCtx_i;

	MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poPcmuEnc, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_GEN_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_PCMUENC_MAGIC_NUM == poPcmuEnc->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nLength = poDataPkt->oGenPkt.nLength;
		unsigned char* pucData = poDataPkt->oGenPkt.pucData;

		MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Raw data length = %d\n", nLength );

		// Encode data
		mulaw_enc( (char*)pucData, (char*)pucData, nLength );
		//nLength /= 2;

		// Change packet type to CODEC
		AllocCodecPkt((h_DataPkt*)&poDataPkt );

		poDataPkt->oCodecPkt.ucPayloadType = 0;     // PCMU
		poDataPkt->oCodecPkt.unNumSamples = nLength;
		poDataPkt->oCodecPkt.unStartOfMedia = poPcmuEnc->unStartOfMedia;
		poDataPkt->oCodecPkt.nLength = nLength/2;
		poDataPkt->oCodecPkt.pucData = pucData;

		poPcmuEnc->unStartOfMedia = 0;

		MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Encoded data length = %d\n", poDataPkt->oCodecPkt.nLength );
		MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Number of samples   = %d\n", poDataPkt->oCodecPkt.unNumSamples );

		MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Data callback function = %p\n", poPcmuEnc->oAppData.pfAppDataCb );

		// Call data callback function of the next layer
		ms_assert_return( NULL != poPcmuEnc->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
		eRetCode = (poPcmuEnc->oAppData.pfAppDataCb)((h_MediaComp)poPcmuEnc, (h_MediaCompAppCtx)poPcmuEnc->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode PcmuEncAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_PcmuEnc* poPcmuEnc = hMediaCompAppCtx_i;

	MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poPcmuEnc, poPcmuEnc->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

