#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "Timer.h"
#include "ThreadFlags.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThreadFlags;                        // thread id
uint32_t flags;//flags
extern osTimerId_t tim_id1;
extern osTimerId_t tim_id2;

void ThreadFlags (void *argument);                   // thread function
 
int Init_ThreadFlags (void) {
 
  tid_ThreadFlags = osThreadNew(ThreadFlags, NULL, NULL);
  if (tid_ThreadFlags == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void ThreadFlags (void *argument) {
	
  while (1) {
    ; // Insert thread code here...
		flags=osThreadFlagsWait(0x00000001U, osFlagsWaitAny, osWaitForever);
		if (flags==0x00000001U){
			Init_Timers();
			osTimerStart(tim_id1, 500U);
			osTimerStart(tim_id2, 5000U);
		}
    osThreadYield();                            // suspend thread
  }
}
