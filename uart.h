#ifndef UART_H
#define UART_H
#include "MKL25Z4.h"
void InitUART1(uint32_t baud_rate);

void InitUART1(uint32_t baud_rate) {
	uint32_t divisor, bus_clock;
	
	SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	PORTE->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTE->PCR[1] |= PORT_PCR_MUX(3);
	
	
	UART1->C2 &= ~((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	
	bus_clock = (DEFAULT_SYSTEM_CLOCK)/2;
	divisor = bus_clock / (baud_rate * 16);
	UART1->BDH = UART_BDH_SBR(divisor >> 8);
	UART1->BDL = UART_BDL_SBR(divisor);
	
	UART1->C1 = 0;
	UART1->S2 = 0;
	UART1->C3 = 0;
	
	NVIC_SetPriority(UART1_IRQn, 128);
	NVIC_ClearPendingIRQ(UART1_IRQn);
	NVIC_EnableIRQ(UART1_IRQn);
	
	UART1->C2 |= (UART_C2_RE_MASK | UART_C2_RIE_MASK);
}
#endif
