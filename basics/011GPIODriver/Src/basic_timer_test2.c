/*
 * basic_timer_test.c
 *
 *  Created on: 29-Aug-2023
 *      Author: naveen
 */

#include "timer_driver.h"
#include "gpio_driver.h"

#define GPIO_PIN_LED_1	GPIO_PIN_12
#define GPIO_PIN_LED_2	GPIO_PIN_13
#define GPIO_PIN_LED_3	GPIO_PIN_14
#define GPIO_PIN_LED_4	GPIO_PIN_15
#define TIM6_IRQ_NUM	54
#define TIM7_IRQ_NUM	55

struct Basic_Timer_Handle_t TIM6_Handle;
struct Basic_Timer_Handle_t TIM7_Handle;

void TIM6_DAC_IRQHandler(void);
void TIM7_IRQHandler(void);
void Basic_Timer_Interrupt_Callback(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);

int main(void)
{

	//GPIO Configuration for on-board LED
	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_1, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_2, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_3, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_4, GPIO_MODE_OUTPUT);

	//Enabling the Interrupt for TIM6 and TIM7 Peripherals in NVIC
	NVIC_EnableIRQ(TIM6_IRQ_NUM);
	NVIC_EnableIRQ(TIM7_IRQ_NUM);

	//TIM6 Configuration
	TIM6_Handle.pBasic_Timer = (struct Basic_Timer_RegDef_t *)TIM6;
	TIM6_Handle.Basic_Timer_Config.Timer_PreScalerValue = 0xffff;
	TIM6_Handle.Basic_Timer_Config.Timer_AutoReloadValue = 0x7f;
	TIM6_Handle.Basic_Timer_Config.Timer_UpdateInterruptConfig = TIMER_UPDATE_INTERRUPT_ENABLE;

	EnablePeriClk(TIM6);
	Basic_Timer_Init(&TIM6_Handle);
	Basic_Timer_PeripheralEnable(&TIM6_Handle);

	//TIM7 Configuration
	TIM7_Handle.pBasic_Timer = (struct Basic_Timer_RegDef_t *)TIM7;
	TIM7_Handle.Basic_Timer_Config.Timer_PreScalerValue = 0xffff;
	TIM7_Handle.Basic_Timer_Config.Timer_AutoReloadValue = 0x2fa;
	TIM7_Handle.Basic_Timer_Config.Timer_UpdateInterruptConfig = TIMER_UPDATE_INTERRUPT_ENABLE;

	EnablePeriClk(TIM7);
	Basic_Timer_Init(&TIM7_Handle);
	Basic_Timer_PeripheralEnable(&TIM7_Handle);

	while(1);

	return 0;
}

void TIM6_DAC_IRQHandler(void)
{
	Basic_Timer_IRQHandling(&TIM6_Handle);
}

void TIM7_IRQHandler(void)
{
	Basic_Timer_IRQHandling(&TIM7_Handle);
}

void Basic_Timer_Interrupt_Callback(struct Basic_Timer_Handle_t *pBasic_Timer_Handle)
{
	Basic_Timer_ClearUpdateEventFlag(pBasic_Timer_Handle);

	if(pBasic_Timer_Handle->pBasic_Timer == (struct Basic_Timer_RegDef_t *)TIM6)
	{
		GPIOTogglePin(GPIOD, GPIO_PIN_LED_1);
		GPIOTogglePin(GPIOD, GPIO_PIN_LED_3);
	}
	else if(pBasic_Timer_Handle->pBasic_Timer == (struct Basic_Timer_RegDef_t *)TIM7)
	{
		GPIOTogglePin(GPIOD, GPIO_PIN_LED_2);
		GPIOTogglePin(GPIOD, GPIO_PIN_LED_4);
	}
}
