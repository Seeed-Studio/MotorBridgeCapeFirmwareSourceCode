#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "stm32f0xx_conf.h"


#define Communication_PORT     GPIOB
#define Communication_SCL   GPIO_Pin_6
#define Communication_SDA   GPIO_Pin_7

#define I2CPort                I2C1


#define I2c_BufferSize 32

typedef struct 
{
	uint8_t IsNeedReset;
	uint8_t ReadAddr;
	uint8_t WriteAddr;
	uint8_t DataLength;
	uint8_t DataBuffer[I2c_BufferSize];
}I2C_Slaver_Communicate_TypeDef;


void I2CInit(uint16_t I2C_OwnAddress, uint32_t ClockSpeed);

extern I2C_Slaver_Communicate_TypeDef I2C_Communication;

#endif






