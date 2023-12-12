/*
 * delay_us.c
 * TIM2 is used as the time base generation unit
 * Default configuration is assumed: HSI Clock at 16 MHz and thus TIM2 Clock frequency of 16 MHz
 *  Created on: 12-Dec-2023
 *      Author: naveen
 */
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "general_purpose_timer.h"
#include "common_utilities.h"

#define GPIO_1				GPIO_PIN_0

void test_delay_us(uint32_t delay);

int main(void)
{
	configure_delay_timer();
	test_delay_us(4000);

	while(1);

	return 0;
}

void test_delay_us(uint32_t delay)
{
	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOA;

	//GPIO Configuration for Seven Segment LEDs
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, GPIO_1, GPIO_MODE_OUTPUT);

	//Initialize Seven Segment LED GPIOs to HIGH (Common Anode)
	GPIOWritePin(GPIOA, GPIO_1,GPIO_LOW);

	pGPIO->ODR ^= (1 << GPIO_1);
	delay_us(delay);
	pGPIO->ODR ^= (1 << GPIO_1);
}
