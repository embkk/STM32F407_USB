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
Purpose   : embOS sample program to demonstrate the SEGGER Stack Overflow Prevention
Literature: [1] Stack Overflow Prevention
                https://wiki.segger.com/Stack_Overflow_Prevention
*/

#include "RTOS.h"
#include "BSP.h"

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
static OS_STACKPTR int StackHP[128], StackLP[128];  // Task stacks
static OS_TASK         TCBHP, TCBLP;                // Task control blocks

/*********************************************************************
*
*       Local functions
*
**********************************************************************
*/
static unsigned int _Factorial(unsigned int F) {
  if (F == 1) {
    return 1;
  }
  return F * _Factorial(F - 1);
}

static void HPTask(void) {
  unsigned int i;

  i = 1u;
  //
  // Enable Stack Overflow Prevention for this task
  //
  OS_STOP_SetTaskContextExtension((OS_U32)StackHP);
  while (1) {
    _Factorial(i);  // Generate task stack overflow
    i++;
    BSP_ToggleLED(0);
    OS_TASK_Delay(50);
  }
}

static void LPTask(void) {
  while (1) {
    BSP_ToggleLED(1);
    OS_TASK_Delay(200);
  }
}

/*********************************************************************
*
*       Global functions
*
**********************************************************************
*/

/*********************************************************************
*
*       main()
*/
int main(void) {
  OS_Init();    // Initialize embOS
  OS_InitHW();  // Initialize required hardware
  BSP_Init();   // Initialize LED ports
  OS_TASK_CREATE(&TCBHP, "HP Task", 100, HPTask, StackHP);
  OS_TASK_CREATE(&TCBLP, "LP Task",  50, LPTask, StackLP);
  OS_Start();   // Start embOS
  return 0;
}

/*************************** End of file ****************************/
