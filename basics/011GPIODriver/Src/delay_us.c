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
#include "common_utils.h"

#define LED_GPIO_PORT	GPIOD
#define LED_GPIO_PIN	GPIO_PIN_0

void test_delay_us(uint32_t delay);

int main(void)
{
	configure_delay_timer();
	test_delay_us(15000000);

	while(1);

	return 0;
}

void test_delay_us(uint32_t delay)
{
	//GPIO Configuration for Seven Segment LEDs
	EnablePeriClk(LED_GPIO_PORT);
	GPIOSetMode(LED_GPIO_PORT,LED_GPIO_PIN, GPIO_MODE_OUTPUT);

	GPIOWritePin(LED_GPIO_PORT,LED_GPIO_PIN, GPIO_HIGH);
	delay_us(delay);
	GPIOWritePin(LED_GPIO_PORT,LED_GPIO_PIN, GPIO_LOW);

	return;
}
