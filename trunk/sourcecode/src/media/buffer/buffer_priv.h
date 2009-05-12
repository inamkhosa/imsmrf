#ifndef _BUFFER_PRIV_H_
#define _BUFFER_PRIV_H_

#include "media/buffer.h"

#define MCT_BUFFER_MAGIC_NUM 0x43218765

typedef struct _t_Buffer
{
	t_MediaComp        oComp;
	t_MediaCompAppData oAppData;

	int nMagicNum;

	int nEventId;

//  h_MediaCompAppCtx hAppDataCtx;
//  f_AppDataCb       pfAppDataCb;
//  f_AppSignalCb     pfAppSignalCb;
} t_Buffer;

void BufferEventHandler( void* pvFrameworkCtx_i, void* pvBufferCtx_i, void* pvEventHandle_i, void* pvDataPkt_i );

#endif /* _BUFFER_PRIV_H_ */

