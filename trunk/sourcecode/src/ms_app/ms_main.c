#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

#include "fw_api.h"
#include "sdp_api.h"
#include "sipstack/SIP_API.h"
#include "call_leg/call_leg_api.h"
#include "ms_main.h"
#include "ms_mm.h"
#include "call_leg_handler.h"
#include "ms_sm_handler.h"
#include "ms_events.h"
#include "mscml_sm/mscml_sm_api.h"
#include "dnsresolver/DNSResolver_Core.h"
#include "ms_ret_code.h"
#include "snmp/snmp_comm.h"

static void daemonize( void )
{
	pid_t pid, sid;
	/* already a daemon */
	if( getppid() == 1 )
	{
		return;
	}
	/* Fork off the parent process */
	pid = fork();
	if( pid < 0 )
	{
		exit( 1 );
	}
	/* If we got a good PID, then we can exit the parent process. */
	if( pid > 0 )
	{
		exit( 0 );
	}
	/* At this point we are executing as the child process */

	/* Change the file mode mask */
	umask( 0 );

	/* Create a new SID for the child process */
	sid = setsid();
	if( sid < 0 )
	{
		exit( 1 );
	}
	/* Change the current working directory.  This prevents the current
	directory from being locked; hence not being able to remove it. */
	if((chdir( "/" )) < 0 )
	{
		exit( 1 );
	}
	/* Redirect standard files to /dev/null */
	freopen( "/dev/null", "r", stdin );
	freopen( "/dev/null", "w", stdout );
	freopen( "/dev/null", "w", stderr );
}

int main( int nArgc_i, char* ppchArgv[] )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;
	unsigned int unSipPort = 5060;
	int nRetVal = 0;
	SIP_HANDLE hSipStack = NULL;
	int nClTraceId;
	AMPS_SCHED_HANDLE hScheduler;
	h_CallLegStack hClStack;
	t_CallLegStackConfig oClStackConfig;

	h_SnmpComm hSnmpComm = NULL;
	t_SnmpCommConfig oSnmpCommConfig;

	t_MsCtx* poMsCtx = NULL;

	h_Sdp hLocalSdp = NULL;
	t_SdpConfig oSdpConfig;

	unsigned int unMaxFd = 8000;

	void* pvFwCtx = NULL;
	unsigned int unFwPort = 5000;
	char* pcTraceFile = NULL;
	int nTraceLevel = 0;

	xmlSchemaParserCtxtPtr poSchemaParserCtx = NULL;
	xmlSchemaPtr poSchema = NULL;

	char* pcLocalSdpFile = NULL;
	char* pcLocalSdpDefFile = NULL;
	char* pcXsdFile = NULL;
	char* pcUserName = NULL;

	char* pcDomainName = NULL;
	char* pcLocalIp = NULL;
	int nLocalPort = 7000;
	int nRemotePort = 7002;
	char* pcRemoteIp = NULL;
	AMPS_BOOL bMakeDaemon = AMPS_FALSE;

	char* pchConfigFile = DATADIR "config.xml";
	char* pchSoundDir = DATADIR;

	int nJitterBufferMs;
	int nSilenceThresholdDb;

	t_SIPStackConfiguration oSipConfig;
	if( nArgc_i == 2 )
	{
		pchConfigFile = ppchArgv[1];
	}
	else if( nArgc_i > 2 )
	{
		printf( "usage: %s [config_file]\n", ppchArgv[0]  );
		printf( "example: %s %s/config.xml\n", ppchArgv[0], DATADIR );
		exit( -1 );
	}
	printf( "Media Server Information\n" );
	printf( "Version = %s\n", PACKAGE_VERSION );
	printf( "Configuration File = %s\n", pchConfigFile );
	printf( "Compilation Time = %s\n", __TIME__ );
	printf( "Compilation Date = %s\n", __DATE__ );
	//printf("Configure arguments = %s\n", CONFIGURE_ARGUMENTS);

	xmlDocPtr poDoc = NULL;
	{
		xmlNodePtr poNode = NULL;
		xmlChar* poProp = NULL;
		poDoc = xmlParseFile( pchConfigFile );
		if( NULL != poDoc )
		{
			poNode = xmlDocGetRootElement( poDoc );
			if( poNode != NULL )
			{
				// Search "request" node
				for( poNode = poNode->xmlChildrenNode; poNode != NULL; poNode = poNode->next )
				{
					if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"ms" ))
					{
						poProp = xmlGetProp( poNode, (xmlChar*)"username" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"ms" );
						}
						pcUserName = strdup((char*)poProp );

						xmlFree( poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"silence_threshold_db" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"0" );
						}
						nSilenceThresholdDb = atoi((char*)poProp );

						xmlFree( poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"jitter_buffer_ms" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"1000" );
						}
						nJitterBufferMs = atoi((char*)poProp );

						xmlFree( poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"local_sdp_file" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"local_sdp" );
						}
						pcLocalSdpFile = strdup((char*)poProp );

						xmlFree( poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"xsd_file" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"mscml.xsd" );
						}
						pcXsdFile = strdup((char*)poProp );

						xmlFree( poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"sound_dir" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)DATADIR );
						}
						pchSoundDir = strdup((char*)poProp );

						xmlFree( poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"maxfd" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"8000" );
						}
						unMaxFd = atoi((char*)poProp );

						xmlFree( poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"daemon" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"no" );
						}
						if( 0 == xmlStrcmp( poProp, "yes" ) )
						{
							bMakeDaemon = AMPS_TRUE;
						}
						else
						{
							bMakeDaemon = AMPS_FALSE;
						}
						xmlFree( poProp );
					}
					else if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"sip" ))
					{
						poProp = xmlGetProp( poNode, (xmlChar*)"port" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"5060" );
						}
						unSipPort = atoi((char*)poProp );

						xmlFree( poProp );
					}
					else if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"framework" ))
					{
						poProp = xmlGetProp( poNode, (xmlChar*)"port" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"5000" );
						}
						unFwPort = atoi((char*)poProp );

						xmlFree( poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"tracefile_prefix" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"." );
						}
						pcTraceFile = strdup((char*)poProp );

						xmlFree( poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"tracelevel" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"31" );
						}
						nTraceLevel = atoi((char*)poProp );

						xmlFree( poProp );
					}
					else if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"system" ))
					{
						poProp = xmlGetProp( poNode, (xmlChar*)"domainname" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"" );
						}
						pcDomainName = strdup((char*)poProp );

						xmlFree( poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"local_ip" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"" );
						}
						pcLocalIp = strdup((char*)poProp );

						xmlFree( poProp );
					}
					else if( 0 == xmlStrcasecmp( poNode->name, (const xmlChar*)"snmp" ))
					{
						poProp = xmlGetProp( poNode, (xmlChar*)"local_port" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"7000" );
						}
						nLocalPort = atoi((char*)poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"remote_port" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"7002" );
						}
						nRemotePort = atoi((char*)poProp );

						poProp = xmlGetProp( poNode, (xmlChar*)"remote_ip" );
						if( NULL == poProp )
						{
							poProp = xmlStrdup((xmlChar*)"" );
						}
						pcRemoteIp = strdup((char*)poProp );

						xmlFree( poProp );
					}
				}
			}
			else
			{
				printf( "mediaserver configuration not found\n" );
				exit( -1 );
			}
		}
		else
		{
			printf( "Error parsing config file = %s\n", pchConfigFile );
			exit( -1 );
		}
	}
	if( bMakeDaemon == AMPS_TRUE )
	{
		daemonize();
	}
	if( 0 != unMaxFd )
	{
		int ret;
		struct rlimit rl;

		ret = getrlimit( RLIMIT_NOFILE, &rl );
		if( ret != 0 )
		{
			return (-1);
		}
		printf( "Limit was %d (max %d), setting to %d\n", rl.rlim_cur, rl.rlim_max, unMaxFd );

		rl.rlim_cur = rl.rlim_max = unMaxFd;

		ret = setrlimit( RLIMIT_NOFILE, &rl );
		if( ret != 0 )
		{
			printf( "Unable to set open file limit.\n" "(setrlimit(RLIMIT_NOFILE, &rl) failed)\n" "(%d, %s)", errno, strerror( errno ));
			return (-1);
		}
	}
	pvFwCtx = FW_Init( AMPS_TIMER_RESOLUTION_20MS, 1, pcTraceFile, AMPS_PORT_POOL_SIZE_10, unFwPort );
	if( NULL == pvFwCtx )
	{
		printf( "AMPS_Init: AMPS_Init is failed.\n" );
		return (AMPS_ERROR_FAILURE);
	}
	AMPS_SetTraceLevel( pvFwCtx, nTraceLevel );

	nClTraceId = AMPS_GetTraceID( pvFwCtx );
	AMPS_SetTraceForTraceID( pvFwCtx, nClTraceId );

//  FW_SetTraceForAMPS(pvFwCtx);
//  FW_SetTraceForSched(pvFwCtx);
//  FW_SetTraceForSAPI(pvFwCtx);

	AMPS_SetTraceMode( pvFwCtx, AMPS_TRACE_MODE_BOTH );
	AMPS_EvtSysSetEvtHandleValidationCallback( pvFwCtx, MS_EventHandleValidationCallback );

	hScheduler = AMPS_SchedInit( pvFwCtx, 10, 10, NULL, NULL, NULL, NULL );

	int nTracId = AMPS_GetTraceID( pvFwCtx );

	eRetCode = MS_InitializeMsCtx( pvFwCtx, nTracId, &poMsCtx );
	ms_assert( MS_ERR_NONE == eRetCode );

	poMsCtx->poDoc = poDoc;
	TS_Set( pcUserName, &poMsCtx->oLocalUri.oUser );
	TS_Set( pcDomainName, &poMsCtx->oLocalUri.oHost );

	{
		t_AMPSDList* poIfList = NULL;
		t_AMPSSList* poIfNode = NULL;

		FW_DListInit( &poIfList );

		nRetVal = FW_GetLocalIfaceAddress( pvFwCtx, poIfList );
		t_AMPSIfaceData* poIfData = NULL;
		if( poIfList->uchCount > 0 )
		{
			for( poIfNode = poIfList->poAMPSSListHead; poIfNode != NULL; poIfNode = poIfNode->poAMPSSListNext )
			{
				poIfData = (t_AMPSIfaceData*)poIfNode->pvData;
				if( 0 == strcasecmp((char*)poIfData->puchIfaceAddress, pcLocalIp ) )
				{
					printf( "Using Local IP address = %s\n", poIfData->puchIfaceAddress );
					TS_Set( (char*)poIfData->puchIfaceAddress, &poMsCtx->oLocalIp );
					break;
				}
			}
			// if local ip does not match
			if( NULL == poIfNode )
			{
				poIfData = poIfList->poAMPSSListHead->pvData;

				printf( "local IP does not match the IP in configuation file\n" );
				printf( "Using the first IP address as local IP= %s\n", poIfData->puchIfaceAddress );

				TS_Set( (char*)poIfData->puchIfaceAddress, &poMsCtx->oLocalIp );
			}
		}
		else
		{
			TS_Set( pcLocalIp, &poMsCtx->oLocalIp );
		}
		FW_DListFree( &poIfList, RemoveIfaceDataCb );
	}

	poMsCtx->oLocalUri.unPort = unSipPort;
	poMsCtx->nJitterBufferMs = nJitterBufferMs;
	poMsCtx->nSilenceThresholdDb = nSilenceThresholdDb;
	poMsCtx->pchSoundDir = pchSoundDir;

	{
		poSchemaParserCtx = xmlSchemaNewParserCtxt( pcXsdFile );
		if( NULL == poSchemaParserCtx )
		{
			printf( "xmlSchemaNewParserCtxt ( \"%s\" ) failed", pcXsdFile );
			exit( -1 );
		}
		poSchema = xmlSchemaParse( poSchemaParserCtx );
		if( NULL == poSchema )
		{
			printf( "xmlSchemaParse() failed\n" );
			exit( -1 );
		}
		poMsCtx->poMscmlValidCtx = xmlSchemaNewValidCtxt( poSchema );
		if( NULL == poMsCtx->poMscmlValidCtx )
		{
			printf( "xmlSchemaNewValidCtxt() failed\n" );
			exit( -1 );
		}
	}
	// Initialize hLocalSdp
	oSdpConfig.nTraceId = poMsCtx->nTraceId;
	if( MS_ERR_NONE != AllocateSdp( &oSdpConfig, &hLocalSdp ) )
	{
		printf( "Error allocating local sdp\n" );
		exit( -1 );
	}
	else
	{
		FILE* poLocalSdpFile = NULL;
		// Read SDP from file
		poLocalSdpFile = fopen( pcLocalSdpFile, "rb" );
		if( NULL == poLocalSdpFile )
		{
			printf( "error opening local sdp file in configuration: %s\n", pcLocalSdpFile );

			pcLocalSdpDefFile = FW_Malloc( strlen( DATADIR ) + strlen( "/local_sdp" ) + 1 );

			strcpy( pcLocalSdpDefFile, DATADIR );
			strcat( pcLocalSdpDefFile, "/local_sdp" );

			poLocalSdpFile = fopen( pcLocalSdpDefFile, "rb" );
			if( NULL == poLocalSdpFile )
			{
				printf( "error opening local sdp file from default path: %s\n", pcLocalSdpDefFile );

				exit( -1 );
			}
			FW_Free( pcLocalSdpDefFile );
		}
		if( NULL != poLocalSdpFile )
		{
			int nSize = 0;
			char pchSdpBuffer[1000] = "";

			fseek( poLocalSdpFile, 0, SEEK_END );

			nSize = ftell( poLocalSdpFile );

			fseek( poLocalSdpFile, 0, SEEK_SET );

			printf( "file size = %d bytes\n", nSize );

			ms_assert( nSize < 1000 );

			nRetVal = fread( pchSdpBuffer, nSize, 1, poLocalSdpFile );
			if( MS_ERR_NONE != SdpDecode( poMsCtx->hSdpParser, hLocalSdp, (unsigned char*)pchSdpBuffer, nSize ) )
			{
				printf( "error decoding local sdp\n" );
				exit( -1 );
			}
			// set local sdp
			MS_SetLocalSdp( poMsCtx, hLocalSdp );
		}
	}
	// Initialize hSnmpComm
	oSnmpCommConfig.nTraceId = poMsCtx->nTraceId;
	strcpy( oSnmpCommConfig.pcLocalIp, poMsCtx->oLocalIp.pucData );
	oSnmpCommConfig.nLocalPort = nLocalPort;
	oSnmpCommConfig.nRemotePort = nRemotePort;
	strcpy( oSnmpCommConfig.pcRemoteIp, pcRemoteIp );
	oSnmpCommConfig.poMsCtx = poMsCtx;
	oSnmpCommConfig.pvFrameworkCtx = poMsCtx->pvFrameworkCtx;

	eRetCode = SNMP_Init( &oSnmpCommConfig, &hSnmpComm );
	ms_assert( MS_ERR_NONE == eRetCode );
	poMsCtx->hSnmpComm = hSnmpComm;

	// Initialize SIP Stack
	oSipConfig.bTraceEnable = 1;
	oSipConfig.unTCPPort = unSipPort;
	oSipConfig.unUDPPort = unSipPort;
	strncpy( oSipConfig.pchIPAddress, poMsCtx->oLocalIp.pucData, poMsCtx->oLocalIp.unLength );
	oSipConfig.pchIPAddress[poMsCtx->oLocalIp.unLength] = '\0';
	oSipConfig.nTraceModuleSIPParser = 0;
	oSipConfig.nTraceModuleTxn = 0;
	oSipConfig.nTraceModuleTpt = 0;
	oSipConfig.nLogMode = AMPS_TRACE_MODE_BOTH;
	strcpy( oSipConfig.puchLogFileName, "sip_logfile.txt" );
	//oSipConfig.nNumberOfRoutes = 1;
	oSipConfig.nNumberOfDomains = 1;
	oSipConfig.nCurrentSIPDomainIndex = 0;
	strcpy( oSipConfig.oSIPStackDomains[0].puchDomainName, pcDomainName );
	oSipConfig.nTxTimerValue = 500;
	oSipConfig.bTunnelingEnabled = AMPS_FALSE;
	oSipConfig.unTunnelingUDPPort = 10000;
	oSipConfig.nTimerMValInMilliSec = 0;

	//oSipConfig.nMemMgrSize = 128;

	oClStackConfig.pvFrameworkCtx = pvFwCtx;
	oClStackConfig.nTraceId     = nClTraceId;
	oClStackConfig.hSipStack    = hSipStack;
	oClStackConfig.pfnNewCallLegCb  = MS_NewCallLeg;
	oClStackConfig.hAppCtx = poMsCtx;

	cl_initstack( &oClStackConfig, &hClStack );

	hSipStack  = SIP_Init( pvFwCtx, hClStack, &oSipConfig, SIP_STACK_MODE_UA );

	cl_set_sip_handle( hClStack, hSipStack );
	MS_SetSipStack( poMsCtx, hSipStack );

	{
		t_SIPConnObject oSIPConnObject;

		memset( &oSIPConnObject, 0, sizeof (t_SIPConnObject));

		strncpy( oSIPConnObject.pchIPAddr, poMsCtx->oLocalIp.pucData, poMsCtx->oLocalIp.unLength );
		oSIPConnObject.pchIPAddr[poMsCtx->oLocalIp.unLength] = '\0';

		oSIPConnObject.nTptType = AMPS_CONNECTION_TYPE_UDP;
		oSIPConnObject.nPort = unSipPort;
		if( AMPS_SUCCESS != SIP_OpenConnection( hSipStack, oSIPConnObject ))
		{
			printf( "SIP_OpenConnection Failed. \n" );
			return (AMPS_ERROR_FAILURE);
		}
	}

	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_INVITE,   cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_ACK,      cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_OPTIONS,  cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_BYE,      cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_CANCEL,   cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_REGISTER,     cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_INFO,     cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_PRACK,    cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_SUBSCRIBE,    cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_NOTIFY,   cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_UPDATE,   cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_MESSAGE,  cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_REFER,    cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_PUBLISH,  cl_handle_sip_request );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_UNKNOWN,  cl_handle_sip_request );

	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_1XX_RESPONSE, cl_handle_sip_response );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_2XX_RESPONSE, cl_handle_sip_response );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_3XX_RESPONSE, cl_handle_sip_response );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_4XX_RESPONSE, cl_handle_sip_response );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_5XX_RESPONSE, cl_handle_sip_response );
	SIP_RegMsgHandlerCallback( hSipStack, SIP_MSG_TYPE_6XX_RESPONSE, cl_handle_sip_response );

	SIP_RegErrorCallback( hSipStack, &nClTraceId, SIP_ERR_CODE_NETWORK_FAILURE,  cl_handle_sip_error );
	SIP_RegErrorCallback( hSipStack, &nClTraceId, SIP_ERR_CODE_MSG_FAILURE,      cl_handle_sip_error );
	SIP_RegErrorCallback( hSipStack, &nClTraceId, SIP_ERR_CODE_TIMER_B_FIRED,    cl_handle_sip_error );
	SIP_RegErrorCallback( hSipStack, &nClTraceId, SIP_ERR_CODE_TIMER_C_FIRED,    cl_handle_sip_error );
	SIP_RegErrorCallback( hSipStack, &nClTraceId, SIP_ERR_CODE_TIMER_F_FIRED,    cl_handle_sip_error );
	SIP_RegErrorCallback( hSipStack, &nClTraceId, SIP_ERR_CODE_TIMER_H_FIRED,    cl_handle_sip_error );
	SIP_RegErrorCallback( hSipStack, &nClTraceId, SIP_ERR_CODE_TIMER_I_FIRED,    cl_handle_sip_error );
	SIP_RegErrorCallback( hSipStack, &nClTraceId, SIP_ERR_CODE_TIMER_J_FIRED,    cl_handle_sip_error );

	SIP_RegMemoryCallback( hSipStack, NULL, MS_Malloc, MS_Free );

	t_CMDNSResolver oCMDNSResolver;
	void* pvDNSResContext = NULL;
	{
		oCMDNSResolver.bTraceEnable = 1;
		pvDNSResContext = DNSResolver_ModuleInit( pvFwCtx, &oCMDNSResolver );
		if( NULL == pvDNSResContext )
		{
			printf( "DNS_Init: DNS_Init is failed.\n" );
			exit( -1 );
		}
	}
	if( AMPS_SUCCESS != FW_EvtSysRegExtEvt( pvFwCtx, SS_EVENT_3263_LOOKUP_RES, cl_HandleDNSLookupResponse, NULL ))
	{
		printf( "AMPS_EvtSysRegExtEvt(SS_EVENT_3263_LOOKUP_RES) failed.\n" );
		exit( -1 );
	}
	nRetVal = FW_EvtSysRegIntEvt( pvFwCtx, PLAYER_REQ, MS_SM_Handler, poMsCtx );
	if( AMPS_SUCCESS != nRetVal )
	{
		printf( "PLAYER_REQ Handler registeration failed.\n" );
		exit( -1 );
	}
	nRetVal = FW_EvtSysRegIntEvt( pvFwCtx, MSCML_PLAY_RES, MS_SM_Handler, poMsCtx );
	if( AMPS_SUCCESS != nRetVal )
	{
		printf( "MSCML_PLAY_RES Handler registeration failed.\n" );
		exit( -1 );
	}
	MOP_RegisterEvent( pvFwCtx, poMsCtx );

	AMPS_Main( pvFwCtx );
	if( AMPS_SUCCESS != FW_EvtSysCancelExtEvt( pvFwCtx, SS_EVENT_3263_LOOKUP_RES, cl_HandleDNSLookupResponse ))
	{
		printf( "FW_EvtSysCancelExtEvt(SS_EVENT_3263_LOOKUP_RES) failed.\n" );
	}
	nRetVal = FW_EvtSysCancelIntEvt( pvFwCtx, PLAYER_REQ, MS_SM_Handler );
	if( AMPS_SUCCESS != nRetVal )
	{
		printf( "PLAYER_REQ Handler cancellation failed.\n" );
	}
	nRetVal = FW_EvtSysCancelIntEvt( pvFwCtx, MSCML_PLAY_RES, MS_SM_Handler );
	if( AMPS_SUCCESS != nRetVal )
	{
		printf( "MSCML_PLAY_RES Handler cancellation failed.\n" );
	}
	MOP_UnregisterEvent( pvFwCtx );

	DNSResolver_ModuleCleanup( pvFwCtx, pvDNSResContext );
	SIP_Cleanup( pvFwCtx, hSipStack );

	eRetCode = SNMP_Destroy( hSnmpComm );
	ms_assert( MS_ERR_NONE == eRetCode );

	errno = 0;
	if( -1 == xmlSaveFile( pchConfigFile, poMsCtx->poDoc ))
	{
		MS_TRACE( poMsCtx->nTraceId, AMPS_TRACE_LEVEL_ERROR, "Error while saving %s: %s\n", pchConfigFile, strerror( errno ) );
	}
	eRetCode = MS_DestroyMsCtx( pvFwCtx, poMsCtx );
	ms_assert( MS_ERR_NONE == eRetCode );

	cl_closestack( &hClStack );

	AMPS_SchedCleanup( pvFwCtx, hScheduler );
	AMPS_Cleanup( pvFwCtx );

	FW_Free( pcLocalSdpFile );
	FW_Free( pcXsdFile );
	FW_Free( pcUserName );
	FW_Free( pcTraceFile );
	FW_Free( pcDomainName );
	FW_Free( pcLocalIp );
	FW_Free( pcRemoteIp );

	xmlSchemaFreeParserCtxt( poSchemaParserCtx );
	xmlSchemaFree( poSchema );

	xmlCleanupParser();

	POST_CONDITION
	{
	}

	return (0);
}

e_MsRetCode MS_InitializeMsCtx( void* pvFwCtx, int nTraceId, t_MsCtx** ppMsCtx_o )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != ppMsCtx_o, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_MsCtx* poMsCtx = NULL;
		t_SdpParserConfig oSdpParserConfig;

		poMsCtx = MS_Malloc( NULL, sizeof (t_MsCtx));

		ms_assert( NULL != poMsCtx );

		// initialize MS Context
		memset( poMsCtx, 0, sizeof (t_MsCtx));
		AMPS_DListInit( &poMsCtx->poConfCtxList );
		poMsCtx->pvFrameworkCtx = pvFwCtx;
		poMsCtx->nTraceId = nTraceId;
		AMPS_SetTraceForTraceID( pvFwCtx, poMsCtx->nTraceId );
		FW_DListInit( &poMsCtx->poUserCtrlCtxList );

		poMsCtx->oLocalUri.unPort = 5060;
		poMsCtx->nJitterBufferMs = 100;
		poMsCtx->nSilenceThresholdDb = 0;

		int pnErrCodeVal[SDP_ERR_MAX] = {
			MS_ERR_NONE,
			MS_ERR_PRE_COND_FAILED,
			MS_ERR_POST_COND_FAILED,
			MS_ERR_MISSING_FIELD,
			MS_ERR_INSUFFICIENT_MEMORY,
			MS_ERR_INVALID_OPERATION,
			MS_ERR_INVALID_INPUT,
			MS_ERR_FRAMEWORK,
			MS_ERR_END_OF_LIST,
			MS_ERR_DECODE,
			MS_ERR_ENCODE,
			MS_ERR_NOT_IMPLEMENTED
		};

		InitSdp( pnErrCodeVal );

		int pnSdpOamErrCodeVal[SOA_ERR_MAX] = {
			MS_ERR_NONE,
			MS_ERR_PRE_COND_FAILED,
			MS_ERR_POST_COND_FAILED,
			MS_ERR_MISSING_FIELD,
			MS_ERR_INVALID_PACKET,
			MS_ERR_INSUFFICIENT_MEMORY,
			MS_ERR_FRAMEWORK,
			MS_ERR_MATCHED
		};

		InitSdpOam(pnSdpOamErrCodeVal);

		oSdpParserConfig.hFramework = pvFwCtx;
		oSdpParserConfig.nTraceId = poMsCtx->nTraceId;
		if( MS_ERR_NONE != InitSdpParser( &oSdpParserConfig, &poMsCtx->hSdpParser ) )
		{
			MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_ERROR, "Error allocating sdp parser\n" );
			eRetCode = MS_ERR_INTERNAL;
		}
		*ppMsCtx_o = poMsCtx;
	}

	POST_CONDITION
	{
		if( eRetCode == MS_ERR_NONE )
		{
			ms_assert( NULL != *ppMsCtx_o );
		}
	}
	/* - */

	MS_TRACE( nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode MS_DestroyMsCtx( void* pvFwCtx, t_MsCtx* poMsCtx_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	PRE_CONDITION
	{
		ms_assert_return( NULL != poMsCtx_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		xmlFreeDoc( poMsCtx_i->poDoc );

		TS_Free( &poMsCtx_i->oLocalUri.oUser );
		TS_Free( &poMsCtx_i->oLocalUri.oHost );
		TS_Free( &poMsCtx_i->oLocalIp );

		FW_Free( poMsCtx_i->pchSoundDir );

		DeallocateSdp( &poMsCtx_i->hLocalSdp );
		DestroySdpParser( &poMsCtx_i->hSdpParser );

		t_AMPSSList* poNode = NULL;

		// Delete user ctrl ctx (non-conf)
		for( poNode = poMsCtx_i->poUserCtrlCtxList->poAMPSSListHead; poNode != NULL; poNode = poNode->poAMPSSListNext )
		{
			MS_DestroyUserCtrlCtx( poMsCtx_i, (t_MsUserCtrlCtx*)poNode->pvData );
			FW_Free( poNode->pvData );
		}
		FW_DListFree( &poMsCtx_i->poUserCtrlCtxList, NULL );

		// Delete conf ctrl ctx
		for( poNode = poMsCtx_i->poConfCtxList->poAMPSSListHead; poNode != NULL; poNode = poNode->poAMPSSListNext )
		{
			MS_DestroyConfCtx( poMsCtx_i, (t_MsConfCtx*)poNode->pvData );
		}
		FW_DListFree( &poMsCtx_i->poConfCtxList, NULL );

		xmlSchemaFreeValidCtxt( poMsCtx_i->poMscmlValidCtx );

		FW_Free( poMsCtx_i );
	}

	POST_CONDITION
	{
	}

	MS_TRACE( poMsCtx_i->nTraceId, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

int RemoveIfaceDataCb( void** ppvData_io )
{
	FW_Free( *ppvData_io );

	*ppvData_io = NULL;
}

int MS_EventHandleValidationCallback( void* r_pvContext, int r_nEventID, void* r_pvEventPayload )
{
	return (AMPS_SUCCESS);
}

