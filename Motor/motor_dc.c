#include "main.h"


/*------------------------------------------------------------------------------------------------------------------------//
Set one DC motor's work mode
DCMx @Ref DCM_Interface

*/
void DCM_SetMode(enum TB_PORT DCMx, enum TB_WORKMODE Mode)
{
	TB_SetWorkMode(DCMx,Mode);
}


/*------------------------------------------------------------------------------------------------------------------------//
Set one DC motor's speed 
the param Speed must adjust between 0~100
Setting to 0 means the motor will be stopped,
100 means the motor will Set to the maxmun speed(maxmun voltage)


*/ 
void DCM_SetSpeed(enum TB_PORT DCMx, unsigned int Speed)
{
	TB_PWM_SetDuty(TB_PWMChannel[DCMx],Speed);
}


void DCM_Run(void)
{
	if(Config.TB_Port[TB_1A].TB_PortMode == TB_DCM)
	{
		if(Config.TB_Port[TB_1A].Direction < TB_WORKMODE_NUM)
		{
		  DCM_SetMode(TB_1A,Config.TB_Port[TB_1A].Direction);
		}
		if(Config.TB_Port[TB_1A].Duty <= MaxDuty)
		{
		  DCM_SetSpeed(TB_1A,Config.TB_Port[TB_1A].Duty);
		}
		
		if(Config.TB_Port[TB_1B].Direction < TB_WORKMODE_NUM)
		{
		  DCM_SetMode(TB_1B,Config.TB_Port[TB_1B].Direction);
		}
		if(Config.TB_Port[TB_1B].Duty  <= MaxDuty)
		{
		  DCM_SetSpeed(TB_1B,Config.TB_Port[TB_1B].Duty);
		}
	}
	
	if(Config.TB_Port[TB_2A].TB_PortMode == TB_DCM)
	{
		if(Config.TB_Port[TB_2A].Direction < TB_WORKMODE_NUM)
		{
		  DCM_SetMode(TB_2A,Config.TB_Port[TB_2A].Direction);
		}
		if(Config.TB_Port[TB_2A].Duty <= MaxDuty)
		{
		  DCM_SetSpeed(TB_2A,Config.TB_Port[TB_2A].Duty);
		}
		
		if(Config.TB_Port[TB_2B].Direction < TB_WORKMODE_NUM)
		{
		  DCM_SetMode(TB_2B,Config.TB_Port[TB_2B].Direction);
		}
		if(Config.TB_Port[TB_2B].Duty  <= MaxDuty)
		{
		  DCM_SetSpeed(TB_2B,Config.TB_Port[TB_2B].Duty);
		}
	}
}

