/*
 * mcu_clock_source_out.c
 *
 *  Created on: Dec 3, 2023
 *      Author: naveen
 */

/* LSE Crystal Oscillator (X2) is Available on STM32F446RE Nucleo Board
 * It is not populated on STM32F407 Discovery Board
 */

#include "stm32f4xx.h"
#include "gpio_driver.h"

#define MCO1_GPIO_PIN					8
#define MCO2_GPIO_PIN					9

#define RCC_CFGR_REG_ADDR				0x40023808UL
#define RCC_BDCR_REG_ADDR				0x40023870UL
#define PWR_CR_REG_ADDR					0x40007000UL
#define RCC_APB1ENR_REG_ADDR			0x40023840UL
#define MCO1_PRESCALAR_BITPOS			24
#define MCO1_SOURCE_BITPOS				21

#define MCO1_SOURCE_HSI					0
#define MCO1_SOURCE_LSE					1
#define MCO1_SOURCE_HSE					2
#define MCO1_SOURCE_PLL					3

#define MCO_PRESCALAR_1					0
#define MCO_PRESCALAR_2					4
#define MCO_PRESCALAR_3					5
#define MCO_PRESCALAR_4					6
#define MCO_PRESCALAR_5					7

int main(void)
{

	uint32_t *pRCC_CFGR = (uint32_t *) RCC_CFGR_REG_ADDR;
	uint32_t *pRCC_BDCR = (uint32_t *) RCC_BDCR_REG_ADDR;
	uint32_t *pPWR_CR = (uint32_t *) PWR_CR_REG_ADDR;
	uint32_t *pAPB1ENR = (uint32_t *) RCC_APB1ENR_REG_ADDR;
	uint8_t MCO1_SOURCE = MCO1_SOURCE_LSE;
	uint8_t MCO1_PRESCALAR = MCO_PRESCALAR_5;

	//Configure GPIO Pins for MCO1
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA,MCO1_GPIO_PIN, GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,MCO1_GPIO_PIN,GPIO_ALTFN_0);

	//Configuring RCC_CFGR for MCO1
	*pRCC_CFGR &= ~(0x3 << MCO1_SOURCE_BITPOS);
	*pRCC_CFGR |= (MCO1_SOURCE << MCO1_SOURCE_BITPOS);
	*pRCC_CFGR &= ~(0x7 << MCO1_PRESCALAR_BITPOS);
	*pRCC_CFGR |= (MCO1_PRESCALAR << MCO1_PRESCALAR_BITPOS);

	//Configuring the LSE Oscillator
	//Reference - https://embedkari.com/stm32-clock-configuration/

	*pAPB1ENR |= (1 << 28);             //PWR interface clock enable
	*pPWR_CR |= (1 << 8);				//Enable writing to backup domain register (RCC_BDCR) with DBP Enable
	*pRCC_BDCR |= 0x1;					//Turning on the LSE Oscillator
	while(!(*pRCC_BDCR & 0x2));			//Checking whether LSE Oscillator is turned on and is stable

	while(1)
	{

	}

	return 0;
}
