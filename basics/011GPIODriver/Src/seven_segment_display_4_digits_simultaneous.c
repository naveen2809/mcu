/*
 * seven_segment_display_4_digits.c
 * Common Cathode 4-Digit Seven Segment Display
 * Reference: https://softwareparticles.com/learn-how-a-4-digit-7-segment-led-display-works-and-how-to-control-it-using-an-arduino/
 * Refreshing the digits at a fast rate in order to provide the illusion of simultaneous display
 *  Created on: 12-Dec-2023
 *      Author: naveen
 */

#include "timer_driver.h"
#include "gpio_driver.h"

#define GPIO_1					GPIO_PIN_0
#define GPIO_2					GPIO_PIN_1
#define GPIO_3					GPIO_PIN_2
#define GPIO_4					GPIO_PIN_3
#define GPIO_5					GPIO_PIN_4
#define GPIO_6					GPIO_PIN_5
#define GPIO_7					GPIO_PIN_6
#define GPIO_DECIMAL			GPIO_PIN_2
#define GPIO_DIGIT_UNITS		GPIO_PIN_9
#define GPIO_DIGIT_TENS			GPIO_PIN_8
#define GPIO_DIGIT_HUNDREDS		GPIO_PIN_7
#define GPIO_DIGIT_THOUSANDS	GPIO_PIN_6

#define DIGIT_UNITS_PLACE 		5
#define DIGIT_TENS_PLACE 		1
#define DIGIT_HUNDREDS_PLACE 	3
#define DIGIT_THOUSANDS_PLACE 	0

#define TIM6_IRQ_NUM		54

struct Basic_Timer_Handle_t TIM6_Handle;

uint32_t decimal_digits[10] = {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
uint32_t default_digit = 0x7F;
uint32_t interrupt_count = 0;

uint8_t display_digits[4] = {DIGIT_UNITS_PLACE,DIGIT_TENS_PLACE,DIGIT_HUNDREDS_PLACE,DIGIT_THOUSANDS_PLACE};
uint8_t num_digits = 3;
uint8_t gpio_common[4] = {GPIO_DIGIT_UNITS, GPIO_DIGIT_TENS, GPIO_DIGIT_HUNDREDS, GPIO_DIGIT_THOUSANDS};
uint8_t prev_digit;
uint8_t current_digit;

void TIM6_DAC_IRQHandler(void);
void Basic_Timer_Interrupt_Callback(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);

int main(void)
{

	//GPIO Configuration for Seven Segment LEDs (Cathodes)
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, GPIO_1, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_2, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_3, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_4, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_5, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_6, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, GPIO_7, GPIO_MODE_OUTPUT);

	//Initialize Seven Segment LED GPIOs to HIGH (Cathodes)
	GPIOWritePort(GPIOA, default_digit);

	//GPIO Configuration for Decimal Point LED (Cathode)
	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOD, GPIO_DECIMAL, GPIO_MODE_OUTPUT);
	GPIOWritePin(GPIOD, GPIO_DECIMAL,GPIO_HIGH);

	//GPIO Configuration for the Digit Enable Pins (Anodes)
	EnablePeriClk(GPIOC);
	GPIOSetMode(GPIOC, GPIO_DIGIT_UNITS, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOC, GPIO_DIGIT_TENS, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOC, GPIO_DIGIT_HUNDREDS, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOC, GPIO_DIGIT_THOUSANDS, GPIO_MODE_OUTPUT);

	//Enabling the Interrupt for TIM6 Peripheral in NVIC
	NVIC_EnableIRQ(TIM6_IRQ_NUM);

	//TIM6 Configuration
	TIM6_Handle.pBasic_Timer = (struct Basic_Timer_RegDef_t *)TIM6;
	TIM6_Handle.Basic_Timer_Config.Timer_PreScalerValue = 0xff;            //Refresh rate configuration
	TIM6_Handle.Basic_Timer_Config.Timer_AutoReloadValue = 0xf;			   //Refresh rate configuration
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

	current_digit = (interrupt_count)%(num_digits);
	prev_digit = (interrupt_count-1)%(num_digits);

	//Disabling the previous digit
	GPIOWritePin(GPIOC, gpio_common[prev_digit],GPIO_LOW);

	//Enabling the current digit
	GPIOWritePin(GPIOC, gpio_common[current_digit],GPIO_HIGH);

	//Writing the current digit to the Seven Segment Display
	GPIOWritePort(GPIOA, decimal_digits[display_digits[current_digit]]);

	//Incrementing the counter
	interrupt_count++;
}
