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
Purpose     : Publics for the WebServer

Note:
  TimeDate is a 32 bit variable in the following format:
    Bit 0-4:   2-second count (0-29)
    Bit 5-10:  Minutes (0-59)
    Bit 11-15: Hours (0-23)
    Bit 16-20: Day of month (1-31)
    Bit 21-24: Month of year (1-12)
    Bit 25-31: Count of years from 1980 (0-127)
*/

#ifndef  IP_WEBS_H
#define  IP_WEBS_H

#ifdef __ICCARM__  // IAR
  #pragma diag_suppress=Pa029  // No warning for unknown pragmas in earlier versions of EWARM
  #pragma diag_suppress=Pa137  // No warning for C-Style-Casts with C++
#endif

/*********************************************************************
*
*       #include section
*
**********************************************************************
*/

#include "IP_FS.h"
#include "WEBS_Conf.h"

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

//
// Switches templating support on
//
#ifndef   WEBS_ENABLE_TEMPLATING
  #define WEBS_ENABLE_TEMPLATING             0
#endif

//
// CGI start and end strings
//
#if WEBS_ENABLE_TEMPLATING
  #ifndef WEBS_MARKER_START_STRING
      #define WEBS_MARKER_START_STRING  "<!--#exec "
  #endif
  #ifndef WEBS_MARKER_END_STRING
      #define WEBS_MARKER_END_STRING    "\"-->"
  #endif
  #ifndef WEBS_MARKER_CGI
      #define WEBS_MARKER_CGI           "cgi=\""
  #endif
  #ifndef WEBS_MARKER_TEMPLATE
      #define WEBS_MARKER_TEMPLATE      "tmpl=\""
  #endif
  #ifndef WEBS_TEMPLATE_TEMP_PATH_SIZE
    #define WEBS_TEMPLATE_TEMP_PATH_SIZE 64
  #endif
  //
  // Limits the depth of template nesting.
  // Each level of template nesting will cause two allocations (via pfAlloc from WEBS_SYS_API):
  // One alloc of WEBS_TEMP_BUFFER_SIZE size for the buffer
  // which will be used to send the file.
  // One alloc of WEBS_TEMPLATE_TEMP_PATH_SIZE for the buffer
  // for the file system path of the template.
  // Additionally, each level of nesting will open an additional file handle (via pfOpenFile from IP_FS_API).
  // When the read-only file system is used (FS_RO) the file handles are not relevant.
  //
  #ifndef   WEBS_MAX_RECURSION_DEPTH
    #define WEBS_MAX_RECURSION_DEPTH              2
  #endif
#else
  #ifndef WEBS_CGI_START_STRING
      #define WEBS_CGI_START_STRING  "<!--#exec cgi=\""
  #endif
  #ifndef WEBS_CGI_END_STRING
      #define WEBS_CGI_END_STRING    "\"-->"
  #endif
#endif


#ifndef   WEBS_IN_BUFFER_SIZE
  #define WEBS_IN_BUFFER_SIZE               256
#endif

#ifndef   WEBS_OUT_BUFFER_SIZE
  #define WEBS_OUT_BUFFER_SIZE              512
#endif

#ifndef   WEBS_TEMP_BUFFER_SIZE
  #define WEBS_TEMP_BUFFER_SIZE             512         // Used as file input buffer and for form parameters
#endif

#ifndef   WEBS_PARA_BUFFER_SIZE
  #define WEBS_PARA_BUFFER_SIZE             0           // Required for dynamic content parameter handling
#endif

#ifndef   WEBS_ERR_BUFFER_SIZE
  #define WEBS_ERR_BUFFER_SIZE              128         // Used in case of connection limit only
#endif

#ifndef   WEBS_AUTH_BUFFER_SIZE
  #define WEBS_AUTH_BUFFER_SIZE             32
#endif

#ifndef   WEBS_FILENAME_BUFFER_SIZE
  #define WEBS_FILENAME_BUFFER_SIZE         32
#endif

#ifndef   WEBS_UPLOAD_FILENAME_BUFFER_SIZE
  #define WEBS_UPLOAD_FILENAME_BUFFER_SIZE  64
#endif

#ifndef   WEBS_SUPPORT_UPLOAD
  #define WEBS_SUPPORT_UPLOAD               0
#endif

#ifndef   WEBS_URI_BUFFER_SIZE
  #define WEBS_URI_BUFFER_SIZE              0
#endif

#ifndef   WEBS_MAX_ROOT_PATH_LEN
  #define WEBS_MAX_ROOT_PATH_LEN            0
#endif

#ifndef   WEBS_STACK_SIZE_CHILD
  #define WEBS_STACK_SIZE_CHILD             (1536 + WEBS_IN_BUFFER_SIZE + WEBS_OUT_BUFFER_SIZE + WEBS_TEMP_BUFFER_SIZE + WEBS_PARA_BUFFER_SIZE + WEBS_AUTH_BUFFER_SIZE + WEBS_FILENAME_BUFFER_SIZE + WEBS_UPLOAD_FILENAME_BUFFER_SIZE)  // This size can not be guaranteed on all systems. Actual size depends on CPU & compiler
#endif

#ifndef   WEBS_USE_PARA
  #define WEBS_USE_PARA(Para)               (void)Para
#endif

#ifndef   WEBS_USE_AUTH_DIGEST
  #define WEBS_USE_AUTH_DIGEST              0
#endif

//
// This is a work in progress configuration that is disabled by default.
// Changes provided by this switch are planned to become defaults in
// an official release at some time in the future (will then be announced).
// Enabling enhanced form handling currently provides the following changes:
//   1) By default VFiles get their parameters via sParameters in a callback
//      instead of executing a CGI callback for each parameter/value pair.
//      In addition to sParameters the CGI callbacks will be executed as well
//      as they would for non-VFiles to use the same concept. The old concept
//      however can still be used as sParameters is still passed to VFile handlers.
//      This is not enabled by default as it might execute CGI callbacks that
//      were previously not executed automatically and might have been implemented
//      to be called from a VFile handler the user has implemented and would
//      therefore be executed twice.
//   2) Upload currently does not provide all form parameters and omits the
//      field name. Instead it adds "file=<UploadedFilename>&state=<UploadResult>"
//      to the end of the parameters in para buffer that is only handled
//      by a VFile. It is not possible to use uploading with a non-VFile.
//      This is changed by adding a callback that can be set that is then
//      executed for each uploaded file, letting the application know the
//      details about the upload like field name, status and size. The callback
//      return value decides if the uploaded file will be kept (== 0) or if the
//      Web server will delete it (!= 0).
//      The upload directory should be considered as a temporary folder
//      that the Web server will try to keep clean of unused files.
//      Therefore files uploaded should be taken care of by the upload
//      callback by moving them out of the "temporary" upload folder and
//      telling the Web server that the upload has been handled (by returning 0).
//
#ifndef   WEBS_USE_ENHANCED_FORM_HANDLING
  #define WEBS_USE_ENHANCED_FORM_HANDLING  0
#endif

//
// Use the PARA buffer for GET parameter parsing if HTTP authorization is used.
// Default is to evaluate GET parameters before authorization (if it is no VFile).
// This however is not secure if GET parameters are used as their callbacks get
// executed before the authentication check.
//
#ifndef   WEBS_AUTH_USE_GET_PARA_BUFFER
  #if WEBS_USE_ENHANCED_FORM_HANDLING
    #define WEBS_AUTH_USE_GET_PARA_BUFFER  1  // Use the PARA buffer for GET parameter parsing if HTTP authorization is used. Default for "new" API is to always parse GET parameters into PARA buffer and evaluate them later (after authentication check).
  #else
    #define WEBS_AUTH_USE_GET_PARA_BUFFER  0  // Use the PARA buffer for GET parameter parsing if HTTP authorization is used. Default is to evaluate GET parameters before authorization (if it is no VFile).
  #endif
#endif

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

//
// Macro overwrite for memcpy.
//
#ifndef   WEBS_MEMCPY
  #include <string.h>
  #define WEBS_MEMCPY  memcpy                          /*lint -e{9087,9005} */
#endif

//
// Macro overwrite for memset.
//
#ifndef   WEBS_MEMSET
  #include <string.h>
  #define WEBS_MEMSET  memset                          /*lint -e{9087,9005} */
#endif

//
// Macro overwrite for memcmp.
//
#ifndef   WEBS_MEMCMP
  #include <string.h>
  #define WEBS_MEMCMP  memcmp                          /*lint -e{9087,9005} */
  //lint -sem(memcmp, pure)          N:100
#endif

//
// Macro overwrite for memmove
//
#ifndef   WEBS_MEMMOVE
  #include <string.h>
  #define WEBS_MEMMOVE  memmove                         /*lint -e{9087,9005} */
#endif

//
// Macro overwrite for strlen
//
#ifndef   WEBS_STRLEN
  #include <string.h>
  #define WEBS_STRLEN   strlen
#endif

//
// Macro overwrite for strcat
//
#ifndef   WEBS_STRCAT
  #include <string.h>
  #define WEBS_STRCAT   strcat
#endif
//
// Macro overwrite for strncat
//
#ifndef   WEBS_STRNCAT
  #include <string.h>
  #define WEBS_STRNCAT   strncat
#endif

//
// Macro overwrite for strrchr
//
#ifndef   WEBS_STRRCHR
  #include <string.h>
  #define WEBS_STRRCHR  strrchr
#endif

//
// Macro overwrite for strcpy
//
#ifndef   WEBS_STRCPY
  #include <string.h>
  #define WEBS_STRCPY  strcpy
#endif

//
// Macro overwrite for strncpy
//
#ifndef   WEBS_STRNCPY
  #include <string.h>
  #define WEBS_STRNCPY  strncpy
#endif

//
// Macro overwrite for strcmp
//
#ifndef   WEBS_STRCMP
  #include <string.h>
  #define WEBS_STRCMP   strcmp
#endif

//
// Macro overwrite for strcasecmp
//
#ifndef   WEBS_STRCASECMP
  #define WEBS_STRCASECMP      SEGGER_strcasecmp
#endif

//
// Macro overwrite for strncasecmp
//
#ifndef   WEBS_STRNCASECMP
  #define WEBS_STRNCASECMP     SEGGER_strncasecmp
#endif

//
// Macro overwrite for snprintf
//
#ifndef   WEBS_SNPRINTF
  #define WEBS_SNPRINTF        SEGGER_snprintf
#endif

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#define OUT_BUFFER_PRE_SPACE  8    // Number of bytes before the actual out buffer. This allows us to prepend data with chunk info.

//
// Return values, error codes are in general negative
// and might not be listed here.
//
#define WEBS_OK                      0
#define WEBS_CONNECTION_DETACHED     1  // Returned by IP_WEBS_Process*() in case a WebSocket connection has been detached from the webserver context.
#define WEBS_HEADER_FIELD_UNTOUCHED  2  // Returned by WEBS_HEADER_FIELD_HOOK callbacks in case the callback only registered the call but did not call
                                        // further API that modified the line. The line will then be regularly processed by the webserver.
#define WEBS_NO_ERROR_PAGE_SENT      3  // The callback set to handle sending custom error pages has NOT sent out an error page, send the default content.



#define WEBS_CLIENT_INIT             0x00
#define WEBS_CLIENT_EXIT             0x01

/*********************************************************************
*
*       Data types
*
**********************************************************************
*/

enum {
  METHOD_NONE,
  METHOD_GET,
  METHOD_HEAD,
  METHOD_POST,
  METHOD_PUT,
  METHOD_PATCH,
  METHOD_DELETE
};

enum {
  HTTP_ENCODING_RAW,
  HTTP_ENCODING_CHUNKED,
  HTTP_ENCODING_FROM_CONTEXT
};

/*********************************************************************
*
*       WEBS_BUFFER_SIZES
*/
typedef struct {
  U32 NumBytesInBuf;
  U32 NumBytesOutBuf;
  U32 NumBytesFilenameBuf;
  U32 MaxRootPathLen;
  U32 NumBytesParaBuf;
  U32 NumBytesFullUriBuf;
} WEBS_BUFFER_SIZES;

typedef int   (*IP_WEBS_tSend)   (const unsigned char *pData, int len, void *pConnectInfo);
typedef int   (*IP_WEBS_tReceive)(      unsigned char *pData, int len, void *pConnectInfo);
typedef void *(*IP_WEBS_tAlloc)  (U32 NumBytesReq);
typedef void  (*IP_WEBS_tFree)   (void *p);

typedef void *WEBS_OUTPUT;

/*********************************************************************
*
*       WEBS_CGI
*
*  Description
*    Used to store the CGI command names and the pointer to the proper callback functions.
*
*  Additional information
*    Refer to Common Gateway Interface (CGI) on page 33 for detailed information about the use of this structure.
*/
typedef struct {
  const char *sName;  // Name of the CGI command (e.g. "Counter").
  void  (*pf)(WEBS_OUTPUT *pOutput, const char *sParameters, const char *sValue); // Pointer to a callback function.
} WEBS_CGI;

/*********************************************************************
*
*       WEBS_VFILES
*/
typedef struct {
  const char *sName;  // e.g. "Counter.cgi"
  void  (*pf)(WEBS_OUTPUT *pOutput, const char *sParameters);
} WEBS_VFILES;

/*********************************************************************
*
*       WEBS_ACCESS_CONTROL
*
*  Description
*    Used to store information for the HTTP Basic/Digest Authentication scheme.
*/
typedef struct {
  const char *sPath;             // A string which defines the path of the resources.
  const char *sRealm;            // A string which defines the realm which requires authentication. Optional, can be NULL.
  const char *sUserPass;         // A string containing the user name[/password] combination.
                                 // * Basic auth: "User:Pass".
                                 // * Digest auth: "User".
                                 // * Optional, can be NULL.
  const char *sCredentialsHash;  // A string containing the Digest credentials hash.
                                 // * Basic auth: Not used.
                                 // * Digest auth: MD5("User:Realm:Pass").
                                 // * Optional, can be NULL.
} WEBS_ACCESS_CONTROL;

/*********************************************************************
*
*       WEBS_APPLICATION
*
*  Description
*    Used to store application-specific parameters.
*/
typedef struct {
  const WEBS_CGI      *paCGI;     // Pointer to an array of structures of type WEBS_CGI.
  WEBS_ACCESS_CONTROL *paAccess;  // Pointer to an array of structures of type WEBS_ACCESS_CONTROL.
  void               (*pfHandleParameter)(WEBS_OUTPUT * pOutput, const char * sPara, const char * sValue); //Fallback callback in case paCGI is NULL.
  const WEBS_VFILES   *paVFiles;  // Pointer to an array of structures of type WEBS_VFILES.
} WEBS_APPLICATION;

/*********************************************************************
*
*       IP_WEBS_FILE_INFO
*
*  Description
*    Used to store file-specific parameters.
*/
typedef struct {
  const IP_FS_API* pFS_API;          // Allows override of FS API to use for a file.
        U32        DateLastMod;      // The date when the file has been last modified. Used for "Last modified" header field.
        U32        DateExp;          // The date of the expiration of the validity. Used for "Expires" header field.
        U8         IsVirtual;        // Flag to indicate if a file is virtual or not. Valid values are 0 for non-virtual, 1 for virtual files.
        U8         AllowDynContent;  // Flag to indicate if a file should be parsed for dynamic content or not. 0 means that the file should not be parsed for dynamic content,
                                     // 1 means that the file should be parsed for dynamic content. Guard to avoid unnecessary parsing of static files.
} IP_WEBS_FILE_INFO;


/*********************************************************************
*
*       WEBS_FILE_TYPE
*
*  Description
*    Used to extend or overwrite the file extension to MIME type correlation.
*/
typedef struct {
  const char *sExt;     // String containing the extension without leading dot.
  const char *sContent; // String containing the MIME type associated to the extension.
} WEBS_FILE_TYPE;


/*********************************************************************
*
*       WEBS_FILE_TYPE_HOOK
*
*  Description
*    Used to extend or overwrite the file extension to MIME type correlation.
*/
typedef struct WEBS_FILE_TYPE_HOOK {
  struct WEBS_FILE_TYPE_HOOK *pNext;    // Pointer to the previously registered element of WEBS_FILE_TYPE_HOOK.
         WEBS_FILE_TYPE       FileType; // Element of Structure WEBS_FILE_TYPE.
} WEBS_FILE_TYPE_HOOK;

/*********************************************************************
*
*       WEBS_IP_API
*/
typedef struct {
  IP_WEBS_tSend    pfSend;
  IP_WEBS_tReceive pfReceive;
} WEBS_IP_API;

/*********************************************************************
*
*       WEBS_SYS_API
*/
typedef struct {
  IP_WEBS_tAlloc   pfAlloc;
  IP_WEBS_tFree    pfFree;
} WEBS_SYS_API;

/*********************************************************************
*
*       WEBS_CONTEXT
*/
typedef struct {
  const WEBS_IP_API      *pIP_API;
  const WEBS_SYS_API     *pSYS_API;
  const IP_FS_API        *pFS_API;
  const WEBS_APPLICATION *pApplication;
        void             *pWebsPara;
        void             *pUpload;
        void             *pAppContext;
} WEBS_CONTEXT;

typedef void (*IP_WEBS_pfGetFileInfo)(const char *sFilename, IP_WEBS_FILE_INFO *pFileInfo);

/*********************************************************************
*
*       WEBS_VFILE_APPLICATION
*
*  Description
*    Used to check if the application can provide content for a simple VFile.
*/
typedef struct {
  int  (*pfCheckVFile)(const char *sFileName, unsigned *pIndex);  //  cPointer to a callback for checking if content for a requested file name can be served.
  void (*pfSendVFile) (void *pContextIn, unsigned Index, const char *sFileName, void (*pf)(void *pContextOut, const char *pData, unsigned NumBytes)); // Pointer to a callback for actually sending the
                                                                  // content for the requested file name using the provided callback pf. In case NumBytes is passed with 0 the callback
                                                                  // expects to find a string and will automatically run strlen() to find out the length of the string internally. In case
                                                                  // NumBytes is not passed >0 only NumBytes from the start of pData will be sent.
} WEBS_VFILE_APPLICATION;

/*********************************************************************
*
*       WEBS_VFILE_HOOK
*
*  Description
*    Used to send application generated content from the application upon request of a specific file name.
*/
typedef struct WEBS_VFILE_HOOK {
  struct WEBS_VFILE_HOOK        *pNext;         // Pointer to the previously registered element of WEBS_VFILE_HOOK.
         WEBS_VFILE_APPLICATION *pVFileApp;     // Pointer to an element of type WEBS_VFILE_APPLICATION.
         U8                      ForceEncoding; // Encoding to use.
                                                // * HTTP_ENCODING_RAW
                                                // * HTTP_ENCODING_CHUNKED
                                                // * HTTP_ENCODING_FROM_CONTEXT
} WEBS_VFILE_HOOK;

/*********************************************************************
*
*       WEBS_URI_HOOK
*
*  Description
*    There are multiple use cases for URI extension that might be used
*    for a more specific use case like a METHOD extension for REST
*    support or an implementation for WebSockets. What all these
*    implementations share is that they need a hook including a
*    next pointer and the URI to search for.
*    For an optimized code all specific use cases can be treated as
*    WEBS_URI_HOOK internally. For this to be able the first two
*    elements of all structures need to be the same and in same order.
*/
typedef struct WEBS_URI_HOOK {
  struct WEBS_URI_HOOK *pNext;
  const char           *sURI;
} WEBS_URI_HOOK;

enum {
  WEBS_URI_HOOK_TYPE_NONE = 0,
  WEBS_URI_HOOK_TYPE_METHOD,
  WEBS_URI_HOOK_TYPE_WEBSOCKET
};

//
// METHOD extension (variation of WEBS_URI_HOOK).
//
/*********************************************************************
*
*       IP_WEBS_pfMethod
*
*  Description
*    Callback used to extend the usage of METHODs in the Web server for a given URI.
*
*  Parameters
*    pContext     : METHOD context for incoming data used with IP_WEBS_METHOD_* routines.
*    pOutput      : Output context for IP_WEBS_* routines.
*    sMethod      : String containing METHOD requested by client.
*    sAccept      : String containing value of Accept field of header sent by client. May be NULL in case there was no such field.
*    sContentType : String containing value of Content-Type field of header sent by client. May be NULL in case there was no such field.
*    sResource    : String containing URI that was accessed.
*    ContentLen   : Length of data submitted by client that can be read. 0 in case no data was sent by client.
*/
typedef int (*IP_WEBS_pfMethod)(void *pContext, WEBS_OUTPUT *pOutput, const char *sMethod, const char *sAccept, const char *sContentType, const char *sResource, U32 ContentLen);

/*********************************************************************
*
*       WEBS_METHOD_HOOK
*
*  Description
*    Used to extend the usage of METHODs in the Web server for a given URI.
*/
typedef struct WEBS_METHOD_HOOK {
  struct WEBS_METHOD_HOOK* pNext;    // Pointer to the previously registered element of WEBS_METHOD_HOOK.
  const char*              sURI;     // URI registered for METHODs callback.
  const char*              sMethod;  // Pointer to a string with a single METHOD to register.
        IP_WEBS_pfMethod   pf;       // Pointer to callback handling the requested method of type Callback IP_WEBS_pfMethod.
} WEBS_METHOD_HOOK;

//
// WebSocket extension (variation of WEBS_URI_HOOK).
//

/*********************************************************************
*
*       IP_WEBS_WEBSOCKET_API
*
*  Description
*    Used for interaction between webserver and a WebSocket implementation.
*/
typedef struct {
  int  (*pfGenerateAcceptKey) (WEBS_OUTPUT* pOutput, void* pSecWebSocketKey, int SecWebSocketKeyLen, void* pBuffer, int BufferSize);  // Input/output buffer is the same. If this is not supported by your calculation callback the input buffer needs to be saved by your callback.
  void (*pfDispatchConnection)(WEBS_OUTPUT* pOutput, void* pConnection);                                                              // Dispatch the connection (/socket) handle from the web server to a WebSocket handler. From now on the new handler is responsible for it.
} IP_WEBS_WEBSOCKET_API;

/*********************************************************************
*
*       WEBS_WEBSOCKET_HOOK
*/
typedef struct WEBS_WEBSOCKET_HOOK {
  struct WEBS_WEBSOCKET_HOOK  *pNext;
  const char*                  sURI;    // Resource location to assign WebSocket functionality to.
  const char*                  sProto;  // Single WebSocket protocol assigned to this hook. For more protocols add a hook with the same sURI and another protocol.
  const IP_WEBS_WEBSOCKET_API *pAPI;    // IP_WEBS_WEBSOCKET_API to use for this hook.
} WEBS_WEBSOCKET_HOOK;

/*********************************************************************
*
*       WEBS_INDEX_AUTOCOMPLETE_ITEM
*
*  Description
*    List item to autocomplete an index request (URI ending with a slash '/').
*    Used as an array of items with a NULL entry terminating the list.
*/
typedef struct {
  const char* sPath;  // Path to add after the slash.
} WEBS_INDEX_AUTOCOMPLETE_ITEM;

/*********************************************************************
*
*       WEBS_INDEX_RULE
*
*  Function description
*    Rule for handling an index request (URI ending with a slash '/').
*    Used as an array of items with a NULL entry terminating the list.
*/
typedef struct {
  const char*                         sURI;           // String with URI this rule applies to (including sub-folders).
                                                      // Do not use the trailing slash, e.g. "/sub/folder" but use the root slash "/".
  const WEBS_INDEX_AUTOCOMPLETE_ITEM* pAutocomplete;  // Pointer to NULL entry terminated array of index autocomplete items.
} WEBS_INDEX_RULE;

/*********************************************************************
*
*       WEBS_HEADER_FIELD_INFO
*/
typedef struct {
        WEBS_OUTPUT* pOutput;
  const char*        sField;   // The header field that the hook has been registered to.
} WEBS_HEADER_FIELD_INFO;

typedef int (*IP_WEBS_pfHeaderField)(WEBS_HEADER_FIELD_INFO* pInfo);

/*********************************************************************
*
*       WEBS_HEADER_FIELD_HOOK
*/
typedef struct WEBS_HEADER_FIELD_HOOK {
  struct WEBS_HEADER_FIELD_HOOK *pNext;
        IP_WEBS_pfHeaderField    pf;
  const char*                    sField;
} WEBS_HEADER_FIELD_HOOK;

/*********************************************************************
*
*       WEBS_REQUEST_NOTIFY_INFO
*
*  Description
*    Used to pass information to a callback registered to be notified upon a request to the Web server.
*/
typedef struct {
  const char*  sUri;    // Pointer to string containing the requested location.
  WEBS_OUTPUT* pOutput; // Connection context.
        U8     Method;  // HTTP METHOD used in the request:
                        // * METHOD_GET
                        // * METHOD_HEAD
                        // * METHOD_POST
} WEBS_REQUEST_NOTIFY_INFO;

/*********************************************************************
*
*       IP_WEBS_pfRequestNotify
*
*  Description
*    Called upon a request to the Web server.
*
*  Parameters
*    pInfo : Pointer to structure containing information about the current request being handled of type Structure WEBS_REQUEST_NOTIFY_INFO.
*/
typedef void (*IP_WEBS_pfRequestNotify)(WEBS_REQUEST_NOTIFY_INFO* pInfo);

/*********************************************************************
*
*       WEBS_REQUEST_NOTIFY_HOOK
*
*  Description
*    Used to get notified of incoming requests to the Web server.
*/
typedef struct WEBS_REQUEST_NOTIFY_HOOK {
  struct WEBS_REQUEST_NOTIFY_HOOK *pNext; // Pointer to the previously registered element of WEBS_REQUEST_NOTIFY_HOOK.
         IP_WEBS_pfRequestNotify   pf;    // Pointer to callback handling the requested method of type Callback IP_WEBS_pfRequestNotify.
} WEBS_REQUEST_NOTIFY_HOOK;


/*********************************************************************
*
*       WEBS_AUTH_FAILED_INFO
*
*  Description
*    Used to pass information to a callback registered to be informed
*    about an unsuccessful authentication attempt.
*/
typedef struct {
  const WEBS_ACCESS_CONTROL *pNeedsAuth;  // Pointer to WEBS_ACCESS_CONTROL entry.
  const char                *acUserPass;  // String containing user:pass combination from request.
  const char                *pPath;       // Contains URI path.
} WEBS_AUTH_FAILED_INFO;

/*********************************************************************
*
*       IP_WEBS_pfAuthFailed
*
*  Description
*    Called upon a failed authentication attempt.
*
*  Parameters
*    pInfo : Pointer to structure containing information about the current request being handled of type Structure WEBS_AUTH_FAILED_INFO.
*/
typedef void (*IP_WEBS_pfAuthFailed)(WEBS_AUTH_FAILED_INFO* pInfo); // Pointer to structure containing information about the current request being handled of type Structure WEBS_AUTH_FAILED_INFO.

//
// Request progress status identifier.
//
typedef enum {
  WEBS_PROGRESS_STATUS_BEGIN = 0u,
  WEBS_PROGRESS_STATUS_METHOD_URI_VER_PARSED,
  WEBS_PROGRESS_STATUS_HEADER_PARSED,
  WEBS_PROGRESS_STATUS_USER_AUTH,  // See "WEBS_PROGRESS_INFO.Data.UserAuth" for further details.
                                   // Currently only successful authentication attempts are reported.
  WEBS_PROGRESS_STATUS_END
} WEBS_PROGRESS_STATUS;

//
// Possible authorization events signaled via WEBS_PROGRESS_INFO .
//
typedef enum {
  WEBS_USER_AUTH_EVENT_SUCCESS = 0u   // The user successfully authorized for the requested resource.
} WEBS_USER_AUTH_EVENT;

/*********************************************************************
*
*       WEBS_USER_AUTH_INFO
*
*  Function description
*    Returns information about user authentication events.
*    Currently only successful auth. attempts are reported.
*/
typedef struct {
  const WEBS_ACCESS_CONTROL* pAccess;  // Pointer to the access control list entry that the user authenticated against.
                                       // Can be NULL if not for a successful authentication attempt.
        WEBS_USER_AUTH_EVENT Event;    // Authorization event that has happened.
                                       // * WEBS_USER_AUTH_EVENT_SUCCESS
} WEBS_USER_AUTH_INFO;

/*********************************************************************
*
*       WEBS_PROGRESS_INFO
*
*  Description
*    Used to pass information to a callback registered to be notified about the progression.
*/
typedef struct {
  WEBS_OUTPUT*          pOutput;  // Connection context.
  WEBS_PROGRESS_STATUS  Status;   // Current progression status:
                                  // * WEBS_PROGRESS_STATUS_BEGIN
                                  // * WEBS_PROGRESS_STATUS_METHOD_URI_VER_PARSED
                                  // * WEBS_PROGRESS_STATUS_HEADER_PARSED
                                  // * WEBS_PROGRESS_STATUS_USER_AUTH
                                  // * WEBS_PROGRESS_STATUS_END
  union {
    WEBS_USER_AUTH_INFO UserAuth;
  } Data;                         // Contains additional information for the state if available. At the moment only WEBS_PROGRESS_STATUS_USER_AUTH has additional information.
                                  // For this status please refer to WEBS_USER_AUTH_INFO for the additional information available.
} WEBS_PROGRESS_INFO;

/*********************************************************************
*
*       IP_WEBS_pfProgress
*
*  Description
*    Called to get notified of the current progression of the requests served.
*
*  Parameters
*    pInfo : Pointer to structure containing information about the current request being handled of type Structure WEBS_PROGRESS_INFO.
*/
typedef void (*IP_WEBS_pfProgress)(WEBS_PROGRESS_INFO* pInfo);

/*********************************************************************
*
*       WEBS_PROGRESS_HOOK
*
*  Description
*    Used to get notified of the current progression of the requests served.
*/
typedef struct WEBS_PROGRESS_HOOK {
  struct WEBS_PROGRESS_HOOK *pNext; // Pointer to the previously registered element of WEBS_PROGRESS_HOOK.
         IP_WEBS_pfProgress  pf;    // Pointer to callback handling the requested method of type Callback IP_WEBS_pfProgress.
} WEBS_PROGRESS_HOOK;

//
// Pre-content output hook.
//
/*********************************************************************
*
*       IP_WEBS_pfPreContentOutput
*
*  Description
*    Called before content is generated and sent by the Web server in regular cases like VFiles
*    and files from a filesystem.
*
*  Parameters
*    pOutput : Output context for IP_WEBS_* routines.
*/
typedef int (*IP_WEBS_pfPreContentOutput)(WEBS_OUTPUT* pOutput);  // Return value: 0: O.K., send content as usual; 1: Suppress output of further content.

/*********************************************************************
*
*       WEBS_PRE_CONTENT_OUTPUT_HOOK
*
*  Description
*    Used to intercept the Web server before content is generated and sent.
*/
typedef struct WEBS_PRE_CONTENT_OUTPUT_HOOK {
  struct WEBS_PRE_CONTENT_OUTPUT_HOOK* pNext; // Pointer to the previously registered element of WEBS_PRE_CONTENT_OUTPUT_HOOK.
         IP_WEBS_pfPreContentOutput    pf;    // Pointer to callback of type Callback IP_WEBS_pfPreContentOutput.
} WEBS_PRE_CONTENT_OUTPUT_HOOK;

#define WEBS_PRE_DYNAMIC_CONTENT_OUTPUT  (1uL << 0)

enum {
  WEBS_UPLOAD_OK = 0,             // Upload of file succeeded.
  WEBS_UPLOAD_ERR_MISC,           // Error states that have no own return code.
  WEBS_UPLOAD_ERR_PARSE,          // Parsing error.
  WEBS_UPLOAD_ERR_CREATE,         // Error creating upload file.
  WEBS_UPLOAD_ERR_WRITE,          // Error writing to upload file.
  WEBS_UPLOAD_ERR_MAX_FILE_SIZE   // Maximum file size for upload exceeded.
};

typedef void* WEBS_AUTH_DIGEST_OUTPUT;

/*********************************************************************
*
*       WEBS_AUTH_DIGEST_APP_API
*
*  Description
*    Used to interact with the application when Digest Authentication is used.
*/
typedef struct {
  void (*pfStoreNonce)(WEBS_OUTPUT* pOutput, WEBS_AUTH_DIGEST_OUTPUT* pDigestOutput, void (*pfStore)(WEBS_OUTPUT* pOutput, WEBS_AUTH_DIGEST_OUTPUT* pDigestOutput, const char* pNonce, unsigned NonceLen), int GenerateNew);
  //
  // Place for further extensions like pfStoreCNonce .
  //
} WEBS_AUTH_DIGEST_APP_API;

/*********************************************************************
*
*       WEBS_SEND_ERROR_PAGE_INFO
*
*  Description
*    Main info structure about the reason why an error page needs to be sent.
*/
typedef struct {
  WEBS_OUTPUT* pOutput;
  const char*  sErrHeader;
  const char*  sErrBody;
  U16          ErrorCode;
} WEBS_SEND_ERROR_PAGE_INFO;

//
// Custom error page callback.
//
typedef int (*IP_WEBS_pfSendErrorPage)(WEBS_SEND_ERROR_PAGE_INFO* pInfo);  // Return value: WEBS_NO_ERROR_PAGE_SENT: let webserver handle the reply; WEBS_OK: Suppress output of further content, page has been sent.

/*********************************************************************
*
*       WEBS_HEADER_CONFIG
*
*  Description
*    Used by IP_WEBS_SendHeader[Ex]() internally or with WEBS_USE_ENHANCED_FORM_HANDLING public.
*/
typedef struct {
  const char* sFileName;   // Filename that will be delivered. Extension is used to determine MIME type.
  const char* sMIMEType;   // Override the MIME type based on the filename extension. Can be NULL
  const char* sAddFields;  // String with additional header fields. Can be NULL.
  const char* sStartLine;  // "HTTP/1.1 200 OK" type start line. Can be NULL, in this case the shown start line will be used.
        U8    ReqKeepCon;  // Typically the connection will be closed on custom headers. Try to keep the connection open if possible (not HTTP1.0 and not ProcessLast).
                           //   0: Close connection, after data transmission.
                           //   1: If possible, keep connection open after data transmission.
} WEBS_HEADER_CONFIG;

/*********************************************************************
*
*       WEBS_STATIC_ENCODED_FILETYPES
*
*  Function description
*    Configures a list of filetypes for which static compressed
*    content is delivered if available.
*/
typedef struct {
  const char* sEncoding;        // Encoding as used by the browser in its "Accept-Encoding" field, such as "gzip".
  const char* sExtension;       // Extension including period letter to add to the end of the URI requested.
                                // The resulting URI will be tried to open and sent back gzip compressed.
  U8          ReplaceLastChar;  // Replace the last character of the requested URI with the first character of the extension.
} WEBS_STATIC_ENCODED_FILETYPES;

//
// Enhanced form handling.
//
#if WEBS_USE_ENHANCED_FORM_HANDLING

/*********************************************************************
*
*       WEBS_UPLOAD_INFO
*
*  Function description
*    Provides information about afile that has just been uploaded.
*/
typedef struct WEBS_UPLOAD_INFO {
  const char* sFieldName;     // The HTTP form field name of the <input> tag.
  const char* sTempFilename;  // The temporary upload filename. This is either the original filename as submitted or
                              // the modified name from IP_WEBS_pfModifyTempFilename .
  const char* sFilename;      // The originally submitted upload filename if not comletely replaced using IP_WEBS_pfModifyTempFilename .
                              // If completely replaced, this is the same as sTempFilename .
  const char* sPath;          // Complete path to the uploaded file.
        U32   Size;           // Size of the uploaded file in bytes.
        U8    Status;         // Status of the upload:
                              //   * Upload succeeded: WEBS_UPLOAD_OK
                              //   * Error           : WEBS_UPLOAD_ERR_*
} WEBS_UPLOAD_INFO;

/*********************************************************************
*
*       IP_WEBS_pfOnUpload()
*
*  Function description
*    Callback executed for each uploaded file.
*
*  Parameters
*    pOutput: Connection context.
*    pInfo  : Pointer to a WEBS_UPLOAD_INFO structure with further
*             information about the uploaded file.
*
*  Return value
*    == 0: Keep the uploaded file.
*    != 0: Delete the uploaded file.
*/
typedef int (*IP_WEBS_pfOnUpload)(WEBS_OUTPUT* pOutput, WEBS_UPLOAD_INFO* pInfo);

/*********************************************************************
*
*       IP_WEBS_pfModifyTempFilename()
*
*  Function description
*    Optional callback executed before each uploaded file that allows
*    modifying the upload filename.
*
*  Parameters
*    pOutput   : Connection context.
*    p         : Pointer to after the upload root path, e.g. "/upload/"
*                (on the '\0').
*    BufferSize: Characters available to be used to set an upload  filename.
*
*  Return value
*    Number of bytes used for the filename including '\0' if used to
*    set an absolute filename instead of prefixing the filename.
*
*  Additional information
*    The original filename can be fetched using IP_WEBS_GetUploadFilename() .
*    Please make sure that characters added plus the original filename
*    do not exceed the available BufferSize .
*
*    Two modifications can be applied to the original filename:
*      1) Storing non-terminated characters into the buffer to prefix the
*         original filename. Example:
*           - "/upload/" is used as upload root path and is stored in the path buffer.
*           - The callback stores the characters "Temp_" without '\0' into the buffer.
*           - The original filename will be added to the buffer resulting in a path
*             like "/upload/Temp_Filename.txt" .
*         The characters added should be random to generate a unique
*         temp filename per Web server child thread. A simple implementation
*         can use the address of a variable that is on the current task stack
*         and prefix it using the callback.
*      2) Storing a terminated string into the buffer to completely change the
*         temporary filename. Example:
*           - "/upload/" is used as upload root path and is stored in the path buffer.
*           - The callback stores the characters "Temp.bin" including '\0' into the buffer.
*           - The temporary path now results in "/upload/Temp.bin" .
*/
typedef int (*IP_WEBS_pfModifyTempFilename)(WEBS_OUTPUT* pOutput, char* p, U32 BufferSize);

typedef struct WEBS_UPLOAD_API {
  IP_WEBS_pfOnUpload           pfOnUpload;
  IP_WEBS_pfModifyTempFilename pfModifyTempFilename;  // Can be NULL.
} WEBS_UPLOAD_API;


#endif

/*********************************************************************
*
*       Extern variables and helper functions for samples.
*/

extern const WEBS_APPLICATION       WebsSample_Application;
extern const WEBS_VFILE_APPLICATION WebsSample_UPnP_VFileAPI;

void WebsSample_SendPageFooter(WEBS_OUTPUT* pOutput);
void WebsSample_SendPageHeader(WEBS_OUTPUT* pOutput, const char* sName);

//
// IP_WEBS_X_SampleConfig() is a helper function used in web server
// samples. It is used as central point to configure sample
// functionality like adding a REST resource or switching between
// Basic and Digest authentication.
//
void IP_WEBS_X_SampleConfig(void);

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/

//
// General Web Server API functions
//
      void   IP_WEBS_Init                        (WEBS_CONTEXT *pContext, const WEBS_IP_API *pIP_API, const WEBS_SYS_API *pSYS_API, const IP_FS_API *pFS_API, const WEBS_APPLICATION *pApplication);
      int    IP_WEBS_AddUpload                   (void);
      int    IP_WEBS_ProcessEx                   (WEBS_CONTEXT *pContext, void *pConnectInfo, const char *sRootPath);
      int    IP_WEBS_ProcessLastEx               (WEBS_CONTEXT *pContext, void *pConnectInfo, const char *sRootPath);
      void   IP_WEBS_ConfigBufSizes              (const WEBS_BUFFER_SIZES *pBufferSizes);
      U32    IP_WEBS_CountRequiredMem            (WEBS_CONTEXT* pContext);
      int    IP_WEBS_Flush                       (WEBS_OUTPUT *pOutput);

      int    IP_WEBS_Process                     (IP_WEBS_tSend pfSend, IP_WEBS_tReceive pfReceive, void *pConnectInfo, const IP_FS_API *pFS_API, const WEBS_APPLICATION *pApplication);
      int    IP_WEBS_ProcessLast                 (IP_WEBS_tSend pfSend, IP_WEBS_tReceive pfReceive, void *pConnectInfo, const IP_FS_API *pFS_API, const WEBS_APPLICATION *pApplication);
      void   IP_WEBS_OnConnectionLimit           (IP_WEBS_tSend pfSend, IP_WEBS_tReceive pfReceive, void *pConnectInfo);

      void * IP_WEBS_GetConnectInfo              (WEBS_OUTPUT *pOutput);
      void * IP_WEBS_GetAppContext               (WEBS_OUTPUT *pOutput);
const char * IP_WEBS_GetProtectedPath            (WEBS_OUTPUT* pOutput);
const char * IP_WEBS_GetURI                      (WEBS_OUTPUT *pOutput, char GetFullURI);
      int    IP_WEBS_Redirect                    (WEBS_OUTPUT *pOutput, const char *sFileName, const char *sMimeType);
      void   IP_WEBS_Reset                       (void);
      void * IP_WEBS_RetrieveUserContext         (WEBS_OUTPUT *pOutput);
      int    IP_WEBS_Send204NoContent            (WEBS_OUTPUT* pOutput);
#if (WEBS_USE_ENHANCED_FORM_HANDLING == 0)
      int    IP_WEBS_SendHeader                  (WEBS_OUTPUT *pOutput, const char *sFileName, const char *sMimeType);
      int    IP_WEBS_SendHeaderEx                (WEBS_OUTPUT *pOutput, const char *sFileName, const char *sMIMEType, U8 ReqKeepCon);
#endif
      int    IP_WEBS_SendMem                     (WEBS_OUTPUT *pOutput, const char *s, unsigned NumBytes);
      int    IP_WEBS_SendFormattedString         (WEBS_OUTPUT *pOutput, const char *sFormat, ...);
      int    IP_WEBS_SendString                  (WEBS_OUTPUT *pOutput, const char *s);
      int    IP_WEBS_SendStringEnc               (WEBS_OUTPUT *pOutput, const char *s);
      int    IP_WEBS_SendUnsigned                (WEBS_OUTPUT *pOutput, unsigned v, unsigned Base, int NumDigits);
      void   IP_WEBS_StoreUserContext            (WEBS_OUTPUT *pOutput, void *pContext);
      void   IP_WEBS_UseRawEncoding              (WEBS_OUTPUT* pOutput);
      void   IP_WEBS_MarkSendingCustomHeader     (WEBS_OUTPUT* pOutput, void* p);

      void   IP_WEBS_ConfigFindGZipFiles         (const char* sExtension, int ReplaceLastChar);
      void   IP_WEBS_ConfigIndexRules            (const WEBS_INDEX_RULE* pRules);
      int    IP_WEBS_ConfigRootPath              (const char* sRootPath);
      void   IP_WEBS_ConfigStaticEncodedFiletypes(const WEBS_STATIC_ENCODED_FILETYPES* paList);
      int    IP_WEBS_ConfigUploadRootPath        (const char* sUploadRootPath);
      void   IP_WEBS_ConfigSendVFileHeader       (U8 OnOff);
      void   IP_WEBS_ConfigSendVFileHookHeader   (U8 OnOff);
      int    IP_WEBS_GetDecodedStrLen            (const char *sBuffer, int Len);
      int    IP_WEBS_GetNumParas                 (const char *sParameters);
      int    IP_WEBS_GetParaValue                (const char *sBuffer, int ParaNum,       char   *sPara, int   ParaLen,       char   *sValue, int   ValueLen);
      int    IP_WEBS_GetParaValuePtr             (const char *sBuffer, int ParaNum, const char **ppPara, int *pParaLen, const char **ppValue, int *pValueLen);
      void   IP_WEBS_DecodeAndCopyStr            (char *pDest, int DestLen, const char *pSrc, int SrcLen);
      int    IP_WEBS_DecodeString                (const char *s);
      void   IP_WEBS_SendLocationHeader          (WEBS_OUTPUT* pOutput, const char* sURI, const char* sCodeDesc);
      void   IP_WEBS_SetErrorPageCallback        (IP_WEBS_pfSendErrorPage pf, void* pConfig);
      void   IP_WEBS_SetFileInfoCallback         (IP_WEBS_pfGetFileInfo pf);
      void   IP_WEBS_SetAuthErrorCallback        (IP_WEBS_pfAuthFailed pf);
      void   IP_WEBS_SetHeaderCacheControl       (const char* sCacheControl);
      void   IP_WEBS_SetUploadFileSystemAPI      (const IP_FS_API* pFS_API);
      void   IP_WEBS_SetUploadMaxFileSize        (U32 NumBytes);
      char   IP_WEBS_CompareFilenameExt          (const char *sFilename, const char *sExt);

      void   IP_WEBS_AddFileTypeHook             (WEBS_FILE_TYPE_HOOK *pHook, const char *sExt, const char *sContent);
      void   IP_WEBS_AddProgressHook             (WEBS_PROGRESS_HOOK* pHook, IP_WEBS_pfProgress pf);
      void   IP_WEBS_AddRequestNotifyHook        (WEBS_REQUEST_NOTIFY_HOOK* pHook, IP_WEBS_pfRequestNotify pf);
      void   IP_WEBS_AddPreContentOutputHook     (WEBS_PRE_CONTENT_OUTPUT_HOOK* pHook, IP_WEBS_pfPreContentOutput pf, U32 Mask);
      void   IP_WEBS_SetOnClientCallback         (void (*pfOnClient)(WEBS_CONTEXT *pWebContext, int Opt));

//
// Web Server VFile extension API functions
//
      void   IP_WEBS_AddVFileHook                (WEBS_VFILE_HOOK *pHook, WEBS_VFILE_APPLICATION *pVFileApp, U8 ForceEncoding);

//
// Web Server METHOD extension API functions
//
      void   IP_WEBS_METHOD_AddHook              (WEBS_METHOD_HOOK *pHook, IP_WEBS_pfMethod pf, const char *sURI);
      void   IP_WEBS_METHOD_AddHook_SingleMethod (WEBS_METHOD_HOOK *pHook, IP_WEBS_pfMethod pf, const char *sURI, const char *sMethod);
      int    IP_WEBS_METHOD_CopyData             (void *pContext, void *pBuffer, unsigned BufferSize);

//
// Web Server AUTH DIGEST API functions
//
      void   IP_WEBS_UseAuthDigest               (const WEBS_AUTH_DIGEST_APP_API* pAPI);
      void   IP_WEBS_AUTH_DIGEST_CalcHA1         (const char* pInput, unsigned InputLen, char* pBuffer, unsigned BufferSize);
      void   IP_WEBS_AUTH_DIGEST_GetURI          (WEBS_OUTPUT* pOutput, WEBS_AUTH_DIGEST_OUTPUT* pDigestOutput, char* pBuffer, unsigned* pNumBytes);

//
// Web Server WebSocket API functions
//
      void   IP_WEBS_WEBSOCKET_AddHook           (WEBS_WEBSOCKET_HOOK* pHook, const IP_WEBS_WEBSOCKET_API* pAPI, const char* sURI, const char* sProto);

//
// Web Server HEADER extension API functions
//
      void   IP_WEBS_HEADER_AddFieldHook         (WEBS_OUTPUT* pOutput, WEBS_HEADER_FIELD_HOOK* pHook, IP_WEBS_pfHeaderField pf, const char* sField);
      int    IP_WEBS_HEADER_CopyData             (const WEBS_OUTPUT* pOutput, void* pBuffer, unsigned BufferSize, unsigned* pNumBytesLeft);
      int    IP_WEBS_HEADER_GetFindToken         (const WEBS_OUTPUT* pOutput, const char* sToken, int TokenLen, char* pBuffer, int BufferSize);
      void   IP_WEBS_HEADER_SetCustomFields      (WEBS_OUTPUT* pOutput, const char* sAddFields);

//
// Enhanced form handling API functions.
//
#if (WEBS_USE_ENHANCED_FORM_HANDLING != 0)
      void     IP_WEBS_SetUploadAPI              (const WEBS_UPLOAD_API* pAPI);
      unsigned IP_WEBS_GetUploadFilename         (WEBS_OUTPUT* pOutput, char* pBuffer, U32 BufferSize);
      void     IP_WEBS_ChangeUploadMaxFileSize   (WEBS_OUTPUT* pOutput, U32 MaxFileSize);
      int      IP_WEBS_SendHeader                (WEBS_OUTPUT* pOutput, const WEBS_HEADER_CONFIG* pConfig);
#endif


#if defined(__cplusplus)
  }
#endif


#endif   /* Avoid multiple inclusion */

/*************************** End of file ****************************/
