/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*       (c) 1995 - 2025 SEGGER Microcontroller GmbH                  *
*                                                                    *
*       Internet: segger.com  Support: support_embos@segger.com      *
*                                                                    *
**********************************************************************
*                                                                    *
*       embOS * Real time operating system                           *
*                                                                    *
*       Please note:                                                 *
*                                                                    *
*       Knowledge of this file may under no circumstances            *
*       be used to write a similar product or a real-time            *
*       operating system for in-house use.                           *
*                                                                    *
*       Thank you for your fairness !                                *
*                                                                    *
**********************************************************************
*                                                                    *
*       OS version: V5.20.0.0                                        *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------
File    : OS_StartLEDBlink.c
Purpose : embOS sample program running two simple tasks, each toggling
          an LED of the target hardware (as configured in BSP.c).
*/

#include "RTOS.h"
#include "BSP.h"

static OS_STACKPTR int Stack0[2000];  // Task stacks
static OS_TASK         TCB0;                // Task control blocks

/*********************************************************************
*
*       main()
*/
int main(void) {
  OS_Init();    // Initialize embOS
  OS_InitHW();  // Initialize required hardware
  BSP_Init();   // Initialize LED ports
  extern void MainTask(void);
  OS_TASK_CREATE(&TCB0, "Main Task", 100, MainTask, Stack0);
  OS_Start();   // Start embOS
  IP_Logf_Application("Sending test ping to gateway...");
int r = IP_SendPingCheckReply(_IFaceId, IP_BYTES2ADDR(192,168,2,1), NULL, 0, 1000);
if (r == 0) {
  IP_Logf_Application("Ping successful!");
} else {
  IP_Logf_Application("Ping failed: %d", r);
}
  return 0;
}

/*************************** End of file ****************************/
