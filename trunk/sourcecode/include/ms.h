#ifndef _MS_H_
#define _MS_H_

#include "ms_events.h"
#include "call_leg/call_leg_api.h"
#include "media/media.h"
#include "sdp_api.h"
#include "sdp_oam_api.h"
#include "tstring.h"
#include "util/sip_util.h"
#include "util/sync_timer.h"
#include "mscml_sm/mscml_sm_api.h"
#include "snmp/snmp_defines.h"

#define MS_CONF_CTRL_CTX_MAGIC_NUM 0x18723654
#define MS_USER_CTRL_CTX_MAGIC_NUM 0x43218765
#define MS_CONF_CTX_MAGIC_NUM 0x13245867

typedef enum
{
	MM_FULL,
	MM_PARKED,
	MM_MUTE,
	MM_PRIVATE,
	MM_PREFERRED
} e_MixMode;

typedef enum
{
	UCS_AVAILABLE,          // the call leg is available
	UCS_INITIALIZED,        // the call leg is in use
	UCS_DESTROY_IN_PROGRESS     // the call leg is being destroyed
} e_UserCtxState;

typedef struct _t_MsConfCtx t_MsConfCtx;

typedef struct _t_MsUserCtrlCtx
{
	int            nMagicNum;
	t_MsConfCtx*   poMsConfCtx;   // pointer to conference
	e_UserCtxState eState;      // state of call leg
	h_CallLeg      hCallLeg;         // handle of associated SIP Call Leg
	h_Sdp          hNegotiatedSdp;           // handle of negotiated SDP
	h_Sdp          hRemoteSdp;           // handle of Remote SDP
	h_MediaChain   hRxChain;          // handle of receiver media chain
	h_MediaChain   hTxChain;          // handle of transmitter media chain
	e_MixMode      eCurrentMode;           // current MSCML mode
	void*          pvMscmlOpCtx;         // handle of play, playrecord, playcollect state machine
	t_String       oTeamMemberId;         // id of this member in team
	t_AMPSDList*   poTeamMateList;        // list of team mates (t_MsUserCtrlCtx)
	t_AMPSDList*   poQuarantineBuffer;    // list of events buffered in quarantine-buffer
	t_AMPSDList*   poNegInfoList;     //list of information related to negotiated SDP
	t_SipAppData   oSipData;        // SIP related information maintained by application
	AMPS_BOOL      bDtmfClamp;       // Ignore DTMF digits

	xmlDocPtr     poPendingMscmlReq;         // xmlDocPtr of pending request
	e_MscmlOpType ePendingOpCtxType;
} t_MsUserCtrlCtx;

struct _t_MsConfCtx
{
	int              nMagicNum;
	t_String         oConfId;
	t_MsUserCtrlCtx* poConfCtrlCtx;
	t_AMPSDList*     poUserCtrlCtxList; // list of t_MsUserCtrlCtx
	t_AMPSDList*     phMixerCompList;   // list of h_MediaComp(MCT_MIXER)
	int              nAvailableCtrlCtx;
	h_SyncTimer      hTimer;     // Timer to provide syncronized time for this conference
};

typedef struct _t_MsCtx
{
	int        nTraceId;
	void*      pvFrameworkCtx;
	h_Sdp      hLocalSdp;
	SIP_HANDLE hSipStack;

	h_CallLegStack hCallLegStack;

	t_Uri                 oLocalUri;
	int                   nJitterBufferMs;
	int                   nSilenceThresholdDb;
	h_SdpParser           hSdpParser;
	t_String              oLocalIp;
	xmlSchemaValidCtxtPtr poMscmlValidCtx;

	char* pchSoundDir;

	t_AMPSDList* poConfCtxList; // list of t_MsConfCtx
	t_AMPSDList* poUserCtrlCtxList; // list fo t_MsUserCtrlCtx of user outside conference

	xmlDocPtr     poDoc; // xml pointer to parsed media server configuration
	h_SnmpComm    hSnmpComm; // handle for communication with snmp
	unsigned long pulPerformanceCtrs[PC_MaxCtrValue]; // Media server performance counters
	unsigned long pulFaultCtrs[FC_MaxCtrValue]; // Media server fault counters
} t_MsCtx;

#endif /* _MS_H_ */

