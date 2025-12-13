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
Purpose : Driver specific header file for the SMSC LAN9115
*/

#ifndef IP_NI_LAN9115_H
#define IP_NI_LAN9115_H

#include "IP.h"

#if defined(__cplusplus)
extern "C" {  // Make sure we have C-declarations in C++ programs
#endif

/*********************************************************************
*
*       Public data
*
**********************************************************************
*/
extern const IP_HW_DRIVER IP_Driver_LAN9115;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
void IP_NI_LAN9115_ISR_Handler   (unsigned Unit);
void IP_NI_LAN9115_ConfigBaseAddr(unsigned Unit, void* pBase);

#if defined(__cplusplus)
  }           // Make sure we have C-declarations in C++ programs
#endif

/********************************************************************/

#endif        // Avoid multiple inclusion

/*************************** End of file ****************************/



