#include "main.h"

/*------------------------------------------------------------------------------------------------------------------------//
config struct and default value



*/
Config_TypeDef Config;

//default config data
Config_TypeDef DeConfigValue = 
{
	ConfigValid,1000,DefaultI2CAddr, \
	//TB_PORT default setting
	{{TB_DCM,TB_STOP,500,5000,0},{TB_DCM,TB_STOP,500,5000,0},{TB_DCM,TB_STOP,500,5000,0},{TB_DCM,TB_STOP,500,5000,0}}, \
	//SVM_PORT default setting
	{{SVM_DISABLE,50,90},{SVM_DISABLE,50,90},{SVM_DISABLE,50,90},{SVM_DISABLE,50,90},{SVM_DISABLE,50,90},{SVM_DISABLE,50,90}}, \
	//IO_default setting
	{{IO_LOW,IO_IN,IO_NP,IO_OD},{IO_LOW,IO_IN,IO_NP,IO_OD},{IO_LOW,IO_IN,IO_NP,IO_OD},{IO_LOW,IO_IN,IO_NP,IO_OD},{IO_LOW,IO_IN,IO_NP,IO_OD},{IO_LOW,IO_IN,IO_NP,IO_OD}}, \
	//
	FW_Date

};

//
GPIO_TypeDef*     GPIO_Index[]={GPIOB,GPIOB,GPIOA,GPIOA,GPIOA,GPIOA};
uint16_t          GPIO_PinIndex[]= {GPIO_Pin_11,GPIO_Pin_10,GPIO_Pin_7,GPIO_Pin_6,GPIO_Pin_5,GPIO_Pin_4};
GPIOMode_TypeDef  GPIO_ModeIndex[]={GPIO_Mode_IN,GPIO_Mode_OUT};
GPIOPuPd_TypeDef  GPIO_PuPdIndex[]={GPIO_PuPd_UP,GPIO_PuPd_DOWN,GPIO_PuPd_NOPULL};
GPIOOType_TypeDef GPIO_PpOdIndex[]={GPIO_OType_PP,GPIO_OType_OD};

__IO uint64_t localtime;
	
/*------------------------------------------------------------------------------------------------------------------------//
param reg 

used for communication.

*/
uint8_t ParamReg[PARAM_REG_NUM] = {0};


void SysInit(void);
void SaveData(void);
void IO_Run(void);




/*------------------------------------------------------------------------------------------------------------------------//
main function


*/

int main(void)
{
	uint8_t tmp_addr = 0X4B;
	SystemClockInit();
	//
	//get config data from flash
	//
	memcpy((uint8_t*)&Config,(uint8_t*)ConfigAddr,sizeof(Config_TypeDef));
	
	//
	//i2c address verification 
	//
	if(Config.I2C_Address > 0 && Config.I2C_Address <= 127)
	{
		tmp_addr = Config.I2C_Address;
	}
	
	//
	//if Config.Valid == ConfigSave, that means user ever saved all config data and want to use personal config data
	//
	if(Config.Valid != ConfigSave)
	{
	  memcpy(&Config,&DeConfigValue,sizeof(Config_TypeDef));
	}
	
	//
	//use i2c address saved in the flash.
	//
	Config.I2C_Address = tmp_addr;
	
	//
	//copy data to ParamReg for communication
	//
	Config.Valid = ConfigValid;
	Config.FWDate = FW_Date;
  memcpy(ParamReg,&Config,sizeof(Config_TypeDef));
	
	//
	//init
	//
	SysInit();
	//
	//main loop
	//
	while(1)
	{
		SaveData();	
		SVM_Run();
		DCM_Run();
		SPM_Run();
		IO_Run();	
	}
	
}
/*------------------------------------------------------------------------------------------------------------------------//
system init


*/
void SysInit(void)
{
	int i;
	
	if(Config.I2C_Address > 0 && Config.I2C_Address <= 127)
	{
	  I2CInit(Config.I2C_Address,100);
	}
	else
	{
		I2CInit(0X4B,100);
	}
	
	for(i=0;i<6;i++)
	{
		if(Config.IO_Port[i].Mode < IO_MODE_NUM && Config.IO_Port[i].PuPd < IO_PUPD_NUM && Config.IO_Port[i].PpOd < IO_PPOD_NUM)
		{
		  GPIOInit(GPIO_Index[i],GPIO_PinIndex[i],GPIO_ModeIndex[Config.IO_Port[i].Mode],GPIO_PuPdIndex[Config.IO_Port[i].PuPd],GPIO_PpOdIndex[Config.IO_Port[i].PpOd]);
		}
		else
		{
			GPIOInit(GPIO_Index[i],GPIO_PinIndex[i],GPIO_ModeIndex[IO_IN],GPIO_PuPdIndex[IO_NP],GPIO_PpOdIndex[IO_PP]);
		}
	}
	
	TB_Init();
	SVM_Init();
	
	if(Config.TB_Port[TB_1A].TB_PortMode == TB_SPM)
	{
		SPM_Init(TB_1A);
	}
	if(Config.TB_Port[TB_2A].TB_PortMode == TB_SPM)
	{
		SPM_Init(TB_2A);
	}
	
}

/*------------------------------------------------------------------------------------------------------------------------//
Set IO State


*/
uint8_t IO_STATE_RegIndex[6]={IO1_STATE,IO2_STATE,IO3_STATE,IO4_STATE,IO5_STATE,IO6_STATE};
void IO_Run(void)
{
	int i;
	for(i=0;i<6;i++)
	{
		if(Config.IO_Port[i].Mode == IO_OUT)
		{
			if(Config.IO_Port[i].State == IO_HIGH)
			{
				GPIO_SetBits(GPIO_Index[i],GPIO_PinIndex[i]);
			}
			else if(Config.IO_Port[i].State == IO_LOW)
			{
				GPIO_ResetBits(GPIO_Index[i],GPIO_PinIndex[i]);
			}
	  }
		else if(Config.IO_Port[i].Mode == IO_IN)
		{
			Config.IO_Port[i].State = (enum IO_STATE)GPIO_ReadInputDataBit(GPIO_Index[i],GPIO_PinIndex[i]);
			ParamReg[IO_STATE_RegIndex[i]] = Config.IO_Port[i].State;
		}
	}
}

/*------------------------------------------------------------------------------------------------------------------------//
flash operation
parameter 'size' is all data's number in byte;

*/
#define PageSize 1024

//
//write operation
//
uint8_t Flash_Write(uint32_t StartAddr,uint32_t *p_data,uint32_t size)
{	
	volatile FLASH_Status FLASHStatus;	
	uint32_t EndAddr=StartAddr+size;	
	__IO uint32_t NbrOfPage = 0;	
	uint32_t EraseCounter = 0x0, Address = 0x0;
	int i;
	int MemoryProgramStatus=1;
	
	FLASH_Unlock();     
	NbrOfPage=((EndAddr-StartAddr)>>10)+1;
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR);
	FLASHStatus=FLASH_COMPLETE;
	
	for(EraseCounter=0;(EraseCounter<NbrOfPage)&&(FLASHStatus==FLASH_COMPLETE);EraseCounter++)
	{		
		FLASHStatus=FLASH_ErasePage(StartAddr+(PageSize*EraseCounter));	
	}
	
	Address = StartAddr;
	i=0;
	
	while((Address<EndAddr)&&(FLASHStatus==FLASH_COMPLETE))	
	{	FLASHStatus=FLASH_ProgramWord(Address,p_data[i++]);
		Address=Address+4;
	}
	
	Address = StartAddr;
	i=0;
	
	while((Address < EndAddr) && (MemoryProgramStatus != 0))
	{	
		if((*(__IO uint32_t*) Address) != p_data[i++])
		{		MemoryProgramStatus = 0;
				return 1;
		}
		Address += 4;
	}
	
	return 0;
}



//
//read operation
//
int Flash_Read(uint32_t StartAddr,uint32_t *p_data,uint32_t size)
{
	uint32_t EndAddr=StartAddr+size;
	int MemoryProgramStatus=1;
	uint32_t Address = 0x0;
	int i=0;
	Address = StartAddr;
	while((Address < EndAddr) && (MemoryProgramStatus != 0))
	{
		p_data[i++]=(*(__IO uint32_t*) Address);
		Address += 4;	
	}
	return 0;	
}



//
//data saving operation
//
void SaveData(void)
{
  Config_TypeDef Config_tmp;
	
	if(Config.Valid == ConfigSave)
	{
		//system reset to load new data
		Flash_Write(ConfigAddr,(uint32_t*)&Config,sizeof(Config_TypeDef));
		NVIC_SystemReset();
	}
	else if(Config.Valid == 0)
	{
		//system reset to load default data
		memcpy((uint8_t*)&Config_tmp,(uint8_t*)ConfigAddr,sizeof(Config_TypeDef));
    Config_tmp.Valid = Config.Valid;
		Flash_Write(ConfigAddr,(uint32_t*)&Config_tmp,sizeof(Config_TypeDef));
		NVIC_SystemReset();	
	}
	else if(Config.Valid == 1)
	{
		//save only one byte of addr
		memcpy((uint8_t*)&Config_tmp,(uint8_t*)ConfigAddr,sizeof(Config_TypeDef));
    Config_tmp.I2C_Address = Config.I2C_Address;
		Flash_Write(ConfigAddr,(uint32_t*)&Config_tmp,sizeof(Config_TypeDef));
		
		NVIC_SystemReset();
	}
	else
	{
		Config.Valid = ConfigValid;
	}
	
	if(I2C_Communication.IsNeedReset == 1)
	{
		SysInit();
		I2C_Communication.IsNeedReset = 0;
	}
}


