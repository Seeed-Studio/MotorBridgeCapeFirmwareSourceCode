#include "main.h"


/*------------------------------------------------------------------------------------------------------------------------//
clock init


*/
void SystemClockInit(void)
{
  RCC_DeInit();     
  RCC_HSEConfig(RCC_HSE_ON);
	while(!RCC_WaitForHSEStartUp());
	
	FLASH_PrefetchBufferCmd(ENABLE);
	FLASH_SetLatency(FLASH_Latency_1); 
	//PCLK=HCLK=SYSCLK
	RCC_HCLKConfig(RCC_SYSCLK_Div1); 
	RCC_PCLKConfig(RCC_HCLK_Div1);
	//PLL=HSE*6 HSE VALUE IS 8M
	RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);   
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
	SystemCoreClock = 48000000;	
}



/*------------------------------------------------------------------------------------------------------------------------//
GPIO init


*/
void GPIOInit(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef Mode,GPIOPuPd_TypeDef PuPdMode,GPIOOType_TypeDef OutputMode)
{
  
	GPIO_InitTypeDef GPIO_MyInit;
	switch((uint32_t)GPIOx)
	{
	  case (uint32_t)GPIOA:RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);break;
	  case (uint32_t)GPIOB:RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB,ENABLE);break;
		case (uint32_t)GPIOC:RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC,ENABLE);break;
		case (uint32_t)GPIOD:RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD,ENABLE);break;
		case (uint32_t)GPIOE:RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE,ENABLE);break;
		case (uint32_t)GPIOF:RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF,ENABLE);break;
	}
	GPIO_MyInit.GPIO_Pin   =  GPIO_Pin;
	GPIO_MyInit.GPIO_Mode  =  Mode;
	GPIO_MyInit.GPIO_Speed =  GPIO_Speed_50MHz;
	GPIO_MyInit.GPIO_PuPd = PuPdMode;
	GPIO_MyInit.GPIO_OType = OutputMode;
	GPIO_Init(GPIOx,&GPIO_MyInit);
  
}

