#include "MKL25Z4.h"                    // Device header

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "colorHandler.h"

#define MASK(x) (1 << (x))
int volatile modValue = 0;
int volatile cnvValue = 0;

void initBuzzerPWM(void) {
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	PORTC->PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[1] |= PORT_PCR_MUX(4);
	
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	TPM0->MOD = 7500;
	
	TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
	TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));
	TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	TPM0_C1V = 0;
}

void calculateModValue(int frequency) {
	modValue = (375000 / frequency);
}

void calculateCnvValue() {
	// dutyCycle
	cnvValue = modValue / 2;
}

void changeFrequency(int frequency) {
	calculateModValue(frequency);
	calculateCnvValue();
	TPM0->MOD = modValue;
	TPM0_C0V = cnvValue;
}

void controlEndBuzzer(void *argument) {
	// end music - rickroll
		changeFrequency(784); // f
		osDelay(1000);
		changeFrequency(880); // g
		osDelay(1000);
		changeFrequency(0);
		osDelay(10);
		changeFrequency(880); // g
		osDelay(1000);
		changeFrequency(988); // a
		osDelay(1000);
		
		changeFrequency(1760); // g 1 octave
		osDelay(250);
		changeFrequency(1567); // f
		osDelay(250);
		changeFrequency(1482); // e
		osDelay(500);
		
		changeFrequency(784); // f
		osDelay(1000);
		changeFrequency(880); // g
		osDelay(1000);
		changeFrequency(0);
		osDelay(10);
		changeFrequency(880); // g
		osDelay(2000);
}

void controlBuzzer(void *argument) {
	//run mode - danny Minecraft
	changeFrequency(880); // g
	osDelay(1000);
	changeFrequency(988); // a
	osDelay(1000);
	changeFrequency(1109); // b
	osDelay(1000);
	changeFrequency(1176); // c
	osDelay(1000);
	changeFrequency(880); // g
	osDelay(1000);
	changeFrequency(988); // a
	osDelay(1000);
	changeFrequency(1109); // b
	osDelay(1000);
	changeFrequency(1320); // d
	osDelay(1000);
	changeFrequency(1176); // c
	osDelay(1000);
	changeFrequency(1109); // b
	osDelay(1000);
	changeFrequency(1176); // c
	osDelay(1000);
	changeFrequency(1320); // d
	osDelay(1000);
	changeFrequency(880); // g
	osDelay(4000);
}

void stopBuzzer(void *argument) {
	TPM0_C1V = 0;
}
