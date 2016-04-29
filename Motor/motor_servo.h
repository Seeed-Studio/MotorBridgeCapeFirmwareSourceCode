#ifndef _MOTOR_SERVO_H_
#define _MOTOR_SERVO_H_

#include "main.h"

#define TIM_DIVn 40

#define SVM1_TIMER TIM1
#define SVM2_TIMER TIM1
#define SVM3_TIMER TIM16
#define SVM4_TIMER TIM17
#define SVM5_TIMER TIM15
#define SVM6_TIMER TIM15

#define SVM1_TIMER_APB RCC_APB2Periph_TIM1
#define SVM2_TIMER_APB RCC_APB2Periph_TIM1
#define SVM3_TIMER_APB RCC_APB2Periph_TIM16
#define SVM4_TIMER_APB RCC_APB2Periph_TIM17
#define SVM5_TIMER_APB RCC_APB2Periph_TIM15
#define SVM6_TIMER_APB RCC_APB2Periph_TIM15

#define SVM1_PWM_CH TIM_Channel_4
#define SVM2_PWM_CH TIM_Channel_1
#define SVM3_PWM_CH TIM_Channel_1
#define SVM4_PWM_CH TIM_Channel_1
#define SVM5_PWM_CH TIM_Channel_1
#define SVM6_PWM_CH TIM_Channel_2

#define SVM1_GPIO GPIOA
#define SVM2_GPIO GPIOA
#define SVM3_GPIO GPIOB
#define SVM4_GPIO GPIOB
#define SVM5_GPIO GPIOA
#define SVM6_GPIO GPIOA

#define SVM1_PIN GPIO_Pin_11
#define SVM2_PIN GPIO_Pin_8
#define SVM3_PIN GPIO_Pin_8
#define SVM4_PIN GPIO_Pin_9
#define SVM5_PIN GPIO_Pin_2
#define SVM6_PIN GPIO_Pin_3

#define SVM1_PINSOURCE GPIO_PinSource11
#define SVM2_PINSOURCE GPIO_PinSource8
#define SVM3_PINSOURCE GPIO_PinSource8
#define SVM4_PINSOURCE GPIO_PinSource9
#define SVM5_PINSOURCE GPIO_PinSource2
#define SVM6_PINSOURCE GPIO_PinSource3


#define systickFreq 20000

void SVM_SetDuty(enum SVM_PORT SVMx,uint16_t Angle);

void SVM_Init(void);

void SVM_Run(void);

extern float SVM_MinDuty;
extern float SVM_MaxDuty;

#endif


