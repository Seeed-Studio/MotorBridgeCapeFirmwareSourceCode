#ifndef _LED_H_
#define _LED_H_

#include "stm32f0xx_conf.h"

void LED_Init(void);

void LED_Cmd(uint8_t state);

void LED_Toggle(void);

void LED_Flash(void);

#endif

