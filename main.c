/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
 
#include "pwm.h"

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
  // ...
  for (;;) {}
}

int main (void) {
	
  // System Initialization
  SystemCoreClockUpdate();
  // ...
	initPWM();
	
	change_freq(300, TPM_1);
	change_duty_cycle(50, TPM_1, 0);
	change_freq(300, TPM_1);
	change_duty_cycle(50, TPM_1, 1);
	change_freq(300, TPM_2);
	change_duty_cycle(50, TPM_2, 0);
	change_freq(300, TPM_2);
	change_duty_cycle(50, TPM_2, 1);
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
