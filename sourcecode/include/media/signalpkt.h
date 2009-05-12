#ifndef _SIGNAL_PKT_H_
#define _SIGNAL_PKT_H_

#include "fw_api.h"
#include "ms_ret_code.h"

typedef enum
{
	SPT_DTMP_Packet = 1,
	SPT_SOUND_DETECTED_Packet = 2,
	SPT_SILENCE_DETECTED_Packet = 3,
	SPT_EOF_Detected = 4,
	SPT_REMOTE_ADDRESS_CHANGE_Packet = 5,
	SPT_CURRENT_SYNC_TIME_Packet = 6,
	SPT_PAYLOAD_TYPE_CHANGE_Packet = 7
} e_SingalPktType;

typedef void*  h_SigPkt;

typedef e_MsRetCode (*f_SignalPktInit)( h_SigPkt h_SigPkt_i );
typedef e_MsRetCode (*f_SignalPktDelete)( h_SigPkt h_SigPkt_i );

typedef struct _t_DtmfPkt
{
	unsigned char ucDtmfKey;
	unsigned char ucVolume;
} t_DtmfPkt;

typedef struct _t_LevelPkt
{
	double dLevelDb;
} t_LevelPkt;

typedef struct _t_FileReadPkt
{
	long lFileOffset;
} t_FileReadPkt;

typedef struct _t_WavReadPkt
{
	long lFileOffset;
} t_WavReadPkt;

typedef struct _t_AddrChangePkt
{
	char pcIp[AMPS_MAX_IP_STRING_LENGTH];
	int  nPort;
} t_AddrChangePkt;

typedef struct _t_PayloadChangePkt
{
	unsigned char ucPayloadType;
} t_PayloadChangePkt;

typedef struct _t_SyncTimePkt
{
	int nDelayMs;
	int nSyncTimeMs;
} t_SyncTimePkt;

typedef struct _t_SigPkt
{
	e_SingalPktType   eType;
	unsigned int      unRefCount;
	f_SignalPktInit   pfnSignalPktInit;
	f_SignalPktDelete pfnSignalPktDelete;

	union
	{
		t_DtmfPkt oDtmfPkt;
		t_LevelPkt oLevelPkt;
		t_FileReadPkt oFileReadPkt;
		t_WavReadPkt oWavReadPkt;
		t_AddrChangePkt oAddrChangePkt;
		t_SyncTimePkt oSyncTimePkt;
		t_PayloadChangePkt oPayloadChangePkt;
	};
} t_SigPkt;

int Signal_LListRemoveLinkDataCallback( void** ppvData_i );

#endif /* _SIGNAL_PKT_H_ */

