/*! @file media.h
*
* @ingroup Media
*
* @brief The file contains defines for media API.
*
*/

/*! @defgroup Media Media Module
*
* @brief This module consists of functionality to handle media.
*
* @{
*
*/

#ifndef _MEDIA_H_
#define _MEDIA_H_

#include "fw_api.h"

#include "media/datapkt.h"
#include "media/signalpkt.h"

/*! @def MEDIA_TRACE( pvCompCtx, nModuleID, nTraceLevel, pMessage, a ... )
*
* @brief Macro to be used in all media components for traces.
*
* The first parameter is context of the media component while the remaining are used in providing traces for that component.
*
* This macro can be disabled at compile time through configure option \b --enable-media-trace=no. For more information, about available configure options, see @ref ConfOptions
*
*/

#ifdef ENABLE_MEDIA_TRACE
 #define MEDIA_TRACE( pvCompCtx, nModuleID, nTraceLevel, pMessage, a ... ) AMPS_Trace( __LINE__, __FILE__, __FUNCTION__, nModuleID, nTraceLevel, "(%p)" pMessage, pvCompCtx, ## a )
#else
 #define MEDIA_TRACE( pvCompCtx, nModuleID, nTraceLevel, pMessage, a ... )
#endif

/*! @enum e_MediaCompType
*
* These are the different types of media components
*
*/

typedef enum
{
	MCT_BUFFER = 0, /*!< Media component for buffering of data */
	MCT_FILEREAD = 1,   /*!< Media component for reading file */
	MCT_FILEWRITE = 2,  /*!< Media component for writing files */
	MCT_LEVEL = 3,  /*!< Media component for leveling */
	MCT_PCMUDEC = 4,    /*!< Media component for PCMU decoding */
	MCT_PCMUENC = 5,    /*!< Media component for PCMU encoding */
	MCT_RTPRECV = 6,    /*!< Media component for receiving RTP packets*/
	MCT_RTPSEND = 7,    /*!< Media component for sending RTP packets */
	MCT_TIMEQUEUE = 8,  /*!< Media component for time queue */
	MCT_UDPRECV = 9,    /*!< Media component for receiving data on UDP */
	MCT_UDPSEND = 10,   /*!< Media component for sending data on UDP */
	MCT_NULL = 11,  /*!< Media component for NULL */
	MCT_WAVREAD = 12,   /*!< Media component for reading wave format audio */
	MCT_WAVWRITE = 13,  /*!< Media component for writing wave format audio */
	MCT_MIXER = 14, /*!< Media component for mixing */
	MCT_GSMDEC = 15,    /*!< Media component for GSM decoding */
	MCT_GSMENC = 16,    /*!< Media component for GSM encoding */
	MCT_G729ENC = 17,    /*!< Media component for G729 encoding */
	MCT_G729DEC = 18,    /*!< Media component for G729 decoding */
	MCT_RESEQBUFFER = 19, /*!< Media component for resequencing packet based on generation time */
	MCT_CHAIN = 20, /*!< Media component for media chain */
	MCT_MAX = 21    /*!< Media component maximum limit */
} //! Enum variable.
/*! All defines for different types of media components */ e_MediaCompType;

/*! @enum e_MediaCompGroup
*
* This is the set of defines for different groups of media components
*
*/

typedef enum
{
	MCG_SOURCE, /*!< Media component of source group */
	MCG_FILTER, /*!< Media component of filter group */
	MCG_SINK,   /*!< Media component of sink group */
	MCG_OTHER   /*!< Media component of other group */
} //! Enum variable.
/*! All defines for different groups of media components */ e_MediaCompGroup;

/*! @enum e_MediaCompStatus
*
* This is the set of defines for the status of media components
*
*/

typedef enum
{
	MCS_START,  /*!< Media component start status */
	MCS_STOP,   /*!< Media component stop status */
	MCS_MAX /*!< Media component maximum status */
} //! Enum variable.
/*! All defines for the status of media components */ e_MediaCompStatus;

/*! @enum e_MediaCompConfig
*
* To get/set a value of the media component attribute at run time. The second part of the name describes the type of the component defined by @ref e_MediaCompType for which this attribute is used.
*
*/

typedef enum
{
	MCC_FILEREAD_FILENAME = 0,  /*!< The name of the file to be read. Used by @ref f_MediaCompSetParam funtion. The data is used as a character pointer and needs not to be allocated/deallocated.  */
	MCC_FILEREAD_FILEOFFSET = 1,    /*!< The offset in the file to start reading from. Used by @ref f_MediaCompSetParam funtion. The data is used as a long value and needs not to be allocated/deallocated. */
	MCC_FILEWRITE_BYTES_WRITTEN = 10,   /*!< The number of bytes written to file. Used by @ref f_MediaCompGetParam funtion. The data is used as a integer pointer and needs to be allocated/deallocated. */
	MCC_FILEWRITE_FILENAME = 11,    /*!< The name of the file to be written. Used by @ref f_MediaCompSetParam funtion. The data is used as a character pointer and needs not to be allocated/deallocated. */
	MCC_UDPRECV_LOCAL_PORT = 20,    /*!< The local port to be used for getting UDP messages. Used by @ref f_MediaCompGetParam funtion. The data is used as a integer pointer and needs to be allocated/deallocated. */
	MCC_UDPRECV_NET_HANDLE = 21,    /*!< The net handle for communcition over UDP. Used by @ref f_MediaCompGetParam funtion. The data is used as a AMPS_NET_HANDLE pointer and needs to be allocated/deallocated. */
	MCC_UDPRECV_REMOTE_IP = 22, /*!< Remote IP of UDP message received from. Used by @ref f_MediaCompSetParam funtion. The data is used as a constant character pointer and needs to be allocated/deallocated. */
	MCC_UDPRECV_REMOTE_PORT = 23,   /*!< Remote port of UDP message received from. Used by @ref f_MediaCompSetParam funtion. The data is used as an integer value and needs not to be allocated/deallocated. */
	MCC_WAVREAD_FILENAME = 30,  /*!< The name of the file for reading audio wave. Used by @ref f_MediaCompSetParam funtion. The data is used as a character pointer and needs not to be allocated/deallocated. */
	MCC_WAVREAD_FILEOFFSET = 31,    /*!< The offset in the file to start reading audio wave. Used by @ref f_MediaCompSetParam funtion. The data is used as a long value and needs not to be allocated/deallocated. */
	MCC_WAVREAD_TIMER_MS = 32,  /*!< The timer for reading wave file. Used by @ref f_MediaCompSetParam funtion. The data is used as an unsigned integer value and needs not to be allocated/deallocated. */
	MCC_WAVREAD_READ_PER_TIMEOUT = 33,  /*!< The reading speed for audio wave. Used by @ref f_MediaCompGetParam funtion. The data is used as an unsigned integer value and needs not to be allocated/deallocated. */
	MCC_WAVREAD_READ_SIZE = 34, /*!< The reading size of audio wave. Used by @ref f_MediaCompSetParam funtion. The data is used as an unsigned integer value and needs not to be allocated/deallocated. */
	MCC_WAVWRITE_BYTES_WRITTEN = 40,    /*!< The number of bytes written for audio wave. Used by @ref f_MediaCompGetParam funtion. The data is used as an integer pointer and needs to be allocated/deallocated. */
	MCC_WAVWRITE_FILENAME = 41, /*!< The name of the file for writing audio wave. Used by @ref f_MediaCompSetParam funtion. The data is used as a character pointer and needs not to be allocated/deallocated. */
	MCC_MIXER_ADD_NODE = 50,    /*!< Mixing node for adding. Used by @ref f_MediaCompSetParam funtion. The data is used as a t_MixerNodeConfig pointer and needs not to be allocated/deallocated. */
	MCC_MIXER_REMOVE_NODE = 51, /*!< Mixing node for removing. Used by @ref f_MediaCompSetParam funtion. The data is used as a void pointer and needs not to be allocated/deallocated. */
	MCC_MIXER_BUFFER_TIME_MS = 52,  /*!< Buffer mixing time. Used by @ref f_MediaCompSetParam funtion. The data is used as an integer value and needs not to be allocated/deallocated. */
	MCC_MIXER_PACKETIZATION_TIME_MS = 53,   /*!< Mixed packetization time. Used by @ref f_MediaCompSetParam funtion. The data is used as an integer value and needs not to be allocated/deallocated. */
	MCC_UDPSEND_REMOTE_IP = 60, /*!< Remote IP for sending UDP message. Used by @ref f_MediaCompSetParam funtion. The data is used as a character pointer and needs not to be allocated/deallocated. */
	MCC_UDPSEND_REMOTE_PORT = 61,   /*!< Remote port for sending UDP message. Used by @ref f_MediaCompSetParam funtion. The data is used as an integer value and needs not to be allocated/deallocated.  */
	MCC_UDPSEND_NET_HANDLE = 62,    /*!< Net handle for sending UDP message. Used by @ref f_MediaCompSetParam and @ref f_MediaCompGetParam funtions. The data is used as a AMPS_NET_HANDLE pointer in get function as well as AMPS_NET_HANDLE in set function and needs to be allocated/deallocated. */
	MCC_RTPRECV_PAYLOAD_TYPE = 70,  /*!< Payload type of the RTP packet received. Used by @ref f_MediaCompSetParam funtion. The data is used as an unsigned character value and needs not to be allocated/deallocated. */
	MCC_RTPRECV_TELEPHONY_EVENT_PAYLOAD_TYPE = 71,  /*!< Telephony event payload type of the RTP packet received. Used by @ref f_MediaCompSetParam funtion. The data is used as an unsigned character value and needs not to be allocated/deallocated. */
	MCC_RTPRECV_SAMPLING_RATE = 72, /*!< Sampling rate for receiving the RTP packets. Used by @ref f_MediaCompSetParam funtion. The data is used as an integer value and needs not to be allocated/deallocated. */
	MCC_RTPRECV_NUM_PKTS_RECV = 73, /*!< Number of RTP packets received. Used by @ref f_MediaCompGetParam funtion. The data is used as an unsigned integer pointer and needs not to be allocated/deallocated. */
	MCC_LEVEL_SILENCE_THRESHOLD_DB = 80,    /*!< Silence thresold level. Used by @ref f_MediaCompSetParam funtion. The data is used as an integer value and needs not to be allocated/deallocated. */
	MCC_RTPSEND_SAMPLING_RATE = 90,    /*!< Sampling rate for sending RTP packets. Used by @ref f_MediaCompSetParam funtion. The data is used as an integer value and needs not to be allocated/deallocated. */
	MCC_RTPSEND_NUM_PKTS_SENT = 91 /*!< Number of RTP packets sent. Used by @ref f_MediaCompGetParam funtion. The data is used as an unsigned integer pointer and needs not to be allocated/deallocated. */
} //! Enum variable.
/*! All defines for the configuration of media components */ e_MediaCompConfig;

/*! Handle for media component */
typedef void* h_MediaComp;
/*! Handle for media component application context */
typedef void* h_MediaCompAppCtx;

/*! Handle for media component configuration. Passed while initializing media component through @ref f_MediaCompeInit function */
typedef void* h_CompConfig;
/*! Handle for media component application data */
typedef void* h_MediaCompAppData;

/*! Handle for media chain */
typedef void* h_MediaChain;

/*! Prototype for data handling callback function */
typedef e_MsRetCode (*f_AppDataCb)( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_DataPkt hDataPkt_i );
/*! Prototype for signal handling callback function */
typedef e_MsRetCode (*f_AppSignalCb)( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

/*! Prototype of function to handle signals from media chain */
typedef e_MsRetCode (*f_MediaChainSignalCb)( h_MediaComp hMediaComp_i, h_SigPkt h_SigPkt_i, h_MediaCompAppCtx hMediaCompAppCtx1_i, h_MediaCompAppCtx hMediaCompAppCtx2_i );

/*! Prototype for initailizing media component */
typedef e_MsRetCode (*f_MediaCompeInit)( h_CompConfig hCompConfig_i, h_MediaComp* phMediaComp_o );
/*! Prototype for setting application data for media component */
typedef e_MsRetCode (*f_MediaCompSetAppData)( h_MediaComp hMediaComp_i, h_MediaCompAppData hCompAppData_i );
/*! Prototype for destroying media component */
typedef e_MsRetCode (*f_MediaCompDestroy)( h_MediaComp hMediaComp_i );
/*! Prototype for setting the status of a media component */
typedef e_MsRetCode (*f_MediaCompSetStatus)( h_MediaComp hMediaComp_i, e_MediaCompStatus eStatus );
/*! Prototype for setting the parameters of a media component */
typedef e_MsRetCode (*f_MediaCompSetParam)( h_MediaComp hMediaComp_i, e_MediaCompConfig e_Config_i, void* pvConfigData_i );
/*! Prototype for getting the parameters of a media component */
typedef e_MsRetCode (*f_MediaCompGetParam)( h_MediaComp hMediaComp_i, e_MediaCompConfig e_Config_i, void* pvConfigData_i );

/*! @struct t_MediaComp
*
* The details about media component
*
*/

typedef struct _t_MediaComp
{
	e_MediaCompType  eType; /*!< Media component type */
	e_MediaCompGroup eGroup;    /*!< Media component group */
	int              nTraceId;  /*!< Trace id for providing traces for media components */
	void*            pvFrameworkCtx;    /*!< Framework context */
} //! Struct variable.
/*! Media component details structure */ t_MediaComp;

/*! @struct t_MediaCompConfig
*
* The configuration for media components
*
*/

typedef struct _t_MediaCompConfig
{
	int   nTraceId; /*!< Trace id for providing traces for media components */
	void* pvFrameworkCtx;   /*!< Framework context */
} //! Struct variable.
/*! Media component configuration structure. */ t_MediaCompConfig;

/*! @struct t_MediaCompAppData
*
* Information of the media component that is linked to this component
*
*/

typedef struct _t_MediaCompAppData
{
	h_MediaCompAppCtx hAppDataCtx;  /*!< Application context of the linked media component when calling data handling callback function. Opaque for the media component for which this is set */
	h_MediaCompAppCtx hAppSignalCtx;    /*!< Application context of the linked media component when calling signal handling callback function. Opaque for the media component for which this is set */
	f_AppDataCb       pfAppDataCb;  /*!< Pointer to the function of data handling function of the next media component. This function defines a link between the two components */
	f_AppSignalCb     pfAppSignalCb;    /*!< Pointer to the function of signal handling function of the next media component. This function defines a link between the two components */
} //! Struct variable.
/*! Media component application data structure. */ t_MediaCompAppData;

void* MediaMallocCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, int r_nSize );

void* MediaReAllocCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, void* r_pvData, int r_nSize );

void MediaFreeCallback( AMPS_HANDLE r_hAMPS_HANDLE, AMPS_APP_HANDLE r_hAMPS_APP_HANDLE, void* r_pvData );

e_MsRetCode MediaChain_AppSignalCb( h_MediaComp hMediaComp_i, h_MediaCompAppCtx hMediaCompAppCtx_i, h_SigPkt h_SigPkt_i );

e_MsRetCode Media_CompCreate( e_MediaCompType eType_i, h_CompConfig hCompConfig_i, h_MediaComp* phMediaComp_o );

e_MsRetCode Media_CompDestroy( h_MediaComp hMediaComp_i );

e_MsRetCode Media_CompSetAppData( h_MediaComp hMediaComp_i, h_MediaCompAppData hCompAppData_i );

e_MsRetCode Media_CompSetStatus( h_MediaComp hMediaComp_i, e_MediaCompStatus eStatus );

e_MsRetCode Media_CompSetParam( h_MediaComp hMediaComp_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

e_MsRetCode Media_CompGetParam( h_MediaComp hMediaComp_i, e_MediaCompConfig eConfig_i, void* pvConfigData_i );

e_MsRetCode Media_ChainCreate( h_CompConfig hCompConfig_i, h_MediaChain* phMediaChain_o );

e_MsRetCode Media_ChainAddInChain( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i );

e_MsRetCode Media_ChainRemoveFromChain( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i );

e_MsRetCode Media_ChainSetLink( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp1_i, h_MediaComp hMediaComp2_i );

e_MsRetCode Media_ChainSetAppCb( h_MediaChain hMediaChain_i, f_MediaChainSignalCb pfMediaChainSignalCb_i, h_MediaCompAppCtx hMediaCompAppCtx1_i, h_MediaCompAppCtx hMediaCompAppCtx2_i );

e_MsRetCode Media_ChainFillAppData( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i, t_MediaCompAppData* poAppData_i );

e_MsRetCode Media_ChainSetStatus( h_MediaChain hMediaChain_i, e_MediaCompStatus eStatus_i );

e_MsRetCode Media_ChainGetCompAtOffset( h_MediaChain hMediaChain_i, int nOffset_i, h_MediaComp* phMediaComp_o );

e_MsRetCode Media_ChainGetCompOfType( h_MediaChain hMediaChain_i, e_MediaCompType eCompType_i, h_MediaComp* phMediaComp_o );

e_MsRetCode Media_ChainGetNextComp( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i, h_MediaComp* phMediaComp_o );

e_MsRetCode Media_ChainGetPrevComp( h_MediaChain hMediaChain_i, h_MediaComp hMediaComp_i, h_MediaComp* phMediaComp_o );

e_MsRetCode Media_CompGetType( h_MediaComp hMediaComp_i, e_MediaCompType* peType_o );

e_MsRetCode Media_ChainDestroy( h_MediaChain hMediaChain_i );

int MediaComp_LListRemoveLinkDataCallback( void** ppvData_io );

#endif /* _MEDIA_H_ */

/*! @} */ // End of Media Module

