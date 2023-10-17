#define PWM_H

enum timer_pin {
	TPM_1 = 1,
	TPM_2 = 2
};

void initPWM(void);

void change_freq(int frequency, enum timer_pin timer);
	
void change_duty_cycle(int duty_cycle, enum timer_pin timer, int channel);

/*
void switchColor(enum color_t color) {
	switch(color) {
		case RED:
			PTD->PDOR |= MASK(BLUE);
		  PTB->PDOR |= MASK(GREEN);
		  PTB->PDOR &= ~MASK(RED); 
			break;
		case GREEN:
		  PTD->PDOR |= MASK(BLUE);
		  PTB->PDOR |= MASK(RED);
		  PTB->PDOR &= ~MASK(GREEN); 
			break;
		case BLUE:
			PTB->PDOR |= MASK(RED) | MASK(GREEN); 
			PTD->PDOR &= ~MASK(BLUE); 
			break;
	}
}
*/