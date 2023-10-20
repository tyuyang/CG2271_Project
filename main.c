/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "MKL25Z4.h"                    // Device header
#include "pwm.h"
#include "motor_control.h"


/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
  // ...
  for (;;) {
		moveForward();
		osDelay(1000);
		stopMotors();
		osDelay(1000);
	}
}

static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}


int main (void) {
	
  // System Initialization
  SystemCoreClockUpdate();
  // ...
	initPWM();
	
	/*
	TPM1_C0V = 3750;
	TPM1_C1V = 3750;
	TPM2_C0V = 3750;
	TPM2_C1V = 3750;
	
	delay(0xffffff);
	*/
	
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}
