#include "mixer_priv.h"

e_MsRetCode MixerInit( h_CompConfig hCompConfig_i, h_MediaComp* phMixer_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MixerConfig* poMixerConfig = (t_MixerConfig*)hCompConfig_i;

	MS_TRACE( poMixerConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMixerConfig, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMixerConfig->oConfig.pvFrameworkCtx, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != phMixer_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}
	/* - */

	{
		t_Mixer* poMixer = NULL;

		// Allocate Mixer structure
		poMixer = FW_Malloc( sizeof (t_Mixer));
		if( NULL != poMixer )
		{
			poMixer->oComp.eType = MCT_MIXER;
			poMixer->oComp.eGroup = MCG_FILTER;
			// Populate Mixer structure
			poMixer->oComp.nTraceId = poMixerConfig->oConfig.nTraceId;
			poMixer->oComp.pvFrameworkCtx = poMixerConfig->oConfig.pvFrameworkCtx;

			poMixer->nMagicNum = MCT_MIXER_MAGIC_NUM;
			poMixer->nBufferTimeMs = poMixerConfig->nBufferTimeMs;
			poMixer->nPacketizationTimeMs = poMixerConfig->nPacketizationTimeMs;

			*phMixer_o = (h_MediaComp) poMixer;
		}
		else
		{
			MS_TRACE( poMixerConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Memory allocation failed: t_Mixer\n" );
			eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMixerConfig->oConfig.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MixerSetAppData( h_MediaComp hMixer_i, h_MediaCompAppData hCompAppData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Mixer* poMixer = hMixer_i;
	t_MediaCompAppData* poAppData_i = (t_MediaCompAppData*)hCompAppData_i;

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMixer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_MIXER_MAGIC_NUM == poMixer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData_i->pfAppDataCb, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poAppData_i->pfAppSignalCb, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		//poMixer->oAppData = *poAppData_i;
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MixerDestroy( h_MediaComp hMixer_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Mixer* poMixer = hMixer_i;
	int nTraceId = poMixer->oComp.nTraceId;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMixer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_MIXER_MAGIC_NUM == poMixer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		cir_list_free_custom( poMixer->poNodeList, MixerNode_Delete, poMixer );
		FW_Free( poMixer );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MixerSetStatus( h_MediaComp hMixer_i, e_MediaCompStatus eStatus )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Mixer* poMixer = hMixer_i;

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMixer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCS_MAX > eStatus, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_MIXER_MAGIC_NUM == poMixer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eStatus )
		{
			case MCS_START:
			{
				break;
			}

			case MCS_STOP:
			{
				break;
			}

			default:
			{
				MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_ERROR, "Invalid status = %d\n", eStatus );

				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MixerGetParam( h_MediaComp hMixer_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Mixer* poMixer = hMixer_i;

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMixer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_MIXER_MAGIC_NUM == poMixer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			default:
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

e_MsRetCode MixerSetParam( h_MediaComp hMixer_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_Mixer* poMixer = hMixer_i;

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != poMixer, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( MCT_MIXER_MAGIC_NUM == poMixer->nMagicNum, eRetCode = MS_ERR_PRE_COND_FAILED );
//		ms_assert_return( NULL != pvConfigData_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		switch( eConfig_i )
		{
			case MCC_MIXER_ADD_NODE:
			{
				t_MixerNode* poMixerNode = NULL;

				poMixerNode = FW_Malloc( sizeof (t_MixerNode));
				if( NULL != poMixerNode )
				{
					poMixerNode->hMixer = poMixer;
					poMixerNode->oNodeConfig = *(t_MixerNodeConfig*)pvConfigData_i;

					poMixerNode->nLastPktGenTime = 0;

					poMixer->poNodeList = cir_list_append( poMixer->poNodeList, poMixerNode );
				}
				else
				{
					eRetCode = MS_ERR_INSUFFICIENT_MEMORY;
				}
				break;
			}

			case MCC_MIXER_REMOVE_NODE:
			{
				poMixer->poNodeList = cir_list_delete( poMixer->poNodeList, pvConfigData_i, MixerNode_CompareConfig, MixerNode_Delete, poMixer );

				break;
			}

			case MCC_MIXER_BUFFER_TIME_MS:
			{
				poMixer->nBufferTimeMs = (int)pvConfigData_i;
				break;
			}

			case MCC_MIXER_PACKETIZATION_TIME_MS:
			{
				poMixer->nPacketizationTimeMs = (int)pvConfigData_i;
				break;
			}

			default:
			{
				eRetCode = MS_ERR_INVALID_INPUT;
			}
		}
	}

	POST_CONDITION
	{
	}

	MEDIA_TRACE( poMixer, poMixer->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

void MixerNode_Delete( void* pvData_i, void* pvUserData_i )
{
	t_MixerNode* poMixerNode = pvData_i;

	cir_list_free_custom( poMixerNode->poInPktList, DataPkt_Delete, NULL );

	FW_Free( poMixerNode );
}

int MixerNode_Compare( void* pvData1_i, void* pvData2_i )
{
	t_MixerNode* poNode1 = pvData1_i;
	t_MixerNode* poNode2 = pvData2_i;
	if( poNode1->oNodeConfig.hSrcMediaComp == poNode2->oNodeConfig.hSrcMediaComp && poNode1->oNodeConfig.hSinkMediaComp == poNode2->oNodeConfig.hSinkMediaComp )
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

int MixerNode_CompareConfig( void* pvData1_i, void* pvData2_i )
{
	t_MixerNode* poNode = pvData1_i;
	t_MixerNodeConfig* poNodeConfig = pvData2_i;
	if( poNode->oNodeConfig.hSrcMediaComp == poNodeConfig->hSrcMediaComp && poNode->oNodeConfig.hSinkMediaComp == poNodeConfig->hSinkMediaComp )
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

int MixerNode_CompareSrc( void* pvData1_i, void* pvData2_i )
{
	t_MixerNode* poNode = pvData1_i;
	if( poNode->oNodeConfig.hSrcMediaComp == pvData2_i )
	{
		return (0);
	}
	else
	{
		return (1);
	}
}

