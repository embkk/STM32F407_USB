/*********************************************************************
*               (c) SEGGER Microcontroller GmbH & Co. KG             *
*                        The Embedded Experts                        *
*                           www.segger.com                           *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

Purpose : Default task priorities used in samples.
--------  END-OF-HEADER  ---------------------------------------------
*/

#ifndef TASKPRIO_H            // Avoid multiple inclusion.
#define TASKPRIO_H

#if defined(__cplusplus)
  extern "C" {                // Make sure we have C-declarations in C++ programs.
#endif

/*********************************************************************
*
*       Types, global
*
**********************************************************************
*/

typedef enum {
   APP_TASK_PRIO_FTP_SERVER_CHILD = 100
  ,APP_TASK_PRIO_WEBSERVER_CHILD
  ,APP_TASK_PRIO_FTP_SERVER_PARENT
  ,APP_TASK_PRIO_WEBSERVER_PARENT
  ,APP_TASK_PRIO_USBD
  ,APP_TASK_PRIO_GUI
  ,APP_TASK_PRIO_MAINTASK_M1   // MainTask minus 1
  ,APP_TASK_PRIO_MAINTASK
  ,APP_TASK_PRIO_MAINTASK_P1   // MainTask plus  1
  ,APP_TASK_PRIO_TOUCH
  ,APP_TASK_PRIO_VNC
  ,APP_TASK_PRIO_USBH_MAIN
  ,APP_TASK_PRIO_USBH_ISR
  ,APP_TASK_PRIO_IP_TASK       // Should be higher than prio of IP applications.
  ,APP_TASK_PRIO_IP_DTASK      // should be higher than prio of the IP_Task .
  ,APP_TASK_PRIO_IP_RXTASK     // Needs to have the single highest prio above all other IP related tasks.
  ,APP_TASK_PRIO_TRIAL_WINDOW  // Highest prio, needs to be shown immediately.
}  APP_TASK_PRIO;

//
// Compatibility macros for old samples.
//
#define TASKPRIO_FTPSCHILD    APP_TASK_PRIO_FTP_SERVER_CHILD
#define TASKPRIO_FTPSPARENT   APP_TASK_PRIO_FTP_SERVER_PARENT
#define TASKPRIO_FTPS         APP_TASK_PRIO_FTP_SERVER_PARENT
#define TASKPRIO_WEBS         APP_TASK_PRIO_WEBSERVER_PARENT
#define TASKPRIO_WEBSCHILD    APP_TASK_PRIO_WEBSERVER_CHILD
#define TASKPRIO_WEBSPARENT   APP_TASK_PRIO_WEBSERVER_PARENT
#define TASKPRIO_USB          APP_TASK_PRIO_USBD
#define TASKPRIO_WINDOW       APP_TASK_PRIO_GUI
#define TASKPRIO_MAINTASK     APP_TASK_PRIO_MAINTASK
#define TASKPRIO_TOUCH        APP_TASK_PRIO_TOUCH
#define TASKPRIO_VNC          APP_TASK_PRIO_VNC
#define TASKPRIO_USBH_MAIN    APP_TASK_PRIO_USBH_MAIN
#define TASKPRIO_USBH_ISR     APP_TASK_PRIO_USBH_ISR
#define TASKPRIO_IPMAIN       APP_TASK_PRIO_IP_TASK
#define TASKPRIO_IPRX         APP_TASK_PRIO_IP_RXTASK
#define TASKPRIO_TRIALWINDOW  APP_TASK_PRIO_TRIAL_WINDOW


#if defined(__cplusplus)
}                             // Make sure we have C-declarations in C++ programs.
#endif

#endif                        // Avoid multiple inclusion.

/*************************** End of file ****************************/
