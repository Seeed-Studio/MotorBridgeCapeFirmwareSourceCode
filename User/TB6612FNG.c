#include "main.h"

#define TB_IN_PORT GPIOB
#define TB_BIN1_PORT GPIOA

#define TB1_AIN1_PIN GPIO_Pin_15
#define TB1_AIN2_PIN GPIO_Pin_14
#define TB1_BIN1_PIN GPIO_Pin_12
#define TB1_BIN2_PIN GPIO_Pin_3

#define TB1_PWMA_PIN GPIO_Pin_4
#define TB1_PWMB_PIN GPIO_Pin_5

#define TB2_AIN1_PIN GPIO_Pin_13
#define TB2_AIN2_PIN GPIO_Pin_12
#define TB2_BIN1_PIN GPIO_Pin_0
#define TB2_BIN2_PIN GPIO_Pin_2

#define TB2_PWMA_PIN GPIO_Pin_0
#define TB2_PWMB_PIN GPIO_Pin_1

#define TB_PWM_TIMER TIM3
#define TB_PWM_TIMER_APB RCC_APB1Periph_TIM3



enum TB_PWM_CHANNEL TB_PWMChannel[] = {TB1_PWMA_CH,TB1_PWMB_CH,TB2_PWMA_CH,TB2_PWMB_CH};



/*------------------------------------------------------------------------------------------------------------------------//

set PWM duty

*/
void TB_PWM_SetDuty(enum TB_PWM_CHANNEL Channel, unsigned int Duty)
{
	uint16_t Period;
	uint32_t Freq;
	if(Config.TB_PWM_Freq >= 1000 && Config.TB_PWM_Freq <= 100000)
	{
		Freq = Config.TB_PWM_Freq;
	}
	else
	{
		Freq = 1000;
	}
	Period =  (SystemCoreClock+Freq/2)/Freq-1; 
	
	if(Channel == TB1_PWMA_CH)
	{
    TB_PWM_TIMER->CCR1 = Period*Duty/MaxDuty;
	}
	else if(Channel == TB1_PWMB_CH)
	{
    TB_PWM_TIMER->CCR2 = Period*Duty/MaxDuty;
	}
	else if(Channel == TB2_PWMA_CH)
	{
    TB_PWM_TIMER->CCR3 = Period*Duty/MaxDuty;
	}
	else if(Channel == TB2_PWMB_CH)
	{
    TB_PWM_TIMER->CCR4 = Period*Duty/MaxDuty;
	}
	
}




/*------------------------------------------------------------------------------------------------------------------------//

set PWM frequency

*/
void TB_SetPWMFreq(uint16_t Freq)
{
	TB_PWM_TIMER->ARR = (SystemCoreClock+Freq/2)/Freq-1;
}




/*------------------------------------------------------------------------------------------------------------------------//


set PWM state into enable or disable

*/
void TB_PWM_ChannelCMD(enum TB_PWM_CHANNEL Channel, unsigned int State)
{	
	if(Channel == TB1_PWMA_CH)
	{
		if(State == 1)
		{
      TB_PWM_TIMER->CCER |=(uint16_t)(0x0001 << 0);
		}
		else if(State == 0)
		{
      TB_PWM_TIMER->CCER &=~(uint16_t)(0x0001 << 0);
		}
	}
	else if(Channel == TB1_PWMB_CH)
	{
		if(State == 1)
		{
      TB_PWM_TIMER->CCER |=(uint16_t)(0x0001 << 4);
		}
		else if(State == 0)
		{
      TB_PWM_TIMER->CCER &=~(uint16_t)(0x0001 << 4);
		}
	}
	else if(Channel == TB2_PWMA_CH)
	{
		if(State == 1)
		{
      TB_PWM_TIMER->CCER |=(uint16_t)(0x0001 << 8);
		}
		else if(State == 0)
		{
      TB_PWM_TIMER->CCER &=~(uint16_t)(0x0001 << 8);
		}
	}
	else if(Channel == TB2_PWMB_CH)
	{
		if(State == 1)
		{
      TB_PWM_TIMER->CCER |=(uint16_t)(0x0001 << 12);
		}
		else if(State == 0)
		{
      TB_PWM_TIMER->CCER &=~(uint16_t)(0x0001 << 12);
		}
	}
}




/*------------------------------------------------------------------------------------------------------------------------//

PWM init for driving TB6612

*/
void TB_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint16_t Period;
	uint32_t Freq;
	if(Config.TB_PWM_Freq >= 1000 && Config.TB_PWM_Freq <= 100000)
	{
		Freq = Config.TB_PWM_Freq;
	}
	else
	{
		Freq = 1000;
	}
	
	Period =  (SystemCoreClock+Freq/2)/Freq-1; 
	TIM_DeInit(TB_PWM_TIMER); 
	
	GPIOInit(TB_IN_PORT,TB1_PWMA_PIN,GPIO_Mode_AF,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(TB_IN_PORT,TB1_PWMB_PIN,GPIO_Mode_AF,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(TB_IN_PORT,TB2_PWMA_PIN,GPIO_Mode_AF,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(TB_IN_PORT,TB2_PWMB_PIN,GPIO_Mode_AF,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	
	GPIO_PinAFConfig(TB_IN_PORT, GPIO_PinSource0, GPIO_AF_1);
	GPIO_PinAFConfig(TB_IN_PORT, GPIO_PinSource1, GPIO_AF_1);
	GPIO_PinAFConfig(TB_IN_PORT, GPIO_PinSource4, GPIO_AF_1);
	GPIO_PinAFConfig(TB_IN_PORT, GPIO_PinSource5, GPIO_AF_1);
	
	RCC_APB1PeriphClockCmd(TB_PWM_TIMER_APB,ENABLE); 
	
	TIM_TimeBaseStructInit(& TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TB_PWM_TIMER,&TIM_TimeBaseStructure);
	
	TIM_OCStructInit(& TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_Pulse = Period ; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TB_PWM_TIMER,&TIM_OCInitStructure);
  TIM_OC2Init(TB_PWM_TIMER,&TIM_OCInitStructure);
	TIM_OC3Init(TB_PWM_TIMER,&TIM_OCInitStructure);
  TIM_OC4Init(TB_PWM_TIMER,&TIM_OCInitStructure);
	
	TIM_ClearFlag(TB_PWM_TIMER, TIM_FLAG_Update);
  TIM_Cmd(TB_PWM_TIMER, ENABLE);
  TIM_CtrlPWMOutputs(TB_PWM_TIMER, ENABLE);
}



/*------------------------------------------------------------------------------------------------------------------------//
TB6612 control mode 

*/
void TB_1A_ShortBreak(void)
{
	GPIO_SetBits(TB_IN_PORT,TB1_AIN1_PIN);
	GPIO_SetBits(TB_IN_PORT,TB1_AIN2_PIN);
}

void TB_1A_CW(void)
{
	GPIO_SetBits(TB_IN_PORT,TB1_AIN1_PIN);
	GPIO_ResetBits(TB_IN_PORT,TB1_AIN2_PIN);
}

void TB_1A_CCW(void)
{
	GPIO_ResetBits(TB_IN_PORT,TB1_AIN1_PIN);
	GPIO_SetBits(TB_IN_PORT,TB1_AIN2_PIN);
}

void TB_1A_Stop(void)
{
	GPIO_ResetBits(TB_IN_PORT,TB1_AIN1_PIN);
	GPIO_ResetBits(TB_IN_PORT,TB1_AIN2_PIN);
}

void TB_1B_ShortBreak(void)
{
	GPIO_SetBits(TB_BIN1_PORT,TB1_BIN1_PIN);
	GPIO_SetBits(TB_IN_PORT,TB1_BIN2_PIN);
}

void TB_1B_CW()
{
	GPIO_SetBits(TB_BIN1_PORT,TB1_BIN1_PIN);
	GPIO_ResetBits(TB_IN_PORT,TB1_BIN2_PIN);
}

void TB_1B_CCW(void)
{
	GPIO_ResetBits(TB_BIN1_PORT,TB1_BIN1_PIN);
	GPIO_SetBits(TB_IN_PORT,TB1_BIN2_PIN);
}

void TB_1B_Stop(void)
{
	GPIO_ResetBits(TB_BIN1_PORT,TB1_BIN1_PIN);
	GPIO_ResetBits(TB_IN_PORT,TB1_BIN2_PIN);
}

void TB_2A_ShortBreak(void)
{
	GPIO_SetBits(TB_IN_PORT,TB2_AIN1_PIN);
	GPIO_SetBits(TB_IN_PORT,TB2_AIN2_PIN);
}

void TB_2A_CW(void)
{
	GPIO_SetBits(TB_IN_PORT,TB2_AIN1_PIN);
	GPIO_ResetBits(TB_IN_PORT,TB2_AIN2_PIN);
}

void TB_2A_CCW(void)
{
	GPIO_ResetBits(TB_IN_PORT,TB2_AIN1_PIN);
	GPIO_SetBits(TB_IN_PORT,TB2_AIN2_PIN);
}

void TB_2A_Stop(void)
{
	GPIO_ResetBits(TB_IN_PORT,TB2_AIN1_PIN);
	GPIO_ResetBits(TB_IN_PORT,TB2_AIN2_PIN);
}

void TB_2B_ShortBreak(void)
{
	GPIO_SetBits(TB_BIN1_PORT,TB2_BIN1_PIN);
	GPIO_SetBits(TB_IN_PORT,TB2_BIN2_PIN);
}

void TB_2B_CW(void)
{
	GPIO_SetBits(TB_BIN1_PORT,TB2_BIN1_PIN);
	GPIO_ResetBits(TB_IN_PORT,TB2_BIN2_PIN);
}

void TB_2B_CCW(void)
{
	GPIO_ResetBits(TB_BIN1_PORT,TB2_BIN1_PIN);
	GPIO_SetBits(TB_IN_PORT,TB2_BIN2_PIN);
}

void TB_2B_Stop(void)
{
	GPIO_ResetBits(TB_BIN1_PORT,TB2_BIN1_PIN);
	GPIO_ResetBits(TB_IN_PORT,TB2_BIN2_PIN);
}

/*------------------------------------------------------------------------------------------------------------------------//
TB6612FNG work mode functuin pointer array

TB_xB:ccw in sofeware is cw in real output
      cw  in software is ccw int real output;
 
*/
void(*TB_WorkModeFunctionPointer[][4])(void)={
														{TB_1A_ShortBreak,TB_1A_CW,TB_1A_CCW,TB_1A_Stop},
														{TB_1B_ShortBreak,TB_1B_CCW,TB_1B_CW,TB_1B_Stop},
														{TB_2A_ShortBreak,TB_2A_CW,TB_2A_CCW,TB_2A_Stop},
														{TB_2B_ShortBreak,TB_2B_CCW,TB_2B_CW,TB_2B_Stop},
													 };

void TB_SetWorkMode(enum TB_PORT TBx, enum TB_WORKMODE Mode)
{
	TB_WorkModeFunctionPointer[TBx][Mode]();
}

/*------------------------------------------------------------------------------------------------------------------------//
TB6612FNG control init


*/
void TB_Init(void)
{
	
	TB_PWM_Init();
	TB_PWM_ChannelCMD(TB1_PWMA_CH,1);
	TB_PWM_ChannelCMD(TB1_PWMB_CH,1);
	TB_PWM_ChannelCMD(TB2_PWMA_CH,1);
	TB_PWM_ChannelCMD(TB2_PWMB_CH,1);
	
	GPIOInit(TB_IN_PORT,TB1_AIN1_PIN,GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(TB_IN_PORT,TB1_AIN2_PIN,GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(TB_BIN1_PORT,TB1_BIN1_PIN,GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(TB_IN_PORT,TB1_BIN2_PIN,GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_PP);
  TB_1A_Stop();TB_1B_Stop();
	
	GPIOInit(TB_IN_PORT,TB2_AIN1_PIN,GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(TB_IN_PORT,TB2_AIN2_PIN,GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(TB_BIN1_PORT,TB2_BIN1_PIN,GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(TB_IN_PORT,TB2_BIN2_PIN,GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_PP);
  TB_2A_Stop();TB_2B_Stop();
}


