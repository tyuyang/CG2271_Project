#include "RTE_Components.h"
#include "MKL25Z4.h"
#include CMSIS_device_header
#include "cmsis_os2.h"

#include "led.h"
#include "uart.h"
#include "motor_control.h"
#include "colorHandler.h"
#include "pwm.h"

#define BAUD_RATE 9600

volatile static uint8_t rx_data;



osEventFlagsId_t movingGreenFlag;
osEventFlagsId_t movingRedFlag;
osEventFlagsId_t stationGreenFlag;
osEventFlagsId_t stationRedFlag;

/**
 * Led flag control section. Activates and deactivates
 * moving or stationary threads when function is called
*/

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

/**
 * UART section. IRQ_Handler obviously handles the irq
 * when it is sent, while UART_led_control controls the
 * motor. Currently the thread does not be blocked, might
 * need to change in the future.
*/
void UART1_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART1_IRQn);
	if (UART1->S1 & UART_S1_RDRF_MASK) {
		rx_data = UART1->D;
	}
}

void UART_led_control(void *argument) {
	for (;;){
		if ((rx_data & 0x0a) == 0x0a) {
			forwardLeft();
      motorMovingFlagsSet();
		}
		else if ((rx_data & 0x06) == 0x06) {
			forwardRight();
      motorMovingFlagsSet();
		}
		else if ((rx_data & 0x02) == 0x02) {
			moveForward();
      motorMovingFlagsSet();
		} 
		else if ((rx_data & 0x01) == 0x01) {
			moveBackward();
      motorMovingFlagsSet();
		}
		else if ((rx_data & 0x08)) {
			rotateLeft();
      motorMovingFlagsSet();
		} else if ((rx_data & 0x04)) {
			rotateRight();
      motorMovingFlagsSet();
		}
		else {
			stopMotors();
      motorStopFlagsSet();
		}
	}
}

/**
 * Led control section. Threads are all controlled using
 * event flags.
*/

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

int main(void)
{
	SystemCoreClockUpdate();

  // Init section
  initGPIOLED();
	InitUART1(BAUD_RATE);
	initPWM();

  // Initial state

  // OS section
	osKernelInitialize();

  // Flag creation section
  movingGreenFlag = osEventFlagsNew(NULL);
  movingRedFlag = osEventFlagsNew(NULL);
  stationGreenFlag = osEventFlagsNew(NULL);
  stationRedFlag = osEventFlagsNew(NULL);

  // Thread init section
	osThreadNew(UART_led_control, NULL, NULL);
  osThreadNew(movingGreenLED, NULL, NULL);
  osThreadNew(movingRedLED, NULL, NULL);
  osThreadNew(stationGreenLED, NULL, NULL);
  osThreadNew(stationRedLED, NULL, NULL);

	osKernelStart();
	for(;;){}
}
