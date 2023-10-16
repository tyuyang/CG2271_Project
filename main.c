/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
 
  // ...
  for (;;) {}
}
 
#define PTB0_Pin 0
#define PTB1_Pin 1
#define PTB2_Pin 2
#define PTB3_Pin 3

#define TPM_1 1
#define TPM_2 2

int volatile modValue1 = 0;
int volatile modValue2 = 0;
int volatile cnvValue1 = 0;
int volatile cnvValue2 = 0;

void initPWM(void) {
	// Enable clock for PortB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Configure MUX settings for TPM1_CH0 and TPM1_CH1
	PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);
	
	// Configure MUX settings for TPM2_CH0 and TPM2_CH1
	PORTB->PCR[PTB2_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB2_Pin] |= PORT_PCR_MUX(3);
	
	PORTB->PCR[PTB3_Pin] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[PTB3_Pin] |= PORT_PCR_MUX(3);
	
	// Enable clock for TPM1 and TPM2
	SIM->SCGC6 |= (SIM_SCGC6_TPM1_MASK | SIM_SCGC6_TPM2_MASK);
	
	// Select clock source for TPM counter clock
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

	TPM1->MOD = modValue1;
	TPM2->MOD = modValue2;

	// Select mode
	// Clear ELSB, ELSA, MSB, MSA to 0
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	
	// Set mode to edge-aligned PWM
	// High-true pulses (clear output on match, set output on reload)
	TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
}

void change_freq(int frequency, int timer) {
	if (timer == TPM_1) {
		modValue1 = 375000 / frequency;
		TPM1->MOD = modValue1;
	} else {
		modValue2 = 375000 / frequency;
		TPM2->MOD = modValue2;
	}
}

void change_duty_cycle(int duty_cycle, int timer, int channel) {
	if (timer == TPM_1) {
		cnvValue1 = (duty_cycle / 100) * modValue1;
		
		if (channel == 0) {
			TPM1_C0V = cnvValue1;
		} else {
			TPM1_C1V= cnvValue1;
		}
	} else {
		cnvValue2 = (duty_cycle / 100) * modValue2;
		
		if (channel == 0) {
			TPM2_C0V = cnvValue2;
		} else {
			TPM2_C1V = cnvValue2;
		}
	}
	
}

int main (void) {
	
  // System Initialization
  SystemCoreClockUpdate();
  // ...
	initPWM();
	
	change_freq(300, TPM_1);
	change_duty_cycle(50, TPM_1, 0);
	change_freq(300, TPM_1);
	change_duty_cycle(50, TPM_1, 1);
	change_freq(300, TPM_2);
	change_duty_cycle(50, TPM_2, 0);
	change_freq(300, TPM_2);
	change_duty_cycle(50, TPM_2, 1);
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, NULL);    // Create application main thread
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
