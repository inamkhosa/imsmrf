#ifndef _MS_MEDIA_H_
#define _MS_MEDIA_H_

#include "ms_ret_code.h"
#include "media/media.h"
#include "media/udprecv.h"
#include "media/rtprecv.h"
#include "media/pcmudec.h"
#include "media/pcmuenc.h"
#include "media/level.h"
#include "media/null.h"
#include "sdp_api.h"
#include "call_leg_handler.h"
#include "media/fileread.h"
#include "media/udpsend.h"
#include "media/rtpsend.h"
#include "media/timequeue.h"
#include "media/filewrite.h"
#include "media/mixer.h"
#include "media/gsmdec.h"
#include "media/gsmenc.h"
#include "media/g729enc.h"
#include "media/g729dec.h"
#include "media/reseqbuffer.h"

#include "tstring.h"
#include "ms_util.h"

e_MsRetCode MS_Media_RxChainInit( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i );

e_MsRetCode MS_Media_RxChainUpdate( t_MsCtx* poMsCtx_i, h_Sdp hLocalSdp_i, h_Sdp hRemoteSdp_i, h_Sdp hNegotiatedSdp_i, t_AMPSDList* poNegInfoList_i, h_MediaChain hMediaChain_i );

e_MsRetCode MS_Media_RxChainDestroy( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i );

e_MsRetCode MS_Media_TxChainInit( t_MsCtx* poMsCtx_i, h_MediaChain hMediaChain_i );

e_MsRetCode MS_Media_TxChainDestroy( t_MsCtx* poMsCtx_i, h_MediaChain phMediaChain_i );

e_MsRetCode MS_Media_RxChainSetMode( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i, e_MixMode eMode_i );

e_MsRetCode MS_Media_TxChainSetMode( t_MsCtx* poMsCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i, e_MixMode eMode_i );

e_MsRetCode MS_Media_AddInMixer( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i );

e_MsRetCode MS_Media_ChainSetMode( t_MsCtx* poMsCtx_i, t_MsConfCtx* poMsConfCtx_i, t_MsUserCtrlCtx* poUserCtrlCtx_i, e_MixMode eMode_i, t_SdpNegMediaInfo* poNegMediaInfo_i );

#endif /* _MS_MEDIA_H_ */

