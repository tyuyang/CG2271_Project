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

osThreadId_t tIdMovingGreen;
osThreadId_t tIdStationGreen;
osThreadId_t tIdMovingRed;
osThreadId_t tIdStationRed;

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void motorMovingFlagsSet() {
  osThreadFlagsSet(tIdMovingGreen, 0x0001);
  osThreadFlagsSet(tIdMovingRed, 0x0001);
  osThreadFlagsClear(tIdStationRed);
  osThreadFlagsClear(tIdStationGreen);
}

void motorStopFlagsSet() {
  osThreadFlagsSet(tIdStationGreen, 0x0001);
  osThreadFlagsSet(tIdStationRed, 0x0001);
  osThreadFlagsClear(tIdMovingRed);
  osThreadFlagsClear(tIdMovingGreen);
}

void motorCommandThread (void *argument) {
  for (;;) {
    startMotors();
    motorMovingFlagsSet();
    osDelay(500);
    stopMotors();
    motorStopFlagsSet();
    osDelay(500);
  }
}

void movingGreenLED (void *argument) {
  for (;;) {
    osThreadFlagsWait(0x0001, osFlagsNoClear, osWaitForever);
		startMovingGreen();
	}
}

void stationGreenLED (void *argument) {
  osThreadFlagsSet(tIdStationGreen, 0x0001);
	 for (;;) {
    osThreadFlagsWait(0x0001, osFlagsNoClear, osWaitForever);
		startStationGreen();
	}
}

void movingRedLED (void *argument) {
	 for (;;) {
    osThreadFlagsWait(0x0001, osFlagsNoClear, osWaitForever);
		startSlowFlashRed();
	}
}

void stationRedLED (void *argument) {
  osThreadFlagsSet(tIdStationRed, 0x0001);
	for (;;) {
    osThreadFlagsWait(0x0001, osFlagsNoClear, osWaitForever);
		startFastFlashRed();
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
  initGPIOLED();
	initPWM();
	
	stopMotors();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(motorCommandThread, NULL, NULL);
  tIdMovingGreen = osThreadNew(movingGreenLED, NULL, NULL);
  tIdMovingRed = osThreadNew(movingRedLED, NULL, NULL);
  tIdStationRed = osThreadNew(stationRedLED, NULL, NULL);
  tIdStationGreen = osThreadNew(stationRedLED, NULL, NULL);
  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}
