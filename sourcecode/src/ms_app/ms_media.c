#include "ms_media.h"

e_MsRetCode MS_Media_CreateUdpRecv( t_MsCtx* poMsCtx_i, h_MediaComp* phUdpRecv_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phUdpRecv_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_UdpRecvConfig oUdpRecvConfig;

		// Create UDP recv component
		memset( &oUdpRecvConfig, 0, sizeof (t_UdpRecvConfig));
		oUdpRecvConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oUdpRecvConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

		strncpy( oUdpRecvConfig.pcLocalIp, (char*)poMsCtx_i->oLocalIp.pucData, poMsCtx_i->oLocalIp.unLength );
		oUdpRecvConfig.pcLocalIp[poMsCtx_i->oLocalIp.unLength] = '\0';
		oUdpRecvConfig.nLocalPort = 0;      // Value to be assigned by system
		oUdpRecvConfig.hUdpNetHandle = NULL;

		strcpy( oUdpRecvConfig.pcRemoteIp, "0.0.0.0" );
		oUdpRecvConfig.nRemotePort = 0;
		oUdpRecvConfig.bManipulateHandle = AMPS_TRUE;

		eRetCode = Media_CompCreate( MCT_UDPRECV, (h_CompConfig)&oUdpRecvConfig, phUdpRecv_o );

		int nLocalPort = -1;

		Media_CompGetParam( *phUdpRecv_o, MCC_UDPRECV_LOCAL_PORT, (void*)&nLocalPort );

		while( (nLocalPort) % 2 != 0 )
		{
			eRetCode = Media_CompSetStatus( *phUdpRecv_o, MCS_STOP );

			eRetCode =  Media_CompSetParam( *phUdpRecv_o, MCC_UDPRECV_LOCAL_PORT, (void*)nLocalPort+1 );

			// Start Component so that port is allocated
			eRetCode = Media_CompSetStatus( *phUdpRecv_o, MCS_START );
			if( MS_ERR_NONE != eRetCode )
			{
				nLocalPort += 2;
			}
			else
			{
				break;
			}
		}
		//MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Local port for media reception = %d\n", nLocalPort+1 );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phUdpRecv_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetUdpRecv( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phUdpRecv_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phUdpRecv_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_UDPRECV, phUdpRecv_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateUdpRecv( poMsCtx_i, phUdpRecv_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phUdpRecv_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phUdpRecv_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateReseqBuffer( t_MsCtx* poMsCtx_i, h_MediaComp* phReseqBuffer_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phReseqBuffer_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_ReseqBufferConfig oReseqBufferConfig;

		// Create ReseqBuffer component
		memset( &oReseqBufferConfig, 0, sizeof (t_ReseqBufferConfig));
		oReseqBufferConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oReseqBufferConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

		oReseqBufferConfig.nTimerMs = poMsCtx_i->nJitterBufferMs;

		eRetCode = Media_CompCreate( MCT_RESEQBUFFER, (h_CompConfig)&oReseqBufferConfig, phReseqBuffer_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phReseqBuffer_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetReseqBuffer( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phReseqBuffer_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phReseqBuffer_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_RESEQBUFFER, phReseqBuffer_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateReseqBuffer( poMsCtx_i, phReseqBuffer_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phReseqBuffer_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phReseqBuffer_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateRtpRecv( t_MsCtx* poMsCtx_i, h_MediaComp* phRtpRecv_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phRtpRecv_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_RtpRecvConfig oRtpRecvConfig;

		oRtpRecvConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oRtpRecvConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;
		oRtpRecvConfig.ucPayloadType = 0;
		oRtpRecvConfig.ucDtmfPayloadType = 101;
		oRtpRecvConfig.nSamplingRate = 8000;
		oRtpRecvConfig.unMaxTimeStampJump = 500000;

		eRetCode = Media_CompCreate( MCT_RTPRECV, (h_CompConfig)&oRtpRecvConfig, phRtpRecv_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phRtpRecv_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetRtpRecv( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phRtpRecv_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phRtpRecv_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_RTPRECV, phRtpRecv_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateRtpRecv( poMsCtx_i, phRtpRecv_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phRtpRecv_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phRtpRecv_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreatePcmuDec( t_MsCtx* poMsCtx_i, h_MediaComp* phPcmuDec_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phPcmuDec_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_PcmuDecConfig oPcmuDecConfig;

		oPcmuDecConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oPcmuDecConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

		eRetCode = Media_CompCreate( MCT_PCMUDEC, (h_CompConfig)&oPcmuDecConfig, phPcmuDec_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phPcmuDec_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetPcmuDec( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phPcmuDec_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phPcmuDec_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_PCMUDEC, phPcmuDec_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreatePcmuDec( poMsCtx_i, phPcmuDec_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phPcmuDec_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phPcmuDec_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateGsmDec( t_MsCtx* poMsCtx_i, h_MediaComp* phGsmDec_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phGsmDec_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_GsmDecConfig oGsmDecConfig;

		oGsmDecConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oGsmDecConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

		eRetCode = Media_CompCreate( MCT_GSMDEC, (h_CompConfig)&oGsmDecConfig, phGsmDec_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phGsmDec_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetGsmDec( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phGsmDec_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phGsmDec_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_GSMDEC, phGsmDec_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateGsmDec( poMsCtx_i, phGsmDec_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phGsmDec_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phGsmDec_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateG729Dec( t_MsCtx* poMsCtx_i, h_MediaComp* phG729Dec_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phG729Dec_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_G729DecConfig oG729DecConfig;

		oG729DecConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oG729DecConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

		eRetCode = Media_CompCreate( MCT_G729DEC, (h_CompConfig)&oG729DecConfig, phG729Dec_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phG729Dec_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetG729Dec( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phG729Dec_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phG729Dec_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_G729DEC, phG729Dec_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateG729Dec( poMsCtx_i, phG729Dec_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phG729Dec_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phG729Dec_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateLevel( t_MsCtx* poMsCtx_i, h_MediaComp* phLevel_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phLevel_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_LevelConfig oLevelConfig;

		oLevelConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oLevelConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;
		oLevelConfig.nSilenceThresholdDb = poMsCtx_i->nSilenceThresholdDb;

		eRetCode = Media_CompCreate( MCT_LEVEL, (h_CompConfig)&oLevelConfig, phLevel_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phLevel_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetLevel( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phLevel_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phLevel_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_LEVEL, phLevel_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateLevel( poMsCtx_i, phLevel_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phLevel_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phLevel_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateWavWrite( t_MsCtx* poMsCtx_i, h_MediaComp* phWavWrite_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phWavWrite_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_FileWriteConfig oWavWriteConfig;

		oWavWriteConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oWavWriteConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;
		oWavWriteConfig.pcFileName = NULL;
		oWavWriteConfig.unMode = AMPS_FILE_MODE_WRITE_CREATE;

		eRetCode = Media_CompCreate( MCT_WAVWRITE, (h_CompConfig)&oWavWriteConfig, phWavWrite_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phWavWrite_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetWavWrite( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phWavWrite_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phWavWrite_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_WAVWRITE, phWavWrite_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateWavWrite( poMsCtx_i, phWavWrite_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phWavWrite_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phWavWrite_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateNull( t_MsCtx* poMsCtx_i, h_MediaComp* phNull_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phNull_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_NullConfig oNullConfig;

		oNullConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oNullConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

		eRetCode = Media_CompCreate( MCT_NULL, (h_CompConfig)&oNullConfig, phNull_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phNull_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetNull( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phNull_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phNull_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_NULL, phNull_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateNull( poMsCtx_i, phNull_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phNull_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phNull_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateUdpSend( t_MsCtx* poMsCtx_i, h_MediaComp* phUdpSend_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phUdpSend_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_UdpSendConfig oUdpSendConfig;

		memset( &oUdpSendConfig, 0, sizeof (t_UdpSendConfig));
		oUdpSendConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oUdpSendConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

		oUdpSendConfig.hUdpNetHandle = NULL;
		strcpy( oUdpSendConfig.pcLocalIp, "0.0.0.0" );
		oUdpSendConfig.nLocalPort = 0;
		strcpy( oUdpSendConfig.pcRemoteIp, "0.0.0.0" );
		oUdpSendConfig.nRemotePort = 0;
		oUdpSendConfig.bManipulateHandle = AMPS_FALSE;

		eRetCode = Media_CompCreate( MCT_UDPSEND, (h_CompConfig)&oUdpSendConfig, phUdpSend_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phUdpSend_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetUdpSend( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phUdpSend_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phUdpSend_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_UDPSEND, phUdpSend_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateUdpSend( poMsCtx_i, phUdpSend_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phUdpSend_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phUdpSend_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreatePcmuEnc( t_MsCtx* poMsCtx_i, h_MediaComp* phPcmuEnc_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phPcmuEnc_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_PcmuEncConfig oPcmuEncConfig;

		oPcmuEncConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oPcmuEncConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

		eRetCode = Media_CompCreate( MCT_PCMUENC, (h_CompConfig)&oPcmuEncConfig, phPcmuEnc_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phPcmuEnc_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetPcmuEnc( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phPcmuEnc_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phPcmuEnc_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_PCMUENC, phPcmuEnc_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreatePcmuEnc( poMsCtx_i, phPcmuEnc_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phPcmuEnc_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phPcmuEnc_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateGsmEnc( t_MsCtx* poMsCtx_i, h_MediaComp* phGsmEnc_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phGsmEnc_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_GsmEncConfig oGsmEncConfig;

		oGsmEncConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oGsmEncConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

		eRetCode = Media_CompCreate( MCT_GSMENC, (h_CompConfig)&oGsmEncConfig, phGsmEnc_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phGsmEnc_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetGsmEnc( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phGsmEnc_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phGsmEnc_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_GSMENC, phGsmEnc_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateGsmEnc( poMsCtx_i, phGsmEnc_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phGsmEnc_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phGsmEnc_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateG729Enc( t_MsCtx* poMsCtx_i, h_MediaComp* phG729Enc_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phG729Enc_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_G729EncConfig oG729EncConfig;

		oG729EncConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oG729EncConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

		eRetCode = Media_CompCreate( MCT_G729ENC, (h_CompConfig)&oG729EncConfig, phG729Enc_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phG729Enc_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetG729Enc( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phG729Enc_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phG729Enc_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_G729ENC, phG729Enc_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateG729Enc( poMsCtx_i, phG729Enc_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phG729Enc_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phG729Enc_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateWavRead( t_MsCtx* poMsCtx_i, h_MediaComp* phWavRead_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phWavRead_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_FileReadConfig oWavReadConfig;

		oWavReadConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oWavReadConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;
		oWavReadConfig.pcFileName = NULL;
		oWavReadConfig.unReadTimerMs = 40;
		oWavReadConfig.unNumReadsPerTimeout = 1;
		oWavReadConfig.unReadSize = 320;

		eRetCode = Media_CompCreate( MCT_WAVREAD, (h_CompConfig)&oWavReadConfig, phWavRead_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phWavRead_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetWavRead( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phWavRead_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phWavRead_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_WAVREAD, phWavRead_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateWavRead( poMsCtx_i, phWavRead_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phWavRead_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phWavRead_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_CreateRtpSend( t_MsCtx* poMsCtx_i, h_MediaComp* phRtpSend_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phRtpSend_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_RtpSendConfig oRtpSendConfig;

		oRtpSendConfig.oConfig.nTraceId = poMsCtx_i->nTraceId;
		oRtpSendConfig.oConfig.pvFrameworkCtx = poMsCtx_i->pvFrameworkCtx;

		oRtpSendConfig.nSamplingRate = 8000;

		eRetCode = Media_CompCreate( MCT_RTPSEND, (h_CompConfig)&oRtpSendConfig, phRtpSend_o );
	}

	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phRtpSend_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_GetRtpSend( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i, h_MediaComp* phRtpSend_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phRtpSend_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		eRetCode = Media_ChainGetCompOfType( hMediaChain_i, MCT_RTPSEND, phRtpSend_o );
		if( MS_ERR_NONE != eRetCode  )
		{
			eRetCode = MS_Media_CreateRtpSend( poMsCtx_i, phRtpSend_o );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, *phRtpSend_o );
			}
		}
	}
	POST_CONDITION
	{
		if( MS_ERR_NONE == eRetCode )
		{
			ms_assert( NULL != *phRtpSend_o );
		}
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_RxChainInit( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		{
			h_MediaComp hUdpRecv = NULL;

			eRetCode = MS_Media_GetUdpRecv( poMsCtx_i, hMediaChain_i, &hUdpRecv );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hRtpRecv = NULL;

			eRetCode = MS_Media_GetRtpRecv( poMsCtx_i, hMediaChain_i, &hRtpRecv );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hReseqBuffer = NULL;

			eRetCode = MS_Media_GetReseqBuffer( poMsCtx_i, hMediaChain_i, &hReseqBuffer );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hDecoder = NULL;

			eRetCode = MS_Media_GetPcmuDec( poMsCtx_i, hMediaChain_i, &hDecoder );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hDecoder = NULL;

			eRetCode = MS_Media_GetGsmDec( poMsCtx_i, hMediaChain_i, &hDecoder );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hDecoder = NULL;

			eRetCode = MS_Media_GetG729Dec( poMsCtx_i, hMediaChain_i, &hDecoder );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hLevel = NULL;

			eRetCode = MS_Media_GetLevel( poMsCtx_i, hMediaChain_i, &hLevel );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hWavWrite = NULL;

			eRetCode = MS_Media_GetWavWrite( poMsCtx_i, hMediaChain_i, &hWavWrite );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hNull = NULL;

			eRetCode = MS_Media_GetNull( poMsCtx_i, hMediaChain_i, &hNull );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MS_Media_RxChainDestroy( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering. \n" );

	eRetCode =  Media_ChainSetStatus( hMediaChain_i, MCS_STOP );
	eRetCode =  Media_ChainDestroy( hMediaChain_i );

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with %s\n", get_ms_ret_code_string( eRetCode ) );
	return (eRetCode);
}

e_MsRetCode MS_Media_TxChainInit( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaChain_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		{
			h_MediaComp hWavRead = NULL;

			eRetCode = MS_Media_CreateWavRead( poMsCtx_i, &hWavRead );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, hWavRead );
			}
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hEncoder = NULL;

			eRetCode = MS_Media_CreatePcmuEnc( poMsCtx_i, &hEncoder );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, hEncoder );
			}
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hEncoder = NULL;

			eRetCode = MS_Media_CreateGsmEnc( poMsCtx_i, &hEncoder );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, hEncoder );
			}
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hRtpSend = NULL;

			eRetCode = MS_Media_CreateRtpSend( poMsCtx_i, &hRtpSend );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, hRtpSend );
			}
		}
		if( MS_ERR_NONE == eRetCode )
		{
			h_MediaComp hUdpSend = NULL;

			eRetCode = MS_Media_CreateUdpSend( poMsCtx_i, &hUdpSend );
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = Media_ChainAddInChain( hMediaChain_i, hUdpSend );
			}
		}
	}

	POST_CONDITION
	{
	}
}

e_MsRetCode MS_Media_TxChainUpdate( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	return (eRetCode);
}

e_MsRetCode MS_Media_TxChainDestroy( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering \n" );

	eRetCode =  Media_ChainSetStatus( hMediaChain_i, MCS_STOP );
	eRetCode =  Media_ChainDestroy( hMediaChain_i );

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));
	return (eRetCode);
}

e_MsRetCode MS_Media_ChainSetMode( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i, e_MixMode eMode_i, t_SdpNegMediaInfo* poNegMediaInfo_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUserCtrlCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUserCtrlCtx_i->poNegInfoList, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering \n" );
	if( eMode_i == MM_MUTE )
	{
		// Connect UdpRecv -> RtpRecv (for DTMF) -> Null
		h_MediaComp hUdpRecv = NULL;
		h_MediaComp hRtpRecv = NULL;
		h_MediaComp hNull = NULL;

		{
			eRetCode = MS_Media_GetUdpRecv( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hUdpRecv );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = MS_Media_GetRtpRecv( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hRtpRecv );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = MS_Media_GetNull( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hNull );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hRxChain, hUdpRecv, hRtpRecv );
			eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hRxChain, hRtpRecv, hNull );
		}
		eRetCode = Media_ChainSetStatus( poUserCtrlCtx_i->hRxChain, MCS_START );
	}
	else if( eMode_i == MM_FULL )
	{
		AMPS_BOOL bRelayMedia = AMPS_FALSE;
		t_MsUserCtrlCtx* poMateCtrlCtx = NULL;
		//t_SdpNegMediaInfo* poNegMediaInfo = NULL;
		if( NULL == poNegMediaInfo_i )
		{
			poNegMediaInfo_i = poUserCtrlCtx_i->poNegInfoList->poAMPSSListHead->pvData;
		}
		// Check if relay is possible
		if( poUserCtrlCtx_i->poTeamMateList->uchCount == 1 )
		{
			t_SdpNegMediaInfo* poMateNegMediaInfo = NULL;

			poMateCtrlCtx = poUserCtrlCtx_i->poTeamMateList->poAMPSSListHead->pvData;
			poMateNegMediaInfo = poMateCtrlCtx->poNegInfoList->poAMPSSListHead->pvData;
			if( poNegMediaInfo_i->ucPayloadType == poMateNegMediaInfo->ucPayloadType )
			{
				MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Relaying data between %p and %p User Ctrl Ctxs on codec with payload type = %d\n", poUserCtrlCtx_i, poMateCtrlCtx, poNegMediaInfo_i->ucPayloadType );
				bRelayMedia = AMPS_TRUE;
			}
		}
		if( bRelayMedia == AMPS_FALSE )
		{
			t_AMPSSList* poUserCtrlCtxNode = NULL;
			t_MsUserCtrlCtx* poUserCtrlCtx = NULL;
			t_SdpNegMediaInfo* poNegMediaInfo = NULL;

			// connect all users to the global mixers
			for( poUserCtrlCtxNode = poMsConfCtx_i->poUserCtrlCtxList->poAMPSSListHead; poUserCtrlCtxNode != NULL; poUserCtrlCtxNode = poUserCtrlCtxNode->poAMPSSListNext )
			{
				poUserCtrlCtx = poUserCtrlCtxNode->pvData;

				poNegMediaInfo = poUserCtrlCtx->poNegInfoList->poAMPSSListHead->pvData;

				// Connect UdpRecv -> RtpRecv -> Decoder -> Mixer

				h_MediaComp hUdpRecv = NULL;
				h_MediaComp hRtpRecv = NULL;
				h_MediaComp hDecoder = NULL;
				h_MediaComp hMixer = NULL;

				MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Transcoding: User Ctrl Ctx = %p, payload type = %d\n", poUserCtrlCtx, poNegMediaInfo->ucPayloadType );

				{
					eRetCode = MS_Media_GetUdpRecv( poMsCtx_i, poUserCtrlCtx->hRxChain, &hUdpRecv );
				}
				if( MS_ERR_NONE == eRetCode )
				{
					eRetCode = MS_Media_GetRtpRecv( poMsCtx_i, poUserCtrlCtx->hRxChain, &hRtpRecv );

					Media_CompSetParam( hRtpRecv, MCC_RTPRECV_PAYLOAD_TYPE, (void*)poNegMediaInfo->ucPayloadType );
				}
				if( MS_ERR_NONE == eRetCode )
				{
					switch( poNegMediaInfo->ucPayloadType )
					{
						case 0: // PCMU
							eRetCode = MS_Media_GetPcmuDec( poMsCtx_i, poUserCtrlCtx->hRxChain, &hDecoder );
							break;

						case 3: // GSM
							eRetCode = MS_Media_GetGsmDec( poMsCtx_i, poUserCtrlCtx->hRxChain, &hDecoder );
							break;

						case 18: // G729
							eRetCode = MS_Media_GetG729Dec( poMsCtx_i, poUserCtrlCtx->hRxChain, &hDecoder );
							break;

						default:
							eRetCode = MS_ERR_END_OF_LIST;
					}
				}
				if( MS_ERR_NONE == eRetCode )
				{
					if( NULL != poMsConfCtx_i )
					{
						ms_assert( poMsConfCtx_i->phMixerCompList->uchCount > 0 );   // there should be atleast one mixer

						hMixer = poMsConfCtx_i->phMixerCompList->poAMPSSListHead->pvData;
					}
				}
				if( MS_ERR_NONE == eRetCode )
				{
					eRetCode = Media_ChainSetLink( poUserCtrlCtx->hRxChain, hUdpRecv, hRtpRecv );
					eRetCode = Media_ChainSetLink( poUserCtrlCtx->hRxChain, hRtpRecv, hDecoder );
					if( NULL != hMixer )
					{
						eRetCode = Media_ChainSetLink( poUserCtrlCtx->hRxChain, hDecoder, hMixer );
					}
				}
				// Connect Mixer -> Encoder -> RtpSend -> UdpSend
				h_MediaComp hEncoder = NULL;
				h_MediaComp hRtpSend = NULL;
				h_MediaComp hUdpSend = NULL;

				{
					switch( poNegMediaInfo->ucPayloadType )
					{
						case 0: // PCMU
							eRetCode = MS_Media_GetPcmuEnc( poMsCtx_i, poUserCtrlCtx->hTxChain, &hEncoder );
							break;

						case 3: // GSM
							eRetCode = MS_Media_GetGsmEnc( poMsCtx_i, poUserCtrlCtx->hTxChain, &hEncoder );
							break;

						case 18: // G729
							eRetCode = MS_Media_GetG729Enc( poMsCtx_i, poUserCtrlCtx->hTxChain, &hEncoder );
							break;

						default:
							eRetCode = MS_ERR_END_OF_LIST;
					}
				}
				if( MS_ERR_NONE == eRetCode )
				{
					eRetCode = MS_Media_GetRtpSend( poMsCtx_i, poUserCtrlCtx->hTxChain, &hRtpSend );
				}
				if( MS_ERR_NONE == eRetCode )
				{
					eRetCode = MS_Media_GetUdpSend( poMsCtx_i, poUserCtrlCtx->hTxChain, &hUdpSend );
				}
				if( MS_ERR_NONE == eRetCode )
				{
					eRetCode = Media_ChainSetLink( poUserCtrlCtx->hRxChain, hEncoder, hRtpSend );
					eRetCode = Media_ChainSetLink( poUserCtrlCtx->hRxChain, hRtpSend, hUdpSend );
				}
				if( MS_ERR_NONE == eRetCode && NULL != hMixer )
				{
					t_MixerNodeConfig oMixerNodeConfig;

					oMixerNodeConfig.eMode = (e_MixerMode)poUserCtrlCtx->eCurrentMode;
					oMixerNodeConfig.hSrcMediaComp = hDecoder;
					oMixerNodeConfig.hSinkMediaComp = hEncoder;
					Media_ChainFillAppData( poUserCtrlCtx->hTxChain, hEncoder, &oMixerNodeConfig.oSinkAppData );

					eRetCode = Media_CompSetParam( hMixer, MCC_MIXER_ADD_NODE, &oMixerNodeConfig );
				}
				eRetCode = Media_ChainSetStatus( poUserCtrlCtx->hRxChain, MCS_START );
				eRetCode = Media_ChainSetStatus( poUserCtrlCtx->hTxChain, MCS_START );
			}
		}
		else
		{
			h_MediaComp hUdpRecv = NULL;
			h_MediaComp hUdpSend = NULL;
			h_MediaComp hRtpRecv = NULL;
			h_MediaComp hRtpSend = NULL;

			h_MediaComp hMateUdpRecv = NULL;
			h_MediaComp hMateUdpSend = NULL;
			h_MediaComp hMateRtpRecv = NULL;
			h_MediaComp hMateRtpSend = NULL;

			MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Negotiated payload type = %d\n", poNegMediaInfo_i->ucPayloadType );

			{
				eRetCode = MS_Media_GetUdpRecv( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hUdpRecv );
			}
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = MS_Media_GetRtpRecv( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hRtpRecv );

				Media_CompSetParam( hRtpRecv, MCC_RTPRECV_PAYLOAD_TYPE, (void*)poNegMediaInfo_i->ucPayloadType );
			}
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = MS_Media_GetUdpSend( poMsCtx_i, poUserCtrlCtx_i->hTxChain, &hUdpSend );
			}
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = MS_Media_GetRtpSend( poMsCtx_i, poUserCtrlCtx_i->hTxChain, &hRtpSend );
			}
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = MS_Media_GetUdpRecv( poMsCtx_i, poMateCtrlCtx->hRxChain, &hMateUdpRecv );
			}
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = MS_Media_GetRtpRecv( poMsCtx_i, poMateCtrlCtx->hRxChain, &hMateRtpRecv );

				Media_CompSetParam( hMateRtpRecv, MCC_RTPRECV_PAYLOAD_TYPE, (void*)poNegMediaInfo_i->ucPayloadType );
			}
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = MS_Media_GetUdpSend( poMsCtx_i, poMateCtrlCtx->hTxChain, &hMateUdpSend );
			}
			if( MS_ERR_NONE == eRetCode )
			{
				eRetCode = MS_Media_GetRtpSend( poMsCtx_i, poMateCtrlCtx->hTxChain, &hMateRtpSend );
			}
			if( MS_ERR_NONE == eRetCode )
			{
				// Connect UdpRecv(1) -> RtpRecv(1) -> RtpSend(2) -> UdpSend(2)
				eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hRxChain, hUdpRecv, hRtpRecv );
				eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hRxChain, hRtpRecv, hMateRtpSend );
				eRetCode = Media_ChainSetLink( poMateCtrlCtx->hTxChain, hMateRtpSend, hMateUdpSend );

				// Connect UdpRecv(2) -> RtpRecv(2) -> RtpSend(1) -> UdpSend(1)
				eRetCode = Media_ChainSetLink( poMateCtrlCtx->hRxChain, hMateUdpRecv, hMateRtpRecv );
				eRetCode = Media_ChainSetLink( poMateCtrlCtx->hRxChain, hMateRtpRecv, hRtpSend );
				eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hTxChain, hRtpSend, hUdpSend );
			}
			eRetCode = Media_ChainSetStatus( poUserCtrlCtx_i->hRxChain, MCS_START );
			eRetCode = Media_ChainSetStatus( poUserCtrlCtx_i->hTxChain, MCS_START );

			eRetCode = Media_ChainSetStatus( poMateCtrlCtx->hRxChain, MCS_START );
			eRetCode = Media_ChainSetStatus( poMateCtrlCtx->hTxChain, MCS_START );
		}
	}
	else if( eMode_i == MM_PARKED )
	{
		// Connect UdpRecv -> RtpRecv -> Decoder -> hWavWrite

		h_MediaComp hUdpRecv = NULL;
		h_MediaComp hRtpRecv = NULL;
		h_MediaComp hDecoder = NULL;
		h_MediaComp hLevel = NULL;
		h_MediaComp hWavWrite = NULL;
		h_MediaComp hMixer = NULL;
		h_MediaComp hReseqBuffer = NULL;
		// Configure media according to first negotiated media information
		if( NULL == poNegMediaInfo_i )
		{
			poNegMediaInfo_i = poUserCtrlCtx_i->poNegInfoList->poAMPSSListHead->pvData;
		}
		MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Negotiated payload type = %d\n", poNegMediaInfo_i->ucPayloadType );

		{
			eRetCode = MS_Media_GetUdpRecv( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hUdpRecv );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = MS_Media_GetRtpRecv( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hRtpRecv );

			Media_CompSetParam( hRtpRecv, MCC_RTPRECV_PAYLOAD_TYPE, (void*)poNegMediaInfo_i->ucPayloadType );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = MS_Media_GetReseqBuffer( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hReseqBuffer );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			switch( poNegMediaInfo_i->ucPayloadType )
			{
				case 0: // PCMU
					eRetCode = MS_Media_GetPcmuDec( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hDecoder );
					break;

				case 3: // GSM
					eRetCode = MS_Media_GetGsmDec( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hDecoder );
					break;

				case 18: // G729
					eRetCode = MS_Media_GetG729Dec( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hDecoder );
					break;

				default:
					eRetCode = MS_ERR_END_OF_LIST;
			}
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = MS_Media_GetLevel( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hLevel );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = MS_Media_GetWavWrite( poMsCtx_i, poUserCtrlCtx_i->hRxChain, &hWavWrite );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hRxChain, hUdpRecv, hRtpRecv );
			eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hRxChain, hRtpRecv, hReseqBuffer );
			eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hRxChain, hReseqBuffer, hDecoder );
			eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hRxChain, hDecoder, hLevel );
			eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hRxChain, hLevel, hWavWrite );
		}
		// Connect WavRead -> Encoder -> RtpSend -> UdpSend
		h_MediaComp hWavRead = NULL;
		h_MediaComp hEncoder = NULL;
		h_MediaComp hRtpSend = NULL;
		h_MediaComp hUdpSend = NULL;
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = MS_Media_GetWavRead( poMsCtx_i, poUserCtrlCtx_i->hTxChain, &hWavRead );

			ms_assert( MS_ERR_NONE == eRetCode );

			eRetCode = Media_CompSetParam( hWavRead, MCC_WAVREAD_TIMER_MS, (void*)20 );
			eRetCode = Media_CompSetParam( hWavRead, MCC_WAVREAD_READ_PER_TIMEOUT, (void*)1 );
			eRetCode = Media_CompSetParam( hWavRead, MCC_WAVREAD_READ_SIZE, (void*)320 );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			switch( poNegMediaInfo_i->ucPayloadType )
			{
				case 0: // PCMU
					eRetCode = MS_Media_GetPcmuEnc( poMsCtx_i, poUserCtrlCtx_i->hTxChain, &hEncoder );
					break;

				case 3: // GSM
					eRetCode = MS_Media_GetGsmEnc( poMsCtx_i, poUserCtrlCtx_i->hTxChain, &hEncoder );
					break;

				case 18: // G729
					eRetCode = MS_Media_GetG729Enc( poMsCtx_i, poUserCtrlCtx_i->hTxChain, &hEncoder );
					break;

				default:
					eRetCode = MS_ERR_END_OF_LIST;
			}
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = MS_Media_GetRtpSend( poMsCtx_i, poUserCtrlCtx_i->hTxChain, &hRtpSend );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = MS_Media_GetUdpSend( poMsCtx_i, poUserCtrlCtx_i->hTxChain, &hUdpSend );
			ms_assert( MS_ERR_NONE == eRetCode );

			void* pvNetHandle = NULL;
			eRetCode = Media_CompGetParam( hUdpRecv, MCC_UDPRECV_NET_HANDLE, &pvNetHandle );

			ms_assert( MS_ERR_NONE == eRetCode );

			char* pcRemoteIp = TS_GetStringZ( &poNegMediaInfo_i->oRemoteIp );

			ms_assert( NULL != pcRemoteIp );

			eRetCode = Media_CompSetParam( hUdpSend, MCC_UDPSEND_NET_HANDLE, pvNetHandle );
			eRetCode = Media_CompSetParam( hUdpSend, MCC_UDPSEND_REMOTE_IP, pcRemoteIp );
			eRetCode = Media_CompSetParam( hUdpSend, MCC_UDPSEND_REMOTE_PORT, (void*)poNegMediaInfo_i->nRemotePort );

			FW_Free( pcRemoteIp );
		}
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hTxChain, hWavRead, hEncoder );
			eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hTxChain, hEncoder, hRtpSend );
			eRetCode = Media_ChainSetLink( poUserCtrlCtx_i->hTxChain, hRtpSend, hUdpSend );
		}
		eRetCode = Media_ChainSetStatus( poUserCtrlCtx_i->hRxChain, MCS_START );
		eRetCode = Media_ChainSetStatus( poUserCtrlCtx_i->hTxChain, MCS_START );
	}
	if( MS_ERR_NONE == eRetCode )
	{
		poUserCtrlCtx_i->eCurrentMode = eMode_i;
	}
	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));
	return (eRetCode);
}

