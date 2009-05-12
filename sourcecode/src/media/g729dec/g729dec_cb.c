#include "g729dec_priv.h"

e_MsRetCode G729DecAppDataCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_DataPkt* poDataPkt = hDataPkt_i;
	t_G729Dec* poG729Dec = hMediaCompAppCtx_i;

	MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poG729Dec, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poDataPkt, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( DPT_GEN_Packet == poDataPkt->eType ||
		                  DPT_UDP_Packet == poDataPkt->eType ||
		                  DPT_RTP_Packet == poDataPkt->eType, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_G729DEC_MAGIC_NUM == poG729Dec->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		Word16* pucData = NULL;
		int nLength = 0;

		switch( poDataPkt->eType )
		{
			case DPT_GEN_Packet:
				pucData = (Word16*)poDataPkt->oGenPkt.pucData;
				nLength = poDataPkt->oGenPkt.nLength;
				break;

			case DPT_UDP_Packet:
				pucData = (Word16*)poDataPkt->oUdpPkt.pucData;
				nLength = poDataPkt->oUdpPkt.nLength;
				break;

			case DPT_RTP_Packet:
				pucData = (Word16*)rtp_packet_get_payload( poDataPkt->oRtpPkt.poRtpPkt );
				nLength = rtp_packet_get_payload_len( poDataPkt->oRtpPkt.poRtpPkt );
				break;

			default:
				ms_assert_return( 0, eRetCode = MS_ERR_INVALID_PACKET );
		}
		MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Packet info = %p(%d)\n", pucData, nLength );

		Word16* pst_out = NULL;
		int nFrame = nLength / M;
		int i;

		MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Processing %d frames\n", nFrame );
		// Allocate decode buffer, double the size of encoded data
		pst_out = (Word16*) FW_Malloc( nFrame * L_FRAME * sizeof (Word16) );
		// Check if allocation is successful
		if( NULL != pst_out )
		{
			MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Decoding buffer allocated successfully = %p\n", pst_out );
			// Decode data

			int nFrameCount = 0;

			while( nFrameCount < nFrame )
			{
//              poG729Dec->poDecoder
//
//              pucData[nFrameCount * M]
//              pst_out[nFrameCount * 160]
				MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Decoding frame number = %d\n", nFrameCount+1 );

				bits2prm_ld8k( &pucData[nFrameCount * (M/2)], &poG729Dec->parm[1] );

				/* the hardware detects frame erasures by checking if all bits
				are set to zero
				*/
				poG729Dec->parm[0] = 1;           /* frame erasure */

				for( i = 0; i < L_FRAME; i++ )
				{
					if( pucData[i] != 0 )
					{
						poG729Dec->parm[0] = 0; /* frame erased     */
						break;
					}
				}
				/* check parity and put 1 in parm[4] if parity error */

				poG729Dec->parm[4] = Check_Parity_Pitch( poG729Dec->parm[3], poG729Dec->parm[4] );

				Decod_ld8k( poG729Dec->parm, poG729Dec->voicing, poG729Dec->synth, poG729Dec->Az_dec, &poG729Dec->T0_first );

				/* Postfilter */

				poG729Dec->voicing = 0;
				poG729Dec->ptr_Az = poG729Dec->Az_dec;

				for( i = 0; i < L_FRAME; i += L_SUBFR )
				{
					Post( poG729Dec->T0_first, &poG729Dec->synth[i], poG729Dec->ptr_Az, &pst_out[i], &poG729Dec->sf_voic );
					if( poG729Dec->sf_voic != 0 )
					{
						poG729Dec->voicing = poG729Dec->sf_voic;
					}
					poG729Dec->ptr_Az += MP1;
				}
				Copy( &poG729Dec->synth_buf[L_FRAME], &poG729Dec->synth_buf[0], M );

				Post_Process( pst_out, L_FRAME );

				nFrameCount++;
			}
			// Delete the previous packet data
			(poDataPkt->pfnDataPktDelete)( poDataPkt );

			// Allocate Generic packet
			AllocGenPkt((h_DataPkt*)&poDataPkt );

			// Update data packet
			poDataPkt->oGenPkt.pucData = pst_out;
			poDataPkt->oGenPkt.nLength = nFrame * L_FRAME * sizeof (Word16);

			MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Calling Data callback function = %p\n", poG729Dec->oAppData.pfAppDataCb );

			// Call data callback function of the next layer
			ms_assert_return( NULL != poG729Dec->oAppData.pfAppDataCb, eRetCode = MS_ERR_NOT_INITIALIZED );
			eRetCode = (poG729Dec->oAppData.pfAppDataCb)((h_MediaComp)poG729Dec, (h_MediaCompAppCtx)poG729Dec->oAppData.hAppDataCtx, (h_DataPkt)poDataPkt );
		}
		else
		{
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode G729DecAppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SigPkt* poSigPkt = h_SigPkt_i;
	t_G729Dec* poG729Dec = hMediaCompAppCtx_i;

	MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	{
		(poSigPkt->pfnSignalPktDelete)( poSigPkt );
		FW_Free( poSigPkt );
	}

	POST_CONDITION
	{
	}
	MEDIA_TRACE( poG729Dec, poG729Dec->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

