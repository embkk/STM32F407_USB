/*********************************************************************
*                   (c) SEGGER Microcontroller GmbH                  *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 2003 - 2025     SEGGER Microcontroller GmbH              *
*                                                                    *
*       www.segger.com     Support: www.segger.com/ticket            *
*                                                                    *
**********************************************************************
*                                                                    *
*       emUSB-Device * USB Device stack for embedded applications    *
*                                                                    *
*       Please note: Knowledge of this file may under no             *
*       circumstances be used to write a similar product.            *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       emUSB-Device version: V3.66.6                                *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : BSP_USB.h
Purpose     : BSP (Board support package) for USB.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef _BSP_USB_H_     // Avoid multiple/recursive inclusion.
#define _BSP_USB_H_  1

#if defined(__cplusplus)
extern "C" {  /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// In order to avoid warnings for unused parameters.
//
#ifndef BSP_USE_PARA
  #if defined(NC30) || defined(NC308)
    #define BSP_USE_PARA(para)
  #else
    #define BSP_USE_PARA(para) (void)para;
  #endif
#endif

/*********************************************************************
*
*       USBD
*
* Functions for USB device controllers (as far as present).
*/
void BSP_USB_InstallISR      (void (*pfISR)(void));
void BSP_USB_InstallISR_Ex   (int ISRIndex, void (*pfISR)(void), int Prio);
void BSP_USB_ISR_Handler     (void);
void BSP_USB_Init            (void);
void BSP_USB_EnableInterrupt (int ISRIndex);
void BSP_USB_DisableInterrupt(int ISRIndex);

/*********************************************************************
*
*       USBH
*
* Functions for USB Host controllers (as far as present).
*/
void BSP_USBH_InstallISR   (void (*pfISR)(void));
void BSP_USBH_InstallISR_Ex(int ISRIndex, void (*pfISR)(void), int Prio);
void BSP_USBH_Init         (void);

/*********************************************************************
*
*       USBC
*
* Functions for USB PD controllers (as far as present).
*/
void BSP_USBC_InstallISR(int ISRIndex, void (*pfISR)(void));


#if defined(__cplusplus)
  }     // Make sure we have C-declarations in C++ programs
#endif

#endif  // Avoid multiple/recursive inclusion

/****** End Of File *************************************************/
