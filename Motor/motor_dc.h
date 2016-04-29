#ifndef _MOTOR_DC_H_
#define _MOTOR_DC_H_

#include "main.h"

void DCM_SetMode(enum TB_PORT DCMx, enum TB_WORKMODE Mode);

void DCM_SetSpeed(enum TB_PORT DCMx, unsigned int Speed);

void DCM_Run(void);

#endif


