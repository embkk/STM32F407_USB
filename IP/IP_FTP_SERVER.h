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
Purpose : FTP server add-on header file.
*/

#ifndef IP_FTP_SERVER_H       // Avoid multiple inclusion.
#define IP_FTP_SERVER_H

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

#include "IP_FTP_SERVER_Conf.h"
#include "SEGGER.h"
#include "IP_FS.h"

#if defined(__cplusplus)
  extern "C" {                // Make sure we have C-declarations in C++ programs.
#endif

/*********************************************************************
*
*       Defines, configurable
*
**********************************************************************
*/

#ifndef   FTPS_USE_PARA                             // Some compiler complain about unused parameters.
  #define FTPS_USE_PARA(Para)           (void)Para  // This works for most compilers.
#endif


//
// Defines below are legacy and should not be used!
// Use IP_FTPS_ConfigBufSizes() instead.
//
#ifndef   FTPS_BUFFER_SIZE
  #define FTPS_BUFFER_SIZE              512  // Default size for in and out buffers. The size of the in and out buffers
#endif                                       // has direct impact on the performance. You can increase the buffer size
                                             // depending on the size of the used MTU. The buffer should be a multiple
                                             // of the MTU size minus FTP/IPv4/TCP/VLAN header sizes for optimal performance.

#ifndef   FTPS_MAX_PATH
  #define FTPS_MAX_PATH                 128  // Max. length of complete path including directory and filename.
#endif

#ifndef   FTPS_MAX_PATH_DIR
  #define FTPS_MAX_PATH_DIR             128  // Max. length of dirname (path without filename).
#endif

#ifndef   FTPS_MAX_FILE_NAME
  #define FTPS_MAX_FILE_NAME            13   // The default is 13 characters because filenames can not be longer than an 8.3 without
#endif                                       // long file name support. 8.3 + 1 character for string termination.

/*********************************************************************
*
*       Defines, fixed
*
**********************************************************************
*/

#define IP_FTPS_PERM_VISIBLE  (1u << 0)
#define IP_FTPS_PERM_READ     (1u << 1)
#define IP_FTPS_PERM_WRITE    (1u << 2)

/*********************************************************************
*
*       Data types
*
**********************************************************************
*/

typedef void* FTPS_SOCKET;
typedef void* FTPS_OUTPUT;

typedef enum {
  FTPS_CMD_OPERATION_NONE,      // No FTP Operation.
  FTPS_CMD_OPERATION_NOOP,      // NOOP (No operation): The client requests no action, often used to keep the connection alive.
  FTPS_CMD_OPERATION_PASS,      // PASS (Password): The client provides the password for authentication.
  FTPS_CMD_OPERATION_QUIT,      // QUIT (Quit): The client requests to terminate the connection.
  FTPS_CMD_OPERATION_FEAT,      // FEAT (Features): The client requests a list of server-supported features.
  FTPS_CMD_OPERATION_SYST,      // SYST (System): The client requests information about the system type (e.g., UNIX, Windows).
  FTPS_CMD_OPERATION_TYPE,      // TYPE (Type): The client specifies the transfer type (ASCII or binary).
  FTPS_CMD_OPERATION_AUTH,      // AUTH (Authentication): The client requests a specific authentication method.
  FTPS_CMD_OPERATION_PROT,      // PROT (Protection Level): The client sets the protection level for the data connection.
  FTPS_CMD_OPERATION_PBSZ,      // PBSZ (Protection Buffer Size): The client requests the buffer size for secure data transfer.
  FTPS_CMD_OPERATION_USER,      // USER (Username): The client provides the username for authentication.
  FTPS_CMD_OPERATION_CDUP,      // CDUP (Change to Parent Directory): The client requests to move to the parent directory.
  FTPS_CMD_OPERATION_CWD,       // CWD (Change Working Directory): The client requests to change the current directory.
  FTPS_CMD_OPERATION_EPRT,      // EPRT (Extended Port): The client provides the extended port number for the data connection.
  FTPS_CMD_OPERATION_EPSV,      // EPSV (Extended Passive Mode): The client requests passive mode with extended features.
  FTPS_CMD_OPERATION_DELE,      // DELE (Delete): The client requests to delete a file or directory.
  FTPS_CMD_OPERATION_LIST,      // LIST (List): The client requests a listing of files and directories.
  FTPS_CMD_OPERATION_MLST,      // MLST (Machine-Readable List): The client requests a detailed listing of files, including metadata.
  FTPS_CMD_OPERATION_MLSD,      // MLSD (Machine-Readable Directory): The client requests a machine-readable listing of directory contents.
  FTPS_CMD_OPERATION_MKD,       // MKD (Make Directory): The client requests to create a new directory.
  FTPS_CMD_OPERATION_NLST,      // NLST (Name List): The client requests a simple list of file names.
  FTPS_CMD_OPERATION_PASV,      // PASV (Passive Mode): The client requests the server to open a passive data connection.
  FTPS_CMD_OPERATION_PORT,      // PORT (Active Mode): The client requests the server to open a data connection on a specific port.
  FTPS_CMD_OPERATION_PWD,       // PWD (Print Working Directory): The client requests the current working directory path.
  FTPS_CMD_OPERATION_RETR,      // RETR (Retrieve): The client requests to retrieve a file from the server.
  FTPS_CMD_OPERATION_RMD,       // RMD (Remove Directory): The client requests to remove a directory.
  FTPS_CMD_OPERATION_RNFR,      // RNFR (Rename From): The client specifies the file to rename (source file).
  FTPS_CMD_OPERATION_RNTO,      // RNTO (Rename To): The client specifies the new name for the file (destination).
  FTPS_CMD_OPERATION_SIZE,      // SIZE (File Size): The client requests the size of a file.
  FTPS_CMD_OPERATION_STOR,      // STOR (Store): The client uploads a file to the server.
  FTPS_CMD_OPERATION_XPWD,      // XPWD (Extended Print Working Directory): The client requests the current directory in an extended format.
  FTPS_CMD_OPERATION_XMKD,      // XMKD (Extended Make Directory): The client requests to create a directory with extended options.
  FTPS_CMD_OPERATION_XRMD,      // XRMD (Extended Remove Directory): The client requests to remove a directory with extended options.
  FTPS_CMD_OPERATION_XCUP,      // XCUP (Extended Change to Parent Directory): The client requests to move to the parent directory in an extended mode.
  FTPS_CMD_OPERATION_UNKNOWN    // UNKNOWN: Represents an unknown or undefined operation (fallback value).
} FTPS_OPERATION_TYPE;


/*********************************************************************
*
*       FTPS_BUFFER_SIZES
*
*  Function description
*    Contains the configuration for the buffer to allocate when using
*    IP_FTPS_ProcessEx() .
*/
typedef struct {
  U32 NumBytesInBuf;             // Size of Rx buffer. By default FTPS_BUFFER_SIZE .
  U32 NumBytesInBufBeforeFlush;  // Number of bytes to collect in Rx buffer before they are written to the filesystem.
                                 // * 0       : Disabled (default). Chunks regardless of their size read will be written
                                 //             directly to the filesystem.
                                 // * NumBytes: Typically the same as NumBytesInBuf and should be a multiple of the
                                 //             block size used by your filesystem e.g. 2k sectors for NAND or SD-card.
                                 //             The data receive function will be called multiple times until the InBuffer
                                 //             gets saturated for the flush or the last chunk of data (connection close)
                                 //             has been read.
  U32 NumBytesOutBuf;            // Size of Tx buffer. By default FTPS_BUFFER_SIZE .
  U32 NumBytesCwdNameBuf;        // Size of buffer used for the Current Working Directory. By default FTPS_MAX_PATH_DIR .
  U32 NumBytesPathNameBuf;       // Size of buffer used for paths (directory + filename). By default FTPS_MAX_PATH .
  U32 NumBytesDirNameBuf;        // Size of buffer used for dir(ectory) names (directory without filename). By default FTPS_MAX_PATH .
  U32 NumBytesFileNameBuf;       // Size of buffer used for filenames. By default FTPS_MAX_FILE_NAME .
} FTPS_BUFFER_SIZES;

typedef struct {
  int (*pfFindUser)   (const char* sUser);
  int (*pfCheckPass)  (int UserId, const char* sPass);
  int (*pfGetDirInfo) (int UserId, const char* sDirIn , char* sDirOut , int SizeOfDirOut);
  int (*pfGetFileInfo)(int UserId, const char* sFileIn, char* sFileOut, int SizeOfFileOut);
} FTPS_ACCESS_CONTROL;

typedef struct {
  int         (*pfSend)      (const unsigned char* pData, int Len, FTPS_SOCKET hSock);
  int         (*pfReceive)   (      unsigned char* pData, int Len, FTPS_SOCKET hSock);
  FTPS_SOCKET (*pfConnect)   (FTPS_SOCKET hCtrlSock, U16 Port);
  void        (*pfDisconnect)(FTPS_SOCKET hDataSock);
  FTPS_SOCKET (*pfListen)    (FTPS_SOCKET hCtrlSock, U16* pPort, U8* pIPAddr);
  int         (*pfAccept)    (FTPS_SOCKET hCtrlSock, FTPS_SOCKET* phDataSocket);
  int         (*pfSetSecure) (FTPS_SOCKET Socket, FTPS_SOCKET Clone);
} IP_FTPS_API;

typedef struct {
  void* (*pfAlloc)(U32 NumBytesReq);
  void  (*pfFree) (void* p);
} FTPS_SYS_API;

typedef struct {
  FTPS_ACCESS_CONTROL* pAccess;
  U32 (*pfGetTimeDate)(void);
} FTPS_APPLICATION;

/*********************************************************************
*
*       FTPS_cbInfo
*
*  Function description
*    Contains relevant information for the operation result callback
*    pfOperationResult.
*/
typedef struct {
  const char*         pPathname;
  const char*         sComment;
  FTPS_OPERATION_TYPE Operation;
  unsigned            Code;
  int                 UserId;
} FTPS_CB_INFO;

/*********************************************************************
*
*       FTPS_SEND_SIGN_ON_MSG_FUNC
*
*  Function description
*    Callback executed for sending a sign on message for a new client.
*
*  Parameters
*    pOutput: Connection context.
*    Code   : The three digit status code of the message.
*    p      : Reserved for future extensions of this API.
*
*  Additional information
*    A sign on message can consist of multiple lines and has to be
*    in the following format (the value 220 is assumed as Code):
*
*      220-A multi line response starts with the code and a hyphen.\\r\\n\n
*      Further lines do not need to use the code in front.\\r\\n\n
*      All lines provided by the callback need to end with CRLF.\\r\\n\n
*      \Intend2 Lines can start with one or multiple whitespaces.\\r\\n\n
*      220 The last line is indicated by the code followed by a whitespace.\\r\\n
*/
typedef void (FTPS_SEND_SIGN_ON_MSG_FUNC)(FTPS_OUTPUT* pOutput, unsigned Code, void* p);

/*********************************************************************
*
*       FTPS_ON_SERVER_OPERATION_FUNC
*
*  Function description
*    Callback executed for modifying file operations, i.e. calls to the
*    FS Layer which modify something. The application can then decide,
*    depending on the return value of the callback, whether to go ahead
*    with the file operation or to cancel it.
*
*  Parameters
*    UserId    : User that is attempting the file operation.
*    pPathname : Filename including path of the file that the operation is done for.
*    operation : File operation (e.g. STOR, DELE, RNTO, ...)
*/
typedef int (FTPS_ON_SERVER_OPERATION_FUNC)(int UserId, char* pPathname, FTPS_OPERATION_TYPE operation);

/*********************************************************************
*
*       FTPS_OPERATION_RESULT_FUNC
*
*  Function description
*    Callback executed for each operation result. Allows the application
*    to be informed about the success or failure of an FTP operation.
*
*  Parameters
*    pOperationResult : Struct with information about the Operation and its result.
*/
typedef void (FTPS_OPERATION_RESULT_FUNC)(FTPS_CB_INFO* pOperationResult);

typedef struct {
  const IP_FTPS_API*                   pIP_API;
  const IP_FS_API*                     pFS_API;
  const FTPS_APPLICATION*              pApplication;
  const FTPS_SYS_API*                  pSYS_API;
        FTPS_SEND_SIGN_ON_MSG_FUNC*    pfSendSignOnMsg;
        FTPS_ON_SERVER_OPERATION_FUNC* pfOnServerOperation;
        FTPS_OPERATION_RESULT_FUNC*    pfOperationResult;
        void*                          pIntContext;
        U8                             RequestedSecurity;
        U8                             IsImplicitMode;
        U8                             UseRenameToFullPath;
} FTPS_CONTEXT;

/*********************************************************************
*
*       Public functions
*
**********************************************************************
*/

void IP_FTPS_ConfigBufSizes     (FTPS_BUFFER_SIZES* pBufferSizes);
U32  IP_FTPS_CountRequiredMem   (FTPS_CONTEXT* pContext);
void IP_FTPS_Init               (FTPS_CONTEXT* pContext, const IP_FTPS_API* pIP_API, const IP_FS_API* pFS_API, const FTPS_APPLICATION* pApplication, const FTPS_SYS_API* pSYS_API);
int  IP_FTPS_Process            (const IP_FTPS_API* pIP_API, FTPS_SOCKET hCtrlSock, const IP_FS_API* pFS_API, const FTPS_APPLICATION* pApplication);
int  IP_FTPS_ProcessEx          (FTPS_CONTEXT* pContext    , FTPS_SOCKET hCtrlSock);
void IP_FTPS_OnConnectionLimit  (const IP_FTPS_API* pIP_API, FTPS_SOCKET hCtrlSock);
void IP_FTPS_SetSignOnMsg       (const char* sSignOnMsg);
int  IP_FTPS_IsDataSecured      (const FTPS_CONTEXT* pContext);
void IP_FTPS_AllowOnlySecured   (FTPS_CONTEXT* pContext, unsigned DataOnOff);
void IP_FTPS_SetImplicitMode    (FTPS_CONTEXT* pContext);
void IP_FTPS_UseRenameToFullPath(FTPS_CONTEXT* pContext);

//
// API related to sending a custom sign on message.
//
void IP_FTPS_SetSignOnMsgCallback         (FTPS_CONTEXT* pContext, FTPS_SEND_SIGN_ON_MSG_FUNC* pf);
void IP_FTPS_SetOnServerOperationCallback (FTPS_CONTEXT* pContext, FTPS_ON_SERVER_OPERATION_FUNC* pf);
void IP_FTPS_SetOperationResultCallback   (FTPS_CONTEXT* pContext, FTPS_OPERATION_RESULT_FUNC* pf);
int  IP_FTPS_SendFormattedString          (FTPS_OUTPUT* pOutput, const char* sFormat, ...);
int  IP_FTPS_SendMem                      (FTPS_OUTPUT* pOutput, const U8* pData, unsigned NumBytes);
int  IP_FTPS_SendString                   (FTPS_OUTPUT* pOutput, const char* s);
int  IP_FTPS_SendUnsigned                 (FTPS_OUTPUT* pOutput, unsigned v, unsigned Base, int NumDigits);

#if defined(__cplusplus)
}                             // Make sure we have C-declarations in C++ programs.
#endif

#endif                        // Avoid multiple inclusion.

/*************************** End of file ****************************/
