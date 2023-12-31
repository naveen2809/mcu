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

#define TIM2_CH2_GPIO		1
#define TIM2_CH3_GPIO		2
#define TIM2_CH4_GPIO		3

#define SLEEP_COUNT			500000

#define MCO1_GPIO_PIN					8
#define MCO2_GPIO_PIN					9

#define RCC_CR_REG_ADDR					0x40023800UL
#define RCC_CFGR_REG_ADDR				0x40023808UL
#define RCC_PLLCFGR_REG_ADDR			0x40023804UL
#define	FLASH_ACR_REG_ADDR				0x40023C00UL

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

#define PLL_P_DIV_2						0
#define PLL_P_DIV_4						1
#define PLL_P_DIV_6						2
#define PLL_P_DIV_8						3

#define APB_PRESCALAR_1					0
#define APB_PRESCALAR_2					4
#define APB_PRESCALAR_4					5
#define APB_PRESCALAR_8					6
#define APB_PRESCALAR_16				7

#define AHB_PRESCALAR_1					0
#define AHB_PRESCALAR_2					8
#define AHB_PRESCALAR_4					9
#define AHB_PRESCALAR_8					10
#define AHB_PRESCALAR_16				11
#define AHB_PRESCALAR_64				12
#define AHB_PRESCALAR_128				13
#define AHB_PRESCALAR_256				14
#define AHB_PRESCALAR_512				15

#define HSE_ON_BITPOS					16
#define HSE_CHECK_BITPOS				17
#define PLL_ON_BITPOS					24
#define PLL_CHECK_BITPOS				25

#define RCC_CFGR_HPRE_BITPOS			4
#define RCC_CFGR_PPRE1_BITPOS			10
#define RCC_CFGR_PPRE2_BITPOS			13

#define FLASH_LATENCY_0_WS				0
#define FLASH_LATENCY_1_WS				1
#define FLASH_LATENCY_2_WS				2
#define FLASH_LATENCY_3_WS				3
#define FLASH_LATENCY_4_WS				4
#define FLASH_LATENCY_5_WS				5
#define FLASH_LATENCY_6_WS				6
#define FLASH_LATENCY_7_WS				7

#define FLASH_LATENCY_MASK				0x7

#define TIM2_IRQ_NUM					28

uint32_t capture_value[10];
uint32_t interrupt_count = 0;

struct General_Purpose_Timer_Handle_t TIM2_Handle;

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

void TIM2_IRQHandler(void)
{
	if(interrupt_count < 10)
	{
		capture_value[interrupt_count] = TIM2_Handle.pGeneral_Purpose_Timer->TIMx_CCR2;
		interrupt_count++;
	}
	else
	{
		NVIC_DisableIRQ(TIM2_IRQ_NUM);
	}
}

int main(void)
{
	uint32_t *pRCC_CR = (uint32_t *) RCC_CR_REG_ADDR;
	uint32_t *pRCC_CFGR = (uint32_t *) RCC_CFGR_REG_ADDR;
	uint32_t *pRCC_PLLCFGR = (uint32_t *) RCC_PLLCFGR_REG_ADDR;
	uint32_t *pFLASH_ACR = (uint32_t *) FLASH_ACR_REG_ADDR;

	uint8_t MCO1_SOURCE = MCO1_SOURCE_PLL;
	uint8_t MCO1_PRESCALAR = MCO_PRESCALAR_5;
	uint8_t MCO2_SOURCE = MCO2_SOURCE_HSE;
	uint8_t MCO2_PRESCALAR = MCO_PRESCALAR_4;

	uint8_t PLL_SOURCE = PLL_SRC_HSE;
	uint8_t SYSCLK_SRC = SYSCLK_SW_PLL;

	uint16_t PLL_M_VALUE = 4;
	uint16_t PLL_N_VALUE = 72;
	uint16_t PLL_P_VALUE = PLL_P_DIV_4;
	uint16_t PLL_Q_VALUE = 3;

	uint8_t HPRE_VALUE = AHB_PRESCALAR_1;
	uint8_t PPRE1_VALUE = APB_PRESCALAR_2;
	uint8_t PPRE2_VALUE = APB_PRESCALAR_2;

	uint8_t FLASH_LATENCY_VALUE = FLASH_LATENCY_2_WS;

	TIM2_Handle.pGeneral_Purpose_Timer = (struct General_Purpose_Timer_RegDef_t *) TIM2;

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
	*pFLASH_ACR |= FLASH_LATENCY_VALUE;						//Changing the FLASH Latency
	while((*pFLASH_ACR & FLASH_LATENCY_MASK) != FLASH_LATENCY_VALUE);

	//Switching the System Clock (SYSCLK)
	*pRCC_CFGR &= ~(0x3 << SYSCLK_SW_BITPOS);
	*pRCC_CFGR |= (SYSCLK_SRC << SYSCLK_SW_BITPOS);

	//Changing the AHB, APB1 and APB2 Pre-Scalers
	*pRCC_CFGR |= (HPRE_VALUE << RCC_CFGR_HPRE_BITPOS);
	*pRCC_CFGR |= (PPRE1_VALUE << RCC_CFGR_PPRE1_BITPOS);
	*pRCC_CFGR |= (PPRE2_VALUE << RCC_CFGR_PPRE2_BITPOS);

	//Timer Configuration
	memset(&TIM2_Handle.General_Purpose_Timer_Config,0,sizeof(TIM2_Handle.General_Purpose_Timer_Config));
	TIM2_Handle.General_Purpose_Timer_Config.Timer_PreScalerValue = 0x0;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_AutoReloadValue = 0xFFFFFFFF;

	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_1_Status = TIMER_CHANNEL_DISABLED;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_3_Status = TIMER_CHANNEL_DISABLED;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_4_Status = TIMER_CHANNEL_DISABLED;

	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_2_Status = TIMER_CHANNEL_ENABLED;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_2_Mode = TIMER_CHANNEL_MODE_INPUT_CAPTURE;

	//GPIO Configuration for Timers
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
	NVIC_EnableIRQ(TIM2_IRQ_NUM);
	General_Purpose_Timer_PeripheralEnable(&TIM2_Handle);

	while(1)
	{

	}

	return 0;
}
