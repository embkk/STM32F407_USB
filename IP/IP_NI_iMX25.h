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
Purpose     : Driver specific header file for Freescale i.MX25
*/

#ifndef IP_DRIVER_IMX25_H  // Avoid multiple inclusion
#define IP_DRIVER_IMX25_H

#include "IP.h"

#if defined(__cplusplus)
extern "C" {  // Make sure we have C-declarations in C++ programs
#endif

/*********************************************************************
*
*       Defines, non configurable
*
**********************************************************************
*/

#define IP_NI_IMX25_TX_BUFFER_SIZE  1536  // Buffer size MUST be divisible by 16 and less than 2047 bytes, but big enough to hold entire frame (MTU 1500 bytes + headers)

/*********************************************************************
*
*       Types, descriptors
*
**********************************************************************
*/

typedef struct {
  union {
    U32 Ctrl;
    struct {
      unsigned DataLength  : 16;
      unsigned Reserved0   :  9;
      unsigned ABC         :  1;
      unsigned TC          :  1;
      unsigned L           :  1;
      unsigned TO2         :  1;
      unsigned W           :  1;
      unsigned TO1         :  1;
      unsigned R           :  1;
    } CtrlBits;
  } Ctrl;
  U32 Addr;
} IP_NI_IMX25_TX_BUFFER_DESC;

typedef struct {
  union {
    U32 Ctrl;
    struct {
      unsigned DataLength  : 16;
      unsigned TR          :  1;
      unsigned OV          :  1;
      unsigned CR          :  1;
      unsigned Reserved0   :  1;
      unsigned NO          :  1;
      unsigned LG          :  1;
      unsigned MC          :  1;
      unsigned BC          :  1;
      unsigned M           :  1;
      unsigned Reserved1   :  2;
      unsigned L           :  1;
      unsigned RO2         :  1;
      unsigned W           :  1;
      unsigned RO1         :  1;
      unsigned E           :  1;
    } CtrlBits;
  } Ctrl;
  U32 Addr;
} IP_NI_IMX25_RX_BUFFER_DESC;

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/

extern const IP_HW_DRIVER IP_Driver_iMX25;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/

void IP_NI_iMX25_ConfigBuffers(U16 NumTxBuffers, U16 NumRxBuffers, U16 RxBufferSize, void * pTxDesc, void * pRxDesc, void * pBuffer);

#if defined(__cplusplus)
  }           // Make sure we have C-declarations in C++ programs
#endif

/********************************************************************/

#endif        // Avoid multiple inclusion

/*************************** End of file ****************************/
