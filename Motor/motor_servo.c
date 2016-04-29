#include "main.h"

/*------------------------------------------------------------------------------------------------------------------------//
systick init for timing to Clear

*/
void SystickInit(void)
{
	SysTick_Config(SystemCoreClock/systickFreq);
}

void SVM_PWM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint16_t Period;
	uint32_t Freq;

	GPIOInit(SVM1_GPIO,SVM1_PIN,GPIO_Mode_AF,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(SVM2_GPIO,SVM2_PIN,GPIO_Mode_AF,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(SVM3_GPIO,SVM3_PIN,GPIO_Mode_OUT,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(SVM4_GPIO,SVM4_PIN,GPIO_Mode_AF,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(SVM5_GPIO,SVM5_PIN,GPIO_Mode_AF,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	GPIOInit(SVM6_GPIO,SVM6_PIN,GPIO_Mode_AF,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	
	GPIO_PinAFConfig(SVM1_GPIO, SVM1_PINSOURCE, GPIO_AF_2);
	GPIO_PinAFConfig(SVM2_GPIO, SVM2_PINSOURCE, GPIO_AF_2);
//	GPIO_PinAFConfig(SVM3_GPIO, SVM3_PINSOURCE, GPIO_AF_2);
	GPIO_PinAFConfig(SVM4_GPIO, SVM4_PINSOURCE, GPIO_AF_2);
	GPIO_PinAFConfig(SVM5_GPIO, SVM5_PINSOURCE, GPIO_AF_0);
	GPIO_PinAFConfig(SVM6_GPIO, SVM6_PINSOURCE, GPIO_AF_0);
	
	Freq = 50;
	Period =  (SystemCoreClock/TIM_DIVn+Freq/2)/Freq-1;
	
	TIM_DeInit(SVM1_TIMER); 
	TIM_DeInit(SVM2_TIMER); 
//	TIM_DeInit(SVM3_TIMER); 
	TIM_DeInit(SVM4_TIMER); 
	TIM_DeInit(SVM5_TIMER); 
	TIM_DeInit(SVM6_TIMER); 
	
	RCC_APB2PeriphClockCmd(SVM1_TIMER_APB,ENABLE); 
	RCC_APB2PeriphClockCmd(SVM2_TIMER_APB,ENABLE); 
//	RCC_APB2PeriphClockCmd(SVM3_TIMER_APB,ENABLE); 
	RCC_APB2PeriphClockCmd(SVM4_TIMER_APB,ENABLE); 
	RCC_APB2PeriphClockCmd(SVM5_TIMER_APB,ENABLE); 
	RCC_APB2PeriphClockCmd(SVM6_TIMER_APB,ENABLE); 
	
	TIM_TimeBaseStructInit(& TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_DIVn-1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_Period = Period;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	
	TIM_TimeBaseInit(SVM1_TIMER,&TIM_TimeBaseStructure);
	TIM_TimeBaseInit(SVM2_TIMER,&TIM_TimeBaseStructure);
	TIM_TimeBaseInit(SVM3_TIMER,&TIM_TimeBaseStructure);
	TIM_TimeBaseInit(SVM4_TIMER,&TIM_TimeBaseStructure);
	TIM_TimeBaseInit(SVM5_TIMER,&TIM_TimeBaseStructure);
	TIM_TimeBaseInit(SVM6_TIMER,&TIM_TimeBaseStructure);
	
	TIM_OCStructInit(& TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_Pulse = Period ; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC4Init(SVM1_TIMER,&TIM_OCInitStructure);
  TIM_OC1Init(SVM2_TIMER,&TIM_OCInitStructure);
//  TIM_OC1Init(SVM3_TIMER,&TIM_OCInitStructure);
  TIM_OC1Init(SVM4_TIMER,&TIM_OCInitStructure);
  TIM_OC1Init(SVM5_TIMER,&TIM_OCInitStructure);
  TIM_OC2Init(SVM6_TIMER,&TIM_OCInitStructure);
	
	TIM_ClearFlag(SVM1_TIMER, TIM_FLAG_Update);
	TIM_ClearFlag(SVM2_TIMER, TIM_FLAG_Update);
//	TIM_ClearFlag(SVM3_TIMER, TIM_FLAG_Update);
	TIM_ClearFlag(SVM4_TIMER, TIM_FLAG_Update);
	TIM_ClearFlag(SVM5_TIMER, TIM_FLAG_Update);
	TIM_ClearFlag(SVM6_TIMER, TIM_FLAG_Update);
	
  TIM_Cmd(SVM1_TIMER, ENABLE);
  TIM_Cmd(SVM2_TIMER, ENABLE);
//  TIM_Cmd(SVM3_TIMER, ENABLE);
  TIM_Cmd(SVM4_TIMER, ENABLE);
  TIM_Cmd(SVM5_TIMER, ENABLE);
  TIM_Cmd(SVM6_TIMER, ENABLE);
	
  TIM_CtrlPWMOutputs(SVM1_TIMER, ENABLE);
  TIM_CtrlPWMOutputs(SVM2_TIMER, ENABLE);
//  TIM_CtrlPWMOutputs(SVM3_TIMER, ENABLE);
  TIM_CtrlPWMOutputs(SVM4_TIMER, ENABLE);
  TIM_CtrlPWMOutputs(SVM5_TIMER, ENABLE);
  TIM_CtrlPWMOutputs(SVM6_TIMER, ENABLE);
	
	SystickInit();

}/*------------------------------------------------------------------------------------------------------------------------//
set enable or disable
1-enable 0-disable

*/
void SVM_PWM_ChannelCMD(enum SVM_PORT SVMx, unsigned int State)
{	
	switch(SVMx)
	{
		case SVM1:
			if(State == 1)
			{
				SVM1_TIMER->CCER |=(uint16_t)(0x0001 << 12);
			}
			else if(State == 0)
			{
				SVM1_TIMER->CCER &=~(uint16_t)(0x0001 << 12);
			}
		  break;
		case SVM2:
			if(State == 1)
			{
				SVM2_TIMER->CCER |=(uint16_t)(0x0001 << 0);
			}
			else if(State == 0)
			{
				SVM2_TIMER->CCER &=~(uint16_t)(0x0001 << 0);
			}
		  break;
		case SVM3:
			if(State == 1)
			{
				SVM3_TIMER->CCER |=(uint16_t)(0x0001 << 0);
			}
			else if(State == 0)
			{
				SVM3_TIMER->CCER &=~(uint16_t)(0x0001 << 0);
			}
		  break;
		case SVM4:
			if(State == 1)
			{
				SVM4_TIMER->CCER |=(uint16_t)(0x0001 << 0);
			}
			else if(State == 0)
			{
				SVM4_TIMER->CCER &=~(uint16_t)(0x0001 << 0);
			}
		  break;
		case SVM5:
			if(State == 1)
			{
				SVM5_TIMER->CCER |=(uint16_t)(0x0001 << 0);
			}
			else if(State == 0)
			{
				SVM5_TIMER->CCER &=~(uint16_t)(0x0001 << 0);
			}
		  break;
		case SVM6:
			if(State == 1)
			{
				SVM6_TIMER->CCER |=(uint16_t)(0x0001 << 4);
			}
			else if(State == 0)
			{
				SVM6_TIMER->CCER &=~(uint16_t)(0x0001 << 4);
			}
		  break;
    default:break;
	}
}

/*------------------------------------------------------------------------------------------------------------------------//
Set PWM FREQ , user for different servo motor

*/
void SVM_SetFreq(enum SVM_PORT SVMx, uint16_t Freq)
{
	switch(SVMx)
	{
		case SVM1:
		  SVM1_TIMER->ARR = (SystemCoreClock/TIM_DIVn+Freq/2)/Freq-1;
		  break;
		case SVM2:
		  SVM2_TIMER->ARR = (SystemCoreClock/TIM_DIVn+Freq/2)/Freq-1;
		  break;
		case SVM3:
		  SVM3_TIMER->ARR = (SystemCoreClock/TIM_DIVn+Freq/2)/Freq-1;
		  break;
		case SVM4:
		  SVM4_TIMER->ARR = (SystemCoreClock/TIM_DIVn+Freq/2)/Freq-1;
		  break;
		case SVM5:
		  SVM5_TIMER->ARR = (SystemCoreClock/TIM_DIVn+Freq/2)/Freq-1;
		  break;
		case SVM6:
		  SVM6_TIMER->ARR = (SystemCoreClock/TIM_DIVn+Freq/2)/Freq-1;
		  break;
    default:break;
	}
}


/*------------------------------------------------------------------------------------------------------------------------//
Set PWM duty

*/
float SVM_MinDuty = 0.025*MaxDuty;
float SVM_MaxDuty = 0.125*MaxDuty;

void SVM_SetDuty(enum SVM_PORT SVMx,uint16_t Angle)
{
	uint16_t Period;
	uint16_t Duty;
	uint32_t Freq ;
	
	Freq = Config.SVM_Port[SVMx].Freq;
	if(Angle>AngleDiv)
	{
		Angle = AngleDiv;
	}
	
	Duty = Angle*((uint16_t)(SVM_MaxDuty-SVM_MinDuty))/AngleDiv+((uint16_t)SVM_MinDuty);
	
	Period =  (SystemCoreClock/TIM_DIVn+Freq/2)/Freq-1; 
	
	switch(SVMx)
	{
		case SVM1:
			SVM1_TIMER->CCR4 = Period*Duty/MaxDuty;
		  break;
		case SVM2:
			SVM2_TIMER->CCR1 = Period*Duty/MaxDuty;
		  break;
		case SVM3:
			SVM3_TIMER->CCR1 = Period*Duty/MaxDuty;
		  break;
		case SVM4:
			SVM4_TIMER->CCR1 = Period*Duty/MaxDuty;
		  break;
		case SVM5:
			SVM5_TIMER->CCR1 = Period*Duty/MaxDuty;
		  break;
		case SVM6:
			SVM6_TIMER->CCR2 = Period*Duty/MaxDuty;
		  break;
    default:break;
	}
}


/*------------------------------------------------------------------------------------------------------------------------//
init the port which is set to SVM Mode

*/
void SVM_Init(void)
{
  SVM_PWM_Init();
	
	if(Config.SVM_Port[SVM1].State < SVM_STATE_NUM)
	{
	  SVM_PWM_ChannelCMD(SVM1,Config.SVM_Port[SVM1].State);
	}
	if(Config.SVM_Port[SVM2].State < SVM_STATE_NUM)
	{
	  SVM_PWM_ChannelCMD(SVM2,Config.SVM_Port[SVM2].State);
	}
	if(Config.SVM_Port[SVM4].State < SVM_STATE_NUM)
	{
	  SVM_PWM_ChannelCMD(SVM4,Config.SVM_Port[SVM4].State);
	}
	if(Config.SVM_Port[SVM5].State < SVM_STATE_NUM)
	{
	  SVM_PWM_ChannelCMD(SVM5,Config.SVM_Port[SVM5].State);
	}
	if(Config.SVM_Port[SVM6].State < SVM_STATE_NUM)
	{
	  SVM_PWM_ChannelCMD(SVM6,Config.SVM_Port[SVM6].State);
	}
	
	if(Config.SVM_Port[SVM1].Freq <= MaxFre)
	{
	  SVM_SetFreq(SVM1,Config.SVM_Port[SVM1].Freq);
	}
	if(Config.SVM_Port[SVM2].Freq <= MaxFre)
	{
	  SVM_SetFreq(SVM2,Config.SVM_Port[SVM2].Freq);
	}
	if(Config.SVM_Port[SVM4].Freq <= MaxFre)
	{
	  SVM_SetFreq(SVM4,Config.SVM_Port[SVM4].Freq);
	}
	if(Config.SVM_Port[SVM5].Freq <= MaxFre)
	{
	  SVM_SetFreq(SVM5,Config.SVM_Port[SVM5].Freq);
	}
	if(Config.SVM_Port[SVM6].Freq <= MaxFre)
	{
	  SVM_SetFreq(SVM6,Config.SVM_Port[SVM6].Freq);
	}
	
//	if(Config.TB_Port[TB_2A].TB_PortMode == TB_DCM)
//	{
//		if(Config.SVM_Port[SVM3].State < SVM_STATE_NUM)
//		{
//	    SVM_PWM_ChannelCMD(SVM3,Config.SVM_Port[SVM3].State);
//		}
//		if(Config.SVM_Port[SVM3].Freq <= MaxFre)
//		{
//	    SVM_SetFreq(SVM3,Config.SVM_Port[SVM3].Freq);
//		}
//	}
}

void SVM_Run(void)
{	
	if(Config.SVM_Port[SVM1].State < SVM_STATE_NUM)
	{
	  SVM_PWM_ChannelCMD(SVM1,Config.SVM_Port[SVM1].State);
	}
		
	if(Config.SVM_Port[SVM2].State < SVM_STATE_NUM)
	{
	  SVM_PWM_ChannelCMD(SVM2,Config.SVM_Port[SVM2].State);
	}
		
	if(Config.SVM_Port[SVM4].State < SVM_STATE_NUM)
	{
	  SVM_PWM_ChannelCMD(SVM4,Config.SVM_Port[SVM4].State);
	}
		
	if(Config.SVM_Port[SVM5].State < SVM_STATE_NUM)
	{
	  SVM_PWM_ChannelCMD(SVM5,Config.SVM_Port[SVM5].State);
	}
		
	if(Config.SVM_Port[SVM6].State < SVM_STATE_NUM)
	{
	  SVM_PWM_ChannelCMD(SVM6,Config.SVM_Port[SVM6].State);
	}
	
	if(Config.SVM_Port[SVM1].Freq <= MaxFre)
	{
	  SVM_SetFreq(SVM1,Config.SVM_Port[SVM1].Freq);
	}
	
	if(Config.SVM_Port[SVM2].Freq <= MaxFre)
	{
	  SVM_SetFreq(SVM2,Config.SVM_Port[SVM2].Freq);
	}
	if(Config.SVM_Port[SVM4].Freq <= MaxFre)
	{
	  SVM_SetFreq(SVM4,Config.SVM_Port[SVM4].Freq);
	}
	if(Config.SVM_Port[SVM5].Freq <= MaxFre)
	{
	  SVM_SetFreq(SVM5,Config.SVM_Port[SVM5].Freq);
	}
		
	if(Config.SVM_Port[SVM6].Freq <= MaxFre)
	{
	  SVM_SetFreq(SVM6,Config.SVM_Port[SVM6].Freq);
	}

	
	if(Config.SVM_Port[SVM1].Angle <= AngleDiv)
	{
	  SVM_SetDuty(SVM1,Config.SVM_Port[SVM1].Angle);
	}
	if(Config.SVM_Port[SVM2].Angle <= AngleDiv)
	{
	  SVM_SetDuty(SVM2,Config.SVM_Port[SVM2].Angle);
	}
	if(Config.SVM_Port[SVM4].Angle <= AngleDiv)
	{
	  SVM_SetDuty(SVM4,Config.SVM_Port[SVM4].Angle);
	}
	if(Config.SVM_Port[SVM5].Angle <= AngleDiv)
	{
	  SVM_SetDuty(SVM5,Config.SVM_Port[SVM5].Angle);
	}
	if(Config.SVM_Port[SVM6].Angle <= AngleDiv)
	{
	  SVM_SetDuty(SVM6,Config.SVM_Port[SVM6].Angle);
	}
	
//	if(Config.TB_Port[TB_2A].TB_PortMode == TB_DCM)
//	{
//		if(Config.SVM_Port[SVM3].State < SVM_STATE_NUM)
//		{
//	    SVM_PWM_ChannelCMD(SVM3,Config.SVM_Port[SVM3].State);
//		}
//		if(Config.SVM_Port[SVM3].Freq <= MaxFre)
//		{
//	    SVM_SetFreq(SVM3,Config.SVM_Port[SVM3].Freq);
//		}
//		if(Config.SVM_Port[SVM3].Angle <= AngleDiv)
//		{
//		  SVM_SetDuty(SVM3,Config.SVM_Port[SVM3].Angle);
//		}
//	}
}


/**
  * @brief  This function handles SysTick Handler.USED FOR SVM3
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	static uint64_t SVM3_Counter = 0;
	
	uint16_t Period = (systickFreq/Config.SVM_Port[SVM3].Freq);
	
	uint16_t _Duty = Period*(Config.SVM_Port[SVM3].Angle*((uint16_t)(SVM_MaxDuty-SVM_MinDuty))/AngleDiv+((uint16_t)SVM_MinDuty))/MaxDuty;
	
	localtime++;
	if(localtime - SVM3_Counter > 0 && localtime - SVM3_Counter <= _Duty)
	{	
		if(Config.SVM_Port[SVM3].State == SVM_ENABLE)
		{
			GPIO_SetBits(SVM3_GPIO,SVM3_PIN);
		}
	}
	else if(localtime - SVM3_Counter > _Duty && localtime - SVM3_Counter < Period)
	{
		if(Config.SVM_Port[SVM3].State == SVM_ENABLE)
		{
			GPIO_ResetBits(SVM3_GPIO,SVM3_PIN);
		}
	}
	else if(localtime - SVM3_Counter >= Period)
	{
		if(Config.SVM_Port[SVM3].State == SVM_ENABLE)
		{
			GPIO_ResetBits(SVM3_GPIO,SVM3_PIN);
		}
		SVM3_Counter = localtime;
	}
}

