/*
 * external_interrupt_config.c
 *
 *  Created on: 19-Dec-2023
 *      Author: naveen
 */

#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "external_interrupt_config.h"

static uint8_t gpio_port_name_to_num(uint32_t *gpio_name);

void configure_external_interrupt(void)
{

	uint32_t *pAPB2ENR = (uint32_t *) APB2_ENR_ADDR;
	uint32_t *pEXTI_IMR	= (uint32_t *) EXTI_IMR_ADDR;
	uint32_t *pEXTI_RTSR = (uint32_t *) EXTI_RTSR_ADDR;
	uint32_t *pEXTI_FTSR = (uint32_t *) EXTI_FTSR_ADDR;
	uint32_t *pSYSCFG_EXTI_CR_ADDR = (uint32_t *) SYSCFG_EXTI_CR_BASE_ADDR;
	uint32_t OFFSET;

	//GPIO Pin Configuration
	EnablePeriClk(INTERRUPT_PORT);
	GPIOSetMode(INTERRUPT_PORT,INTERRUPT_PIN,GPIO_MODE_INPUT);
	GPIOSetOutputType(INTERRUPT_PORT,INTERRUPT_PIN,GPIO_OPTYPE_OD);
	GPIOSetOutputSpeed(INTERRUPT_PORT,INTERRUPT_PIN,GPIO_OPSPEED_HIGH);
	GPIOSetPullUpDownConfig(INTERRUPT_PORT,INTERRUPT_PIN,GPIO_PULL_UP);

	//Button Interrupt Configuration

	// 1. Configuring the EXTI Controller (External Interrupt Controller)

	*pEXTI_IMR |= (1 << INTERRUPT_PIN);  	// Setting the Interrupt Mask Register

	if(EXTI_RISING_TRIGGER_ENABLE == TRUE)
	{
		*pEXTI_RTSR |= (1 << INTERRUPT_PIN); 	// Setting the Rising Trigger Set Register
	}

	if(EXTI_FALLING_TRIGGER_ENABLE == TRUE)
	{
		*pEXTI_FTSR |= (1 << INTERRUPT_PIN); 	// Setting the Falling Trigger Set Register
	}

	*pAPB2ENR |= (1 << 14);							// Enabling the clock for the System Configuration Block

	OFFSET = INTERRUPT_PIN%4;
	pSYSCFG_EXTI_CR_ADDR += OFFSET;

	// Enabling GPIO Port x  input on EXTIx line
	*pSYSCFG_EXTI_CR_ADDR &= ~(0xF << OFFSET*4);
	*pSYSCFG_EXTI_CR_ADDR |= (gpio_port_name_to_num(INTERRUPT_PORT) << OFFSET*4);

	// 2. // Enabling the interrupt for the button
	NVIC_EnableIRQ(INTERRUPT_IRQ_NO);					// Enabling the interrupt
}

static uint8_t gpio_port_name_to_num(uint32_t *gpio_name)
{
	uint8_t gpio_num;

	if(gpio_name == GPIOA)
		gpio_num = 0;
	else if(gpio_name == GPIOB)
		gpio_num = 1;
	else if(gpio_name == GPIOC)
		gpio_num = 2;
	else if(gpio_name == GPIOD)
		gpio_num = 3;
	else if(gpio_name == GPIOE)
		gpio_num = 4;
	else if(gpio_name == GPIOF)
		gpio_num = 5;
	else if(gpio_name == GPIOG)
		gpio_num = 6;
	else if(gpio_name == GPIOH)
		gpio_num = 7;
	else if(gpio_name == GPIOI)
		gpio_num = 8;

	return gpio_num;
}
