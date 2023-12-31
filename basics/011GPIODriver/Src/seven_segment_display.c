/*
 * seven_segment_display.c
 * Common Anode Seven Segment Display Driver
 * Reference: https://www.electronicsforu.com/resources/7-segment-display-pinout-understanding
 *  Created on: 12-Dec-2023
 *      Author: naveen
 */

#include "timer_driver.h"
#include "gpio_driver.h"

#define GPIO_1				GPIO_PIN_0
#define GPIO_2				GPIO_PIN_1
#define GPIO_3				GPIO_PIN_2
#define GPIO_4				GPIO_PIN_3
#define GPIO_5				GPIO_PIN_4
#define GPIO_6				GPIO_PIN_5
#define GPIO_7				GPIO_PIN_6
#define GPIO_DECIMAL		GPIO_PIN_10
#define TIM6_IRQ_NUM		54
#define TIM7_IRQ_NUM		55

struct Basic_Timer_Handle_t TIM6_Handle;

uint32_t decimal_digits[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
uint32_t default_digit = 0x7F;
uint32_t interrupt_count = 0;

void TIM6_DAC_IRQHandler(void);
void Basic_Timer_Interrupt_Callback(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);

int main(void)
{

	//GPIO Configuration for Seven Segment LEDs
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, GPIO_1, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_2, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_3, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_4, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_5, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_6, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_7, GPIO_MODE_OUTPUT);

	//Initialize Seven Segment LED GPIOs to HIGH (Common Anode)
	GPIOWritePort(GPIOA, default_digit);

	//GPIO Configuration for Decimal Point LED
	EnablePeriClk(GPIOC);
	GPIOSetMode(GPIOC, GPIO_DECIMAL, GPIO_MODE_OUTPUT);
	GPIOWritePin(GPIOC, GPIO_DECIMAL,GPIO_HIGH);

	//Enabling the Interrupt for TIM6 Peripheral in NVIC
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

	//Clearing the previous digit
	GPIOWritePort(GPIOA, default_digit);

	//Writing the next digit to the Seven Segment Display
	GPIOWritePort(GPIOA, decimal_digits[interrupt_count%10]);

	//Incrementing the counter
	interrupt_count++;
}
