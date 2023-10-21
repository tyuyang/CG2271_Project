/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "MKL25Z4.h"                    // Device header
#include "pwm.h"
#include "colorHandler.h"

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

osEventFlagsId_t movingGreenFlag;
osEventFlagsId_t movingRedFlag;
osEventFlagsId_t stationGreenFlag;
osEventFlagsId_t stationRedFlag;

void motorCommandThread (void *argument) {
  for (;;) {
    startMotors();
    osDelay(500);
    stopMotors();
    osDelay(500);
  }
}

void movingGreenLED (void *argument) {
  for (;;) {
    osEventFlagsWait(movingGreenFlag, 0x0001, osFlagsNoClear, osWaitForever);
		startMovingGreen();
	}
}

void stationGreenLED (void *argument) {
  osEventFlagsSet(stationGreenFlag, 0x0001);
	 for (;;) {
    osEventFlagsWait(stationGreenFlag, 0x0001, osFlagsNoClear, osWaitForever);
		startStationGreen();
	}
}

void movingRedLED (void *argument) {
	 for (;;) {
    osEventFlagsWait(movingRedFlag, 0x0001, osFlagsNoClear, osWaitForever);
		startSlowFlashRed();
	}
}

void stationRedLED (void *argument) {
  osEventFlagsSet(stationRedFlag, 0x0001);
	for (;;) {
    osEventFlagsWait(stationRedFlag, 0x0001, osFlagsNoClear, osWaitForever);
		startFastFlashRed();
	}
}


int main (void) {
	
  // System Initialization
  SystemCoreClockUpdate();
  // ...
  initGPIOLED();
	initPWM();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS

  // Creating the led event flags
  movingGreenFlag = osEventFlagsNew(NULL);
  movingRedFlag = osEventFlagsNew(NULL);
  stationGreenFlag = osEventFlagsNew(NULL);
  stationRedFlag = osEventFlagsNew(NULL);

  osThreadNew(motorCommandThread, NULL, NULL);
  osThreadNew(movingGreenLED, NULL, NULL);
  osThreadNew(movingRedLED, NULL, NULL);
  osThreadNew(stationRedLED, NULL, NULL);
  osThreadNew(stationRedLED, NULL, NULL);
  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}
