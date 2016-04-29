#include "main.h"


void SPM1_TimerInit(void);

void SPM2_TimerInit(void);

void SPM_Init(enum TB_PORT SPMx)
{
	if(SPMx == TB_1A)
	{
		TB_PWM_SetDuty(TB_PWMChannel[TB_1A],MaxDuty);
		TB_PWM_SetDuty(TB_PWMChannel[TB_1B],MaxDuty);
	  SPM1_TimerInit();
	}
	else if(SPMx == TB_2A)
	{ 
		TB_PWM_SetDuty(TB_PWMChannel[TB_2A],MaxDuty);
		TB_PWM_SetDuty(TB_PWMChannel[TB_2B],MaxDuty);
	  SPM2_TimerInit();
	}
} 

/*--------------------------------//
A+ 1
A- 0
B+ 0
B- 0

*/

void SPM1_Beat1(void)
{
		TB_SetWorkMode(TB_1A,TB_CW);
		TB_SetWorkMode(TB_1B,TB_SHORT_BREAK);
}

void SPM2_Beat1(void)
{
		TB_SetWorkMode(TB_2A,TB_CW);
		TB_SetWorkMode(TB_2B,TB_SHORT_BREAK);
}
/*--------------------------------//
A+ 1
A- 0
B+ 1
B- 0
*/
void SPM1_Beat2(void)
{
	
		TB_SetWorkMode(TB_1A,TB_CW);
		TB_SetWorkMode(TB_1B,TB_CW);
}
void SPM2_Beat2(void)
{
		TB_SetWorkMode(TB_2A,TB_CW);
		TB_SetWorkMode(TB_2B,TB_CW);
}
/*--------------------------------//
A+ 0
A- 0
B+ 1
B- 0

*/

void SPM1_Beat3(void)
{
	
		TB_SetWorkMode(TB_1A,TB_SHORT_BREAK);
		TB_SetWorkMode(TB_1B,TB_CW);
	}

void SPM2_Beat3(void)
{
	
		TB_SetWorkMode(TB_2A,TB_SHORT_BREAK);
		TB_SetWorkMode(TB_2B,TB_CW);

}
/*--------------------------------//
A+ 0
A- 1
B+ 1
B- 0

*/
void SPM1_Beat4(void)
{
	
		TB_SetWorkMode(TB_1A,TB_CCW);
		TB_SetWorkMode(TB_1B,TB_CW);
	}

void SPM2_Beat4(void)
{
	
		TB_SetWorkMode(TB_2A,TB_CCW);
		TB_SetWorkMode(TB_2B,TB_CW);
}
/*--------------------------------//
A+ 0
A- 1
B+ 0
B- 0

*/
void SPM1_Beat5(void)
{
	
		TB_SetWorkMode(TB_1A,TB_CCW);
		TB_SetWorkMode(TB_1B,TB_SHORT_BREAK);
	}

void SPM2_Beat5(void)
{
		TB_SetWorkMode(TB_2A,TB_CCW);
		TB_SetWorkMode(TB_2B,TB_SHORT_BREAK);
}
/*--------------------------------//
A+ 0
A- 1
B+ 0
B- 1

*/
void SPM1_Beat6(void)
{

		TB_SetWorkMode(TB_1A,TB_CCW);
		TB_SetWorkMode(TB_1B,TB_CCW);
}

void SPM2_Beat6(void)
{
		TB_SetWorkMode(TB_2A,TB_CCW);
		TB_SetWorkMode(TB_2B,TB_CCW);
}
/*--------------------------------//
A+ 0
A- 0
B+ 0
B- 1

*/

void SPM1_Beat7(void)
{
	
		TB_SetWorkMode(TB_1A,TB_SHORT_BREAK);
		TB_SetWorkMode(TB_1B,TB_CCW);
	}

void SPM2_Beat7(void)
{
	
		TB_SetWorkMode(TB_2A,TB_SHORT_BREAK);
		TB_SetWorkMode(TB_2B,TB_CCW);
}
/*--------------------------------//
A+ 1
A- 0
B+ 0
B- 1

*/

void SPM1_Beat8(void)
{
	
		TB_SetWorkMode(TB_1A,TB_CW);
		TB_SetWorkMode(TB_1B,TB_CCW);
	}

void SPM2_Beat8(void)
{
	
		TB_SetWorkMode(TB_2A,TB_CW);
		TB_SetWorkMode(TB_2B,TB_CCW);
}

void (*SPM1_Beatx[])(void)={SPM1_Beat1,SPM1_Beat2,SPM1_Beat3,SPM1_Beat4,SPM1_Beat5,SPM1_Beat6,SPM1_Beat7,SPM1_Beat8};
void (*SPM2_Beatx[])(void)={SPM2_Beat1,SPM2_Beat2,SPM2_Beat3,SPM2_Beat4,SPM2_Beat5,SPM2_Beat6,SPM2_Beat7,SPM2_Beat8};


void SPM1_Run(uint8_t Beatx)
{
	SPM1_Beatx[Beatx]();
}


void SPM2_Run(uint8_t Beatx)
{
	SPM2_Beatx[Beatx]();
}

#define SPM1_TIMER TIM14
#define SPM1_TIMER_APB RCC_APB1Periph_TIM14
#define SPM2_TIMER TIM16
#define SPM2_TIMER_APB RCC_APB2Periph_TIM16
/*------------------------------------------------------------------------------------------------------------------------//

timer for changing speed

*/
void SPM1_TimerInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(SPM1_TIMER_APB,ENABLE); 
	TIM_DeInit(SPM1_TIMER); 
	
	TIM_TimeBaseStructInit(& TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/1000000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_Period = 1000;  
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	
	TIM_TimeBaseInit(SPM1_TIMER,&TIM_TimeBaseStructure);
	
	TIM_ClearFlag(SPM1_TIMER , TIM_FLAG_Update);
	TIM_ARRPreloadConfig(SPM1_TIMER,DISABLE);
	TIM_ITConfig(SPM1_TIMER,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM14_IRQn);
	TIM_Cmd(SPM1_TIMER,ENABLE);
}

void SPM2_TimerInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(SPM2_TIMER_APB,ENABLE); 
	TIM_DeInit(SPM2_TIMER); 
	
	TIM_TimeBaseStructInit(& TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock/1000000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseStructure.TIM_Period = 1000;  
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	
	TIM_TimeBaseInit(SPM2_TIMER,&TIM_TimeBaseStructure);
	
	TIM_ClearFlag(SPM2_TIMER , TIM_FLAG_Update);
	TIM_ARRPreloadConfig(SPM2_TIMER,DISABLE);
	TIM_ITConfig(SPM2_TIMER,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM16_IRQn);
	TIM_Cmd(SPM2_TIMER,ENABLE);
}
/*------------------------------------------------------------------------------------------------------------------------//

//for SPM1 STEP RUN

*/
void TIM14_IRQHandler(void)
{
	static uint8_t SPM1_Beatx;
	static uint32_t LastDir = 0;
//	static uint32_t Step;
	
	if(TIM_GetITStatus(TIM14, TIM_IT_Update) == SET)
	{
		if(Config.TB_Port[TB_1A].TB_PortMode == TB_SPM)
		{
			if(Config.TB_Port[TB_1A].Direction == TB_CW || Config.TB_Port[TB_1A].Direction == TB_CCW)
			{
				if(Config.TB_Port[TB_1A].SPM_Step > 0)
				{
					SPM1_Run(SPM1_Beatx);
					if(Config.TB_Port[TB_1A].Direction == TB_CW)
					{
						if(LastDir == TB_CCW)
						{
							SPM1_Beatx = 0;
						}
						else
						{
						  SPM1_Beatx = SPM1_Beatx < 7 ? SPM1_Beatx+1 : 0;
						}
						LastDir = TB_CW;
					}
					else if(Config.TB_Port[TB_1A].Direction == TB_CCW)
					{
						if(LastDir == TB_CW)
						{
							SPM1_Beatx = 0;
						}
						else
						{
						  SPM1_Beatx = SPM1_Beatx > 0 ? SPM1_Beatx-1 : 7;
						}
						LastDir = TB_CCW;
					}
					Config.TB_Port[TB_1A].SPM_Step--;
				}
				else
				{
					TB_SetWorkMode(TB_1A,TB_SHORT_BREAK); 
					TB_SetWorkMode(TB_1B,TB_SHORT_BREAK);
					memcpy(&ParamReg[TB_1A_SPM_STEP],&Config.TB_Port[TB_1A].SPM_Step,4);
				}
			}
		}
	}
	TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
}

/*------------------------------------------------------------------------------------------------------------------------//

//for SPM2 STEP RUN



*/
void TIM16_IRQHandler(void)
{			
	static uint8_t SPM2_Beatx;
	static uint32_t LastDir = 0;
	
	if(TIM_GetITStatus(TIM16, TIM_IT_Update) == SET)
	{
		if(Config.TB_Port[TB_2A].TB_PortMode == TB_SPM)
		{
			if(Config.TB_Port[TB_2A].Direction == TB_CW || Config.TB_Port[TB_2A].Direction == TB_CCW)
			{
				if(Config.TB_Port[TB_2A].SPM_Step > 0)
				{
					SPM2_Run(SPM2_Beatx);
					if(Config.TB_Port[TB_2A].Direction == TB_CW)
					{
						if(LastDir == TB_CCW)
						{
							SPM2_Beatx = 0;
						}
						else
						{
							SPM2_Beatx = SPM2_Beatx < 7 ? SPM2_Beatx+1 : 0;
						}
						LastDir = TB_CW;
					}
					else if(Config.TB_Port[TB_2A].Direction == TB_CCW)
					{
						if(LastDir == TB_CW)
						{
							SPM2_Beatx = 0;
						}
						else
						{
						  SPM2_Beatx = SPM2_Beatx > 0 ? SPM2_Beatx-1 : 7;
						}
						LastDir = TB_CCW;
					}
					Config.TB_Port[TB_2A].SPM_Step--;
				}
				else
				{
					TB_SetWorkMode(TB_2A,TB_SHORT_BREAK);
					TB_SetWorkMode(TB_2B,TB_SHORT_BREAK);
					memcpy(&ParamReg[TB_2A_SPM_STEP],&Config.TB_Port[TB_2A].SPM_Step,4);
				}
			}
		}
	}
	TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
}

/*------------------------------------------------------------------------------------------------------------------------//
SPM1_SetSpeed 


How long for 1 step,unit : us

*/
void SPM1_SetSpeed(void)
{
	SPM1_TIMER->ARR = Config.TB_Port[TB_1A].SPM_Speed;
}

/*------------------------------------------------------------------------------------------------------------------------//
SPM1_SetDuty

this will change output voltage of TB port
when duty is 50%, and Vbat(input voltage) is 10V, than TB port output max voltage is 5V.

*/
void SPM1_SetDuty(void)
{
	if(Config.TB_Port[TB_1A].Duty <= MaxDuty)
	{
	  TB_PWM_SetDuty(TB_PWMChannel[TB_1A],Config.TB_Port[TB_1A].Duty);
	}
	if(Config.TB_Port[TB_1B].Duty <= MaxDuty)
	{
	  TB_PWM_SetDuty(TB_PWMChannel[TB_1B],Config.TB_Port[TB_1B].Duty);
	}
}

/*------------------------------------------------------------------------------------------------------------------------//
SPM2_SetSpeed

*/
void SPM2_SetSpeed(void)
{
	SPM2_TIMER->ARR = Config.TB_Port[TB_2A].SPM_Speed;
}

/*------------------------------------------------------------------------------------------------------------------------//
SPM2_SetDuty


*/
void SPM2_SetDuty(void)
{
	if(Config.TB_Port[TB_2A].Duty <= MaxDuty)
	{
	  TB_PWM_SetDuty(TB_PWMChannel[TB_2A],Config.TB_Port[TB_2A].Duty);
	}
	if(Config.TB_Port[TB_2B].Duty <= MaxDuty)
	{
	  TB_PWM_SetDuty(TB_PWMChannel[TB_2B],Config.TB_Port[TB_2B].Duty);
	}
}

/*------------------------------------------------------------------------------------------------------------------------//
motor run 


*/
void SPM_Run(void)
{
  if(Config.TB_Port[TB_1A].TB_PortMode == TB_SPM)
	{
	  if(Config.TB_Port[TB_1A].Direction == TB_SHORT_BREAK || Config.TB_Port[TB_1A].Direction == TB_STOP)
		{
			
		}
		else if(Config.TB_Port[TB_1A].Direction == TB_CW || Config.TB_Port[TB_1A].Direction == TB_CCW)
		{
      SPM1_SetSpeed();
		  SPM1_SetDuty();
		}
	}
	if(Config.TB_Port[TB_2A].TB_PortMode == TB_SPM)
	{	  
		if(Config.TB_Port[TB_2A].Direction == TB_SHORT_BREAK || Config.TB_Port[TB_2A].Direction == TB_STOP)
		{
			
		}
		else if(Config.TB_Port[TB_2A].Direction == TB_CW || Config.TB_Port[TB_2A].Direction == TB_CCW)
		{
      SPM2_SetSpeed();
		  SPM2_SetDuty();
		}
	}
}

