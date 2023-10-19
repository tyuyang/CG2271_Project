
#define PWM_H
#include "MKL25Z4.h"  

void initPWM(void);
void startMotors(void);
void stopMotors(void);
void changeFrequency(int frequency, int timer, int channel);
