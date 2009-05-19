/*! @file sdp_oam_api.h
*
* @ingroup SdpOam
*
* @brief The file contains defines for SDP offer answer model API.
*
* @author Razi-ud-Din Chaudhry
*
*/

/*! @defgroup SdpOam SDP Offer Answer Module
*
* @brief This module consist of functionality for generation of answer for the given SDP offer.
*
* @author Razi-ud-Din Chaudhry
*
* @{
*
*/

#ifndef _SDP_OAM_API_H_
#define _SDP_OAM_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tstring.h"
#include "sdp_api.h"

/*! @enum e_SdpOamRetCode
*@brief This is list of all return codes that SDP OAM api returns
*/
typedef enum
{
	SOA_ERR_NONE = 0,       /*!< No error */
	SOA_ERR_PRE_COND_FAILED = 1,    /*!< Pre-condition of function has failed */
	SOA_ERR_POST_COND_FAILED = 2,   /*!< Post-condition of function has failed */
	SOA_ERR_MISSING_FIELD = 3,  /*!< A field is missing */
	SOA_ERR_INVALID_PACKET = 4, /*!< Input is invalid */
	SOA_ERR_INSUFFICIENT_MEMORY = 5, /*!< Memory allocation failed */
	SOA_ERR_FRAMEWORK = 6,      /*!< Error returned from framework */
	SOA_ERR_MATCHED = 7,        /*!< A match was found */
	SOA_ERR_MAX = 8         /*!< Maximum number of errors */
} //! Enum variable.
/*! SDP OAM Return code */ e_SdpOamRetCode;

/*! @struct t_SdpNegMediaInfo
*
* The media information related to negotiated media.
*
* @author Razi-ud-Din Chaudhry
*
*/

typedef struct _t_SdpNegMediaInfo
{
	t_String      oRemoteIp;    /*!< Remote IP address */
	unsigned char ucPayloadType;    /*!< Payload Type */
	int           nRemotePort;  /*!< Remote Port */
	int           nNegMediaDescIdx;       /*!< Index in Negotiated media SDP (that can be passed to SdpMediaDescGet* functions) */
	int           nNegMediaDescAttrIdx;   /*!< Index in Negotiated media SDP of attribute corresponding to this codec */
} //! Struct variable.
/*! negotiated media information structure. */ t_SdpNegMediaInfo;

/*! @struct t_SdpOamConfig
*
* The configuration of the SDP offer answer model received from the client.
*
* @author Razi-ud-Din Chaudhry
*
*/

typedef struct _t_SdpOamConfig
{
	int         nTraceId;   /*!< Trace Id */
	h_SdpParser hSdpParser; /*!< Handle of Sdp Parser */
} //! Struct variable.
/*! sdp offer answer model configuration structure. */ t_SdpOamConfig;

e_SdpOamRetCode SdpOamGenerateAnswer( h_Sdp hLocalSdp_i, h_Sdp hRemoteSdp_i, t_SdpOamConfig* poSdpOamConfig_i, h_Sdp* phNegSdp_o,  t_AMPSDList* poNegInfoList_o );

e_SdpOamRetCode SdpOamAddDefaultMediaDescAttr( h_Sdp Sdp_io, t_SdpOamConfig* poSdpOamConfig_i );

int SdpOamNegMediaInfoRemoveLinkCb( void** ppvNegMediaInfo_i );

int InitSdpOam( int* pnErrCodeVal_i );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SDP_OAM_API_H_ */

/*! @} */ // End of SdpOam Module

