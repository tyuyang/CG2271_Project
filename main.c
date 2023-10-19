/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "MKL25Z4.h"                    // Device header
#include "pwm.h"

#define MASK(x) (1 << (x))

// Not used as of now
// #define PTB0_Pin 0
// #define PTB1_Pin 1
// #define PTB2_Pin 2
// #define PTB3_Pin 3

// #define TPM_1 1
// #define TPM_2 2

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void motorCommandThread (void *argument) {
  for (;;) {}

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
  startMotors();
	
	delay(0xffffff);
	
	stopMotors();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(motorCommandThread, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}
