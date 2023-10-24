#include "MKL25Z4.h"
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "queue.h"
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void initUART1(uint32_t baud_rate) {
	uint32_t divisor, bus_clock;
	
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	PORTA->PCR[18] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[18] |= PORT_PCR_MUX(3);
	
	PORTA->PCR[19] &= ~PORT_PCR_MUX_MASK;
	PORTA->PCR[19] |= PORT_PCR_MUX(3);
	
	
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
	
	UART1->C2 |= ((UART_C2_TE_MASK) | (UART_C2_RE_MASK));
	Q_Init(&tx_q);
	Q_Init(&rx_q);
}
