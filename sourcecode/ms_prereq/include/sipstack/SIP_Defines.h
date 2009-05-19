/******************************************************************************
** Copyright (c) 2006-2007 
** 
*******************************************************************************
** FILE NAME      : SIP_Defines.h
** DESCRIPTION    : The file contains defines and return codes.
** PROJECT NAME   : Softswitch
** TYPE           : ANSI C
** CONTRIBUTOR(S) : Muhammad Mushtaq Ilyas
*******************************************************************************
** $Id: SIP_Defines.h,v 1.74 2009/02/11 15:27:35 arif Exp $
** $Source: /work/cvsroot/sipstack/common/SIP_Defines.h,v $
*******************************************************************************
*******************************************************************************
*/
#ifndef __HEADER_SIP_DEFINES_H__
#define __HEADER_SIP_DEFINES_H__

#ifdef __cplusplus
    extern "C" {
#endif

#include "AMPS_Defines.h"

#define AMPS_TRACE_MODULE_LCR			(16384 * 21)

enum
{
	SIP_NO_CONN_HANDLE = -2,
	SIP_RET_CODE_NO_HEADER= -3,
	SIP_INVALID_CONN_HANDLE=-4
}e_SIPReturnCodes;

#define SIP_URI_SCHEME_SIP_STRING			"sip"
#define SIP_URI_SCHEME_SIP_STRING_LENGTH	3

#define SIP_URI_SCHEME_SIPS_STRING			"sips"
#define SIP_URI_SCHEME_SIPS_STRING_LENGTH	4

#define SIP_MAX_UDP_PKT_LENGTH				64*1024

#define SIP_MAX_NUMBER_OF_TARGET_URI		5

typedef struct _SIPTag					t_SIPTag;
typedef struct _SIPString               t_SIPString;
typedef struct _SIPURL                  t_SIPURL;
typedef struct _SIPParam                t_SIPParam;
typedef struct _SIPRoute                t_SIPRoute;
typedef struct _SIPRoute                t_SIPRecordRoute;
typedef struct _SIPVia                  t_SIPVia;
typedef struct _SIPContact              t_SIPContact;
typedef struct _SIPRouteSet    			t_SIPRouteSet;
typedef struct _SIPHandle				t_SIPHandle;
typedef struct _SIPRouteSetEntry    	t_SIPRouteSetEntry;
typedef struct _SIPAddress				t_SIPAddress;
typedef struct _SIPContentType          t_SIPContentType;
typedef struct _SIPStackDomains			t_SIPStackDomains;
typedef struct _SIPStackConfiguration   t_SIPStackConfiguration;

typedef struct _SIPAuthorization        t_SIPAuthorization;
typedef struct _SIPAuthenticate			t_SIPAuthenticate;
typedef struct _SIPDomainInfo			t_SIPDomainInfo;

typedef struct _SIPChargingVector		t_SIPChargingVector;

typedef struct _SIPMsgPayload			t_SIPMsgPayload;

typedef void*							SIP_HANDLE;				//sip context
typedef struct _SIPConnHandle			SIP_CONN_HANDLE;		//sip session context
typedef void*							SIP_MSG_HANDLE;			//sip message context
typedef void*							SIP_SDP_MSG_HANDLE;		//sip sdp message context
typedef void*							SIP_OA_HANDLE;			//sip offer-answer context
typedef void*							SIP_APP_HANDLE;			//sip user application context
typedef void*							SIP_APP_CONN_HANDLE;	//sip user application session context
typedef void*							SIP_APP_MM_HANDLE;		//sip user application's memory manager context
typedef t_SIPMsgPayload*				SIP_MSG_PAYLOAD_HANDLE;		//sip message's payload context

typedef struct _SIPConnObject 			t_SIPConnObject;

typedef unsigned char*	SIP_TAG_VALUE;	//sip tag value
typedef unsigned char*	SIP_SDP_TAG_VALUE;	//sip sdp tag value


typedef enum
{
	SIP_STACK_MODE_PROXY = 1,
	SIP_STACK_MODE_MEDIA_PROXY,
	SIP_STACK_MODE_UA

}e_SIPStackMode;


///* Test if @c is a hex digit. 
#define SIP_IS_HEX(c)        (((c) >= '0' && (c) <= '9') || ((c) >= 'A' && (c) <= 'F') || ((c) >= 'a' && (c) <= 'f'))

struct _SIPString
{
    unsigned char*  poString;      //pointer to the message data
    unsigned int    unLength;       //size of buffer
};

#define MAX_DOMAIN_NAME_LENGTH	128

struct _SIPDomainInfo
{
	unsigned char       puchSIPDomainIP[MAX_DOMAIN_NAME_LENGTH];	//host IP, string Null terminated
	int        			nPort;
};

struct _SIPConnHandle
{
	t_AMPSHashTableKey	oHASHTableKey;
	int	nSIPSessionHandle;

	void* pvSIPSession;

	t_SIPString	oLocalTagString;
	t_SIPString	oRemoteTagString;
};

#define SIP_SIZE_OF_FILE_PATH							256
#define SIP_SIZE_OF_DOMAIN_NAME_ARRAY 					10
#define SIP_DOMAIN_NAME_LEN 							128

struct _SIPStackDomains
{
	unsigned char	puchDomainName[SIP_DOMAIN_NAME_LEN];
	int    			nDomainPort;
};

struct _SIPStackConfiguration
{
	AMPS_BOOL				bTraceEnable;
	AMPS_BOOL				bLoggingEnabled;

	unsigned int 			unTCPPort;
	unsigned int 			unUDPPort;

	int						nUDPPortCount;
	unsigned int 			unUDPPorts[5];

	char					pchIPAddress[AMPS_MAX_IP_STRING_LENGTH];
	AMPS_BOOL 				nTraceModuleSIPParser;
	AMPS_BOOL 				nTraceModuleTxn;
	AMPS_BOOL 				nTraceModuleTpt;

	int						nLogMode;
	char					puchLogFileName[SIP_SIZE_OF_FILE_PATH];

	int						nNumberOfDomains;
	int						nCurrentSIPDomainIndex;
	t_SIPStackDomains	    oSIPStackDomains[SIP_SIZE_OF_DOMAIN_NAME_ARRAY];

	int						nTxTimerValue;

	AMPS_BOOL				bTunnelingEnabled;
	unsigned int 			unTunnelingTCPPort;
	unsigned int 			unTunnelingUDPPort;
	char					pchTunnelingKey[AMPS_MAX_TUNNELING_KEY_LENGTH];

	int						nTimerMValInMilliSec;
};

struct _SIPParam
{
    t_SIPString     oName;
    t_SIPString     oValue;
};


#define SIP_MAX_DNS_NAME_LENGTH              	256
#define SIP_MAX_UNAME_STRING_LENGTH         	256

#define SIP_MAX_SCHEME_LENGTH           		8
#define SIP_MAX_TRANSPORT_LENGTH        		10

#define SIP_MAX_NUMBER_OF_TRANSPORT_TUPLES		3

struct _SIPHandle
{
	AMPS_BOOL 			bVerifyHandle;
	int			   		nHashId;
	t_AMPSHashTableKey 	oHASHTableKey;
};

struct _SIPConnObject
{
	int 				nPort;
	char				pchIPAddr[AMPS_MAX_IP_STRING_LENGTH];
	int                 nTptType;
};

struct _SIPAddress
{
	t_SIPConnObject	oSIPConnObject[SIP_MAX_NUMBER_OF_TRANSPORT_TUPLES];
	int				nNumberOfTuple;
	int				nCurrentTuple;
};

struct _SIPRouteSetEntry
{
	unsigned char		puchAuthUserName[SIP_MAX_UNAME_STRING_LENGTH];
	unsigned char 		puchPasswd[SIP_MAX_UNAME_STRING_LENGTH];

	unsigned char   	puchScheme[SIP_MAX_SCHEME_LENGTH];
	unsigned char	   	puchAddress[SIP_MAX_DNS_NAME_LENGTH];
	unsigned char 		puchUserName[SIP_MAX_UNAME_STRING_LENGTH];
	unsigned char       puchDestName[SIP_MAX_UNAME_STRING_LENGTH];
	unsigned int      	unPort;
	unsigned char       puchTransport[SIP_MAX_TRANSPORT_LENGTH];
	char       			pchFwdAddress[SIP_MAX_DNS_NAME_LENGTH];

	t_SIPAddress		oSIPAddress;
};

struct _SIPRouteSet
{
	t_SIPRouteSetEntry	oSIPRouteSetEntry[SIP_MAX_NUMBER_OF_TARGET_URI];
	int				    nNumberofRoutes;
	int				    nCurrentRoute;
};


#define SIP_PARAMS_ARRAY_COUNT 			6
#define SIP_MIN_PARAMS_ARRAY_COUNT 		2

#define SIP_MAX_URI_PARAMS_COUNT 		10
#define	SIP_MAX_DISPLAYNAME_LENGTH		32
#define SIP_MAX_URI_SCHEME_LENGTH			5
#define SIP_MAX_USER_LENGTH				64
#define SIP_MAX_HOST_LENGTH				64
#define SIP_MAX_PORT_LENGTH				4
struct _SIPURL
{
	unsigned char	puchDisplayName[SIP_MAX_DISPLAYNAME_LENGTH];
	unsigned int	unDisplayNameLength;
	unsigned char	puchScheme[SIP_MAX_URI_SCHEME_LENGTH];
	unsigned int 	unSchemeLength;
	unsigned char 	puchUser[SIP_MAX_USER_LENGTH];
	unsigned int	unUserLength;
	unsigned char 	puchHost[SIP_MAX_HOST_LENGTH]; //Either contains the DNS name or IP Address
	unsigned int	unHostLength;
	unsigned int	unPort;

    t_SIPString     oMaddr;
    t_SIPString     oTransport;
    t_SIPString     oLr;
    t_SIPString     oComp;
    t_SIPString     oTtl;
    t_SIPString     oMethod;
    t_SIPString     oUsr;
    int 			nParamsCount;
    t_SIPParam      oParamsArray[SIP_MAX_URI_PARAMS_COUNT];
};

#define SIP_MAX_ROUTE_PARAMS	6
struct _SIPRoute
{
    t_SIPURL        oAddr;                   /*  Route URL */

	t_SIPString     oReceived;
	t_SIPString     oRPort;

	int				nParamsCount;
    t_SIPParam      oParamsArray[SIP_MAX_ROUTE_PARAMS];         /*  Route Parameters */
};

#define SIP_MIN_VIA_PARAMS_ARRAY_COUNT 		4
#define	SIP_MAX_VIA_PROTO_LENGTH			4
#define	SIP_MAX_VIA_VERSION_LENGTH			4
#define	SIP_MAX_VIA_TPT_LENGTH				8

struct _SIPVia
{
	unsigned char 	puchProto[SIP_MAX_VIA_PROTO_LENGTH];
	unsigned int 	unProtoLength;
	unsigned char	puchVersion[SIP_MAX_VIA_VERSION_LENGTH];
	unsigned int	unVersionLength;
	unsigned char 	puchTpt[SIP_MAX_VIA_TPT_LENGTH];
	unsigned int	unTptLength;
	unsigned char	puchHost[SIP_MAX_HOST_LENGTH];
	unsigned int	unHostLength;
	unsigned int	unPort;
    
	t_SIPString     oMaddr;
    t_SIPString     oReceived;
    t_SIPString     oBranch;
    t_SIPString     oRPort;
	t_SIPString     oSigComp;

	int 			nParamsCount;
	t_SIPParam      oParamsArray[SIP_MIN_VIA_PARAMS_ARRAY_COUNT];
};

#define SIP_MAX_CONTACT_PARAMS_COUNT	3
struct _SIPContact
{
    t_SIPURL        oSIPURL;            /*  maddr/addr-spec */
    t_SIPString     oExpires;           /*  expiry field */
    t_SIPString     oQValue;            /* q value for preference*/
	int 			nParamsCount;
	t_SIPParam      oParamsArray[SIP_MAX_CONTACT_PARAMS_COUNT];
    t_SIPContact*   poSIPNextContact;   /*  next contact in list */ 
};

struct _SIPContentType
{
    t_SIPString     oType;       /*  Pointer to type/subtype */
    t_SIPString     oSubtype;    /*  Points after first slash in type */
};


#define SIP_MAX_AUTH_SCHEME_LENGTH		12
#define SIP_MAX_NO_OF_CREDENTIALS		15
struct _SIPAuthorization 
{
	unsigned char   puchScheme[SIP_MAX_AUTH_SCHEME_LENGTH];
	unsigned int	unSchemeLength;
	t_SIPString     oUsername;
	t_SIPString     oRealm;
	t_SIPString     oNonce;
	t_SIPString     oDigesturi;
	t_SIPString     oResponse;
	t_SIPString     oAlgorithm;
	t_SIPString     oCnonce;
	t_SIPString     oOpaque;
	t_SIPString     oMessageqop;
	t_SIPString     oNoncecount;
	t_SIPString     oAuthparam;
	int 			nParamsCount;
	t_SIPParam      oParamsArray[SIP_MAX_NO_OF_CREDENTIALS];
};

struct _SIPAuthenticate 
{
	unsigned char   puchScheme[SIP_MAX_AUTH_SCHEME_LENGTH];
	unsigned int	unSchemeLength;

	t_SIPString     oRealmString;
	t_SIPString		oDomainString;
	t_SIPString     oNonceString;
	t_SIPString     oOpaqueString;
	t_SIPString		oStaleString;
	t_SIPString		oQOPOptionsString;
	t_SIPString     oAlgorithmString;
	int 			nParamsCount;
	t_SIPParam      oParamsArray[SIP_MAX_NO_OF_CREDENTIALS];
};

struct _SIPMsgPayload
{
	t_SIPContentType	oSIPContentType;
	t_SIPString			oSIPStringForContent;
};

struct _SIPChargingVector
{
    t_SIPString                 oChargingId;
	t_SIPString                 oChargingGenAddr;
	t_SIPString                 oOrigIoI;
	t_SIPString                 oTermIoI;
};

typedef enum
{
	SIP_MSG_TYPE_INVITE = 1,
	SIP_MSG_TYPE_ACK,
	SIP_MSG_TYPE_OPTIONS,
	SIP_MSG_TYPE_BYE,
	SIP_MSG_TYPE_CANCEL,
	SIP_MSG_TYPE_REGISTER,
	SIP_MSG_TYPE_INFO,
	SIP_MSG_TYPE_PRACK,
	SIP_MSG_TYPE_SUBSCRIBE,
	SIP_MSG_TYPE_NOTIFY,
	SIP_MSG_TYPE_UPDATE,
	SIP_MSG_TYPE_MESSAGE,
	SIP_MSG_TYPE_REFER,
	SIP_MSG_TYPE_PUBLISH,
	SIP_MSG_TYPE_1XX_RESPONSE,
	SIP_MSG_TYPE_2XX_RESPONSE,
	SIP_MSG_TYPE_3XX_RESPONSE,
	SIP_MSG_TYPE_4XX_RESPONSE,
	SIP_MSG_TYPE_5XX_RESPONSE,
	SIP_MSG_TYPE_6XX_RESPONSE,
	SIP_MSG_TYPE_UNKNOWN,
	SIP_MSG_TYPE_MAX,
}e_SIPMsgType;



typedef enum
{
	SIP_RES_TRYING = 100,
	SIP_RES_RINGING=180,
	SIP_RES_CALL_BEING_FORWARDED,
	SIP_RES_QUEUED,
	SIP_RES_SESSION_PROGRESS,
	
	SIP_RES_OK=200,
	SIP_RES_ACCEPTED=202,

	SIP_RES_MULTIPLE_CHOICES=300,
	SIP_RES_MOVED_PERMANENTLY,
	SIP_RES_MOVED_TEMPORARILY,
	SIP_RES_USE_PROXY=305,
	SIP_RES_ALTERNATE_SERVICE=380,

	SIP_RES_BAD_REQUEST=400,
	SIP_RES_UNAUTHORIZED,
	SIP_RES_PAYMENT_REQUIRED,
	SIP_RES_FORBIDDEN,
	SIP_RES_NOT_FOUND,
	SIP_RES_METHOD_NOT_ALLOWED,
	SIP_RES_NOT_ACCEPTABLE,
	SIP_RES_PROXY_AUTHENTICATION_REQUIRED,
	SIP_RES_REQUEST_TIMEOUT,
	SIP_RES_CONFLICT,
	SIP_RES_GONE,
	SIP_RES_CONDITIONAL_REQUEST_FAILED=412,
	SIP_RES_REQUEST_ENTITY_TOO_LARGE,
	SIP_RES_REQUEST_URI_TOO_LONG,
	SIP_RES_UNSUPPORTED_MEDIA_TYPE,
	SIP_RES_UNSUPPORTED_URI_SCHEME,
	SIP_RES_UNKNOWN_RESOURCE_PRIORITY,
	SIP_RES_BAD_EXTENSION=420,
	SIP_RES_EXTENSION_REQUIRED,
	SIP_RES_SESSION_INTERVAL_TOO_SMALL,
	SIP_RES_INTERVAL_TOO_BRIEF,
	SIP_RES_USE_IDENTITY_HEADER=428,
	SIP_RES_PROVIDE_REFERER_IDENTITY,
	SIP_RES_BAD_IDENTITY_INFO=436,
	SIP_RES_UNSUPPORTED_CERTIFICATE,
	SIP_RES_INVALID_IDENTITY_HEADER,
	SIP_RES_TEMPORARILY_UNAVAILABLE=480,
	SIP_RES_CALL_TRANSACTION_DOES_NOT_EXIST,
	SIP_RES_LOOP_DETECTED,
	SIP_RES_TOO_MANY_HOPS,
	SIP_RES_ADDRESS_INCOMPLETE,
	SIP_RES_AMBIGUOUS,
	SIP_RES_BUSY_HERE,
	SIP_RES_REQUEST_TERMINATED,
	SIP_RES_NOT_ACCEPTABLE_HERE,
	SIP_RES_BAD_EVENT,
	SIP_RES_REQUEST_PENDING=491,
	SIP_RES_UNDECIPHERABLE=493,
	SIP_RES_SECURITY_AGREEMENT_REQUIRED,

	SIP_RES_SERVER_INTERNAL_ERROR=500,
	SIP_RES_NOT_IMPLEMENTED,
	SIP_RES_BAD_GATEWAY,
	SIP_RES_SERVICE_UNAVAILABLE,
	SIP_RES_SERVER_TIMEOUT,
	SIP_RES_VERSION_NOT_SUPPORTED,
	SIP_RES_MESSAGE_TOO_LARGE=513,
	SIP_RES_PRECONDITION_FAILURE=580,

	SIP_RES_BUSY_EVERYWHERE=600,
	SIP_RES_DECLINE=603,
	SIP_RES_DOES_NOT_EXIST_ANYWHERE,
	SIP_RES_NOT_ACCEPTABLE_ANYWHERE
}e_SIPResStatusCodes;

typedef enum
{
	SIP_ERR_CODE_NETWORK_FAILURE = 1,
	SIP_ERR_CODE_MSG_FAILURE,
	SIP_ERR_CODE_TIMER_B_FIRED,
	SIP_ERR_CODE_TIMER_C_FIRED,
	SIP_ERR_CODE_TIMER_F_FIRED,
	SIP_ERR_CODE_TIMER_H_FIRED,
	SIP_ERR_CODE_TIMER_I_FIRED,
	SIP_ERR_CODE_TIMER_J_FIRED,
	SIP_ERR_CODE_TIMER_L_FIRED
}e_SIPErrCode;

typedef enum
{
	SIP_TAG_RL_METHOD = 1,
	SIP_TAG_RL_REQUEST_URI,
	SIP_TAG_RL_VERSION,
	SIP_TAG_SL_STATUS_CODE,
	SIP_TAG_SL_REASON_PHRASE,
	SIP_TAG_SL_VERSION,
	SIP_TAG_FROM_URI,
	SIP_TAG_TO_URI,
	SIP_TAG_CALL_ID,
    SIP_TAG_USER_AGENT,
	SIP_TAG_CSEQ_NUMBER,
	SIP_TAG_CSEQ_METHOD,
	SIP_TAG_EXPIRES,

	SIP_TAG_ROUTE,
	SIP_TAG_RECORD_ROUTE,
	SIP_TAG_VIA,
	SIP_TAG_CONTACT,
	SIP_TAG_PROXY_ATHZ,

	SIP_TAG_SUPPORTED,
	SIP_TAG_REQUIRE,
	SIP_TAG_ALLOW,
	SIP_TAG_CHARGING_CONF_ID,
	SIP_TAG_PIN,
	SIP_TAG_ANI,
	SIP_TAG_SESSION_ID,
	SIP_TAG_CUSTOMER_ID,

	SIP_TAG_CONTENT_LENGTH,
	SIP_TAG_CONTENT_TYPE,
	SIP_TAG_MAX_FORWARD,

	SIP_TAG_CONTENT_BODY,

	SIP_TAG_AUTHORIZATION_HDR,	
	SIP_TAG_PXY_AUTHORIZATION_HDR,
	SIP_TAG_WWW_AUTHENTICATE_HDR,
	SIP_TAG_PXY_AUTHENTICATE_HDR,

	SIP_TAG_RURI_PARAM,
	SIP_TAG_TO_PARAM,
	SIP_TAG_TO_URI_PARAM,
	SIP_TAG_FROM_PARAM,
	SIP_TAG_FROM_URI_PARAM,
    SIP_TAG_CONTACT_PARAM,
	SIP_TAG_CONTACT_URI_PARAM

}e_SIPTagType;

struct _SIPTag
{
	e_SIPTagType			oSIPTagType;

	union
	{
		t_SIPString				oSIPStringRLMethod;
		t_SIPURL				oRURI;
		t_SIPString				oSIPStringRLVersion;
		unsigned int	 		unStatusCode;
		t_SIPString				oSIPStringReasonPhrase;
		t_SIPString				oSIPStringSLVersion;
		t_SIPURL				oFromURI;
		t_SIPURL				oToURI;
		t_SIPString				oSIPStringCallId;
		t_SIPURL				oContactURI;

		
		t_SIPRoute				oSIPRoute;
		t_SIPRecordRoute		oSIPRecordRoute;
		t_SIPVia				oSIPVia;
		t_SIPContact			oSIPContact;

		t_SIPString				oSIPStringUsreAgent;
		t_AMPSDList*			poSupportedList;
		t_AMPSDList*			poRequireList;
		t_AMPSDList*			poAllowedList;
		t_SIPString				oSIPChargingConfId;
		t_SIPString				oSIPStringPin;
		t_SIPString				oSIPStringANI;
		t_SIPString				oSIPStringSessionId;
		t_SIPString				oSIPStringCustomerId;

		unsigned int	 		unContentLength;
		t_SIPContentType		oContentType;
		unsigned int			unMaxForward;
		unsigned int			unCSeqNumber;
		unsigned int			unCSeqMethod;
		unsigned int 			unExpires;

		t_SIPAuthorization		oSIPAuthorization;
		t_SIPAuthenticate		oSIPAuthenticate;

	}u_SIPTagValue;
};


#define MAX_NUMBER_OF_TRANSPORT_TUPLES		3

#define SIP_HASHTABLESIZE  		11003   // Prime Nos : 1009, 11003, 11027, 50021, 50023, 100019

#define SIP_MAX_PORT_LEN		6
#define SIP_TCP_LISTEN_PORT		5060
#define SIP_UDP_LISTEN_PORT		5060
#define SIP_TLS_LISTEN_PORT		5061

#define SIP_TRANSPORT_UDP_STRING				"udp"
#define SIP_TRANSPORT_UDP_STRING_LENGTH			3
#define SIP_TRANSPORT_TCP_STRING				"tcp"
#define SIP_TRANSPORT_TCP_STRING_LENGTH			3
#define SIP_TRANSPORT_TLS_STRING				"tls"
#define SIP_TRANSPORT_TLS_STRING_LENGTH			3
#define SIP_TRANSPORT_DTLS_STRING				"dtls"
#define SIP_TRANSPORT_DTLS_STRING_LENGTH		4
#define SIP_TRANSPORT_SCTP_STRING				"sctp"
#define SIP_TRANSPORT_SCTP_STRING_LENGTH		4
#define SIP_TRANSPORT_TLS_SCTP_STRING			"tls-sctp"
#define SIP_TRANSPORT_TLS_SCTP_STRING_LENGTH	8


#define HDR_TRAIL_STRING					"\r\n" 	
#define HDR_TRAIL_STRING_LENGTH				2 	

#define RANDOM_STRING_LEN					64
#define BRANCHID_MAGIC_COOKIE				"z9hG4bK"
#define BRANCHID_MAGIC_COOKIE_LEN			7

#define SIP_VIA_BRANCH_PARAM_STRING			"branch"
#define SIP_VIA_BRANCH_PARAM_STRING_LENGTH	6

#define SIP_VIA_HEADER_PROTO		 		"SIP"
#define SIP_VIA_HEADER_PROTO_LENGTH 		3 

#define SIP_VIA_HEADER_VERSION		 		"2.0"
#define SIP_VIA_HEADER_VERSION_LENGTH		3 

#define SIP_VIA_HEADER_TRANSPORT_TCP		"TCP"
#define SIP_VIA_HEADER_TRANSPORT_TCP_LENGTH	3

#define SIP_VIA_HEADER_TRANSPORT_UDP		"UDP"
#define SIP_VIA_HEADER_TRANSPORT_UDP_LENGTH	3

#define SIP_VERSION_STRING                  "SIP/2.0"
#define SIP_VERSION_STRING_LENGTH           7

//SIP Authentication Related
#define SIP_MAX_REALM_LENGTH				128
#define SIP_MAX_DOMAIN_LENGTH				128
#define SIP_MAX_NONCE_LENGTH				32
#define SIP_MAX_OPAQUE_LENGTH				32
#define SIP_MAX_ALGORITHM_LENGTH			32
#define SIP_MAX_QOP_OPTIONS_LENGTH			32
#define SIP_MAX_AUTH_PARAM_LENGTH			32
#define SIP_MAX_RESPONSE_LENGTH				32

//Authentication Status Codes
typedef enum
{
	SIP_AUTH_STATUS_FAILURE	= -1,
	SIP_AUTH_STATUS_SUCCESS,
	SIP_AUTH_STATUS_NO_AUTHORIZATION_HEADER_FOUND,
	SIP_AUTH_STATUS_NONCE_ERROR,
	SIP_AUTH_STATUS_NONCE_STALE_ERROR,
	SIP_AUTH_STATUS_SCHEME_ERROR,
	SIP_AUTH_STATUS_USER_NAME_ERROR,
	SIP_AUTH_STATUS_RESPONSE_ERROR,

}e_SIPAuthStatusdCode;


#ifdef __cplusplus
   }
#endif

#endif /* __HEADER_SIP_DEFINES_H__ */

