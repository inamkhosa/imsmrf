#ifndef _MIXER_PRIV_H_
#define _MIXER_PRIV_H_

#include "media/mixer.h"

#include "media/genpkt.h"

#include "util/circular_list.h"

#define MCT_MIXER_MAGIC_NUM 0x41328576

typedef struct _t_MixerNode
{
	h_MediaComp       hMixer;
	t_CirList*        poInPktList;
	int               nLastPktGenTime;            // Generation time of last pkt sent
	int               nLastPktCurrentTime;        // Current time of last pkt sent
	t_MixerNodeConfig oNodeConfig;
} t_MixerNode;

typedef struct _t_Mixer
{
	t_MediaComp oComp;

	int nMagicNum;

	int        nBufferTimeMs;      // time to store the packet in input-queues of each source, for mixing
	int        nPacketizationTimeMs;   // amount of audio in one packet
	t_CirList* poNodeList;  // list of t_MixerNode
} t_Mixer;

typedef struct _t_MixData
{
	int        nTraceId;
	int        nBufferTimeMs;      // IN:Buffer the packet if the generation time of pkt is greater than buffer-time lesser than current-time
	int        nCurrentTimeMs;     // IN:Current time (synchronized with all input packet generation time)
	int        nMixTimeMs;         // IN:Time to get the packet for mixing
	AMPS_BOOL  bMixPkt;      // INOUT:Whether the packets are to be mixed or not
	t_CirList* poMixPktList;    // OUT:List of packets to be mixed
} t_MixData;

void MixerNode_Delete( void* pvData_i, void* pvUserData_i );

int MixerNode_Compare( void* pvData1_i, void* pvData2_i );

int MixerNode_CompareConfig( void* pvData1_i, void* pvData2_i );

int MixerNode_CompareSrc( void* pvData1_i, void* pvData2_i );

void MixerNode_GetPacket( void* pvData_i, void* pvUserData_i );

void MixerNode_DeleteOldPacket( void* pvData_i, void* pvUserData_i );

void DataPkt_Mix( void* pvData_i, void* pvUserData_i );

void MixerMix( t_Mixer* poMixer_i, int nCurrentTimeMs_i );

void MixerNodeProcess( void* pvData_i, void* pvUserData_i );

int MixerGetCurrentSyncTimeMs( t_Mixer* poMixer_i, t_MixerNode* poNode_i );

void MixerNode_SendPacket( void* pvData_i, void* pvUserData_i );

#endif /* _MIXER_PRIV_H_ */

