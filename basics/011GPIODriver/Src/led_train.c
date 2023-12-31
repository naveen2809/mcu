/*
 * basic_timer_test.c
 *
 *  Created on: 29-Aug-2023
 *      Author: naveen
 */

#include "timer_driver.h"
#include "gpio_driver.h"

#define GPIO_1				GPIO_PIN_1
#define GPIO_2				GPIO_PIN_3
#define GPIO_3				GPIO_PIN_5
#define GPIO_4				GPIO_PIN_7
#define TIM6_IRQ_NUM		54
#define TIM7_IRQ_NUM		55

struct Basic_Timer_Handle_t TIM6_Handle;

uint8_t gpio_pins[4] = {1,3,5,7};
uint32_t interrupt_count = 0;
uint8_t prev_pin;
uint8_t current_pin;

void TIM6_DAC_IRQHandler(void);
void Basic_Timer_Interrupt_Callback(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);

int main(void)
{

	//GPIO Configuration for on-board LED
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, GPIO_1, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_2, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_3, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_4, GPIO_MODE_OUTPUT);

	//Initialize all GPIOs to LOW
	GPIOWritePin(GPIOA,GPIO_1,GPIO_LOW);
	GPIOWritePin(GPIOA,GPIO_2,GPIO_LOW);
	GPIOWritePin(GPIOA,GPIO_3,GPIO_LOW);
	GPIOWritePin(GPIOA,GPIO_4,GPIO_LOW);

	//Enabling the Interrupt for TIM6 and TIM7 Peripherals in NVIC
	NVIC_EnableIRQ(TIM6_IRQ_NUM);

	//TIM6 Configuration
	TIM6_Handle.pBasic_Timer = (struct Basic_Timer_RegDef_t *)TIM6;
	TIM6_Handle.Basic_Timer_Config.Timer_PreScalerValue = 0xffff;
	TIM6_Handle.Basic_Timer_Config.Timer_AutoReloadValue = 0xff;
	TIM6_Handle.Basic_Timer_Config.Timer_UpdateInterruptConfig = TIMER_UPDATE_INTERRUPT_ENABLE;

	EnablePeriClk(TIM6);
	Basic_Timer_Init(&TIM6_Handle);
	Basic_Timer_PeripheralEnable(&TIM6_Handle);

	while(1);

	return 0;
}

void TIM6_DAC_IRQHandler(void)
{
	Basic_Timer_IRQHandling(&TIM6_Handle);
}

void Basic_Timer_Interrupt_Callback(struct Basic_Timer_Handle_t *pBasic_Timer_Handle)
{
	Basic_Timer_ClearUpdateEventFlag(pBasic_Timer_Handle);

	current_pin = (interrupt_count)%4;
	prev_pin = (interrupt_count-1)%4;

	GPIOWritePin(GPIOA,gpio_pins[prev_pin],GPIO_LOW);
	GPIOWritePin(GPIOA,gpio_pins[current_pin],GPIO_HIGH);

	interrupt_count++;
}
