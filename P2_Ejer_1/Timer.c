#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Timer.h"
#include "main.h"
 
/*----------------------------------------------------------------------------
 *      Timer: Sample timer functions
 *---------------------------------------------------------------------------*/
 
/*----- One-Shoot Timer Example -----*/
osTimerId_t tim_id1;                            // timer id
static uint32_t exec1;                          // argument for the timer call back function
osTimerId_t tim_id2;                            // timer id

// One-Shoot Timer Function
static void Timer1_Callback (void const *arg) {
  // add user code here
HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}
 static void Timer2_Callback (void const *arg) {
  // add user code here
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,1);
osTimerStop(tim_id1);
}
 
 
// Example: Create and Start timers
int Init_Timers (void) {
  osStatus_t status;                            // function return status
 
  // Create one-shoot timer
  exec1 = 1U;
  tim_id1 = osTimerNew((osTimerFunc_t)&Timer1_Callback, osTimerPeriodic, &exec1, NULL);
  if (tim_id1 != NULL) {  // One-shot timer created
    // Timer de 3 segundos
    status = osTimerStart(tim_id1, 500U); 
    if (status != osOK) {
      return -1;
    }
  }
	  tim_id2 = osTimerNew((osTimerFunc_t)&Timer2_Callback, osTimerOnce, &exec1, NULL);
  if (tim_id1 != NULL) {  // One-shot timer created
    // Timer de 3 segundos
    status = osTimerStart(tim_id2, 500U); 
    if (status != osOK) {
      return -1;
    }
  }
 
}
