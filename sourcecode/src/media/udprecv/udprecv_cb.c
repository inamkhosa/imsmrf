#include "udprecv_priv.h"

int UdpRecvDataCallback( AMPS_HANDLE hAMPS_HANDLE_i, AMPS_APP_HANDLE hAMPS_APP_HANDLE_i, AMPS_NET_HANDLE hAMPS_NET_HANDLE_i, unsigned char* pucBuff_i, int nBuffLength_i, int nPort_i, char* pcIPAddress_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = NULL;
	t_UdpRecv* poUdpRecv = hAMPS_APP_HANDLE_i;

	MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poUdpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_UDPRECV_MAGIC_NUM == poUdpRecv->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( hAMPS_HANDLE_i == poUdpRecv->oComp.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( hAMPS_APP_HANDLE_i == poUdpRecv, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( hAMPS_NET_HANDLE_i == poUdpRecv->hUdpNetHandle, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pucBuff_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( 0 < nBuffLength_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pcIPAddress_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( 0 < nPort_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	// Allocate data packet
	eRetCode = AllocUdpPkt((h_DataPkt*)&poDataPkt );
	if( MS_ERR_NONE == eRetCode )
	{
		// Populate data packet
		poDataPkt->nPktGenTime = FW_GetCurrentTimeInMilliSec( poUdpRecv->oComp.pvFrameworkCtx );
		poDataPkt->oUdpPkt.pucData = pucBuff_i;
		poDataPkt->oUdpPkt.nLength = nBuffLength_i;
		strncpy( poDataPkt->oUdpPkt.pcRemoteIpAddress, pcIPAddress_i, sizeof (poDataPkt->oUdpPkt.pcRemoteIpAddress)-1 );
		poDataPkt->oUdpPkt.nRemotePort = nPort_i;

		struct in_addr oRemoteAddr;
		inet_ntoa( pcIPAddress_i, &oRemoteAddr );
		// If there is change in remote IP or port
		if( oRemoteAddr.s_addr != poUdpRecv->oLastRemoteAddr.s_addr || nPort_i != poUdpRecv->nLastRemotePort )
		{
			// Update Last Remote Address
			poUdpRecv->oLastRemoteAddr = oRemoteAddr;
			poUdpRecv->nLastRemotePort = nPort_i;
			//

			MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Remote Address changed to %s:%d\n", pcIPAddress_i, nPort_i );

			// Allocate signal packet
			t_SigPkt* poSigPkt = FW_Malloc( sizeof (t_SigPkt));
			// Check if allocation is successful
			if( NULL != poSigPkt )
			{
				// Initialize signal packet
				poSigPkt->eType = SPT_REMOTE_ADDRESS_CHANGE_Packet;
				poSigPkt->pfnSignalPktInit =  AddrChangeSigPktInit;
				poSigPkt->pfnSignalPktDelete =  AddrChangeSigPktDelete;

				(poSigPkt->pfnSignalPktInit)( poSigPkt );

				// Populate values
				strncpy( poSigPkt->oAddrChangePkt.pcIp, pcIPAddress_i, AMPS_MAX_IP_STRING_LENGTH );
				poSigPkt->oAddrChangePkt.nPort = nPort_i;

				MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Singal callback function\n" );

				// Call signal callback function
				ms_assert_return( NULL != poUdpRecv->oAppData.pfAppSignalCb, eRetCode = MS_ERR_NOT_INITIALIZED );
				eRetCode = (poUdpRecv->oAppData.pfAppSignalCb)((h_MediaComp)poUdpRecv, (h_MediaCompAppCtx)poUdpRecv->oAppData.hAppSignalCtx, (h_DataPkt)poSigPkt );
			}
		}
		MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Packet length = %d\n", poDataPkt->oUdpPkt.nLength );
		// Call data callback function of the next layer
		ms_assert_return( NULL != poUdpRecv->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
		eRetCode = (poUdpRecv->oAppData.pfAppDataCb)((h_MediaComp)poUdpRecv, (h_MediaCompAppCtx)poUdpRecv->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
	}
	POST_CONDITION
	{
	}

	MEDIA_TRACE( poUdpRecv, poUdpRecv->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (AMPS_SUCCESS);
}

