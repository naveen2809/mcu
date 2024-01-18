/*
 * rtc_stm32_test.c
 *
 *  Created on: 06-Jan-2024
 *      Author: naveen
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "common_utils.h"
#include "external_interrupt_config.h"

#define BTN_INT_GPIO_PORT		GPIOD
#define BTN_INT_GPIO_PIN		GPIO_PIN_0
#define BTN_INT_IRQ_NUM			6

#define OUTPUT_LED_GPIO_PORT	GPIOD
#define OUTPUT_LED_GPIO_PIN		GPIO_PIN_15

#define SLEEP_COUNT				50000000

static void delay(void);
static void disable_interrupts(void);
static void enable_interrupts(void);

int main(void)
{

	//Configure the Button Interrupt
	configure_external_gpio_interrupt(BTN_INT_GPIO_PORT,BTN_INT_GPIO_PIN,EXTI_FALLING_TRIGGER,BTN_INT_IRQ_NUM);

	//Configure the Output LED GPIO Pin
	EnablePeriClk(OUTPUT_LED_GPIO_PORT);
	GPIOSetMode(OUTPUT_LED_GPIO_PORT,OUTPUT_LED_GPIO_PIN,GPIO_MODE_OUTPUT);
	GPIOWritePin(OUTPUT_LED_GPIO_PORT,OUTPUT_LED_GPIO_PIN,GPIO_LOW);

	disable_interrupts();
	delay();
	enable_interrupts();

	while(1)
	{

	}

	return 0;
}

void EXTI0_IRQHandler(void)
{
	uint32_t *pEXTI_PR = (uint32_t *) EXTI_PR_ADDR;

	//Handling the Interrupt
	GPIOWritePin(OUTPUT_LED_GPIO_PORT,OUTPUT_LED_GPIO_PIN,GPIO_HIGH);

	//Clearing the Button Interrupt
	*pEXTI_PR |= (1 << BTN_INT_GPIO_PIN);		// Clearing the EXTI_PR Register

	return;
}

static void disable_interrupts(void)
{
	__asm volatile ("CPSID I");

	return;
}

static void enable_interrupts(void)
{
	__asm volatile ("CPSIE I");

	return;
}

static void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}