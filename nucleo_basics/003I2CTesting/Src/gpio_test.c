/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define SLEEP_COUNT						50000

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{

	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT);

	printf("Hello World\r\n");

	//EnablePeriClk(GPIOC);
	//GPIOSetMode(GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT);

	//GPIOWritePort(GPIOD, GPIOPortValue);

	//GPIOIDRValue = GPIOReadIDR(GPIOD);

	/* Loop forever */
	for(;;)
	{

		GPIOTogglePin(GPIOA, GPIO_PIN_5);
		delay();

		/*
		if(GPIOReadPin(GPIOC, GPIO_PIN_13) == GPIO_LOW)
		{
			GPIOTogglePin(GPIOA, GPIO_PIN_5);
			delay();
		}
		*/

		/*
		if(GPIOReadPin(GPIOD, GPIO_PIN_11) == GPIO_HIGH)
		{
			GPIOTogglePin(GPIOD, GPIO_PIN_9);
			delay();
		}

		if(GPIOReadPin(GPIOA, GPIO_PIN_0) == GPIO_HIGH)
		{
					GPIOTogglePin(GPIOD, GPIO_PIN_12);
					GPIOTogglePin(GPIOD, GPIO_PIN_13);
					GPIOTogglePin(GPIOD, GPIO_PIN_14);
					GPIOTogglePin(GPIOD, GPIO_PIN_15);
					delay();
		}
*/

/*
		GPIOWritePort(GPIOD, GPIOPortValueOn);
		delay();
		GPIOPortValueOff = GPIOPortValueOn ^ GPIOReadODR(GPIOD);
		GPIOWritePort(GPIOD, GPIOPortValueOff);
		delay();
*/


	}

}