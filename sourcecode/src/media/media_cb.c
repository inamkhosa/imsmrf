/*! @file media_cb.c
*
* @ingroup Media
*
* @brief The file contains implementation of additional functions for media API.
*
*/

#include "media/buffer.h"
#include "media/fileread.h"
#include "media/filewrite.h"
#include "media/level.h"
#include "media/pcmudec.h"
#include "media/pcmuenc.h"
#include "media/rtprecv.h"
#include "media/rtpsend.h"
#include "media/timequeue.h"
#include "media/udprecv.h"
#include "media/udpsend.h"
#include "media/wavread.h"
#include "media/wavwrite.h"
#include "media/null.h"
#include "media/mixer.h"
#include "media/gsmdec.h"
#include "media/gsmenc.h"
#include "media/g729enc.h"
#include "media/g729dec.h"
#include "media/reseqbuffer.h"

#include "media_priv.h"

/*! @fn void* MediaMallocCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, int r_nSize )
*
* @brief This function allocates memory for media.
*
* @param[in] r_hAMPS_HANDLE is the AMPS handle.
*
* @param[in] r_hAMPS_APP_HANDLE is the handle AMPS application.
*
* @param[in] r_nSize is the size of the required memory allocation.
*
* @return Pointer to the allocated memory.
*
*/

void* MediaMallocCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, int r_nSize )
{
	return (FW_Malloc( r_nSize ));
}

/*! @fn void* MediaReAllocCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, void* r_pvData, int r_nSize )
*
* @brief This function reallocates memory for media.
*
* @param[in] r_hAMPS_HANDLE is the AMPS handle.
*
* @param[in] r_hAMPS_APP_HANDLE is the handle AMPS application.
*
* @param[in] r_pvData is the pointer to tbe already allocated memory.
*
* @param[in] r_nSize is the size of the required memory allocation.
*
* @return Pointer to the reallocated memory.
*
*/

void* MediaReAllocCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, void* r_pvData, int r_nSize )
{
	return (FW_Realloc( r_pvData, r_nSize ));
}

/*! @fn void MediaFreeCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, void* r_pvData )
*
* @brief This function frees the allocated memory for media.
*
* @param[in] r_hAMPS_HANDLE is the AMPS handle.
*
* @param[in] r_hAMPS_APP_HANDLE is the handle AMPS application.
*
* @param[in] r_pvData is the pointer to the allocated memory.
*
*/

void MediaFreeCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, void* r_pvData )
{
	FW_Free( r_pvData );
}

/*! @fn e_MsRetCode MediaChain_AppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
*
* @brief This callback function provides a signal to the components in the media chain.
*
* @param[in] hMediaComp_i is the handle of the media component.
*
* @param[in] hMediaCompAppCtx_i is the handle the application context for the media component.
*
* @param[in] h_SigPkt_i is the handle of the data packet received as a result of the signal.
*
* @return e_MsRetCode the code for success or error.
*
*/

e_MsRetCode MediaChain_AppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_MediaChain* poMediaChain = (t_MediaChain*)hMediaCompAppCtx_i;
	//t_MediaComp* poMediaComp = (t_MediaComp*)hMediaComp_i;
	t_SigPkt* poSigPkt = h_SigPkt_i;

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hMediaCompAppCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hMediaComp_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != h_SigPkt_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( NULL != poMediaChain->pfMediaChainSignalCb )
		{
			eRetCode = (poMediaChain->pfMediaChainSignalCb)( hMediaComp_i, h_SigPkt_i, poMediaChain->hMediaCompAppCtx1, poMediaChain->hMediaCompAppCtx2 );
		}
		else
		{
			(poSigPkt->pfnSignalPktDelete)( poSigPkt );
			FW_Free( poSigPkt );
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMediaChain->oComp.nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ));

	return (eRetCode);
}

/*! @fn int MediaComp_LListRemoveLinkDataCallback( void** ppvData_io )
*
* @brief This callback function removes a link to a media component.
*
* @param[in] ppvData_io is the pointer to the pointer for media component whose link is to removed.
*
* @return Error code for success or error.
*
*/

int MediaComp_LListRemoveLinkDataCallback( void** ppvData_io )
{
	Media_CompDestroy((h_MediaComp)*ppvData_io );

	*ppvData_io = NULL;

	return (AMPS_SUCCESS);
}

t_MediaCompApi ppfn_CompApi[MCT_MAX] = {
	{
		BufferInit,
		BufferSetAppData,
		BufferDestroy,
		BufferSetStatus,
		BufferAppDataCb,
		BufferAppSignalCb,
		NULL,
		NULL
	},
	{
		FileReadInit,
		FileReadSetAppData,
		FileReadDestroy,
		FileReadSetStatus,
		NULL,
		FileReadAppSignalCb,
		FileReadSetParam,
		NULL
	},
	{
		FileWriteInit,
		FileWriteSetAppData,
		FileWriteDestroy,
		FileWriteSetStatus,
		FileWriteAppDataCb,
		FileWriteAppSignalCb,
		FileWriteSetParam,
		FileWriteGetParam
	},
	{
		LevelInit,
		LevelSetAppData,
		LevelDestroy,
		LevelSetStatus,
		LevelAppDataCb,
		LevelAppSignalCb,
		LevelSetParam,
		NULL
	},
	{
		PcmuDecInit,
		PcmuDecSetAppData,
		PcmuDecDestroy,
		PcmuDecSetStatus,
		PcmuDecAppDataCb,
		PcmuDecAppSignalCb,
		NULL,
		NULL
	},
	{
		PcmuEncInit,
		PcmuEncSetAppData,
		PcmuEncDestroy,
		PcmuEncSetStatus,
		PcmuEncAppDataCb,
		PcmuEncAppSignalCb,
		NULL,
		NULL
	},
	{
		RtpRecvInit,
		RtpRecvSetAppData,
		RtpRecvDestroy,
		RtpRecvSetStatus,
		RtpRecvAppDataCb,
		RtpRecvAppSignalCb,
		RtpRecvSetParam,
		RtpRecvGetParam
	},
	{
		RtpSendInit,
		RtpSendSetAppData,
		RtpSendDestroy,
		RtpSendSetStatus,
		RtpSendAppDataCb,
		RtpSendAppSignalCb,
		RtpSendSetParam,
		RtpSendGetParam
	},
	{
		TimeQueueInit,
		TimeQueueSetAppData,
		TimeQueueDestroy,
		TimeQueueSetStatus,
		TimeQueueAppDataCb,
		TimeQueueAppSignalCb,
		NULL,
		NULL
	},
	{
		UdpRecvInit,
		UdpRecvSetAppData,
		UdpRecvDestroy,
		UdpRecvSetStatus,
		NULL,
		NULL,
		UdpRecvSetParam,
		UdpRecvGetParam
	},
	{
		UdpSendInit,
		UdpSendSetAppData,
		UdpSendDestroy,
		UdpSendSetStatus,
		UdpSendAppDataCb,
		UdpSendAppSignalCb,
		UdpSendSetParam,
		UdpSendGetParam
	},
	{
		NullInit,
		NullSetAppData,
		NullDestroy,
		NullSetStatus,
		NullAppDataCb,
		NullAppSignalCb,
		NULL,
		NULL
	},
	{
		WavReadInit,
		WavReadSetAppData,
		WavReadDestroy,
		WavReadSetStatus,
		NULL,
		WavReadAppSignalCb,
		WavReadSetParam,
		NULL
	},
	{
		WavWriteInit,
		WavWriteSetAppData,
		WavWriteDestroy,
		WavWriteSetStatus,
		WavWriteAppDataCb,
		WavWriteAppSignalCb,
		WavWriteSetParam,
		WavWriteGetParam
	},
	{
		MixerInit,
		MixerSetAppData,
		MixerDestroy,
		MixerSetStatus,
		MixerAppDataCb,
		MixerAppSignalCb,
		MixerSetParam,
		MixerGetParam
	},
	{
		GsmDecInit,
		GsmDecSetAppData,
		GsmDecDestroy,
		GsmDecSetStatus,
		GsmDecAppDataCb,
		GsmDecAppSignalCb,
		GsmDecSetParam,
		NULL
	},
	{
		GsmEncInit,
		GsmEncSetAppData,
		GsmEncDestroy,
		GsmEncSetStatus,
		GsmEncAppDataCb,
		GsmEncAppSignalCb,
		NULL,
		NULL
	},
	{
		G729EncInit,
		G729EncSetAppData,
		G729EncDestroy,
		G729EncSetStatus,
		G729EncAppDataCb,
		G729EncAppSignalCb,
		NULL,
		NULL
	},
	{
		G729DecInit,
		G729DecSetAppData,
		G729DecDestroy,
		G729DecSetStatus,
		G729DecAppDataCb,
		G729DecAppSignalCb,
		NULL,
		NULL
	},
	{
		ReseqBufferInit,
		ReseqBufferSetAppData,
		ReseqBufferDestroy,
		ReseqBufferSetStatus,
		ReseqBufferAppDataCb,
		ReseqBufferAppSignalCb,
		NULL,
		NULL
	},
};

