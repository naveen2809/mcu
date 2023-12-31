/*
 * mcu_clock_source_out_pll.c
 *
 *  Created on: Dec 6, 2023
 *      Author: naveen
 */

#include "stm32f4xx.h"
#include "gpio_driver.h"

#define MCO1_GPIO_PIN					8
#define MCO2_GPIO_PIN					9

#define RCC_CR_REG_ADDR					0x40023800UL
#define RCC_CFGR_REG_ADDR				0x40023808UL
#define RCC_PLLCFGR_REG_ADDR			0x40023804UL

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

#define PLL_SRC_HSI						0
#define PLL_SRC_HSE						1
#define PLL_M_BITPOS					0
#define PLL_N_BITPOS					6
#define PLL_P_BITPOS					16
#define PLL_SRC_BITPOS					22

#define HSE_ON_BITPOS					16
#define HSE_CHECK_BITPOS				17
#define PLL_ON_BITPOS					24
#define PLL_CHECK_BITPOS				25

int main(void)
{

	uint32_t *pRCC_CR = (uint32_t *) RCC_CR_REG_ADDR;
	uint32_t *pRCC_CFGR = (uint32_t *) RCC_CFGR_REG_ADDR;
	uint32_t *pRCC_PLLCFGR = (uint32_t *) RCC_PLLCFGR_REG_ADDR;
	uint8_t MCO1_SOURCE = MCO1_SOURCE_PLL;
	uint8_t MCO1_PRESCALAR = MCO_PRESCALAR_5;
	uint8_t MCO2_SOURCE = MCO1_SOURCE_PLL;
	uint8_t MCO2_PRESCALAR = MCO_PRESCALAR_5;
	uint8_t PLL_SOURCE = PLL_SRC_HSE;
	uint16_t PLL_M_VALUE = 4;
	uint16_t PLL_N_VALUE = 50;
	uint16_t PLL_P_VALUE = 1;

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

	//Turning on the HSE Oscillator as the PLL Source
	*pRCC_CR |= (1 << HSE_ON_BITPOS);					//Turning on the HSE Oscillator
	while(!(*pRCC_CR & (1 << HSE_CHECK_BITPOS)));		//Checking whether HSE Oscillator is turned on and is stable

	//Configuring the PLL Oscillator
	// 1. Configuring the PLL Configuration Register (RCC_PLLCFGR)
	// 1a. Configuring 'M' Value
	*pRCC_PLLCFGR &= ~(63 << PLL_M_BITPOS);
	*pRCC_PLLCFGR |= (PLL_M_VALUE << PLL_M_BITPOS);

	// 1b. Configuring 'N' Value
	*pRCC_PLLCFGR &= ~(511 << PLL_N_BITPOS);
	*pRCC_PLLCFGR |= (PLL_N_VALUE << PLL_N_BITPOS);

	// 1c. Configuring 'P' Value
	*pRCC_PLLCFGR &= ~(3 << PLL_P_BITPOS);
	*pRCC_PLLCFGR |= (PLL_P_VALUE << PLL_P_BITPOS);

	// 1.d Configuring the PLL Source (HSI or HSE)
	*pRCC_PLLCFGR &= ~(1 << PLL_SRC_BITPOS);
	*pRCC_PLLCFGR |= (PLL_SOURCE << PLL_SRC_BITPOS);

	// 2. Turning on the PLL Oscillator
	*pRCC_CR |= (1 << PLL_ON_BITPOS);					//Turning on the PLL Oscillator
	while(!(*pRCC_CR & (1 << PLL_CHECK_BITPOS)));		//Checking whether PLL Oscillator is turned on and is stable

	while(1)
	{

	}

	return 0;
}
