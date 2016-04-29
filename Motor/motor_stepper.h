#ifndef _MOTOR_STEPPER_H_
#define _MOTOR_STEPPER_H_


#include "main.h"

void SPM1_Run(uint8_t Beatx);

void SPM2_Run(uint8_t Beatx);

void SPM_Init(enum TB_PORT SPMx);

void SPM1_SetSpeed(void);

void SPM2_SetSpeed(void);

void SPM1_SetDuty(void);

void SPM2_SetDuty(void);

void SPM_Run(void);

#endif



