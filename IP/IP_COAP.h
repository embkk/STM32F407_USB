/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2007 - 2025    SEGGER Microcontroller GmbH               *
*                                                                    *
*       www.segger.com     Support: www.segger.com/ticket            *
*                                                                    *
**********************************************************************
*                                                                    *
*       emNet * TCP/IP stack for embedded applications               *
*                                                                    *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product for in-house use.         *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       emNet version: V3.62.4                                       *
*                                                                    *
**********************************************************************
-------------------------- END-OF-HEADER -----------------------------
Purpose : Header file for the Constrained Application Protocol (CoAP)
          Client / Server application.
*/

#ifndef IP_COAP_H            // Avoid multiple inclusion.
#define IP_COAP_H

#if defined(__cplusplus)
  extern "C" {                // Make sure we have C-declarations in C++ programs.
#endif

/*********************************************************************
*
*       #include section
*
**********************************************************************
*/

#include "SEGGER.h"
#include "IP_COAP_Conf.h"

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

//
// UDP connection definitions.
//
#define IP_COAP_IPV4         0u
#define IP_COAP_IPV6         1u
#define IP_COAP_DEFAULT_PORT 5683u

//
// CoAP types.
//
#define IP_COAP_TYPE_CON  0x0u
#define IP_COAP_TYPE_NON  0x1u
#define IP_COAP_TYPE_ACK  0x2u
#define IP_COAP_TYPE_RST  0x3u

//
// CoAP codes.
//
#define IP_COAP_CODE2NUM(c,d)             (U8)((((c) & 0x7u) << 5) | ((d) & 0x1Fu))
#define IP_COAP_GET_CLASS(n)              (((n) >> 5) & 0x7u)

#define IP_COAP_CLASS_REQUEST             0u
#define IP_COAP_CLASS_SUCCESS             2u
#define IP_COAP_CLASS_CLIENT_ERROR        4u
#define IP_COAP_CLASS_SERVER_ERROR        5u

#define IP_COAP_CODE_EMPTY                IP_COAP_CODE2NUM(0u, 0u)   // Empty

#define IP_COAP_CODE_REQ_GET              IP_COAP_CODE2NUM(0u, 1u)   // GET
#define IP_COAP_CODE_REQ_POST             IP_COAP_CODE2NUM(0u, 2u)   // POST
#define IP_COAP_CODE_REQ_PUT              IP_COAP_CODE2NUM(0u, 3u)   // PUT
#define IP_COAP_CODE_REQ_DEL              IP_COAP_CODE2NUM(0u, 4u)   // DELETE

#define IP_COAP_CODE_SUCCESS_CREATED      IP_COAP_CODE2NUM(2u, 1u)   // 2.01 Created
#define IP_COAP_CODE_SUCCESS_DELETED      IP_COAP_CODE2NUM(2u, 2u)   // 2.02 Deleted
#define IP_COAP_CODE_SUCCESS_VALID        IP_COAP_CODE2NUM(2u, 3u)   // 2.03 Valid
#define IP_COAP_CODE_SUCCESS_CHANGED      IP_COAP_CODE2NUM(2u, 4u)   // 2.04 Changed
#define IP_COAP_CODE_SUCCESS_CONTENT      IP_COAP_CODE2NUM(2u, 5u)   // 2.05 Content
#define IP_COAP_CODE_SUCCESS_CONTINUE     IP_COAP_CODE2NUM(2u,31u)   // 2.31 Continue

#define IP_COAP_CODE_CERROR_BAD_REQ       IP_COAP_CODE2NUM(4u, 0u)   // 4.00 Bad Request
#define IP_COAP_CODE_CERROR_UNAUTHOR      IP_COAP_CODE2NUM(4u, 1u)   // 4.01 Unauthorized
#define IP_COAP_CODE_CERROR_BAP_OPT       IP_COAP_CODE2NUM(4u, 2u)   // 4.02 Bad Option
#define IP_COAP_CODE_CERROR_FORBID        IP_COAP_CODE2NUM(4u, 3u)   // 4.03 Forbidden
#define IP_COAP_CODE_CERROR_NOT_FOUND     IP_COAP_CODE2NUM(4u, 4u)   // 4.04 Not Found
#define IP_COAP_CODE_CERROR_NOT_ALLOW     IP_COAP_CODE2NUM(4u, 5u)   // 4.05 Method Not Allowed
#define IP_COAP_CODE_CERROR_NOT_ACCEPT    IP_COAP_CODE2NUM(4u, 6u)   // 4.06 Not Acceptable
#define IP_COAP_CODE_CERROR_REQ_INCOMP    IP_COAP_CODE2NUM(4u, 8u)   // 4.08 Request Entity Incomplete
#define IP_COAP_CODE_CERROR_PRECOND       IP_COAP_CODE2NUM(4u,12u)   // 4.12 Precondition Failed
#define IP_COAP_CODE_CERROR_TOO_LARGE     IP_COAP_CODE2NUM(4u,13u)   // 4.13 Request Entity Too Large
#define IP_COAP_CODE_CERROR_CONT_FORMAT   IP_COAP_CODE2NUM(4u,15u)   // 4.15 Unsupported Content-Format

#define IP_COAP_CODE_SERROR_INTERNAL      IP_COAP_CODE2NUM(5u, 0u)   // 5.00 Internal Server Error
#define IP_COAP_CODE_SERROR_NOT_IMP       IP_COAP_CODE2NUM(5u, 1u)   // 5.01 Not Implemented
#define IP_COAP_CODE_SERROR_BAD_GW        IP_COAP_CODE2NUM(5u, 2u)   // 5.02 Bad Gateway
#define IP_COAP_CODE_SERROR_UNAVAIL       IP_COAP_CODE2NUM(5u, 3u)   // 5.03 Service Unavailable
#define IP_COAP_CODE_SERROR_GW_TIME       IP_COAP_CODE2NUM(5u, 4u)   // 5.04 Gateway Timeout
#define IP_COAP_CODE_SERROR_NO_PROXY      IP_COAP_CODE2NUM(5u, 5u)   // 5.05 Proxying Not Supported

//
// Internal code for client error report.
// Not to be used in client/server exchange.
//
#define IP_COAP_CODE_INTERN_REQ_TIMEOUT   IP_COAP_CODE2NUM(6u, 0u)   // 6.00 Request timeout.

//
// CoAP options.
//
#define IP_COAP_OPTION_IF_MATCH            1u
#define IP_COAP_OPTION_URI_HOST            3u
#define IP_COAP_OPTION_ETAG                4u
#define IP_COAP_OPTION_IF_NONE_MATCH       5u
#define IP_COAP_OPTION_OBSERVE             6u
#define IP_COAP_OPTION_URI_PORT            7u
#define IP_COAP_OPTION_LOCATION_PATH       8u
#define IP_COAP_OPTION_URI_PATH           11u
#define IP_COAP_OPTION_CONTENT_FORMAT     12u
#define IP_COAP_OPTION_MAX_AGE            14u
#define IP_COAP_OPTION_URI_QUERY          15u
#define IP_COAP_OPTION_ACCEPT             17u
#define IP_COAP_OPTION_LOCATION_QUERY     20u
#define IP_COAP_OPTION_BLOCK2             23u
#define IP_COAP_OPTION_BLOCK1             27u
#define IP_COAP_OPTION_SIZE2              28u
#define IP_COAP_OPTION_PROXY_URI          35u
#define IP_COAP_OPTION_PROXY_SCHEME       39u
#define IP_COAP_OPTION_SIZE1              60u

//
// Options internal mask (needs to be ordered in
// growing CoAP option number).
//
#define IP_COAP_OPTMASK_IF_MATCH          (1uL      )
#define IP_COAP_OPTMASK_URI_HOST          (1uL <<  1)
#define IP_COAP_OPTMASK_ETAG              (1uL <<  2)
#define IP_COAP_OPTMASK_IF_NONE_MATCH     (1uL <<  3)
#define IP_COAP_OPTMASK_OBSERVE           (1uL <<  4)
#define IP_COAP_OPTMASK_URI_PORT          (1uL <<  5)
#define IP_COAP_OPTMASK_LOCATION_PATH     (1uL <<  6)
#define IP_COAP_OPTMASK_URI_PATH          (1uL <<  7)
#define IP_COAP_OPTMASK_CONTENT_FORMAT    (1uL <<  8)
#define IP_COAP_OPTMASK_MAX_AGE           (1uL <<  9)
#define IP_COAP_OPTMASK_URI_QUERY         (1uL << 10)
#define IP_COAP_OPTMASK_ACCEPT            (1uL << 11)
#define IP_COAP_OPTMASK_LOCATION_QUERY    (1uL << 12)
#define IP_COAP_OPTMASK_BLOCK2            (1uL << 13)
#define IP_COAP_OPTMASK_BLOCK1            (1uL << 14)
#define IP_COAP_OPTMASK_SIZE2             (1uL << 15)
#define IP_COAP_OPTMASK_PROXY_URI         (1uL << 16)
#define IP_COAP_OPTMASK_PROXY_SCHEME      (1uL << 17)
#define IP_COAP_OPTMASK_SIZE1             (1uL << 18)

//
// Content Format.
//
#define IP_COAP_CT_TXT                     0u  // plain/text
#define IP_COAP_CT_LINK_FORMAT            40u  // application/link-format
#define IP_COAP_CT_XML                    41u  // application/xml
#define IP_COAP_CT_OCTET_STREAM           42u  // application/octet-stream
#define IP_COAP_CT_EXI                    47u  // application/exi
#define IP_COAP_CT_JSON                   50u  // application/json

//
// Flag to disable major features.
//
#define IP_COAP_CONFIG_DISABLE_BLOCK1     (1u     )
#define IP_COAP_CONFIG_DISABLE_BLOCK2     (1u << 1)
#define IP_COAP_CONFIG_DISABLE_BLOCKS     (IP_COAP_CONFIG_DISABLE_BLOCK1 | IP_COAP_CONFIG_DISABLE_BLOCK2)
#define IP_COAP_CONFIG_DISABLE_OBSERVE    (1u << 2)

//
// Observer configuration
//
#define IP_COAP_OBS_OBSERVABLE            (1u     )
#define IP_COAP_OBS_AUTO_CON_ON_MAX_AGE   (1u << 1)
#define IP_COAP_OBS_AUTO_NON_ON_MAX_AGE   (1u << 2)

/*********************************************************************
*
*       Data types
*
**********************************************************************
*/

//
// Status of the connection between a client and a server.
//
enum {
  IP_COAP_CONNECTION_NONE                  = 0,
  IP_COAP_CONNECTION_WAIT_NEXT_BLOCK_REQ,          // Waiting for the next GET when doing block transfer.
  IP_COAP_CONNECTION_WAIT_DATA_READY,              // Separate ACK sent, waiting for the data to be ready
  IP_COAP_CONNECTION_DATA_READY,                   // Users data are ready, need to send a CON/NON.
  IP_COAP_CONNECTION_WAIT_ACK,                     // CON/NON is sent, wait for ACK/NON.
  IP_COAP_CONNECTION_WAIT_MULTICAST,               // Multicast request received, waiting to send the reply.
  IP_COAP_CONNECTION_CLOSE_ON_END,                 // Connection will be closed (error occured).
  IP_COAP_CONNECTION_CLOSE_ON_ERROR                // Connection will be closed, transfer is completed.
};


//
// Return values for the CoAP functions and callbacks.
//
enum {
  //
  // Negative values are errors.
  //
  IP_COAP_RETURN_NO_DATA                  = -9,
  IP_COAP_RETURN_CONDITION_FAILED         = -8,
  IP_COAP_RETURN_PROXY_NOT_SUPPORTED      = -7,
  IP_COAP_RETURN_CT_FORMAT_ERROR          = -6,
  IP_COAP_RETURN_OPTION_ERROR             = -5,
  IP_COAP_RETURN_BUFFER_TOO_SMALL         = -4,
  IP_COAP_RETURN_NO_PAYLOAD               = -3,
  IP_COAP_RETURN_NOT_ALLOWED              = -2,
  IP_COAP_RETURN_ERR                      = -1,

  //
  // Positive values are information.
  //
  IP_COAP_RETURN_OK                =  0,
  IP_COAP_RETURN_SEND_SEPARATE,
  IP_COAP_RETURN_SEND_BLOCK,
  IP_COAP_RETURN_SEND_END,
  IP_COAP_RETURN_IGNORE_BLOCK,
  IP_COAP_RETURN_IGNORE_END,
  IP_COAP_RETURN_WAIT_ACK,
  IP_COAP_RETURN_RESET,
  IP_COAP_RETURN_OK_NO_DELETE
};

/*********************************************************************
*
*       IP_COAP_CONN_INFO
*
*  Description
*    UDP connection definition.
*/
typedef struct {
  void* hSock;
  union {
    U8  IPAddrV6[16];
    U32 IPAddrV4;
  } Addr;
  U16   Port;
  U8    Family;
} IP_COAP_CONN_INFO;

/*********************************************************************
*
*       IP_COAP_HEADER_INFO
*
*  Description
*    Parsed CoAP header.
*/
typedef struct {
  U8  Type;         // CoAP message type (IP_COAP_TYPE_xxx).
  U8  Code;         // CoAP message code (IP_COAP_CODE_xxx).
  U16 MessageId;    // CoAP message id.
  U8  aToken[8];    // Length of aToken. Could be 0 if no token is present.
  U8  TokenLength;  // CoAP message token.
} IP_COAP_HEADER_INFO;

/*********************************************************************
*
*       IP_COAP_BLOCK_INFO
*
*  Description
*    Definition of the block used for Block1 and Block2 options.
*/
typedef struct {
  U16 Index;  // Index of the block. 0 is the first block.
  U16 Size;   // Size of the block.
} IP_COAP_BLOCK_INFO;

//
// If-Match structure used by the client
// to send many If-Match options in one
// request.
//
/*********************************************************************
*
*       IP_COAP_IF_MATCH_INFO
*
*  Description
*    Utility structure to parametrize the If-Match option.
*
*  Additional information
*    This list is used to allow requesting more than one ETag in the If-Match option. When the
*    ETagLength is 0 (pETag is NULL), the check is for all ETags, meaning it is an "existence"
*    verification.
*/
typedef struct IP_COAP_IF_MATCH_INFO {
  struct IP_COAP_IF_MATCH_INFO* pNext;      // Anchor of the linked list.
  U8*                           pETag;      // Pointer on the ETag value.
  U8                            ETagLength; // Length of the ETag value.
} IP_COAP_IF_MATCH_INFO;

/*********************************************************************
*
*       IP_COAP_OPTIONS_INFO
*
*  Description
*    CoAP options description.
*
*  Parameters
*    pBlock    : Block information. See IP_COAP_BLOCK_INFO.
*    pConnInfo : UDP connection information. See IP_COAP_CONN_INFO.
*    pHeader   : Parsed CoAP message header. See IP_COAP_HEADER_INFO.
*    pOptDesc  : CoAP message option description. See IP_COAP_OPTIONS_INFO.
*
*  Additional information
*    In the callbacks, it is recommended to access the options through the utility functions
*     instead of directly accessing this structure as some options might have been reformatted.
*/
typedef struct {
  U32                      OptionPresentMask; // Mask indicating the presence of options (IP_COAP_OPTMASK_xxx).
  //
  U8*                      pHost;             // Pointer on the Uri-Host.
  U8*                      pURI;              // Pointer on the Uri-Path.
  U8*                      pQuery;            // Pointer on the Uri-Query.
  U8*                      pETag;             // Pointer on the ETag.
  U8*                      pBlockLast;        // Pointer used internally.
  U8*                      pProxyURI;         // Pointer on the Proxy-Uri.
  U8*                      pProxyScheme;      // Pointer on the Proxy-Scheme.
  IP_COAP_IF_MATCH_INFO*   pIfMatch;          // If-Match linked list start.
  U32                      Observe;           // Observe value.
  U32                      MaxAge;            // Max-Age value.
  U32                      Size1;             // Size1 value.
  U32                      Block2;            // Block2 value.
  U32                      Block1;            // Block1 value.
  U32                      Size2;             // Size2 value.
  U16                      URIPort;           // URIPort value.
  U16                      Accept;            // Accept value.
  U16                      ContentFormat;     // Content-Format value.
  U16                      ProxyURILength;    // Length of pProxyURI.
  U8                       ProxySchemeLength; // Length of pProxyScheme.
  U8                       HostLength;        // Length of pHost.
  U8                       URILength;         // Length of pURI.
  U8                       QueryLength;       // Length of pQuery.
  U8                       ETagLength;        // Length of pETag.
} IP_COAP_OPTIONS_INFO;

/*********************************************************************
*
*       IP_COAP_CALLBACK_PARAM
*
*  Description
*    Argument of the payload callbacks describing connection parameters.
*/
typedef struct {
  IP_COAP_BLOCK_INFO*    pBlock;    // Block information. See IP_COAP_BLOCK_INFO.
  IP_COAP_CONN_INFO*     pConnInfo; // UDP connection information. See IP_COAP_CONN_INFO.
  IP_COAP_HEADER_INFO*   pHeader;   // Parsed CoAP message header. See IP_COAP_HEADER_INFO.
  IP_COAP_OPTIONS_INFO*  pOptDesc;  // CoAP message option description. See IP_COAP_OPTIONS_INFO.
} IP_COAP_CALLBACK_PARAM;

/*********************************************************************
*
*       IP_COAP_pfReceive
*
*  Description
*    Callback used to receive UDP packets.
*
*  Parameters
*    pBuffer      : Start of the message buffer.
*    BufferSize   : Length of the message buffer.
*    pInfo        : UDP connection information.
*    pIsMulticast : Flag to set to indicate that the received message is a multicast/broadcast (1) or a unicast (0) message. If not known always set to 0.
*/
typedef int   (*IP_COAP_pfReceive)(U8* pBuffer, unsigned BufferSize, IP_COAP_CONN_INFO* pInfo, unsigned* pIsMulticast);

/*********************************************************************
*
*       IP_COAP_pfSend
*
*  Description
*    Callback used to send UDP packets.
*
*  Parameters
*    pBuffer      : Start of the message buffer.
*    BufferSize   : Length of the message buffer.
*    pInfo        : UDP connection information.
*/
typedef int   (*IP_COAP_pfSend)(U8* pBuffer, unsigned BufferSize, IP_COAP_CONN_INFO* pInfo);

/*********************************************************************
*
*       IP_COAP_pfGetTimeMs
*
*  Description
*    Callback used to get the current time in milliseconds.
*/
typedef U32   (*IP_COAP_pfGetTimeMs)(void);


/*********************************************************************
*
*       IP_COAP_API
*
*  Description
*    Contains the functions to perform the UDP receive and send as well as a function to get the current time in ms.
*/
typedef struct {
  IP_COAP_pfReceive            pfReceive;   // Callback to receive UDP packets (see IP_COAP_pfReceive).
  IP_COAP_pfSend               pfSend;      // Callback to send UDP packets (see IP_COAP_pfSend).
  IP_COAP_pfGetTimeMs          pfGetTimeMs; // Callback to get the current time in milliseconds (see IP_COAP_pfGetTimeMs).
} IP_COAP_API;

/////////////////////////////////////////////////////////
// Server.
/////////////////////////////////////////////////////////

struct IP_COAP_SERVER_CONTEXT;

/*********************************************************************
*
*       IP_COAP_pfGETPayload
*
*  Description
*    Server data callback to fill the payload of a GET request.
*
*  Parameters
*    pContext : Pointer on the server context.
*    ppBuffer : Start of the payload location. To be moved to the end of the copied data.
*    pLength  : Length of the remaining bytes in the message/block. To be reduced from the number of added data.
*    pParam   : Callback parameters. Refer to IP_COAP_CALLBACK_PARAM.
*
*  Additional information
*    The callback parameter contains information regarding the request, especially the block
*    information.
*   The return values are of type IP_COAP_RETURN. The possible values are:
*   * IP_COAP_RETURN_SEND_END: The payload is added and this is the last block (or no block used).
*   * IP_COAP_RETURN_SEND_SEPARATE: The data are not ready, send an ACK. IP_COAP_SERVER_UpdateData() will be called
*     later to reply to the client with a new request.
*   * IP_COAP_RETURN_SEND_BLOCK: The payload is added but there are more data to copy. The block should be fully filled.
*   * Any one of the defined error return values. If an error is returned, it could be useful for the operator to add
*     a short description (null-terminated string) that would be appended to the error message.
*/
typedef int (*IP_COAP_pfGETPayload)(struct IP_COAP_SERVER_CONTEXT* pContext, U8** ppBuffer, U16* pLength, IP_COAP_CALLBACK_PARAM* pParam);

/*********************************************************************
*
*       IP_COAP_pfDELHandler
*
*  Description
*    Server data callback to check the authorization to perform a DELETE request.
*
*  Parameters
*    pContext : Pointer on the server context.
*    pParam   : Callback parameters. Refer to IP_COAP_CALLBACK_PARAM.
*
*  Additional information
*    The callback parameter contains information regarding the request, especially the client UDP connection info.
*    The return values are of type IP_COAP_RETURN. The possible values are:
*    * IP_COAP_RETURN_OK: The resource is deleted. Do not call IP_COAP_SERVER_RemoveData(). It will be done by the server.
*    * IP_COAP_RETURN_OK_NO_DELETE: The server will reply with a 2.02 Deleted but will not actually remove the data (no
*      call to IP_COAP_SERVER_RemoveData()).
*    * Any one of the defined error return values. If an error is returned, it could be useful for the operator to add a
*      short description (null-terminated string) that would be appended to the error message.
*/
typedef int (*IP_COAP_pfDELHandler)(struct IP_COAP_SERVER_CONTEXT* pContext, IP_COAP_CALLBACK_PARAM* pParam);

/*********************************************************************
*
*       IP_COAP_pfPUTPayload
*
*  Description
*    Server data callback to store the payload of a PUT request.
*
*  Parameters
*    pContext : Pointer on the server context.
*    pPayload : Start of the payload memory.
*    Length   : Length of the payload.
*    pParam   : Callback parameters. Refer to IP_COAP_CALLBACK_PARAM.
*
*  Additional information
*    The callback parameter contains information regarding the request, especially the block information.
*    The return values are of type IP_COAP_RETURN. The possible values are:
*    * IP_COAP_RETURN_OK: The resource is updated with the received data block.
*    * Any one of the defined error return values. If an error is returned, it could be useful for
*      the operator to add a short description (null-terminated string) that would be appended
*      to the error message. For example IP_COAP_RETURN_NO_PAYLOAD will trigger an error "4.08 Request Entity Incomplete".
*/
typedef int (*IP_COAP_pfPUTPayload)(struct IP_COAP_SERVER_CONTEXT* pContext, U8* pPayload, U16 Length, IP_COAP_CALLBACK_PARAM* pParam);

//
// Server data structure. Defines a item of the server.
// The first '/' shall be ommited in the sURI string.
//
/*********************************************************************
*
*       IP_COAP_SERVER_DATA
*
*  Description
*    Server context to describe a resource.
*/
typedef struct IP_COAP_SERVER_DATA {
  struct IP_COAP_SERVER_DATA* pNext;          // Anchor for the linked list. Internal use only.
  //
  const char*                 sURI;           // Null terminated string for the resource Uri-Path. The first '/' is omitted.
  const char*                 sDescription;   // Null terminated string for the description of the resource in CoRE format.
                                              // If the resource is an observable, "obs" is automatically added and should not be set here.
  U8                          ETag[8];        // entity-tag (ETag) of the resource.
  U32                         MaxAge;         // MaxAge (value validity) duration of a the resource after a GET.
  U32                         Size2;          // Size of the resource sent in a GET reply if configured so.
  U32                         DefGetOptMask;  // Default options characterizing the resource sent in a GET reply (see below).
  U16                         ContentFormat;  // Content-Format of the resource. Some format are defined in the RFC and are in IP_COAP.h (IP_COAP_CT_xxx) but other numerical values could be used.
  U8                          ETagLength;     // Length of ETag.
  U8                          ObsConfig;      // Configuration of the observe parameters of the resource (see below). Set to 0 if the resource is not observable.
  //
  // Callbacks to handle the requests.
  //
  IP_COAP_pfGETPayload    pfGETPayload;       // Callback used to fill the payload when replying to a GET (see IP_COAP_pfGETPayload ). If set to NULL,
                                              // the server will reply with a 4.05 Not Allowed when receiving the request.
  IP_COAP_pfPUTPayload    pfPUTPayload;       // Callback used to handle the payload when receiving a PUT or POST (see IP_COAP_pfPUTPayload ). If set
                                              // to NULL, the server will reply with a 4.05 Not Allowed when receiving the request.
  IP_COAP_pfDELHandler    pfDELHandler;       // Callback used to check the authorization of a DELETE request (see IP_COAP_pfDELHandler ). If set to
                                              // NULL, the server will reply with a 4.05 Not Allowed when receiving the request.
} IP_COAP_SERVER_DATA;


//
// Server definition of a connection with a client.
//
typedef struct IP_COAP_CONNECTION_STRUCT IP_COAP_CONNECTION;
struct IP_COAP_CONNECTION_STRUCT { // NOLINT(clang-analyzer-optin.performance.Padding) (Public struct)
  U8                         ConnStatus;
  U8                         Procedure;
  IP_COAP_SERVER_DATA*       pServerData;
  IP_COAP_HEADER_INFO        Header;
  IP_COAP_OPTIONS_INFO       Options;
  //
  // Retry.
  //
  U32                        UpdateTime;
  U32                        RetryTimeout;
  U8                         RetryNum;
  //
  // Block information.
  //
  U8                         BlockSzx;  // 0xFF for no block.
  U8                         FirstBlockSent;
};


//
// Server definition of a client.
//
typedef struct IP_COAP_SERVER_CLIENT_INFO_STRUCT IP_COAP_SERVER_CLIENT_INFO;
struct IP_COAP_SERVER_CLIENT_INFO_STRUCT {
  IP_COAP_SERVER_CLIENT_INFO*    pNext;
  //
  IP_COAP_CONN_INFO              ConnInfo;
  //
  // According to RFC, there is only NSTART ongoing connections
  // between a client and a server.
  //
  IP_COAP_CONNECTION             aConnection[IP_COAP_NSTART];
  int                            ConnectionIndex;
};

//
// Server definition of an observer.
//
typedef struct IP_COAP_OBSERVER_STRUCT IP_COAP_OBSERVER;
struct IP_COAP_OBSERVER_STRUCT {
  IP_COAP_OBSERVER*       pNext;
  //
  IP_COAP_SERVER_DATA*    pServerData;
  IP_COAP_CONN_INFO       ConnInfo;
  IP_COAP_CONNECTION*     pConnection;
  U8                      aToken[8];
  U32                     Observe;
  U32                     Time;
  U32                     ForceConTimeout;  // Timestamp after which we force sending a CON instead of NON to check if the client is still alive.
  U8                      TokenLength;
  U8                      Status;
  U8                      Szx;
};

/*********************************************************************
*
*       PF_POST_HANDLER
*
*  Description
*    Server callback to create a new resource when receiving a POST.
*
*  Parameters
*    pContext      : Pointer on the server context.
*    pParam        : Parameter structure. See IP_COAP_CALLBACK_PARAM.
*    PayloadLength : Indication on the payload length. It is either the Size1 parameter if given or the length of the 1st block.
*    ppServerData  : Receives the created IP_COAP_SERVER_DATA. Don't call IP_COAP_SERVER_AddData() as it will be done by the server if the pointer is set.
*
*  Additional information
*    The return values are of type IP_COAP_RETURN. The possible values are:
*    * IP_COAP_RETURN_OK: Entry created.
*    * Any one of the defined error return values. If an error is returned, it could be usefull for the
*      operator to add a short description (null-terminated string) that would be appended to the error message.
*    The actual payload will be handle by the PUT payload handler (pfPUTPayload) of the newly created
*    resource. If an error happens the resource will be removed from the server context.
*/
typedef int   (*PF_POST_HANDLER)(struct IP_COAP_SERVER_CONTEXT* pContext, IP_COAP_CALLBACK_PARAM* pParam, U32 PayloadLength, IP_COAP_SERVER_DATA** ppServerData);

/*********************************************************************
*
*       IP_COAP_SERVER_CONTEXT
*
*  Description
*    Main context of the server.
*
*  Additional information
*    The fields of this structure should be set only via APIs or internal functions.
*
*    A client pool is given to the server with the function IP_COAP_SERVER_AddClientBuffer().
*    The size of this pool defines the number of clients connected to the server at the same time.
*    The server could handle more clients as the connections are closed once data transfers are
*    completed. Thus a buffer of at least one client is mandatory but even in this case, many
*    more actual clients could be served especially if there are no long block transfers and CON
*    is used to mitigate congestions.
*
*    An observer pool is given to the server with the function IP_COAP_SERVER_AddObserverBuffer().
*    The size of this pool defines the number of observers handled by the server at the same time.
*    It could be null if no observable resources are configured.
*/
typedef struct IP_COAP_SERVER_CONTEXT { // NOLINT(clang-analyzer-optin.performance.Padding) (Public struct)
  //
  // APIs for UDP transmission and time.
  //
  IP_COAP_pfReceive            pfReceive;         // Callback to receive UDP packets (see IP_COAP_pfReceive)
  IP_COAP_pfSend               pfSend;            // Callback to send UDP packets (see IP_COAP_pfSend)
  IP_COAP_pfGetTimeMs          pfGetTimeMs;       // Callback to get the current time in milliseconds (see IP_COAP_pfGetTimeMs)
  //
  // Server parameters.
  //
  const char*                  sHostName;         // If it is requested to send the Uri-Host option by a IP_COAP_SERVER_DATA , the hostname used is this one. (initialized to a null
                                                  // pointer). All resources associated to a server are using the same host name. This is a null terminated string.
  const char*                  sErrorDesc;        // Null terminated string sent in an error message.
  IP_COAP_SERVER_DATA*         pFirstData;        // Start of the linked list of server resources. Used by IP_COAP_SERVER_AddData() and IP_COAP_SERVER_RemoveData().
  IP_COAP_SERVER_DATA          DataWellKnownCore; // Resource used to reply to a discover (GET .well-known/core). This will automatically return the list of all resources
                                                  // of the server with the CoRE format.
  //
  IP_COAP_SERVER_CLIENT_INFO*  pFirstClient;      // Start of the linked list of the currently connected clients. Internal usage only.
  IP_COAP_SERVER_CLIENT_INFO*  pFreeCLient;       // Linked list of the client structure pool. Initialized with IP_COAP_SERVER_AddClientBuffer().
  //
  IP_COAP_OBSERVER*            pFirstObs;         // Start of the linked list of the currently connected observers. Internal usage only.
  IP_COAP_OBSERVER*            pFreeObs;          //  Linked list of the observer structure pool. Initialized with IP_COAP_SERVER_AddObserverBuffer().
  //
  PF_POST_HANDLER              pfPOSTCreateEntry; // Call back used to create a new entry when getting a POST command. Initialized with IP_COAP_SERVER_SetPOSTHandler().
                                                  // If left unset (NULL), the server won't allow the creation of new resource entries and will automatically reply with
                                                  // a 4.05 Method Not Allowed.
  //
  // Internal data.
  //
  IP_COAP_SERVER_DATA*         pDataFirstFound;
  U8*                          pPayload;
  U32                          OptionMask;
  U16                          UDPPort;
  U16                          PayloadLength;
  U16                          StructureETag;
  U16                          URILengthReq;
  U16                          RxMsgLength;
  U16                          MsgLength;
  U16                          BadOption;
  U16                          MessageId;
  U8                           DefaultSzx;
  U8                           ConfigMask;
  //
  // Buffer for the UDP packet handling (Rx/Tx).
  //
  U8*                          pMsgBuffer;        // Buffer used to receive and send CoAP messages in UDP. Initialized in IP_COAP_SERVER_Init().
  U16                          MsgBufferSize;     // Size of the message buffer.
} IP_COAP_SERVER_CONTEXT;

/////////////////////////////////////////////////////////
// Client.
/////////////////////////////////////////////////////////

struct IP_COAP_CLIENT_CONTEXT;

/*********************************************************************
*
*       PF_CLIENT_PAYLOAD
*
*  Description
*    Client callback to handle the payload of GET/PUT.
*
*  Parameters
*    pContext  : Pointer on the server context.
*    ppPayload : Start of the payload location. To be moved to the end of the copied data.
*    pLength   : Length of the remaining bytes in the message/block. To be reduced from the number of added data.
*    pParam    : Callback parameters. Refer to IP_COAP_CALLBACK_PARAM.
*
*  Additional information
*    The return values are of type IP_COAP_RETURN. The possible values are:
*    * IP_COAP_RETURN_SEND_END: The payload is added to PUT or POST request and this is the last block (or no block used).
*    * IP_COAP_RETURN_SEND_BLOCK: The payload is added to PUT or POST request and there are more data to copy.
       The block should be fully filled.
*    * IP_COAP_RETURN_NO_PAYLOAD: For some reason, there are no payload to copy.
*    * IP_COAP_RETURN_OK: The payload from a GET reply is handled.
*    * Any one of the defined error return values. If an error is returned, it could be useful for the operator to add a
*      short description (null-terminated string) that would be appended to the error message. if no notification is received
*      for some time.
*/
typedef int    (*PF_CLIENT_PAYLOAD)(struct IP_COAP_CLIENT_CONTEXT* pContext, U8** ppPayload, U16* pLength, IP_COAP_CALLBACK_PARAM* pParam);

//
// Callback to mark the end of an observe transfer.
//
/*********************************************************************
*
*       PF_OBS_END_TRANSFER
*
*  Description
*    Observer end-of-transfer indication. This function is called by the client process whenever
*    a notification from an observed data is totally received.
*
*  Parameters
*    Code    : CoAP code of the received notification.
*    IsFinal : Indication that the server canceled the observe, either because of some errors or because
*              the server didn't reply with the observe option.
*    pParam  : Parameter given to the function IP_COAP_CLIENT_OBS_SetEndCallback(). It is unused by the
*              the client process (application usage only)
*
*  Additional information
*    The server may "lose" the observer information without informing the client (due to loss of
*     packets for example). This is the role of the application to refresh the observe registration
*     if no notification is received for some time.
*/
typedef void   (*PF_OBS_END_TRANSFER)(U8 Code, int IsFinal, void* pParam);

//
// Client observer definition.
//
typedef struct IP_COAP_CLIENT_OBS {
  struct IP_COAP_CLIENT_OBS*  pNext;
  //
  // Observe is identified by it's Token and Observe number.
  //
  U8                          aToken[8];
  U8                          TokenLength;
  U8                          Type;
  U32                         Observe;
  //
  // Callbacks.
  //
  PF_CLIENT_PAYLOAD           pfPayloadCallback;
  PF_OBS_END_TRANSFER         pfObsEndTransfer;
  void*                       pParam;
} IP_COAP_CLIENT_OBS;

//
// Definition of a request made by the client.
//
typedef struct { // NOLINT(clang-analyzer-optin.performance.Padding) (Public struct)
  IP_COAP_HEADER_INFO     Header;
  IP_COAP_OPTIONS_INFO    Options;
  IP_COAP_OPTIONS_INFO    OptionsSaved;  // Workaround when using the "Options" member to parse received options
                                         // but we need to send more "original" options for example for further blocks.
  U32                     OptionsMask;
  //
  U8*                     pPayload;
  PF_CLIENT_PAYLOAD       pfPayloadCallback;
  U16                     PayloadLength;
  U8                      ConnStatus;
  U8                      Procedure;
  //
  // Retry.
  //
  U32                     UpdateTime;
  U32                     RetryTimeout;
  U8                      RetryNum;
  //
  // Observable context
  //
  IP_COAP_CLIENT_OBS*     pObs;
} IP_COAP_CLIENT_REQUEST;


/*********************************************************************
*
*       IP_COAP_CLIENT_CONTEXT
*
*  Description
*    Main context of the client. A client is dedicated to a server. If
*    it is needed to have activities with two servers during the same
*    period of time, then two clients are needed.
*
*  Additional information
*    This structure shouldn't be directly modified. Only APIs should be used.
*/
typedef struct IP_COAP_CLIENT_CONTEXT { // NOLINT(clang-analyzer-optin.performance.Padding) (Public struct)
  //
  // Buffer for the UDP packet handling (Rx/Tx).
  //
  U8*                     pMsgBuffer;                 // Buffer used to receive and send CoAP messages in UDP. Initialized in IP_COAP_CLIENT_Init().
  U16                     MsgBufferSize;              // Size of the message buffer.
  //
  // UDP transmission function.
  //
  IP_COAP_pfReceive       pfReceive;                  // Callback to receive UDP packets (see IP_COAP_pfReceive)
  IP_COAP_pfSend          pfSend;                     // Callback to send UDP packets (see IP_COAP_pfSend)
  IP_COAP_pfGetTimeMs     pfGetTimeMs;                // Callback to get the current time in milliseconds (see IP_COAP_pfGetTimeMs).
  //
  // Client parameters.
  //
  IP_COAP_CLIENT_REQUEST  aRequest[IP_COAP_NSTART];   // Structure used to describe a request to the server.
  IP_COAP_CONN_INFO       ConnInfo;                   // UDP connection information of the server. Set by calling IP_COAP_CLIENT_SetServerAddress().
  IP_COAP_CLIENT_OBS*     pFirstObs;                  // Start of the linked list of the active observers. Modified through IP_COAP_CLIENT_OBS_Init() and IP_COAP_CLIENT_OBS_Abort()..
  //
  // Internal data.
  //
  U8*                     pPayload;
  U16                     PayloadLength;
  U16                     MessageId;
  U8                      DefaultSzx;
} IP_COAP_CLIENT_CONTEXT;

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/

//
// Utility.
//
int IP_COAP_CheckAcceptFormat (IP_COAP_CALLBACK_PARAM* pParam, U16 Format);
int IP_COAP_GetAcceptFormat   (IP_COAP_CALLBACK_PARAM* pParam, U16* pFormat);
int IP_COAP_CheckContentFormat(IP_COAP_CALLBACK_PARAM* pParam, U16 Format, unsigned OptionMandatory);
int IP_COAP_GetContentFormat  (IP_COAP_CALLBACK_PARAM* pParam, U16* pFormat);
int IP_COAP_IsLastBlock       (IP_COAP_CALLBACK_PARAM* pParam, U8 Code);
int IP_COAP_GetURIHost        (IP_COAP_CALLBACK_PARAM* pParam, U8** ppHost, U8* pHostLength);
int IP_COAP_GetURIPath        (IP_COAP_CALLBACK_PARAM* pParam, U8** ppURI, U8* pURILength);
int IP_COAP_GetURIPort        (IP_COAP_CALLBACK_PARAM* pParam, U16* pURIPort);
int IP_COAP_GetQuery          (IP_COAP_CALLBACK_PARAM* pParam, U8** ppQuery, U16* pQueryLength);
int IP_COAP_GetETag           (IP_COAP_CALLBACK_PARAM* pParam, U8** ppETag, U8* pETagLength);
int IP_COAP_GetMaxAge         (IP_COAP_CALLBACK_PARAM* pParam, U32* pMaxAge);
int IP_COAP_GetSize1          (IP_COAP_CALLBACK_PARAM* pParam, U32* pSize1);
int IP_COAP_GetSize2          (IP_COAP_CALLBACK_PARAM* pParam, U32* pSize2);
int IP_COAP_GetLocationPath   (IP_COAP_CALLBACK_PARAM* pParam, U8** ppLoc, U8* pLocLength);
int IP_COAP_GetLocationQuery  (IP_COAP_CALLBACK_PARAM* pParam, U8** ppQuery, U16* pQueryLength);
int IP_COAP_GetObserve        (IP_COAP_CALLBACK_PARAM* pParam, U32* pObserve);

//
// Server.
//
int IP_COAP_SERVER_Init               (IP_COAP_SERVER_CONTEXT* pContext, U8* pMsgBuffer, U16 MsgBufferSize, const IP_COAP_API* pAPI);
int IP_COAP_SERVER_Process            (IP_COAP_SERVER_CONTEXT* pContext, IP_COAP_CONN_INFO* pConnInfo);
U8* IP_COAP_SERVER_GetMsgBuffer       (IP_COAP_SERVER_CONTEXT* pContext, U16* pMsgLength);
int IP_COAP_SERVER_AddData            (IP_COAP_SERVER_CONTEXT* pContext, IP_COAP_SERVER_DATA* pData);
int IP_COAP_SERVER_RemoveData         (IP_COAP_SERVER_CONTEXT* pContext, IP_COAP_SERVER_DATA* pData);
int IP_COAP_SERVER_AddClientBuffer    (IP_COAP_SERVER_CONTEXT* pContext, IP_COAP_SERVER_CLIENT_INFO* pClientInfo, unsigned NumClientInfo);
int IP_COAP_SERVER_AddObserverBuffer  (IP_COAP_SERVER_CONTEXT* pContext, IP_COAP_OBSERVER* pObs, unsigned NumObservers);
int IP_COAP_SERVER_UpdateData         (IP_COAP_SERVER_CONTEXT* pContext, IP_COAP_SERVER_DATA* pData, U8 ObsUpdateType, unsigned AutoETag);
int IP_COAP_SERVER_SetDefaultBlockSize(IP_COAP_SERVER_CONTEXT* pContext, U16 BlockSize);
int IP_COAP_SERVER_SetPOSTHandler     (IP_COAP_SERVER_CONTEXT* pContext, PF_POST_HANDLER pfPOSTCreateEntry);
int IP_COAP_SERVER_ConfigSet          (IP_COAP_SERVER_CONTEXT* pContext, U8 ConfigMask);
int IP_COAP_SERVER_ConfigClear        (IP_COAP_SERVER_CONTEXT* pContext, U8 ConfigMask);
int IP_COAP_SERVER_SetURIPort         (IP_COAP_SERVER_CONTEXT* pContext, U16 Port);
int IP_COAP_SERVER_SetHostName        (IP_COAP_SERVER_CONTEXT* pContext, const char* sHostName);
int IP_COAP_SERVER_SetErrorDescription(IP_COAP_SERVER_CONTEXT* pContext, const char* sErrorDesc);

//
// Client.
//
int IP_COAP_CLIENT_Init                  (IP_COAP_CLIENT_CONTEXT* pContext, U8* pMsgBuffer, U16 MsgBufferSize, const IP_COAP_API* pAPI);
int IP_COAP_CLIENT_Process               (IP_COAP_CLIENT_CONTEXT* pContext);
int IP_COAP_CLIENT_GetFreeRequestIdx     (IP_COAP_CLIENT_CONTEXT* pContext, unsigned* pIndex);
int IP_COAP_CLIENT_AbortRequestIdx       (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index);
int IP_COAP_CLIENT_SetServerAddress      (IP_COAP_CLIENT_CONTEXT* pContext, IP_COAP_CONN_INFO* pConnInfo);
int IP_COAP_CLIENT_SetDefaultBlockSize   (IP_COAP_CLIENT_CONTEXT* pContext, U16 BlockSize);
int IP_COAP_CLIENT_SetCommand            (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8 Type, U8 Code);
int IP_COAP_CLIENT_SetToken              (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8* pToken, U8 TokenLength);
int IP_COAP_CLIENT_SetPayloadHandler     (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, PF_CLIENT_PAYLOAD pf);
int IP_COAP_CLIENT_SetReplyWaitTime      (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U32 Seconds);
int IP_COAP_CLIENT_BuildAndSend          (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index);
int IP_COAP_CLIENT_GetLastResult         (IP_COAP_CLIENT_CONTEXT* pContext, U8* pCode, U8** ppError, U16* pLength);
U8* IP_COAP_CLIENT_GetMsgBuffer          (IP_COAP_CLIENT_CONTEXT* pContext, U16* pMsgLength);
int IP_COAP_CLIENT_GetLocationPath       (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8** ppLoc, U8* pLocLength);
int IP_COAP_CLIENT_GetLocationQuery      (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8** ppQuery, U8* pQueryLength);
int IP_COAP_CLIENT_SetOptionURIPath      (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8* pURI, U8 URILength);
int IP_COAP_CLIENT_SetOptionURIHost      (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8* pHost, U8 HostLength);
int IP_COAP_CLIENT_SetOptionURIPort      (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U16 Port);
int IP_COAP_CLIENT_SetOptionURIQuery     (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8* pQuery, U8 QueryLength);
int IP_COAP_CLIENT_SetOptionETag         (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8* pETag, U8 ETagLength);
int IP_COAP_CLIENT_SetOptionBlock        (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U16 Size);
int IP_COAP_CLIENT_SetOptionAccept       (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U16 Value);
int IP_COAP_CLIENT_SetOptionContentFormat(IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U16 Value);
int IP_COAP_CLIENT_SetOptionIfNoneMatch  (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index);
int IP_COAP_CLIENT_SetOptionLocationPath (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8* pLocation, U8 LocationLength);
int IP_COAP_CLIENT_SetOptionLocationQuery(IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8* pQuery, U8 QueryLength);
int IP_COAP_CLIENT_SetOptionProxyURI     (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8* pURI, U16 URILength);
int IP_COAP_CLIENT_SetOptionProxyScheme  (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U8* pScheme, U8 SchemeLength);
int IP_COAP_CLIENT_SetOptionSize1        (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, U32 Value);
int IP_COAP_CLIENT_SetOptionAddIFMatch   (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, IP_COAP_IF_MATCH_INFO* pIFMatch);
//
// Client observe.
//
int IP_COAP_CLIENT_OBS_Init             (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, IP_COAP_CLIENT_OBS* pObs, unsigned AutoToken);
int IP_COAP_CLIENT_OBS_Abort            (IP_COAP_CLIENT_CONTEXT* pContext, unsigned Index, IP_COAP_CLIENT_OBS* pObs, unsigned TryActiveAbort);
int IP_COAP_CLIENT_OBS_SetEndCallback   (IP_COAP_CLIENT_OBS* pObs, PF_OBS_END_TRANSFER pfObsEndTransfer, void* pParam);

#if defined(__cplusplus)
}                             // Make sure we have C-declarations in C++ programs.
#endif

#endif                        // Avoid multiple inclusion.

/*************************** End of file ****************************/
