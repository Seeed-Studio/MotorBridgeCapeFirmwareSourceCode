#ifndef _TB6612FNG_H_
#define _TB6612FNG_H_

#include "main.h"

#define TB_PWM_TIMER TIM3

void TB_PWM_SetDuty(enum TB_PWM_CHANNEL Channel, unsigned int Duty);

void TB_SetPWMFreq(uint16_t Freq);

void TB_SetWorkMode(enum TB_PORT TBx, enum TB_WORKMODE Mode);

void TB_Init(void);

#endif


