#include <stdio.h>

#include "sdp_api.h"

char pcSdpMsg[] = "v=0\r\n\
o=alice 2890844526 2890844526 IN IP4 host.atlanta.example.com\r\n\
s=\r\n\
c=IN IP4 host.atlanta.example.com\r\n\
t=0 0\r\n\
m=audio 49170 RTP/AVP 0 8 97\r\n\
c=IN IP4 192.168.0.256\r\n\
a=rtpmap:0 PCMU/8000\r\n\
a=rtpmap:8 PCMA/8000\r\n\
a=rtpmap:97 iLBC/8000\r\n\
m=video 51372 RTP/AVP 31 32\r\n\
a=rtpmap:31 H261/90000\r\n\
a=rtpmap:32 MPV/90000\r\n"                                                                                                                                                                                                                                                                                                                                                                   ;

/*
char pcSdpMsg[] = "v=0\r\n \
o=alice 2890844526 2890844526 IN IP4 host.atlanta.example.com\r\n \
s=\r\n \
c=IN IP4 host.atlanta.example.com\r\n \
t=0 0\r\n \
m=audio 49170 RTP/AVP 0 8 97\r\n \
a=rtpmap:0 PCMU/8000\r\n \
a=rtpmap:8 PCMA/8000\r\n \
a=rtpmap:97 iLBC/8000\r\n";
*/

typedef enum
{
	APP_INTERNAL_EVENT_MAX
} e_RTP_InteranlEvent;

typedef enum
{
	APP_EXTERNAL_EVENT_MAX
} e_RTP_ExternalEvent;

int main( int argc, char* argv[] )
{
	void* pvAMPSContext = NULL;
//	t_SDPParserCtxt* poSDPParserCtxt = NULL;
	//int nRetVal = AMPS_SUCCESS;
	int nTraceId;
//	char pchIpAddr[50] = "";

	//t_SDP* poSDP = NULL;
	e_MsRetCode eRetCode = MS_ERR_NONE;
	t_SdpParserConfig oSdpParserConfig;
	t_SdpConfig oSdpConfig;
	h_SdpParser hSdpParser;
	h_Sdp hSdp;
/*	t_SDPVersion* poSDPVersion;
t_SDPOrigin* poSDPOrigin;
t_SDPSessionName* poSDPSessionName;
t_SDPConnection* poSDPConnection;
t_SDPMedia* poSDPMedia;
t_SDPTiming* poSdpTime;*/
//    t_SDPAttribute* poSDPAttribute;

	//unsigned char* pchEncodedSdp = NULL;
	t_String oEncodedSdp = TSTRING_INITIALIZER;

//	unsigned char pchEncodedSdp[2000] = "";
	//int nLength = 0;

	pvAMPSContext = AMPS_Init( AMPS_TIMER_RESOLUTION_20MS, 1, ".", AMPS_PORT_POOL_SIZE_10, 5000 );
	if( NULL == pvAMPSContext )
	{
		printf( "AMPS_Init: AMPS_Init is failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	else
	{
		printf( "pvAMPSContext = %p\n", pvAMPSContext );
	}
	AMPS_SetTraceMode( pvAMPSContext, AMPS_TRACE_MODE_BOTH );
	AMPS_SetTraceLevel( pvAMPSContext, 31 );
	AMPS_EnableLogging( pvAMPSContext, "logfile.txt" );

	AMPS_EvtSysSetEvtHandleValidationCallback( pvAMPSContext, NULL );
	if( NULL == AMPS_SchedInit( pvAMPSContext, APP_INTERNAL_EVENT_MAX, APP_EXTERNAL_EVENT_MAX, NULL, NULL, NULL, NULL ))
	{
		printf( "AMPS_SchedInit failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	nTraceId = AMPS_GetTraceID( pvAMPSContext );
	AMPS_SetTraceForTraceID( pvAMPSContext, nTraceId );

	oSdpParserConfig.hFramework = pvAMPSContext;
	oSdpParserConfig.nTraceId = nTraceId;

	eRetCode = InitSdpParser( &oSdpParserConfig, &hSdpParser );
	if( MS_ERR_NONE == eRetCode )
	{
		oSdpConfig.nTraceId = nTraceId;

		eRetCode = AllocateSdp( &oSdpConfig, &hSdp );
		if( MS_ERR_NONE == eRetCode )
		{
			eRetCode = SdpDecode( hSdpParser, hSdp, (unsigned char*)pcSdpMsg, strlen( pcSdpMsg ) );
			if( MS_ERR_NONE == eRetCode )
			{
				printf( "Decode Successful\n" );
//              eRetCode = SdpGetProtoVersion( hSdpParser, hSdp, &poSDPVersion );
//              eRetCode = SdpRemoveProtoVersion( hSdpParser, hSdp );
//              eRetCode = SdpGetProtoVersion( hSdpParser, hSdp, &poSDPVersion );
//
//              eRetCode = SdpGetOrigin( hSdpParser, hSdp, &poSDPOrigin );
//              eRetCode = SdpGetSessionName( hSdpParser, hSdp, &poSDPSessionName );
//              eRetCode = SdpGetConnection( hSdpParser, hSdp, &poSDPConnection );
//              eRetCode = SdpMediaDescGetMedia( hSdpParser, hSdp, 1, &poSDPMedia );
//
//              eRetCode = SdpGetTime( hSdpParser, hSdp, -1, &poSdpTime );
//              eRetCode = SdpRemoveTime( hSdpParser, hSdp, -1 );
//              eRetCode = SdpRemoveTime( hSdpParser, hSdp, -1 );
//              eRetCode = SdpGetTime( hSdpParser, hSdp, -1, &poSdpTime );

/*
eRetCode = SdpMediaDescGetAttribute( hSdpParser, hSdp, -1, -1, &poSDPAttribute);
eRetCode = SdpMediaDescRemoveAttribute( hSdpParser, hSdp, -1, -1 );
eRetCode = SdpMediaDescRemoveAttribute( hSdpParser, hSdp, -1, -1 );
eRetCode = SdpMediaDescRemoveAttribute( hSdpParser, hSdp, -1, -1 );
eRetCode = SdpMediaDescGetAttribute( hSdpParser, hSdp, -1, -1, &poSDPAttribute);

eRetCode = SdpMediaDescRemoveMedia( hSdpParser, hSdp, 3 );
eRetCode = SdpMediaDescRemoveMedia( hSdpParser, hSdp, 2 );
eRetCode = SdpMediaDescRemoveMedia( hSdpParser, hSdp, -1 );

eRetCode = SdpMediaDescGetMedia( hSdpParser, hSdp, 2, &poSDPMedia );
*/
/*
eRetCode = SdpRemoveConnection( hSdpParser, hSdp );
eRetCode = SdpRemoveConnection( hSdpParser, hSdp );
eRetCode = SdpGetConnection( hSdpParser, hSdp, &poSDPConnection );
eRetCode = SdpMediaDescGetConnection( hSdpParser, hSdp, 0, -1, &poSDPConnection );
eRetCode = SdpMediaDescRemoveConnection( hSdpParser, hSdp, 0, -1 );
eRetCode = SdpMediaDescRemoveConnection( hSdpParser, hSdp, 0, -1 );
eRetCode = SdpMediaDescGetConnection( hSdpParser, hSdp, 0, -1, &poSDPConnection );*/

				SdpEncode( hSdpParser, hSdp, &oEncodedSdp );
				printf( "string after encoding = %s\n", oEncodedSdp.pucData );
				TS_Free( &oEncodedSdp );
			}
		}
	}
//  poSDPParserCtxt = SDP_Init( nTraceId );
//
//  printf( "poSDPParserCtxt = %p\n", poSDPParserCtxt );
//
//  SDP_InitSDPStateMachine( poSDPParserCtxt );
//
//  poSDP = AMPS_Malloc( sizeof (t_SDP));
//  if( NULL == poSDP )
//  {
//      MS_TRACE( SDP_TRACE_ID( poSDPParserCtxt ), AMPS_TRACE_LEVEL_ERROR, "AMPS_Malloc Failed.\n" );
//      return (AMPS_ERROR_FAILURE);
//  }
//  nRetVal = SDP_DecodeSDP( poSDPParserCtxt, poSDP, (unsigned char*)pcSdpMsg, strlen( pcSdpMsg ));
//  if( nRetVal == AMPS_SUCCESS )
//  {
//      MS_TRACE( SDP_TRACE_ID( poSDPParserCtxt ), AMPS_TRACE_LEVEL_INFO, "SDP_DecodeSDP Successul.\n" );
//  }
//  else
//  {
//      MS_TRACE( SDP_TRACE_ID( poSDPParserCtxt ), AMPS_TRACE_LEVEL_ERROR, "SDP_DecodeSDP Failed.\n" );
//  }
//  nRetVal = SDP_GetConnection( poSDPParserCtxt, poSDP, pchIpAddr );
//
//  printf( "IP address = \"%s\"\n", pchIpAddr );
//
//  strcpy( pchIpAddr, "127.0.0.1" );
//
//  nRetVal = SDP_SetConnection( poSDPParserCtxt, poSDP, pchIpAddr );
//
//  nRetVal = SDP_GetConnection( poSDPParserCtxt, poSDP, pchIpAddr );
//
//  printf( "IP address = \"%s\"\n", pchIpAddr );
//
//  nRetVal = SDP_Encode( poSDPParserCtxt, poSDP, pchEncodedSdp, &nLength );
//
//  pchEncodedSdp[nLength] = '\0';

	AMPS_Main( pvAMPSContext );

//	printf( "original sdp = \n%s", pcSdpMsg );
//	printf( "\n\nencoded sdp = \n%s", pchEncodedSdp );

	AMPS_Cleanup( pvAMPSContext );

	return (0);
}

