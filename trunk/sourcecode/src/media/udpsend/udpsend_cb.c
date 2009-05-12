#include "udpsend_priv.h"

e_MsRetCode UdpSendAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_UdpSend* poUdpSend = hMediaCompAppCtx_i;

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_UDP_Packet == poDataPkt->eType ||
		                  DPT_RTP_Packet == poDataPkt->eType ||
		                  DPT_GEN_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPSEND_MAGIC_NUM == poUdpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nRetCode = AMPS_SUCCESS;
		unsigned char* pucData = NULL;
		int nLength = 0;

		switch( poDataPkt->eType )
		{
			case DPT_GEN_Packet:
				pucData = poDataPkt->oGenPkt.pucData;
				nLength = poDataPkt->oGenPkt.nLength;
				break;

			case DPT_UDP_Packet:
				pucData = poDataPkt->oUdpPkt.pucData;
				nLength = poDataPkt->oUdpPkt.nLength;
				break;

			case DPT_CODEC_Packet:
				pucData = poDataPkt->oCodecPkt.pucData;
				nLength = poDataPkt->oCodecPkt.nLength;
				break;

			case DPT_RTP_Packet:
				pucData = rtp_packet_get_header( poDataPkt->oRtpPkt.poRtpPkt );
				nLength = rtp_packet_get_packet_len( poDataPkt->oRtpPkt.poRtpPkt );
				break;

			default:
				ms_assert_return( 0, eRetCode = MS_ERR_INVALID_PACKET );
		}

		MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Sending %d bytes on %s:%d\n", nLength,  poUdpSend->pcRemoteIp, poUdpSend->nRemotePort );

		{
			char pchLocalIp[32] = "";
			int nLocalPort = 0;

			FW_GetLocalIPAddressFromNetHandle( poUdpSend->oComp.pvFrameworkCtx, poUdpSend->hUdpNetHandle, pchLocalIp );

			FW_GetLocalPortFromNetHandle( poUdpSend->oComp.pvFrameworkCtx, poUdpSend->hUdpNetHandle, &nLocalPort );

			MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Sending %d bytes from %s:%d\n", nLength,  pchLocalIp, nLocalPort );
		}

		nRetCode = AMPS_SndBuffer( poUdpSend->oComp.pvFrameworkCtx, poUdpSend->hUdpNetHandle, pucData, nLength, poUdpSend->nRemotePort, poUdpSend->pcRemoteIp );
		//poDataPkt->oUdpPkt.nRemotePort, poDataPkt->oUdpPkt.pcRemoteIpAddress);
		if( AMPS_SUCCESS != nRetCode )
		{
			eRetCode = MS_ERR_FRAMEWORK;
		}
		// Delete packet
		(poDataPkt->pfnDataPktDelete)( poDataPkt );
		FW_Free( poDataPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode UdpSendAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_UdpSend* poUdpSend = hMediaCompAppCtx_i;

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

int UdpSendDataCallback( AMPS_HANDLE hAMPS_HANDLE_i, AMPS_APP_HANDLE hAMPS_APP_HANDLE_i, AMPS_NET_HANDLE hAMPS_NET_HANDLE_i, unsigned char* pucBuff_i, int nBuffLength_i, int nPort_i, char* pcIPAddress_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_UdpSend* poUdpSend = hAMPS_APP_HANDLE_i;

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( hAMPS_HANDLE_i == poUdpSend->oComp.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( hAMPS_APP_HANDLE_i == poUdpSend, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( hAMPS_NET_HANDLE_i == poUdpSend->hUdpNetHandle, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pucBuff_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( 0 < nBuffLength_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pcIPAddress_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( 0 < nPort_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPSEND_MAGIC_NUM == poUdpSend->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = MS_ERR_INVALID_PACKET;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poUdpSend, poUdpSend->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (AMPS_SUCCESS);
}

