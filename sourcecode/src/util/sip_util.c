#include "util/sip_util.h"

e_MsRetCode SU_AddContentInMsg( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poContentType_i, t_String* poContentBody_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hSipStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poContentType_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poContentBody_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		if( 0 < poContentType_i->unLength && 0 < poContentBody_i->unLength )
		{
			TS_Print( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Adding Content = ", poContentBody_i );
			TS_Print( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Content sub-type = ", poContentType_i );

			t_SIPMsgPayload oSipMsgPayload;
			memset( &oSipMsgPayload, 0, sizeof (t_SIPMsgPayload));

			oSipMsgPayload.oSIPContentType.oType.poString = (unsigned char*)"application";
			oSipMsgPayload.oSIPContentType.oType.unLength = sizeof ("application")-1;

			oSipMsgPayload.oSIPContentType.oSubtype.poString = poContentType_i->pucData;
			oSipMsgPayload.oSIPContentType.oSubtype.unLength = poContentType_i->unLength;

			oSipMsgPayload.oSIPStringForContent.poString = poContentBody_i->pucData;
			oSipMsgPayload.oSIPStringForContent.unLength = poContentBody_i->unLength;
			if( AMPS_SUCCESS != SIP_SetSIPMsgPayload( hSipStack_i, hSipMsg_i, &oSipMsgPayload ) )
			{
				eRetCode = MS_ERR_SIP_STACK;
			}
		}
	}

	POST_CONDITION
	{
	}
	/* - */

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode SU_SetRequestLineMethod( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poMethod_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hSipStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poMethod_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nRetVal = 0;
		t_SIPTag oSipTag;

		memset( &oSipTag, 0, sizeof (t_SIPTag));

		oSipTag.oSIPTagType = SIP_TAG_RL_METHOD;
		oSipTag.u_SIPTagValue.oSIPStringRLMethod.poString = poMethod_i->pucData;
		oSipTag.u_SIPTagValue.oSIPStringRLMethod.unLength = poMethod_i->unLength;

		nRetVal = SIP_SetTagToMsg( hSipStack_i, hSipMsg_i, &oSipTag );
		if( nRetVal != AMPS_SUCCESS )
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode SU_SetRequestUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poUser_i, t_String* poHost_i, unsigned int unPort_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hSipStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUser_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poHost_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nRetVal = 0;
		t_SIPTag oSipTag;
		t_SIPString oTempString = {NULL, 0};
		t_SIPString oSipUserString = {NULL, 0};
		t_SIPString oSipHostString = {NULL, 0};

		memset( &oSipTag, 0, sizeof (t_SIPTag));

		oSipTag.oSIPTagType = SIP_TAG_RL_REQUEST_URI;

		oSipUserString.poString = (unsigned char*) poUser_i->pucData;
		oSipUserString.unLength = poUser_i->unLength;

		oSipHostString.poString = (unsigned char*) poHost_i->pucData;
		oSipHostString.unLength = poHost_i->unLength;

		nRetVal = SIP_SetURI( hSipStack_i, &oSipTag.u_SIPTagValue.oRURI, &oTempString, &oSipUserString, &oSipHostString, unPort_i );

		nRetVal = SIP_SetTagToMsg( hSipStack_i, hSipMsg_i, &oSipTag );
		if( nRetVal != AMPS_SUCCESS )
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode SU_SetFromUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poUser_i, t_String* poHost_i, unsigned int unPort_i, t_String* poTag_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hSipStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUser_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poHost_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poTag_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nRetVal = 0;
		t_SIPTag oSipTag;
		t_SIPString oTempString = {NULL, 0};
		t_SIPString oSipUserString = {NULL, 0};
		t_SIPString oSipHostString = {NULL, 0};

		memset( &oSipTag, 0, sizeof (t_SIPTag));

		oSipTag.oSIPTagType = SIP_TAG_FROM_URI;

		oSipUserString.poString = (unsigned char*) poUser_i->pucData;
		oSipUserString.unLength = poUser_i->unLength;

		oSipHostString.poString = (unsigned char*) poHost_i->pucData;
		oSipHostString.unLength = poHost_i->unLength;

		nRetVal = SIP_SetURI( hSipStack_i, &oSipTag.u_SIPTagValue.oFromURI, &oTempString, &oSipUserString, &oSipHostString, unPort_i );

		nRetVal = SIP_SetTagToMsg( hSipStack_i, hSipMsg_i, &oSipTag );
		if( nRetVal != AMPS_SUCCESS )
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
		else
		{
			t_SIPString oTagName = {(unsigned char*)"tag", sizeof ("tag")-1};
			t_SIPString oParamTagValue = {NULL, 0};

			oParamTagValue.poString = poTag_i->pucData;
			oParamTagValue.unLength = poTag_i->unLength;

			//Set TO tag
			nRetVal = SIP_SetParamToMsgTag( hSipStack_i, hSipMsg_i, SIP_TAG_FROM_PARAM, oTagName, oParamTagValue );
			if( nRetVal != AMPS_SUCCESS )
			{
				eRetCode = MS_ERR_SIP_STACK;
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode SU_SetToUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poUser_i, t_String* poHost_i, unsigned int unPort_i, t_String* poTag_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hSipStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUser_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poHost_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poTag_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nRetVal = 0;
		t_SIPTag oSipTag;
		t_SIPString oTempString = {NULL, 0};
		t_SIPString oSipUserString = {NULL, 0};
		t_SIPString oSipHostString = {NULL, 0};

		memset( &oSipTag, 0, sizeof (t_SIPTag));

		oSipTag.oSIPTagType = SIP_TAG_TO_URI;

		oSipUserString.poString = (unsigned char*) poUser_i->pucData;
		oSipUserString.unLength = poUser_i->unLength;

		oSipHostString.poString = (unsigned char*) poHost_i->pucData;
		oSipHostString.unLength = poHost_i->unLength;

		nRetVal = SIP_SetURI( hSipStack_i, &oSipTag.u_SIPTagValue.oToURI, &oTempString, &oSipUserString, &oSipHostString, unPort_i );

		nRetVal = SIP_SetTagToMsg( hSipStack_i, hSipMsg_i, &oSipTag );
		if( nRetVal != AMPS_SUCCESS )
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
		else
		{
			t_SIPString oTagName = {(unsigned char*)"tag", sizeof ("tag")-1};
			t_SIPString oParamTagValue = {NULL, 0};

			oParamTagValue.poString = poTag_i->pucData;
			oParamTagValue.unLength = poTag_i->unLength;

			//Set TO tag
			nRetVal = SIP_SetParamToMsgTag( hSipStack_i, hSipMsg_i, SIP_TAG_TO_PARAM, oTagName, oParamTagValue );
			if( nRetVal != AMPS_SUCCESS )
			{
				eRetCode = MS_ERR_SIP_STACK;
			}
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode SU_SetContactUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_String* poUser_i, t_String* poHost_i, unsigned int unPort_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hSipStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUser_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poHost_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		int nRetVal = 0;
		t_SIPTag oSipTag;
		t_SIPString oTempString = {NULL, 0};
		t_SIPString oSipUserString = {NULL, 0};
		t_SIPString oSipHostString = {NULL, 0};

		memset( &oSipTag, 0, sizeof (t_SIPTag));

		oSipTag.oSIPTagType = SIP_TAG_CONTACT;

		oSipUserString.poString = (unsigned char*) poUser_i->pucData;
		oSipUserString.unLength = poUser_i->unLength;

		oSipHostString.poString = (unsigned char*) poHost_i->pucData;
		oSipHostString.unLength = poHost_i->unLength;

		nRetVal = SIP_SetURI( hSipStack_i, &oSipTag.u_SIPTagValue.oSIPContact.oSIPURL, &oTempString, &oSipUserString, &oSipHostString, unPort_i );

		nRetVal = SIP_AddListTagToMsg( hSipStack_i, hSipMsg_i, &oSipTag, 0 );
		if( nRetVal != AMPS_SUCCESS )
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode SU_GetContactUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_Uri* poRemoteContact_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hSipStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poRemoteContact_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_SIPTag oSipTag;

		oSipTag.oSIPTagType = SIP_TAG_CONTACT;

		memset( &oSipTag.u_SIPTagValue.oSIPContact, 0, sizeof (t_SIPContact));
		if( AMPS_SUCCESS == SIP_GetListTagFromMsg( hSipStack_i, hSipMsg_i, &oSipTag, 0 ))
		{
			//get user part
			if( NULL != oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.puchUser )
			{
				TS_SetN((char*)oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.puchUser, oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.unUserLength, &poRemoteContact_i->oUser );
			}
			if( NULL != oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.puchHost )
			{
				TS_SetN((char*)oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.puchHost, oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.unHostLength, &poRemoteContact_i->oHost );
			}
			poRemoteContact_i->unPort = oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.unPort;
		}
		else
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

e_MsRetCode SU_GetFromUri( int nTraceId_i, SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, t_UriInfo* poUriInfo_i )
{
	e_MsRetCode eRetCode = MS_ERR_NONE;

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Entering\n" );

	//PRE_CONDITION
	{
		ms_assert_return( NULL != hSipStack_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != hSipMsg_i, eRetCode = MS_ERR_PRE_COND_FAILED );
		ms_assert_return( NULL != poUriInfo_i, eRetCode = MS_ERR_PRE_COND_FAILED );
	}

	{
		t_SIPTag oSipTag;

		memset( &oSipTag, 0, sizeof (t_SIPTag));
		oSipTag.oSIPTagType = SIP_TAG_FROM_URI;
		if( AMPS_SUCCESS == SIP_GetTagFromMsg( hSipStack_i, hSipMsg_i, &oSipTag ))
		{
			TS_SetN((char*)&oSipTag.u_SIPTagValue.oFromURI.puchUser, oSipTag.u_SIPTagValue.oFromURI.unUserLength, &poUriInfo_i->oUri.oUser );

			TS_SetN((char*)&oSipTag.u_SIPTagValue.oFromURI.puchHost, oSipTag.u_SIPTagValue.oFromURI.unHostLength, &poUriInfo_i->oUri.oHost );

			poUriInfo_i->oUri.unPort = oSipTag.u_SIPTagValue.oFromURI.unPort;

			// Get Tag from message
			t_SIPString oTagName = {(unsigned char*)"tag", sizeof ("tag")-1};
			t_SIPString oParamTagValue = {NULL, 0};

			memset( &oSipTag, 0, sizeof (t_SIPTag));
			oSipTag.oSIPTagType = SIP_TAG_FROM_PARAM;
			if( AMPS_SUCCESS == SIP_GetParamFromMsgTag( hSipStack_i, hSipMsg_i, SIP_TAG_FROM_PARAM, &oTagName, &oParamTagValue ))
			{
				TS_SetN((char*)oParamTagValue.poString, oParamTagValue.unLength, &poUriInfo_i->oTag );
			}
			else
			{
				TS_Free( &poUriInfo_i->oUri.oUser );
				TS_Free( &poUriInfo_i->oUri.oHost );
				eRetCode = MS_ERR_SIP_STACK;
			}
		}
		else
		{
			eRetCode = MS_ERR_SIP_STACK;
		}
	}

	POST_CONDITION
	{
	}

	MS_TRACE( nTraceId_i, AMPS_TRACE_LEVEL_INFO, "Leaving with err code = %s\n", get_ms_ret_code_string( eRetCode ) );

	return (eRetCode);
}

void SU_CopyUriToRouteSet( t_SIPRouteSetEntry* poSIPRouteSetEntry_i, t_SIPURL* poSIPUri_i )
{
	memcpy( poSIPRouteSetEntry_i->puchScheme, poSIPUri_i->puchScheme, poSIPUri_i->unSchemeLength );

	memcpy( poSIPRouteSetEntry_i->puchUserName, poSIPUri_i->puchUser, poSIPUri_i->unUserLength );

	memcpy( poSIPRouteSetEntry_i->puchAddress, poSIPUri_i->puchHost, poSIPUri_i->unHostLength );

	poSIPRouteSetEntry_i->unPort = poSIPUri_i->unPort;

	memcpy( poSIPRouteSetEntry_i->puchTransport, poSIPUri_i->oTransport.poString, poSIPUri_i->oTransport.unLength );
}

/*
int set_contact_uri(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcUser_i, char* pcHost_i, unsigned int unPort_i)
{
int nRetVal = 0;
t_SIPTag oSipTag;
t_SIPString oTempString={0,0};
t_SIPString oSipUserString;
t_SIPString oSipHostString;

oSipTag.oSIPTagType = SIP_TAG_CONTACT;

oSipUserString.poString = (unsigned char*) pcUser_i;
oSipUserString.unLength = strlen(pcUser_i);

oSipHostString.poString = (unsigned char*) pcHost_i;
oSipHostString.unLength = strlen(pcHost_i);

memset (&oSipTag.u_SIPTagValue.oSIPContact, 0, sizeof(t_SIPContact));

nRetVal = SIP_SetURI(hSipStack_i, &oSipTag.u_SIPTagValue.oSIPContact.oSIPURL, &oTempString, &oSipUserString, &oSipHostString, unPort_i);

nRetVal = SIP_AddListTagToMsg(hSipStack_i, hSipMsg_i,&oSipTag, 0);

return nRetVal;
}

int get_contact_uri(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i, char* pcUser_o, char* pcHost_o, unsigned int* unPort_o )
{
t_SIPTag oSipTag;

oSipTag.oSIPTagType = SIP_TAG_CONTACT;

memset (&oSipTag.u_SIPTagValue.oSIPContact, 0, sizeof(t_SIPContact));

if (AMPS_SUCCESS == SIP_GetListTagFromMsg(hSipStack_i, hSipMsg_i, &oSipTag, 0))
{
//get uer part
if (NULL != oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.puchUser) {
memcpy(pcUser_o, oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.puchUser, oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.unUserLength );
//strncpy(pcUser_o, (const char*)oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.puchUser, oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.unUserLength );
}

//get host part
if (NULL != oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.puchHost) {
memcpy(pcHost_o, oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.puchHost , oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.unHostLength );
//strncpy((char*) pcHost_o, (const char*)oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.puchHost , oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.unHostLength );
}
else
return -1;

//get port
*unPort_o  = oSipTag.u_SIPTagValue.oSIPContact.oSIPURL.unPort;
}
else
return -1;

return 0;
}

void PBX_CopyURIToRouteSet(t_SIPRouteSetEntry* poSIPRouteSetEntry_i, t_SIPURL* poSIPUri_i)
{
//MS_TRACE(PXY_TRACE_ID(r_poPxyCtxt), AMPS_TRACE_LEVEL_INFO, "Entering.\n");

memcpy(poSIPRouteSetEntry_i->puchScheme, poSIPUri_i->puchScheme, poSIPUri_i->unSchemeLength);

memcpy(poSIPRouteSetEntry_i->puchUserName, poSIPUri_i->puchUser, poSIPUri_i->unUserLength);

memcpy(poSIPRouteSetEntry_i->puchAddress, poSIPUri_i->puchHost, poSIPUri_i->unHostLength);

poSIPRouteSetEntry_i->unPort = poSIPUri_i->unPort;

memcpy(poSIPRouteSetEntry_i->puchTransport, poSIPUri_i->oTransport.poString, poSIPUri_i->oTransport.unLength);

//MS_TRACE(PXY_TRACE_ID(r_poPxyCtxt), AMPS_TRACE_LEVEL_INFO, "Leaving.\n");
}



int duplicate_msg_header(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSrcSipMsg_i, SIP_MSG_HANDLE hDstSipMsg_i, e_SIPTagType oSIPTagType_i)
{
int nRetVal = 0;

SIP_DuplicateTag(hSipStack_i, hSrcSipMsg_i, oSIPTagType_i, hDstSipMsg_i);

return nRetVal;
}

int PBX_IntToString( unsigned char* puchBuffer_i, unsigned long nValue_i, int nBase_i )
{
int nLen = 0;
unsigned char pucReverse[MAX_LENGTH+1]; // plus one for the null
unsigned char* pucTemp;
char cSign = nValue_i < 0;

nValue_i = abs( nValue_i );
pucReverse[MAX_LENGTH] = 0;
pucTemp = pucReverse+MAX_LENGTH;

do
{
*--pucTemp = "0123456789abcdefghijklmnopqrstuvwxyz"[nValue_i % nBase_i];
nValue_i /= nBase_i;
}
while( nValue_i );
if( cSign )
{
*--pucTemp = '-';
}
nLen = strlen((char*)pucTemp );
memcpy( puchBuffer_i, pucTemp, nLen );
return (nLen);
}

int PBX_GenerateRandomString(unsigned char* r_puchRandomString, int r_nRequestedLength)
{
unsigned long nRandomVal =0;
int nOffset = 0;
unsigned char puchIntString[32];
int nIntStringSize = 0;
int nCount = 0;

for (nCount = 0; nCount < 2; nCount++)
{
nRandomVal += (KISS + SWB);

memset(puchIntString, 0, sizeof(puchIntString));
nIntStringSize = PBX_IntToString(puchIntString, nRandomVal, 10);
memcpy(r_puchRandomString+nOffset, puchIntString, nIntStringSize);

nOffset += nIntStringSize;

}
return 0;

}

int PBX_set_from_tag(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i,t_SIPString* oParamTagValue)
{
e_SIPTagType etagType=SIP_TAG_FROM_PARAM;
t_SIPString oTagName;
int nRetVal;
oTagName.poString=(unsigned char*)AMPS_Malloc(strlen("tag")+1);
if (NULL == oTagName.poString)
{
return -1;
}

oTagName.unLength=strlen("tag");
memset (oTagName.poString,0, oTagName.unLength );
memcpy(oTagName.poString,"tag",oTagName.unLength);
//strcpy((char*)oTagName.poString,"tag");// use memcopy instaesd
nRetVal = SIP_SetParamToMsgTag(hSipStack_i, hSipMsg_i, etagType, oTagName, *oParamTagValue);
AMPS_Free (oTagName.poString );
oTagName.poString = NULL;

if (0 == nRetVal)
return 0;
else
return -1;
}
int PBX_set_to_tag(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i,t_SIPString* oParamTagValue)
{
e_SIPTagType etagType=SIP_TAG_TO_PARAM;
t_SIPString oTagName;
oTagName.poString=(unsigned char*)malloc(strlen("tag")+1);
oTagName.unLength=strlen("tag");
memset (oTagName.poString,0, oTagName.unLength );
memcpy(oTagName.poString,"tag",oTagName.unLength);
if(!SIP_SetParamToMsgTag(hSipStack_i, hSipMsg_i, etagType, oTagName, *oParamTagValue))
return 0;
else
return -1;
}

int PBX_get_to_tag(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i,t_SIPString* oParamTagValue)
{
e_SIPTagType tagType=SIP_TAG_TO_PARAM;
t_SIPString oTag;
oTag.poString=(unsigned char*)malloc(strlen("tag")+1);
oTag.unLength=strlen("tag");
memset (oTag.poString,0, oTag.unLength );
memcpy(oTag.poString,"tag",oTag.unLength);
if(!SIP_GetParamFromMsgTag(hSipStack_i,hSipMsg_i,tagType,&oTag,oParamTagValue))
return 0;
return 1;
}

int PBX_get_from_tag(SIP_HANDLE hSipStack_i, SIP_MSG_HANDLE hSipMsg_i,t_SIPString* oParamTagValue)
{
e_SIPTagType tagType=SIP_TAG_FROM_PARAM;
t_SIPString oTag;
oTag.poString=(unsigned char*)malloc(strlen("tag")+1);
oTag.unLength=strlen("tag");
memcpy(oTag.poString,"tag",oTag.unLength);
if(!SIP_GetParamFromMsgTag(hSipStack_i,hSipMsg_i,tagType,&oTag,oParamTagValue))
return 0;
return 1;
}

int PBX_alloc_string(unsigned char** ppucDst_o, unsigned char* pucSrc_i, unsigned int* pnLength_o)
{
int nLen;
if(pucSrc_i!=NULL)
{
nLen=strlen((char*)pucSrc_i)+1;
*ppucDst_o=AMPS_Malloc(nLen);
if(*ppucDst_o!=NULL)
{
memset(*ppucDst_o,0,nLen);
memcpy(*ppucDst_o,pucSrc_i,nLen-1);
*pnLength_o=nLen-1;
return 0;
}


}

return -1;

}*/

