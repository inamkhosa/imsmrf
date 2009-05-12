#ifndef _MS_MEDIA_HANDLER_H_
#define _MS_MEDIA_HANDLER_H_

#include "ms_events.h"
#include "ms_media.h"

e_MsRetCode MS_Media_RxChainCb( h_MediaComp hMediaComp_i, h_SigPkt h_SigPkt_i, h_MediaCompAppCtx hMediaCompAppCtx1_i, h_MediaCompAppCtx hMediaCompAppCtx2_i );

e_MsRetCode MS_Media_TxChainCb( h_MediaComp hMediaComp_i, h_SigPkt h_SigPkt_i, h_MediaCompAppCtx hMediaCompAppCtx1_i, h_MediaCompAppCtx hMediaCompAppCtx2_i );

#endif /* _MS_MEDIA_HANDLER_H_ */

