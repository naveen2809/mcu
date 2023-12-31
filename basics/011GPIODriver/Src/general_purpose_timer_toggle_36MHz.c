/*
 * timer_input_capture.c
 *
 *  Created on: 07-Dec-2023
 *      Author: naveen
 */
#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "general_purpose_timer.h"
#include "usart_driver.h"

#define TIM2_CH2_GPIO		1
#define TIM2_CH3_GPIO		2
#define TIM2_CH4_GPIO		3

#define SLEEP_COUNT			500000

#define RCC_CR_REG_ADDR					0x40023800UL
#define RCC_CFGR_REG_ADDR				0x40023808UL
#define RCC_PLLCFGR_REG_ADDR			0x40023804UL
#define	FLASH_ACR_REG_ADDR				0x40023C00UL

#define SYSCLK_SW_HSI					0
#define SYSCLK_SW_HSE					1
#define SYSCLK_SW_PLL					2
#define SYSCLK_SW_BITPOS				0

#define PLL_SRC_HSI						0
#define PLL_SRC_HSE						1
#define PLL_M_BITPOS					0
#define PLL_N_BITPOS					6
#define PLL_P_BITPOS					16
#define PLL_Q_BITPOS					24
#define PLL_SRC_BITPOS					22

#define HSE_ON_BITPOS					16
#define HSE_CHECK_BITPOS				17
#define PLL_ON_BITPOS					24
#define PLL_CHECK_BITPOS				25

#define RCC_CFGR_PPRE1_BITPOS			10
#define RCC_CFGR_PPRE2_BITPOS			13

struct General_Purpose_Timer_Handle_t TIM2_Handle;

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{

	uint32_t *pRCC_CR = (uint32_t *) RCC_CR_REG_ADDR;
	uint32_t *pRCC_CFGR = (uint32_t *) RCC_CFGR_REG_ADDR;
	uint32_t *pRCC_PLLCFGR = (uint32_t *) RCC_PLLCFGR_REG_ADDR;
	uint32_t *pFLASH_ACR = (uint32_t *) FLASH_ACR_REG_ADDR;

	uint8_t PLL_SOURCE = PLL_SRC_HSE;
	uint8_t SYSCLK_SRC = SYSCLK_SW_PLL;

	uint16_t PLL_M_VALUE = 8;
	uint16_t PLL_N_VALUE = 144;
	uint16_t PLL_P_VALUE = 2;
	uint16_t PLL_Q_VALUE = 3;

	uint8_t PPRE1_VALUE = 4;
	uint8_t PPRE2_VALUE = 4;

	//Configuring the HSE Oscillator
	*pRCC_CR |= (1 << 16);					//Turning on the HSE Oscillator
	while(!(*pRCC_CR & (1 << 17)));			//Checking whether HSE Oscillator is turned on and is stable

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

	// 1d. Configuring 'Q' Value
	*pRCC_PLLCFGR &= ~(15 << PLL_Q_BITPOS);
	*pRCC_PLLCFGR |= (PLL_Q_VALUE << PLL_Q_BITPOS);

	// 1.e Configuring the PLL Source (HSI or HSE)
	*pRCC_PLLCFGR &= ~(1 << PLL_SRC_BITPOS);
	*pRCC_PLLCFGR |= (PLL_SOURCE << PLL_SRC_BITPOS);

	// 2. Turning on the PLL Oscillator
	*pRCC_CR |= (1 << PLL_ON_BITPOS);					//Turning on the PLL Oscillator
	while(!(*pRCC_CR & (1 << PLL_CHECK_BITPOS)));		//Checking whether PLL Oscillator is turned on and is stable

	//Changing the FLASH Latency
	*pFLASH_ACR |= 0x3;						//Changing the FLASH Latency
	while(!(*pFLASH_ACR & 0x3));

	// Changing the System Clock (SYSCLK)
	//Changing the APB1 and APB2 Pre-Scalers
	*pRCC_CFGR |= (PPRE1_VALUE << RCC_CFGR_PPRE1_BITPOS);
	*pRCC_CFGR |= (PPRE2_VALUE << RCC_CFGR_PPRE2_BITPOS);
	*pRCC_CFGR &= ~(0x3 << SYSCLK_SW_BITPOS);
	*pRCC_CFGR |= (SYSCLK_SRC << SYSCLK_SW_BITPOS);
	while(!(*(pRCC_CFGR) & (0x2 << 2)));

	delay();
	delay();
	delay();
	delay();
	delay();

	TIM2_Handle.pGeneral_Purpose_Timer = (struct General_Purpose_Timer_RegDef_t *) TIM2;
	memset(&TIM2_Handle.General_Purpose_Timer_Config,0,sizeof(TIM2_Handle.General_Purpose_Timer_Config));
	TIM2_Handle.General_Purpose_Timer_Config.Timer_PreScalerValue = 0x1;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_AutoReloadValue = 0x2328;

	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_1_Status = TIMER_CHANNEL_DISABLED;

	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_2_Status = TIMER_CHANNEL_ENABLED;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_2_Mode = TIMER_CHANNEL_MODE_OUTPUT_TOGGLE;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_2_OutputCompareRegisterValue = 0x2328;

	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_3_Status = TIMER_CHANNEL_DISABLED;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_3_Mode = TIMER_CHANNEL_MODE_OUTPUT_TOGGLE;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_3_OutputCompareRegisterValue = 0xFA0;

	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_4_Status = TIMER_CHANNEL_DISABLED;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_4_Mode = TIMER_CHANNEL_MODE_OUTPUT_TOGGLE;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_4_OutputCompareRegisterValue = 0xFA0;

	//GPIO Configuration
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, TIM2_CH2_GPIO, GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,TIM2_CH2_GPIO,GPIO_ALTFN_1);
	GPIOSetMode(GPIOA, TIM2_CH3_GPIO, GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,TIM2_CH3_GPIO,GPIO_ALTFN_1);
	GPIOSetMode(GPIOA, TIM2_CH4_GPIO, GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,TIM2_CH4_GPIO,GPIO_ALTFN_1);

	// Timer 2 Configuration
	EnablePeriClk(TIM2);
	General_Purpose_Timer_Init(&TIM2_Handle);
	General_Purpose_Timer_PeripheralEnable(&TIM2_Handle);

	while(1)
	{

	}

	return 0;
}
