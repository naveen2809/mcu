/*
 * stm32f4xx.c
 *
 *  Created on: Jun 26, 2023
 *      Author: naveen
 */

#include <stdint.h>
#include "stm32f4xx.h"

/* Peripheral Clock Enable and Disable Functions */



void EnablePeriClk(uint32_t *PeripheralAddress)
{
	uint32_t *pRCCPeriClkReg;

	if(PeripheralAddress == GPIOA) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << GPIOA_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOB) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << GPIOB_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOC) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << GPIOC_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOD) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << GPIOD_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOE) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << GPIOE_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOF) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << GPIOF_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOG) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << GPIOG_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOH) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << GPIOH_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOI) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << GPIOI_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == SPI1) {
		pRCCPeriClkReg = (uint32_t *) APB2_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << SPI1_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == SPI2) {
		pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << SPI2_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == SPI3) {
		pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << SPI3_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == I2C1) {
		pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << I2C1_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == I2C2) {
		pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << I2C2_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == I2C3) {
		pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << I2C3_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == USART1) {
		pRCCPeriClkReg = (uint32_t *) APB2_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << USART1_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == USART2) {
		pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << USART2_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == USART3) {
		pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << USART3_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == UART4) {
		pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << UART4_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == UART5) {
		pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << UART5_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == USART6) {
		pRCCPeriClkReg = (uint32_t *) APB2_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << USART6_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM6) {
		pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << TIM6_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM7) {
		pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << TIM7_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == DMA1) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << DMA1_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == DMA2) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg |= (1 << DMA2_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM2) {
			pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
			*pRCCPeriClkReg |= (1 << TIM2_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM3) {
			pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
			*pRCCPeriClkReg |= (1 << TIM3_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM4) {
			pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
			*pRCCPeriClkReg |= (1 << TIM4_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM5) {
			pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
			*pRCCPeriClkReg |= (1 << TIM5_RCC_PERI_CLK_BIT_POS);
	}
}

void DisablePeriClk(uint32_t *PeripheralAddress)
{
	uint32_t *pRCCPeriClkReg;

	if(PeripheralAddress == GPIOA) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg &= ~(1 << GPIOA_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOB) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg &= ~(1 << GPIOB_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOC) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg &= ~(1 << GPIOC_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOD) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg &= ~(1 << GPIOD_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOE) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg &= ~(1 << GPIOE_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOF) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg &= ~(1 << GPIOF_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOG) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg &= ~(1 << GPIOG_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOH) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg &= ~(1 << GPIOH_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == GPIOI) {
		pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
		*pRCCPeriClkReg &= ~(1 << GPIOI_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == SPI1) {
				pRCCPeriClkReg = (uint32_t *) APB2_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << SPI1_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == SPI2) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << SPI2_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == SPI3) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << SPI3_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == I2C1) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << I2C1_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == I2C2) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << I2C2_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == I2C3) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << I2C3_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == USART1) {
				pRCCPeriClkReg = (uint32_t *) APB2_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << USART1_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == USART2) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << USART2_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == USART3) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << USART3_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == UART4) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << UART4_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == UART5) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << UART5_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == USART6) {
				pRCCPeriClkReg = (uint32_t *) APB2_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << USART6_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM6) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << TIM6_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM7) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << TIM7_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == DMA1) {
			pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
			*pRCCPeriClkReg &= ~(1 << DMA1_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == DMA2) {
			pRCCPeriClkReg = (uint32_t *) AHB1_ENR_ADDR;
			*pRCCPeriClkReg &= ~(1 << DMA2_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM2) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << TIM2_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM3) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << TIM3_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM4) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << TIM4_RCC_PERI_CLK_BIT_POS);
	}
	else if (PeripheralAddress == TIM5) {
				pRCCPeriClkReg = (uint32_t *) APB1_ENR_ADDR;
				*pRCCPeriClkReg &= ~(1 << TIM5_RCC_PERI_CLK_BIT_POS);
	}
}

void NVIC_EnableIRQ(uint8_t IRQNum)
{
	uint32_t *pNVIC_ISER;
	uint32_t NVICRegisterNum = IRQNum/32;
	uint32_t NVICBitPos = IRQNum%32;

	pNVIC_ISER = (uint32_t*) NVIC_ISER_BASE_ADDR + NVICRegisterNum;

	*pNVIC_ISER |= (1 << NVICBitPos);
}

void NVIC_DisableIRQ(uint8_t IRQNum)
{
		uint32_t *pNVIC_ICER;
		uint32_t NVICRegisterNum = IRQNum/32;
		uint32_t NVICBitPos = IRQNum%32;

		pNVIC_ICER = (uint32_t*) NVIC_ICER_BASE_ADDR + NVICRegisterNum;

		*pNVIC_ICER |= (1 << NVICBitPos);
}

void NVIC_IRQSetPending(uint8_t IRQNum)
{
	uint32_t *pNVIC_ISPR;
	uint32_t NVICRegisterNum = IRQNum/32;
	uint32_t NVICBitPos = IRQNum%32;

	pNVIC_ISPR = (uint32_t*) NVIC_ISPR_BASE_ADDR + NVICRegisterNum;

	*pNVIC_ISPR |=  (1 << NVICBitPos);

	return;
}

void NVIC_IRQClearPending(uint8_t IRQNum)
{
	uint32_t *pNVIC_ICPR;
	uint32_t NVICRegisterNum = IRQNum/32;
	uint32_t NVICBitPos = IRQNum%32;

	pNVIC_ICPR = (uint32_t*) NVIC_ICPR_BASE_ADDR + NVICRegisterNum;

	*pNVIC_ICPR |=  (1 << NVICBitPos);

	return;
}
