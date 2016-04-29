#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32f0xx.h"
#include "stm32f0xx_conf.h"
#include "string.h"

#define FW_Date  0x20160421

#define ConfigAddr  0X800FC00

#define ConfigValid 0x006fb67c
#define ConfigSave 0X00F6B6C7

#define DefaultI2CAddr 0X4B

#define MaxDuty 1000
#define MaxFre  10000
#define AngleDiv 180

enum PARAM_REG
{
  CONFIG_VALID = 0,  
  CONFIG_TB_PWM_FREQ = CONFIG_VALID + 4,
  
  I2C_ADDRESS = CONFIG_TB_PWM_FREQ + 4, 
  
  TB_1A_MODE = I2C_ADDRESS + 1,
  TB_1A_DIR = TB_1A_MODE + 1,
  TB_1A_DUTY = TB_1A_DIR + 1,
  TB_1A_SPM_SPEED = TB_1A_DUTY + 2,
  TB_1A_SPM_STEP = TB_1A_SPM_SPEED + 4,
  
  TB_1B_MODE = TB_1A_SPM_STEP + 4,
  TB_1B_DIR = TB_1B_MODE + 1,
  TB_1B_DUTY = TB_1B_DIR + 1,
  TB_1B_SPM_SPEED = TB_1B_DUTY + 2,
  TB_1B_SPM_STEP = TB_1B_SPM_SPEED + 4,
  
  TB_2A_MODE = TB_1B_SPM_STEP + 4,
  TB_2A_DIR = TB_2A_MODE + 1,
  TB_2A_DUTY = TB_2A_DIR + 1,
  TB_2A_SPM_SPEED = TB_2A_DUTY + 2,
  TB_2A_SPM_STEP = TB_2A_SPM_SPEED + 4,
  
  TB_2B_MODE = TB_2A_SPM_STEP + 4,
  TB_2B_DIR = TB_2B_MODE + 1,
  TB_2B_DUTY = TB_2B_DIR + 1,
  TB_2B_SPM_SPEED = TB_2B_DUTY + 2,
  TB_2B_SPM_STEP = TB_2B_SPM_SPEED + 4,

  SVM1_STATE = TB_2B_SPM_STEP + 4,
  SVM1_FREQ = SVM1_STATE + 1,
  SVM1_ANGLE = SVM1_FREQ + 2,
  
  SVM2_STATE = SVM1_ANGLE + 2,
  SVM2_FREQ = SVM2_STATE + 1,
  SVM2_ANGLE = SVM2_FREQ + 2,
  
  SVM3_STATE = SVM2_ANGLE + 2,
  SVM3_FREQ = SVM3_STATE + 1,
  SVM3_ANGLE = SVM3_FREQ + 2,
  
  SVM4_STATE = SVM3_ANGLE + 2,
  SVM4_FREQ = SVM4_STATE + 1,
  SVM4_ANGLE = SVM4_FREQ + 2,
  
  SVM5_STATE = SVM4_ANGLE + 2,
  SVM5_FREQ = SVM5_STATE + 1,
  SVM5_ANGLE = SVM5_FREQ + 2,
  
  SVM6_STATE = SVM5_ANGLE + 2,
  SVM6_FREQ = SVM6_STATE + 1,
  SVM6_ANGLE = SVM6_FREQ + 2,

  IO1_STATE = SVM6_ANGLE + 2,
  IO1_MODE = IO1_STATE + 1,
  IO1_PUPD = IO1_MODE + 1,
  IO1_PPOD = IO1_PUPD + 1,

  IO2_STATE = IO1_PPOD + 1,
  IO2_MODE = IO2_STATE + 1,
  IO2_PUPD = IO2_MODE + 1,
  IO2_PPOD = IO2_PUPD + 1,
  
  IO3_STATE = IO2_PPOD + 1,
  IO3_MODE = IO3_STATE + 1,
  IO3_PUPD = IO3_MODE + 1,
  IO3_PPOD = IO3_PUPD + 1,
  
  IO4_STATE = IO3_PPOD + 1,
  IO4_MODE = IO4_STATE + 1,
  IO4_PUPD = IO4_MODE + 1,
  IO4_PPOD = IO4_PUPD + 1,
  
  IO5_STATE = IO4_PPOD + 1,
  IO5_MODE = IO5_STATE + 1,
  IO5_PUPD = IO5_MODE + 1,
  IO5_PPOD = IO5_PUPD + 1,
  
  IO6_STATE = IO5_PPOD + 1,
  IO6_MODE = IO6_STATE + 1,
  IO6_PUPD = IO6_MODE + 1,
  IO6_PPOD = IO6_PUPD + 1,
  
	FW_DATE = IO6_PPOD + 1,
	
  PARAM_REG_NUM = FW_DATE + 4
};

enum TB_PWM_CHANNEL
{
 TB1_PWMA_CH = TIM_Channel_1,
 TB1_PWMB_CH = TIM_Channel_2,
 TB2_PWMA_CH = TIM_Channel_3,
 TB2_PWMB_CH = TIM_Channel_4
};

enum TB_PORT
{
	TB_1A = 0,
	TB_1B,
	TB_2A,
	TB_2B,
	TB_PORT_NUM
};

enum TB_WORKMODE
{
	TB_SHORT_BREAK = 0,
	TB_CW,
	TB_CCW,
	TB_STOP,
	TB_WORKMODE_NUM
};

enum TB_PORTMODE
{
	TB_DCM = 0,
	TB_SPM,
	TB_PORTMODE_NUM
};

enum SVM_PORT
{
	SVM1 = 0,
	SVM2,
	SVM3,
	SVM4,
	SVM5,
	SVM6,
	SVM_PORT_NUM
};

enum SVM_STATE
{
	SVM_DISABLE = 0,
	SVM_ENABLE,
	SVM_STATE_NUM
};

enum IO_MODE
{
	IO_IN = 0,
  IO_OUT,
  IO_MODE_NUM	
};

enum IO_PUPD
{
	IO_PU = 0,
	IO_PD,
	IO_NP,
	IO_PUPD_NUM
};

enum IO_PPOD
{
	IO_PP = 0,
	IO_OD,
	IO_PPOD_NUM
};

enum IO_STATE
{
	IO_LOW = 0,
	IO_HIGH,
	IO_STATE_NUM
};

enum IO_PORT
{
  IO1 = 0,
	IO2,
	IO3,
	IO4,
	IO5,
	IO6,
	IO_NUM
};

#pragma pack(1)
typedef struct 
{
	enum TB_PORTMODE TB_PortMode;
	enum TB_WORKMODE Direction;
	uint16_t Duty;  //in spm mode,it is used to adapt different motor,in svm mode,it is use to set different duty , in dcm mode .it is used to set speed;
	uint32_t SPM_Speed;  //only for spm mode;
	uint32_t SPM_Step;
}TB_PortProperty_TypeDef;

typedef struct
{
	enum IO_STATE State;
	enum IO_MODE Mode;
	enum IO_PUPD PuPd;
	enum IO_PPOD PpOd;
}IO_Property_TypeDef;

typedef struct
{
	enum SVM_STATE State;
	uint16_t Freq;
	uint16_t Angle;
}SVM_PortProperty_TypeDef;

typedef struct 
{
	uint32_t Valid;
	
  uint32_t TB_PWM_Freq;
	
	uint8_t I2C_Address;

	TB_PortProperty_TypeDef  TB_Port[TB_PORT_NUM];
	SVM_PortProperty_TypeDef SVM_Port[SVM_PORT_NUM];
	IO_Property_TypeDef      IO_Port[IO_NUM];
	
	uint32_t FWDate;

}Config_TypeDef;

#pragma pack()


extern Config_TypeDef Config;

extern enum TB_PWM_CHANNEL TB_PWMChannel[];

extern uint8_t ParamReg[PARAM_REG_NUM];

extern __IO uint64_t localtime;

#include "init.h"
#include "led.h"
#include "TB6612FNG.h"
#include "communication.h"
#include "motor_dc.h"
#include "motor_stepper.h"
#include "motor_servo.h"
#include "communication.h"

#endif

