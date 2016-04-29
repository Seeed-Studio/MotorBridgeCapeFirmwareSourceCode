#ifndef _INIT_H_
#define _INIT_H_


#include "stm32f0xx_conf.h"

void SystemClockInit(void);

void GPIOInit(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef Mode,GPIOPuPd_TypeDef PuPdMode,GPIOOType_TypeDef OutputMode);

#endif


