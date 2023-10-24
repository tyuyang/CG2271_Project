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

void UART1_IRQHandler(void) {
	NVIC_ClearPendingIRQ(UART1_IRQn);
	// TX
	if (UART1->S1 & UART_S1_TDRE_MASK) {
		if (!Q_Empty(&tx_q)) {
			UART1->D =  Q_Dequeue(&tx_q);
		} else {
			UART1->C2 &= ~UART_C2_TE_MASK;
		}
	}
	// RX
	if (UART1->S1 & UART_S1_RDRF_MASK) {
		if (!Q_Full(&rx_q)) {
			Q_Enqueue(&rx_q, UART1->D);
		}
	}
	// error
	if (UART1->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
		unsigned char foo;
		foo = UART1->D;
	}
}

void commandHandler(Q_T *q) {
	unsigned char cmd = Q_Dequeue(q);
	//buzzer(cmd)
	if ((cmd & 0x02) == 0x02) {
		// forward
	}
	else if ((cmd & 0x01) == 0x01) {
		// backward
	}
	else if ((cmd & 0x08) == 0x08) {
		// left
	}
	else if ((cmd & 0x04) == 0x04) {
		// right
	}
	else if ((cmd & 0x0a) == 0x0a) {
		// forward left
	}
	else if ((cmd & 0x06) == 0x06) {
		// forward right
	}
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
  osKernelStart();                      // Start thread execution
	
  for (;;) {}
}