#include "main.h"

#define LED_PORT GPIOA
#define LED_Pin GPIO_Pin_4

#define LED_Flash_Delay 500

/*------------------------------------------------------------------------------------------------------------------------//
LED init


*/
void LED_Init(void)
{
	GPIOInit(LED_PORT,LED_Pin,GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIO_SetBits(LED_PORT,LED_Pin);
}

/*------------------------------------------------------------------------------------------------------------------------//
Set LED ON OR OFF


*/
void LED_Cmd(uint8_t state)
{
	if(state)
	{
	  GPIO_SetBits(LED_PORT,LED_Pin);
	}
	else
	{
	  GPIO_ResetBits(LED_PORT,LED_Pin);
	}
}

/*------------------------------------------------------------------------------------------------------------------------//
SET LED with a contrary state


*/
void LED_Toggle(void)
{
	LED_Cmd(!GPIO_ReadOutputDataBit(LED_PORT,LED_Pin));
}

/*------------------------------------------------------------------------------------------------------------------------//
lED TOGGLE per 500ms(LED_Flash_Delay)


*/
void LED_Flash(void)
{
	static uint64_t Counter = 0;
	if(localtime - Counter > LED_Flash_Delay)
	{
		LED_Toggle();
		Counter = 0;
		Counter = localtime;
	}
}


