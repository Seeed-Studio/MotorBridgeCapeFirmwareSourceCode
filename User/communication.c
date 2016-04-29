#include "main.h"

I2C_Slaver_Communicate_TypeDef I2C_Communication;

//uint8_t CheckResetIndex[23] = {I2C_ADDRESS,TB_1A_MODE,TB_1B_MODE,TB_2A_MODE,TB_2B_MODE,
//                                   IO1_MODE,IO1_PUPD,IO1_PPOD,
//                                   IO2_MODE,IO2_PUPD,IO2_PPOD,
//                                   IO3_MODE,IO3_PUPD,IO3_PPOD,
//                                   IO4_MODE,IO4_PUPD,IO4_PPOD,
//                                   IO5_MODE,IO5_PUPD,IO5_PPOD,
//                                   IO6_MODE,IO6_PUPD,IO6_PPOD,
//                                   };

//
//this index used for check the data which has modified and need to restart initialization.
//
//
uint8_t CheckResetIndex[23] = {
TB_1A_MODE,TB_1B_MODE,TB_2A_MODE,TB_2B_MODE,
IO1_MODE,IO1_PUPD,IO1_PPOD,
IO2_MODE,IO2_PUPD,IO2_PPOD,
IO3_MODE,IO3_PUPD,IO3_PPOD,
IO4_MODE,IO4_PUPD,IO4_PPOD,
IO5_MODE,IO5_PUPD,IO5_PPOD,
IO6_MODE,IO6_PUPD,IO6_PPOD,
};

uint8_t IsNeedReset(uint8_t addr)
{
	int i;
	for(i = 0; i<23; i++)
	{
		if(addr == CheckResetIndex[i])
		{
			return 1;
	  }
	}
	return 0;
}





/*------------------------------------------------------------------------------------------------------------------------//
I2C init

*/
void I2CInit(uint16_t I2C_OwnAddress, uint32_t ClockSpeed)
{
	I2C_InitTypeDef I2C_MyInit;

	GPIOInit(GPIOB,GPIO_Pin_6,GPIO_Mode_AF,GPIO_PuPd_NOPULL,GPIO_OType_OD);
	GPIOInit(GPIOB,GPIO_Pin_7,GPIO_Mode_AF,GPIO_PuPd_NOPULL,GPIO_OType_OD);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);
	
	I2C_DeInit(I2CPort);
	
	RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 ,ENABLE);
	
	I2C_MyInit.I2C_Ack = I2C_Ack_Enable ;
	I2C_MyInit.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_MyInit.I2C_Mode = I2C_Mode_I2C ;
	I2C_MyInit.I2C_OwnAddress1 = I2C_OwnAddress<<1;
	I2C_MyInit.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_MyInit.I2C_DigitalFilter = 0x00;
	I2C_MyInit.I2C_Mode = I2C_Mode_I2C;
	switch(ClockSpeed)
	{
		case 100:
			I2C_MyInit.I2C_Timing = 0x10805E89;
			break;
		case 400:
			break;
	}
 //This parameter must be set by referring to I2C_Timing_Config_Tool|I2C_IT_TXI
	I2C_Init(I2CPort , &I2C_MyInit);
	//I2C_ITConfig(I2CPort,I2C_IT_ERRI|I2C_IT_STOPI|I2C_IT_NACKI|I2C_IT_ADDRI|I2C_IT_RXI|I2C_IT_TCI|I2C_IT_TXI,ENABLE);
	I2C_ITConfig(I2CPort,I2C_IT_STOPI|I2C_IT_ADDRI|I2C_IT_RXI|I2C_IT_TXI,ENABLE);
	I2C_ClearFlag(I2CPort,I2C_FLAG_ADDR|I2C_FLAG_NACKF|I2C_FLAG_STOPF|I2C_FLAG_BERR|I2C_FLAG_ARLO|I2C_FLAG_OVR|I2C_FLAG_PECERR|I2C_FLAG_TIMEOUT|I2C_FLAG_ALERT);
	I2C_ClearITPendingBit(I2CPort,I2C_IT_ADDR|I2C_IT_NACKF|I2C_IT_STOPF|I2C_IT_BERR|I2C_IT_ARLO|I2C_IT_OVR|I2C_IT_PECERR|I2C_IT_TIMEOUT|I2C_IT_ALERT);
	NVIC_EnableIRQ(I2C1_IRQn);
	I2C_Cmd(I2CPort, ENABLE);
}




/*------------------------------------------------------------------------------------------------------------------------//
I2Cs interrupt handler


*/
void I2C1_IRQHandler(void)
{
	static uint8_t Counter = 0;  
	//
	//receive a byte
	//
	if(I2C_GetFlagStatus(I2CPort,I2C_FLAG_RXNE) == SET && I2C_GetFlagStatus(I2CPort,I2C_FLAG_BUSY))
	{
		I2C_Communication.DataBuffer[I2C_Communication.DataLength] = I2C_ReceiveData(I2CPort);
		I2C_Communication.DataLength++;
	}
	//
	//send a byte
	//
	else if(I2C_GetFlagStatus(I2CPort,I2C_FLAG_TXIS) == SET && I2C_GetFlagStatus(I2CPort,I2C_FLAG_BUSY) == SET)
	{	
		 if(I2C_Communication.DataLength > Counter)
		 {
			 I2C_SendData(I2CPort,I2C_Communication.DataBuffer[Counter]);
			 Counter++;
		 }
		 else
		 {
			 I2C_Communication.DataLength = 0;
			 I2C_SendData(I2CPort,0);
		   I2C_AcknowledgeConfig(I2CPort,DISABLE);
			 Counter = 0;
		 }
		 I2CPort->ISR |= (I2C_FLAG_TXE);
		 I2CPort->ISR |= (I2C_FLAG_TXIS);
	}
	//
	//NACK
	//
	else if (I2C_GetFlagStatus(I2CPort,I2C_FLAG_NACKF) == SET)
	{
		I2C_ClearFlag(I2CPort,I2C_FLAG_NACKF);
	}
	//
	//address matched
	//
	else if(I2C_GetFlagStatus(I2CPort,I2C_FLAG_ADDR) == SET && I2C_GetFlagStatus(I2CPort,I2C_FLAG_RXNE) != SET)
	{		 
     I2C_GenerateSTOP(I2CPort, DISABLE);	 
		 if(I2C_Communication.DataLength == 0)
		 {
		   I2C_AcknowledgeConfig(I2CPort,ENABLE);
		 }
		 else if(I2C_Communication.DataLength >= 3 && I2C_Communication.DataBuffer[0] == 0)
		 {
			 I2C_Communication.DataLength = I2C_Communication.DataBuffer[2];
			 I2C_Communication.ReadAddr = I2C_Communication.DataBuffer[1];
			 memset(I2C_Communication.DataBuffer,0,I2c_BufferSize);
			 memcpy(I2C_Communication.DataBuffer,ParamReg+I2C_Communication.ReadAddr,I2C_Communication.DataLength);
			 I2C_SendData(I2CPort,I2C_Communication.DataBuffer[0]);
			 Counter = 1;
		 }
		I2C_ClearFlag(I2CPort,I2C_FLAG_ADDR);
	}
	//
	//stop single matched
	//
	else if(I2C_GetFlagStatus(I2CPort,I2C_FLAG_STOPF) == SET)
	{  
		 I2C_GenerateSTOP(I2CPort, ENABLE); 
		 //PER FRAME Must more than 2 bytes,and less than 3 + PARAM_REG_NUM , first bytes is mean read or write
		 if(I2C_Communication.DataLength >= 3 && I2C_Communication.DataBuffer[0] == 1 && I2C_Communication.DataLength < 3 + PARAM_REG_NUM)
		 {
			 I2C_Communication.WriteAddr = I2C_Communication.DataBuffer[1];
			 if(I2C_Communication.WriteAddr < FW_DATE) //FW_DATE is not allow to be writed.
			 {
				 //copy to ParamReg,it is use to read by user;
				 memcpy(ParamReg+I2C_Communication.WriteAddr,&I2C_Communication.DataBuffer[2],I2C_Communication.DataLength-2);
				 
				 memcpy(&Config,ParamReg,sizeof(Config_TypeDef));
				 //memcpy(&Config+I2C_Communication.WriteAddr,&I2C_Communication.DataBuffer[2],I2C_Communication.DataLength-2);
				 if(IsNeedReset(I2C_Communication.WriteAddr))
				 {
					 I2C_Communication.IsNeedReset = 1;
				 }
			 }    
			 //clear buffer.
			 memset(&I2C_Communication.DataBuffer,0,I2C_Communication.DataLength);
			 I2C_Communication.DataLength = 0;
		 }
		I2C_ClearFlag(I2CPort,I2C_FLAG_STOPF);
	} 
}


