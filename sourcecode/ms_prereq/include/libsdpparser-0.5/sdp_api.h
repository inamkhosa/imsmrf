#ifndef _SDP_API_H_
#define _SDP_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
 #include "tstring.h"
#else
 #include "tstring.h"
#endif

typedef enum
{
	SDP_ERR_NONE,
	SDP_ERR_PRE_COND_FAILED,
	SDP_ERR_POST_COND_FAILIED,
	SDP_ERR_MISSING_FIELD,
	SDP_ERR_INSUFFICIENT_MEMORY,
	SDP_ERR_INVALID_OPERATION,
	SDP_ERR_INVALID_INPUT,
	SDP_ERR_FRAMEWORK,
	SDP_ERR_END_OF_LIST,
	SDP_ERR_DECODE,
	SDP_ERR_ENCODE,
	SDP_ERR_NOT_IMPLEMENTED,
	SDP_ERR_MAX
} e_SdpRetCode;

typedef struct _t_SdpParserConfig
{
	AMPS_HANDLE hFramework;
	int         nTraceId;
} t_SdpParserConfig;

typedef void* h_SdpParser;

typedef struct _t_SdpConfig
{
	int nTraceId;
} t_SdpConfig;

typedef void* h_Sdp;

////////////////////////////////////////////////////////////////
typedef struct _t_SDPVersion
{
	int nVersion;
} t_SDPVersion;

typedef struct _t_SDPInfo
{
	t_String oDescription;
} t_SDPInfo;

typedef struct _t_SDPURI
{
	t_String oScheme;
	t_String oAuthority;
	t_String oPath;
	t_String oQuery;
	t_String oFragment;
} t_SDPURI;

typedef struct _t_SDPPhoneNumber
{
	t_String oPhone;
} t_SDPPhoneNumber;

typedef struct _t_SDPEmail
{
	t_String oEmail;
} t_SDPEmail;

typedef struct _t_SDPRegularTime
{
	int nTemp;
} t_SDPRegularTime;

typedef struct _t_SDPBandwidth
{
	t_String oBandwidthType;
	int      nBandwidth;
} t_SDPBandwidth;

typedef struct _t_SDPTiming
{
	int nStartTime;
	int nStopTime;
} t_SDPTiming;

typedef struct _t_SDPEncryptKey
{
	unsigned char uchDummy;
} t_SDPEncryptKey;

typedef struct _t_SDPConnection
{
	t_String oNetType;
	t_String oAddrType;
	t_String oConnectionAddress;         //     multicast-address  / unicast-address  ==>   IP4-multicast  / IP6-multicast  / FQDN  / extn-addr
} t_SDPConnection;

typedef struct _t_SDPOrigin
{
	t_String        oUserName;
	int             nSessionId;
	int             nSessionVersion;
	t_SDPConnection oSDPConnectionData;
} t_SDPOrigin;

typedef struct _t_SDPSessionName
{
	t_String oName;
} t_SDPSessionName;

typedef struct _t_SDPAttribute
{
	t_String oAttributeField;
	int      nAttribValuePresent;
	union
	{
		t_String oAttributeValue;
	} attribute;
} t_SDPAttribute;

typedef struct _t_SDPMedia
{
	t_String     oMediaType;                                      //TOKEN === Media Type (audio/video/text/control/message)
	int          nMediaPrimaryPort;                               //Port
	int          nMediaSecondaryPort;                             //Port for [/]
	t_String     oMediaProto;                                     //Proto e.g TCP, TLC/SCTP
	t_AMPSDList* poMediaFormatList;
	//t_String     oMediaFormat;                                    //RTP payload numbers etc.
} t_SDPMedia;

////////////////////////////////////////////////////////////////

int InitSdp( int* pnErrCodeVal_i );

// Allocation / Deallocation Functions
int InitSdpParser( t_SdpParserConfig* poSdpParserConfig_i, h_SdpParser* phSdpParser_o );

int DestroySdpParser( h_SdpParser* phSdpParser_io );

int AllocateSdp( t_SdpConfig* poSdpConfig_i, h_Sdp* phSdp_o );

int DeallocateSdp( h_Sdp* phSdp_io );

// Get Functions

int SdpGetProtoVersion( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPVersion** ppoSdpVersion_o );

int SdpGetOrigin( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPOrigin** ppoSdpOrigin_o );

int SdpGetSessionName( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPSessionName** ppoSdpSessionName_o );

int SdpGetInfo( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPInfo** ppoSdpInfo_o );

int SdpGetURI( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPURI** ppoSdpURI_o );

int SdpGetEmail( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPEmail** ppoSdpEmail_o );

int SdpGetPhone( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPPhoneNumber** ppoSdpPhoneNumber_o );

int SdpGetConnection( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPConnection** ppoSdpConnection_o );

int SdpGetKey( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPEncryptKey** ppoSdpKey_o );

int SdpGetBandwidth( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i, t_SDPBandwidth** ppoSdpBandwidth_o );

int SdpGetTime( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i, t_SDPTiming** ppoSdpTime_o );

int SdpGetRegularTime( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i, t_SDPRegularTime** ppoSdpRegTime_o );

int SdpGetAttribute( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i, t_SDPAttribute** ppoSdpAttr_o );

int SdpMediaDescGetMedia( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, t_SDPMedia** ppoSdpMedia_o );

int SdpMediaDescGetAttribute( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, int nFieldIndex_i, t_SDPAttribute** ppoSdpAttr_o );

int SdpMediaDescGetInfo( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, t_SDPInfo** ppoSdpInfo_o );

int SdpMediaDescGetBandwidth( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, int nFieldIndex_i, t_SDPBandwidth** ppoSdpBandwidth_o );

int SdpMediaDescGetConnection( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, int nFieldIndex_i, t_SDPConnection** ppoSdpConnection_o );

int SdpMediaDescGetKey( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, t_SDPEncryptKey** ppoSdpKey_o );

// Set Functions
int SdpSetProtoVersion( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPVersion* poSdpVersion_i );

int SdpSetOrigin( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPOrigin* poSdpOrigin_i );

int SdpSetSessionName( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPSessionName* poSdpSessionName_i );

int SdpSetInfo( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPInfo* poSdpInfo_i );

int SdpSetURI( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPURI* poSdpURI_i );

int SdpSetEmail( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPEmail* poSdpEmail_i );

int SdpSetPhone( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPPhoneNumber* poSdpPhoneNumber_i );

int SdpSetConnection( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPConnection* poSdpConnection_i );

int SdpSetKey( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_SDPEncryptKey* poSdpKey_i );

int SdpSetBandwidth( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i, t_SDPBandwidth* poSdpBandwidth_i );

int SdpSetTime( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i, t_SDPTiming* poSdpTime_i );

int SdpSetRegularTime( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i, t_SDPRegularTime* poSdpRegTime_i );

int SdpSetAttribute( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i, t_SDPAttribute* poSdpAttr_i );

int SdpMediaDescSetMedia( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, t_SDPMedia* poSdpMedia_i );

int SdpMediaDescSetAttribute( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, int nFieldIndex_i, t_SDPAttribute* poSdpAttr_i );

int SdpMediaDescSetInfo( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, t_SDPInfo* poSdpInfo_i );

int SdpMediaDescSetBandwidth( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, int nFieldIndex_i, t_SDPBandwidth* poSdpBandwidth_i );

int SdpMediaDescSetConnection( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, int nFieldIndex_i, t_SDPConnection* poSdpConnection_i );

int SdpMediaDescSetKey( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, t_SDPEncryptKey* poSdpKey_i );

// Add Functions

int SdpAddBandwidth( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i, t_SDPBandwidth* poSdpBandwidth_i );

int SdpAddAttribute( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i, t_SDPAttribute* poSdpAttr_i );

int SdpMediaDescAddMedia( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, t_SDPMedia* poSdpMedia_i );

int SdpMediaDescAddAttribute( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, int nFieldIndex_i, t_SDPAttribute* poSdpAttr_i );

int SdpAddTime( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i, t_SDPTiming* poSdpTime_i );

//Remove funtions
int SdpRemoveProtoVersion( h_SdpParser hSdpParser_i, h_Sdp hSdp_i );

int SdpRemoveOrigin( h_SdpParser hSdpParser_i, h_Sdp hSdp_i );

int SdpRemoveSessionName( h_SdpParser hSdpParser_i, h_Sdp hSdp_i );

int SdpRemoveInfo( h_SdpParser hSdpParser_i, h_Sdp hSdp_i );

int SdpRemoveURI( h_SdpParser hSdpParser_i, h_Sdp hSdp_i );

int SdpRemoveEmail( h_SdpParser hSdpParser_i, h_Sdp hSdp_i );

int SdpRemovePhone( h_SdpParser hSdpParser_i, h_Sdp hSdp_i );

int SdpRemoveConnection( h_SdpParser hSdpParser_i, h_Sdp hSdp_i );

int SdpRemoveKey( h_SdpParser hSdpParser_i, h_Sdp hSdp_i );

int SdpRemoveBandwidth( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i );

int SdpRemoveTime( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i );

int SdpRemoveRegularTime( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i );

int SdpRemoveAttribute( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nIndex_i );

int SdpMediaDescRemoveMedia( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i );

int SdpMediaDescRemoveAttribute( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, int nFieldIndex_i );

int SdpMediaDescRemoveInfo( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i );

int SdpMediaDescRemoveBandwidth( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, int nFieldIndex_i );

int SdpMediaDescRemoveConnection( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i, int nFieldIndex_i );

int SdpMediaDescRemoveKey( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int nDescIndex_i );

// Encode / Decode Functions
int SdpDecode( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, unsigned char* pucSdpBuffer_i, int nSdpLen_i );

//int SdpEncode( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, unsigned char** pucSdpBuffer_i );

int SdpEncode( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, t_String* poSdpBuffer_i );

int SdpGetLength( h_SdpParser hSdpParser_i, h_Sdp hSdp_i, int* pnSdpLen_o );

// Copy Functions

int SdpCopyBandwidth( t_SDPBandwidth* poSrcBw_i, t_SDPBandwidth* poDestBw_i );

int SdpCopyTime( t_SDPTiming* poSrc_i, t_SDPTiming* poDest_i );

int SdpCopyAttribute( t_SDPAttribute* poSrc_i, t_SDPAttribute* poDest_i );

int SdpCopyInfo( t_SDPInfo* poSrc_i, t_SDPInfo* poDest_i );

int SdpCopyVersion( t_SDPVersion* poSrc_i, t_SDPVersion* poDest_i );

int SdpCopyOrigin( t_SDPOrigin* poSrc_i, t_SDPOrigin* poDest_i );

int SdpCopySessionName( t_SDPSessionName* poSrc_i, t_SDPSessionName* poDest_i );

int SdpCopyURI( t_SDPURI* poSrc_i, t_SDPURI* poDest_i );

int SdpCopyEmail( t_SDPEmail* poSrc_i, t_SDPEmail* poDest_i );

int SdpCopyPhone( t_SDPPhoneNumber* poSrc_i, t_SDPPhoneNumber* poDest_i );

int SdpCopyConnection( t_SDPConnection* poSrc_i, t_SDPConnection* poDest_i );

int SdpCopyKey( t_SDPEncryptKey* poSrc_i, t_SDPEncryptKey* poDest_i );

int SdpCopyMedia( t_SDPMedia* poSrc_i, t_SDPMedia* poDest_i );

// Free Content of structure
int SdpFreeBandwidth( t_SDPBandwidth* poBw_i );

int SdpFreeAttribute( t_SDPAttribute* poAttrib_i );

int SdpFreeInfo( t_SDPInfo* poInfo_i );

int SdpFreeOrigin( t_SDPOrigin* poOrigin_i );

int SdpFreeConnection( t_SDPConnection* poConnection_i );

int SdpFreeSessionName( t_SDPSessionName* poSessName_i );

int SdpFreeUri( t_SDPURI* poUri_i );

int SdpFreeEmail( t_SDPEmail* poEmail_i );

int SdpFreePhoneNumber( t_SDPPhoneNumber* poPhoneNum_i );

int SdpFreeMedia( t_SDPMedia* poMedia_i );

#ifdef __cplusplus
}
#endif

#endif /* _SDP_API_H_ */

