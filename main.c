/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "colorHandler.h"
 
#define MASK(x) (1 << (x))

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void movingGreenLED (void *argument) {

  for (;;) {
		startMovingGreen();
	}
}

void stationGreenLED (void *argument) {
	
	 for (;;) {
		startStationGreen();
	}
}

void movingRedLED (void *argument) {
	 for (;;) {
		 startSlowFlashRed();
	 }
}

void stationRedLED (void *argument) {
	 for (;;) {
		 startFastFlashRed();
	 }
}


int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  initGPIOLED();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(movingGreenLED, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
