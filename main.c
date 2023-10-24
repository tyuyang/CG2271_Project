/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"

#include "MKL25Z4.h"                    // Device header
#include "pwm.h"
#include "motor_control.h"
#include "colorHandler.h"
#include "buzzer.h"

osEventFlagsId_t movingGreenFlag;
osEventFlagsId_t movingRedFlag;
osEventFlagsId_t stationGreenFlag;
osEventFlagsId_t stationRedFlag;

volatile int musicCmd;

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
void motorMovingFlagsSet() {
  osEventFlagsSet(movingGreenFlag, 0x0001);
  osEventFlagsSet(movingRedFlag, 0x0001);
  osEventFlagsClear(stationGreenFlag,0x0001);
  osEventFlagsClear(stationRedFlag,0x0001);
}

void motorStopFlagsSet() {
  osEventFlagsSet(stationGreenFlag, 0x0001);
  osEventFlagsSet(stationRedFlag, 0x0001);
  osEventFlagsClear(movingGreenFlag,0x0001);
  osEventFlagsClear(movingRedFlag,0x0001);
}

void motorThread (void *argument) {
  for (;;) {
    moveForward();
    motorMovingFlagsSet();
    osDelay(2000);
    stopMotors();
    motorStopFlagsSet();
    osDelay(2000);
  }
}


static void delay(volatile uint32_t nof) {
  while(nof!=0) {
    __asm("NOP");
    nof--;
  }
}

void movingGreenLED (void *argument) {
  for (;;) {
    osEventFlagsWait(movingGreenFlag, 0x0001, osFlagsNoClear, osWaitForever);
		startMovingGreen();
	}
}

void stationGreenLED (void *argument) {
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
	for (;;) {
    osEventFlagsWait(stationRedFlag, 0x0001, osFlagsNoClear, osWaitForever);
		startFastFlashRed();
	}
}

void buzzerThread(void *argument) {
  // Default dont play music, then play on start and play end music on stop
  for(;;) {
    if (musicCmd == 1) {
      controlBuzzer();
    } else if (musicCmd == 2) {
      controlEndBuzzer();
    }
  }
}


int main (void) {
	
  // System Initialization
  SystemCoreClockUpdate();
  // ...
	initPWM();
	initGPIOLED();
	initBuzzerPWM();
	
	TPM1_C0V = 3750;
	/*
	TPM1_C0V = 3750;
	TPM1_C1V = 3750;
	TPM2_C0V = 3750;
	TPM2_C1V = 3750;
	
	delay(0xffffff);
	*/
  musicCmd = 0;
	
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	
	// Creating the led event flags
  movingGreenFlag = osEventFlagsNew(NULL);
  movingRedFlag = osEventFlagsNew(NULL);
  stationGreenFlag = osEventFlagsNew(NULL);
  stationRedFlag = osEventFlagsNew(NULL);
	
  osThreadNew(motorThread, NULL, NULL);    // Create application main thread
	osThreadNew(movingGreenLED, NULL, NULL);
  osThreadNew(movingRedLED, NULL, NULL);
  osThreadNew(stationGreenLED, NULL, NULL);
  osThreadNew(stationRedLED, NULL, NULL);
  osThreadNew(buzzerThread, NULL, NULL);
  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}