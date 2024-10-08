/*
 * general_purpose_timer.h
 *
 *  Created on: 02-Dec-2023
 *      Author: naveen
 */

#ifndef INC_GENERAL_PURPOSE_TIMER_H_
#define INC_GENERAL_PURPOSE_TIMER_H_

#include "stm32f4xx.h"


struct General_Purpose_Timer_RegDef_t
{
	uint32_t TIMx_CR1;
	uint32_t TIMx_CR2;
	uint32_t TIMx_SMCR;
	uint32_t TIMx_DIER;
	uint32_t TIMx_SR;
	uint32_t TIMx_EGR;
	uint32_t TIMx_CCMR1;
	uint32_t TIMx_CCMR2;
	uint32_t TIMx_CCER;
	uint32_t TIMx_CNT;
	uint32_t TIMx_PSC;
	uint32_t TIMx_ARR;
	uint32_t Reserved1;
	uint32_t TIMx_CCR1;
	uint32_t TIMx_CCR2;
	uint32_t TIMx_CCR3;
	uint32_t TIMx_CCR4;
	uint32_t Reserved2;
	uint32_t TIMx_DCR;
	uint32_t TIMx_DMAR;
	uint32_t TIM2_OR;
	uint32_t TIM5_OR;
};

struct General_Purpose_Timer_Config_t
{
	uint32_t 	Timer_PreScalerValue;
	uint32_t 	Timer_AutoReloadValue;
	uint8_t 	Timer_UpdateInterruptConfig;
	uint8_t 	Timer_Channel_1_Status;
	uint8_t 	Timer_Channel_1_Mode;
	uint32_t 	Timer_Channel_1_OutputCompareRegisterValue;
	uint8_t 	Timer_Channel_2_Status;
	uint8_t 	Timer_Channel_2_Mode;
	uint32_t 	Timer_Channel_2_OutputCompareRegisterValue;
	uint8_t 	Timer_Channel_3_Status;
	uint8_t 	Timer_Channel_3_Mode;
	uint32_t 	Timer_Channel_3_OutputCompareRegisterValue;
	uint8_t 	Timer_Channel_4_Status;
	uint8_t 	Timer_Channel_4_Mode;
	uint32_t 	Timer_Channel_4_OutputCompareRegisterValue;
};

struct General_Purpose_Timer_Handle_t
{
	struct General_Purpose_Timer_RegDef_t *pGeneral_Purpose_Timer;
	struct General_Purpose_Timer_Config_t General_Purpose_Timer_Config;
};

#define TIMx_CR1_CEN		0
#define TIMx_CR1_UDIS		1
#define TIMx_CR1_URS		2
#define TIMx_CR1_OPM		3
#define TIMx_CR1_ARPE		7

#define TIMx_CR2_MMS		4

#define TIMx_DIER_UIE		0
#define TIMx_DIER_UDE		8
#define TIMx_DIER_CC2IE		2

#define TIMx_SR_UIF			0

#define TIMx_EGR_UG			0

#define TIMx_CCMR1_CC1S		0
#define TIMx_CCMR1_OC1FE	2
#define TIMx_CCMR1_OC1PE	3
#define TIMx_CCMR1_OC1M		4
#define TIMx_CCMR1_OC1CE	7
#define TIMx_CCMR1_CC2S		8
#define TIMx_CCMR1_OC2FE	10
#define TIMx_CCMR1_OC2PE	11
#define TIMx_CCMR1_OC2M		12
#define TIMx_CCMR1_OC2CE	15

#define TIMx_CCMR1_IC1F		4
#define TIMx_CCMR1_IC1PSC	2
#define TIMx_CCMR1_IC2F		12
#define TIMx_CCMR1_IC2PSC	10

#define TIMx_CCMR2_CC3S		0
#define TIMx_CCMR2_OC3FE	2
#define TIMx_CCMR2_OC3PE	3
#define TIMx_CCMR2_OC3M		4
#define TIMx_CCMR2_OC3CE	7
#define TIMx_CCMR2_CC4S		9
#define TIMx_CCMR2_OC4FE	10
#define TIMx_CCMR2_OC4PE	11
#define TIMx_CCMR2_OC4M		12
#define TIMx_CCMR2_OC4CE	15

#define TIMx_CCER_CC1E		0
#define TIMx_CCER_CC1P		1
#define TIMx_CCER_CC1NP		3
#define TIMx_CCER_CC2E		4
#define TIMx_CCER_CC2P		5
#define TIMx_CCER_CC2NP		7
#define TIMx_CCER_CC3E		8
#define TIMx_CCER_CC3P		9
#define TIMx_CCER_CC3NP		11
#define TIMx_CCER_CC4E		12
#define TIMx_CCER_CC4P		13
#define TIMx_CCER_CC4NP		15

#define TIMx_FLAG_UIF		(1 << TIMx_SR_UIF)

#define TIMER_UPDATE_INTERRUPT_ENABLE	1
#define TIMER_UPDATE_INTERRUPT_DISABLE	0

#define TIMER_ONE_PULSE_MODE_ENABLE		1
#define TIMER_ONE_PULSE_MODE_DISABLE	0

#define TIMER_UPDATE_INTERRUPT_DISABLE	0

#define TIMER_CHANNEL_ENABLED			0
#define TIMER_CHANNEL_DISABLED			1

#define TIMER_CHANNEL_MODE_OUTPUT_HIGH			0
#define TIMER_CHANNEL_MODE_OUTPUT_LOW			1
#define TIMER_CHANNEL_MODE_OUTPUT_TOGGLE		2
#define TIMER_CHANNEL_MODE_OUTPUT_PWM1			3
#define TIMER_CHANNEL_MODE_OUTPUT_PWM2			4
#define TIMER_CHANNEL_MODE_INPUT_1				5
#define TIMER_CHANNEL_MODE_INPUT_2				6
#define TIMER_CHANNEL_MODE_INPUT_3				7
#define TIMER_CHANNEL_MODE_INPUT_CAPTURE		8

void General_Purpose_Timer_Init(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle);
void General_Purpose_Timer_PeripheralEnable(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle);
void General_Purpose_Timer_PeripheralDisable(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle);
void General_Purpose_Timer_EnableUpdateEventInterrupt(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle);
void General_Purpose_Timer_DisableUpdateEventInterrupt(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle);
void General_Purpose_Timer_ClearUpdateEventFlag(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle);
void General_Purpose_Timer_SetARPEFlag(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle);
void General_Purpose_Timer_IRQHandling(struct General_Purpose_Timer_Handle_t *pGeneral_Purpose_Timer_Handle);

#endif /* INC_GENERAL_PURPOSE_TIMER_H_ */
