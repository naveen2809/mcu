/*
 * mcu_clock_source_out_hse.c
 *
 *  Created on: Dec 3, 2023
 *      Author: naveen
 */

/* 	HSE Crystal Oscillator (X1) is available on STM32F407 Discovery Board
 * 	It is not populated on STM32F446RE Nucleo Board
*/

#include "stm32f4xx.h"
#include "gpio_driver.h"

#define MCO1_GPIO_PIN					8
#define MCO2_GPIO_PIN					9

#define RCC_CR_REG_ADDR					0x40023800UL
#define RCC_CFGR_REG_ADDR				0x40023808UL

#define MCO1_PRESCALAR_BITPOS			24
#define MCO1_SOURCE_BITPOS				21
#define MCO2_PRESCALAR_BITPOS			27
#define MCO2_SOURCE_BITPOS				30

#define MCO1_SOURCE_HSI					0
#define MCO1_SOURCE_LSE					1
#define MCO1_SOURCE_HSE					2
#define MCO1_SOURCE_PLL					3

#define MCO2_SOURCE_SYSCLK				0
#define MCO2_SOURCE_PLLI2S				1
#define MCO2_SOURCE_HSE					2
#define MCO2_SOURCE_PLL					3

#define MCO_PRESCALAR_1					0
#define MCO_PRESCALAR_2					4
#define MCO_PRESCALAR_3					5
#define MCO_PRESCALAR_4					6
#define MCO_PRESCALAR_5					7

int main(void)
{

	uint32_t *pRCC_CR = (uint32_t *) RCC_CR_REG_ADDR;
	uint32_t *pRCC_CFGR = (uint32_t *) RCC_CFGR_REG_ADDR;
	uint8_t MCO1_SOURCE = MCO1_SOURCE_HSE;
	uint8_t MCO1_PRESCALAR = MCO_PRESCALAR_4;
	uint8_t MCO2_SOURCE = MCO2_SOURCE_HSE;
	uint8_t MCO2_PRESCALAR = MCO_PRESCALAR_4;

	//Configure GPIO Pins for MCO1
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA,MCO1_GPIO_PIN, GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,MCO1_GPIO_PIN,GPIO_ALTFN_0);

	//Configure GPIO Pins for MCO2
	EnablePeriClk(GPIOC);
	GPIOSetMode(GPIOC,MCO2_GPIO_PIN, GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOC,MCO2_GPIO_PIN,GPIO_ALTFN_0);

	//Configuring RCC_CFGR for MCO1
	*pRCC_CFGR &= ~(0x3 << MCO1_SOURCE_BITPOS);
	*pRCC_CFGR |= (MCO1_SOURCE << MCO1_SOURCE_BITPOS);
	*pRCC_CFGR &= ~(0x7 << MCO1_PRESCALAR_BITPOS);
	*pRCC_CFGR |= (MCO1_PRESCALAR << MCO1_PRESCALAR_BITPOS);

	//Configuring RCC_CFGR for MCO2
	*pRCC_CFGR &= ~(0x3 << MCO2_SOURCE_BITPOS);
	*pRCC_CFGR |= (MCO2_SOURCE << MCO2_SOURCE_BITPOS);
	*pRCC_CFGR &= ~(0x7 << MCO2_PRESCALAR_BITPOS);
	*pRCC_CFGR |= (MCO2_PRESCALAR << MCO2_PRESCALAR_BITPOS);

	//Configuring the HSE Oscillator
	*pRCC_CR |= (1 << 16);					//Turning on the HSE Oscillator
	while(!(*pRCC_CR & (1 << 17)));			//Checking whether HSE Oscillator is turned on and is stable

	while(1)
	{

	}

	return 0;
}
