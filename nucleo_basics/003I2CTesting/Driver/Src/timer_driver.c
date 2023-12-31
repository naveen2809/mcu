/*
 * timer_driver.c
 *
 *  Created on: 29-Aug-2023
 *      Author: naveen
 */


#include "timer_driver.h"

extern void Basic_Timer_Interrupt_Callback(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);

void Basic_Timer_Init(struct Basic_Timer_Handle_t *pBasic_Timer_Handle)
{

	//1. Configure the PreScaler Value

	pBasic_Timer_Handle->pBasic_Timer->TIMx_PSC = pBasic_Timer_Handle->Basic_Timer_Config.Timer_PreScalerValue;

	//2. Configure the Auto Reload Value

	pBasic_Timer_Handle->pBasic_Timer->TIMx_ARR = pBasic_Timer_Handle->Basic_Timer_Config.Timer_AutoReloadValue;

	//3. Enable One Pulse Mode

	if(pBasic_Timer_Handle->Basic_Timer_Config.Timer_OnePulseModeConfig == TIMER_ONE_PULSE_MODE_ENABLE)
	{
			pBasic_Timer_Handle->pBasic_Timer->TIMx_CR1 |= (1 << TIMx_CR1_OPM);
	}

	//4. Enable/Disable the Update Event Interrupt

	if(pBasic_Timer_Handle->Basic_Timer_Config.Timer_UpdateInterruptConfig == TIMER_UPDATE_INTERRUPT_ENABLE)
	{
		pBasic_Timer_Handle->pBasic_Timer->TIMx_DIER |= (1 << TIMx_DIER_UIE);
	}
	else if(pBasic_Timer_Handle->Basic_Timer_Config.Timer_UpdateInterruptConfig == TIMER_UPDATE_INTERRUPT_DISABLE)
	{
		pBasic_Timer_Handle->pBasic_Timer->TIMx_DIER &= ~(1 << TIMx_DIER_UIE);
	}

}

void Basic_Timer_PeripheralEnable(struct Basic_Timer_Handle_t *pBasic_Timer_Handle)
{
	pBasic_Timer_Handle->pBasic_Timer->TIMx_CR1 |= (1 << TIMx_CR1_CEN);
}

void Basic_Timer_PeripheralDisable(struct Basic_Timer_Handle_t *pBasic_Timer_Handle)
{
	pBasic_Timer_Handle->pBasic_Timer->TIMx_CR1 &= ~(1 << TIMx_CR1_CEN);
}

void Basic_Timer_EnableUpdateEventInterrupt(struct Basic_Timer_Handle_t *pBasic_Timer_Handle)
{
	pBasic_Timer_Handle->pBasic_Timer->TIMx_DIER |= (1 << TIMx_DIER_UIE);
}

void Basic_Timer_DisableUpdateEventInterrupt(struct Basic_Timer_Handle_t *pBasic_Timer_Handle)
{
	pBasic_Timer_Handle->pBasic_Timer->TIMx_DIER &= ~(1 << TIMx_DIER_UIE);
}

void Basic_Timer_ClearUpdateEventFlag(struct Basic_Timer_Handle_t *pBasic_Timer_Handle)
{
	pBasic_Timer_Handle->pBasic_Timer->TIMx_SR &= ~(1 << TIMx_SR_UIF);
}

void Basic_Timer_SetARPEFlag(struct Basic_Timer_Handle_t *pBasic_Timer_Handle)
{
	pBasic_Timer_Handle->pBasic_Timer->TIMx_CR1 |= (1 << TIMx_CR1_ARPE);
}

void Basic_Timer_IRQHandling(struct Basic_Timer_Handle_t *pBasic_Timer_Handle)
{
	Basic_Timer_Interrupt_Callback(pBasic_Timer_Handle);
}
