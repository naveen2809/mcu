/*
 * timer_driver.h
 *
 *  Created on: 29-Aug-2023
 *      Author: naveen
 */

#ifndef INC_TIMER_DRIVER_H_
#define INC_TIMER_DRIVER_H_

#include "stm32f4xx.h"


struct Basic_Timer_RegDef_t
{
	uint32_t TIMx_CR1;
	uint32_t TIMx_CR2;
	uint32_t TIMx_Reserved1;
	uint32_t TIMx_DIER;
	uint32_t TIMx_SR;
	uint32_t TIMx_EGR;
	uint32_t TIMx_Reserved2;
	uint32_t TIMx_Reserved3;
	uint32_t TIMx_Reserved4;
	uint32_t TIMx_CNT;
	uint32_t TIMx_PSC;
	uint32_t TIMx_ARR;
};

struct Basic_Timer_Config_t
{
	uint16_t Timer_PreScalerValue;
	uint16_t Timer_AutoReloadValue;
	uint16_t Timer_OnePulseModeConfig;
	uint16_t Timer_UpdateInterruptConfig;
};

struct Basic_Timer_Handle_t
{
	struct Basic_Timer_RegDef_t *pBasic_Timer;
	struct Basic_Timer_Config_t Basic_Timer_Config;
};

#define TIMx_CR1_CEN		0
#define TIMx_CR1_UDIS		1
#define TIMx_CR1_URS		2
#define TIMx_CR1_OPM		3
#define TIMx_CR1_ARPE		7

#define TIMx_CR2_MMS		4

#define TIMx_DIER_UIE		0
#define TIMx_DIER_UDE		8

#define TIMx_SR_UIF			0

#define TIMx_EGR_UG			0

#define TIMx_FLAG_UIF		(1 << TIMx_SR_UIF)

#define TIMER_UPDATE_INTERRUPT_ENABLE	1
#define TIMER_UPDATE_INTERRUPT_DISABLE	0

#define TIMER_ONE_PULSE_MODE_ENABLE		1
#define TIMER_ONE_PULSE_MODE_DISABLE	0

#define TIMER_UPDATE_INTERRUPT_DISABLE	0

void Basic_Timer_Init(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);
void Basic_Timer_PeripheralEnable(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);
void Basic_Timer_PeripheralDisable(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);
void Basic_Timer_EnableUpdateEventInterrupt(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);
void Basic_Timer_DisableUpdateEventInterrupt(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);
void Basic_Timer_ClearUpdateEventFlag(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);
void Basic_Timer_SetARPEFlag(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);
void Basic_Timer_IRQHandling(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);

#endif /* INC_TIMER_DRIVER_H_ */
