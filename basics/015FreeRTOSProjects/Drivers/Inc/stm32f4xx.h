/*
 * stm32f4xx.h
 *
 *  Created on: Jun 26, 2023
 *      Author: naveen
 */

#ifndef INC_STM32F4XX_H_
#define INC_STM32F4XX_H_

#include <stdint.h>

/* Base Addresses */
#define AHB1_BASE_ADDR					0x40020000UL
#define AHB2_BASE_ADDR					0x50000000UL
#define AHB3_BASE_ADDR					0xA0000000UL
#define APB1_BASE_ADDR					0x40000000UL
#define APB2_BASE_ADDR					0x40010000UL

/* RCC Base Address */
#define RCC_BASE_ADDR					0x40023800UL

/* GPIO Port Base Addresses */
#define GPIOA_BASE_ADDR					0x40020000UL
#define GPIOB_BASE_ADDR					0x40020400UL
#define GPIOC_BASE_ADDR					0x40020800UL
#define GPIOD_BASE_ADDR					0x40020C00UL
#define GPIOE_BASE_ADDR					0x40021000UL
#define GPIOF_BASE_ADDR					0x40021400UL
#define GPIOG_BASE_ADDR					0x40021800UL
#define GPIOH_BASE_ADDR					0x40021C00UL
#define GPIOI_BASE_ADDR					0x40022000UL

#define GPIOA							(uint32_t*)GPIOA_BASE_ADDR
#define GPIOB							(uint32_t*)GPIOB_BASE_ADDR
#define GPIOC							(uint32_t*)GPIOC_BASE_ADDR
#define GPIOD							(uint32_t*)GPIOD_BASE_ADDR
#define GPIOE							(uint32_t*)GPIOE_BASE_ADDR
#define GPIOF							(uint32_t*)GPIOF_BASE_ADDR
#define GPIOG							(uint32_t*)GPIOG_BASE_ADDR
#define GPIOH							(uint32_t*)GPIOH_BASE_ADDR
#define GPIOI							(uint32_t*)GPIOI_BASE_ADDR

#define GPIOA_RCC_PERI_CLK_BIT_POS		0
#define GPIOB_RCC_PERI_CLK_BIT_POS		1
#define GPIOC_RCC_PERI_CLK_BIT_POS		2
#define GPIOD_RCC_PERI_CLK_BIT_POS		3
#define GPIOE_RCC_PERI_CLK_BIT_POS		4
#define GPIOF_RCC_PERI_CLK_BIT_POS		5
#define GPIOG_RCC_PERI_CLK_BIT_POS		6
#define GPIOH_RCC_PERI_CLK_BIT_POS		7
#define GPIOI_RCC_PERI_CLK_BIT_POS		8

#define SPI1_RCC_PERI_CLK_BIT_POS		12
#define SPI2_RCC_PERI_CLK_BIT_POS		14
#define SPI3_RCC_PERI_CLK_BIT_POS		15

#define I2C1_RCC_PERI_CLK_BIT_POS		21
#define I2C2_RCC_PERI_CLK_BIT_POS		22
#define I2C3_RCC_PERI_CLK_BIT_POS		23

#define USART1_RCC_PERI_CLK_BIT_POS		4
#define USART2_RCC_PERI_CLK_BIT_POS		17
#define USART3_RCC_PERI_CLK_BIT_POS		18
#define UART4_RCC_PERI_CLK_BIT_POS		19
#define UART5_RCC_PERI_CLK_BIT_POS		20
#define USART6_RCC_PERI_CLK_BIT_POS		5

#define TIM6_RCC_PERI_CLK_BIT_POS		4
#define TIM7_RCC_PERI_CLK_BIT_POS		5
#define TIM2_RCC_PERI_CLK_BIT_POS		0
#define TIM3_RCC_PERI_CLK_BIT_POS		1
#define TIM4_RCC_PERI_CLK_BIT_POS		2
#define TIM5_RCC_PERI_CLK_BIT_POS		3

#define DMA1_RCC_PERI_CLK_BIT_POS		21
#define DMA2_RCC_PERI_CLK_BIT_POS		22

#define AHB1_ENR_ADDR					(RCC_BASE_ADDR + 0x30UL)
#define AHB2_ENR_ADDR					(RCC_BASE_ADDR + 0x34UL)
#define AHB3_ENR_ADDR					(RCC_BASE_ADDR + 0x38UL)
#define APB1_ENR_ADDR					(RCC_BASE_ADDR + 0x40UL)
#define APB2_ENR_ADDR					(RCC_BASE_ADDR + 0x44UL)

#define SPI1_BASE_ADDR					0x40013000UL
#define SPI2_BASE_ADDR					0x40003800UL
#define SPI3_BASE_ADDR					0x40003C00UL

#define SPI1							(uint32_t*)SPI1_BASE_ADDR
#define SPI2							(uint32_t*)SPI2_BASE_ADDR
#define SPI3							(uint32_t*)SPI3_BASE_ADDR

#define I2C1_BASE_ADDR					0x40005400UL
#define I2C2_BASE_ADDR					0x40005800UL
#define I2C3_BASE_ADDR					0x40005C00UL

#define I2C1							(uint32_t*)I2C1_BASE_ADDR
#define I2C2							(uint32_t*)I2C2_BASE_ADDR
#define I2C3							(uint32_t*)I2C3_BASE_ADDR

#define USART1_BASE_ADDR				0x40011000UL
#define USART2_BASE_ADDR				0x40004400UL
#define USART3_BASE_ADDR				0x40004800UL
#define UART4_BASE_ADDR					0x40004C00UL
#define UART5_BASE_ADDR					0x40005000UL
#define USART6_BASE_ADDR				0x40011400UL

#define USART1							(uint32_t*)USART1_BASE_ADDR
#define USART2							(uint32_t*)USART2_BASE_ADDR
#define USART3							(uint32_t*)USART3_BASE_ADDR
#define UART4							(uint32_t*)UART4_BASE_ADDR
#define UART5							(uint32_t*)UART5_BASE_ADDR
#define USART6							(uint32_t*)USART6_BASE_ADDR

/* Basic Timers */
#define TIM6_BASE_ADDR					0x40001000UL
#define TIM7_BASE_ADDR					0x40001400UL

#define TIM6							(uint32_t*)TIM6_BASE_ADDR
#define TIM7							(uint32_t*)TIM7_BASE_ADDR

/* General Purpose Timers - TIM2 to TIM5 */
#define TIM2_BASE_ADDR					0x40000000UL
#define TIM3_BASE_ADDR					0x40000400UL
#define TIM4_BASE_ADDR					0x40000800UL
#define TIM5_BASE_ADDR					0x40000C00UL

#define TIM2							(uint32_t*)TIM2_BASE_ADDR
#define TIM3							(uint32_t*)TIM3_BASE_ADDR
#define TIM4							(uint32_t*)TIM4_BASE_ADDR
#define TIM5							(uint32_t*)TIM5_BASE_ADDR

#define DMA1_BASE_ADDR					0x40026000UL
#define DMA2_BASE_ADDR					0x40026400UL

#define DMA1							(uint32_t*)DMA1_BASE_ADDR
#define DMA2							(uint32_t*)DMA2_BASE_ADDR

#define DMA1_STREAM_0					(uint32_t*) (DMA1_BASE_ADDR + 0x0010)
#define DMA1_STREAM_1					(uint32_t*) (DMA1_BASE_ADDR + 0x0028)
#define DMA1_STREAM_2					(uint32_t*) (DMA1_BASE_ADDR + 0x0040)
#define DMA1_STREAM_3					(uint32_t*) (DMA1_BASE_ADDR + 0x0058)
#define DMA1_STREAM_4					(uint32_t*) (DMA1_BASE_ADDR + 0x0070)
#define DMA1_STREAM_5					(uint32_t*) (DMA1_BASE_ADDR + 0x0088)
#define DMA1_STREAM_6					(uint32_t*) (DMA1_BASE_ADDR + 0x00A0)
#define DMA1_STREAM_7					(uint32_t*) (DMA1_BASE_ADDR + 0x00B8)

#define DMA2_STREAM_0					(uint32_t*) (DMA2_BASE_ADDR + 0x0010)
#define DMA2_STREAM_1					(uint32_t*) (DMA2_BASE_ADDR + 0x0028)
#define DMA2_STREAM_2					(uint32_t*) (DMA2_BASE_ADDR + 0x0040)
#define DMA2_STREAM_3					(uint32_t*) (DMA2_BASE_ADDR + 0x0058)
#define DMA2_STREAM_4					(uint32_t*) (DMA2_BASE_ADDR + 0x0070)
#define DMA2_STREAM_5					(uint32_t*) (DMA2_BASE_ADDR + 0x0088)
#define DMA2_STREAM_6					(uint32_t*) (DMA2_BASE_ADDR + 0x00A0)
#define DMA2_STREAM_7					(uint32_t*) (DMA2_BASE_ADDR + 0x00B8)

#define NVIC_BASE_ADDR					0xE000E100UL
#define NVIC_ISER_BASE_ADDR				0xE000E100UL
#define NVIC_ICER_BASE_ADDR				0XE000E180UL
#define NVIC_ISPR_BASE_ADDR				0xE000E200UL
#define NVIC_ICPR_BASE_ADDR				0XE000E280UL
#define NVIC_IPR_BASE_ADDR				0XE000E400UL

#define NVIC_PRIO_REG_BASE				(uint32_t*)NVIC_IPR_BASE_ADDR

#define RTC_BASE_ADDR					0x40002800UL

#define FLASH_BASE_ADDR					0x40023C00UL


#define FLAG_SET 			1
#define FLAG_RESET         	0

/* Peripheral Clock Enable and Disable Functions */

void EnablePeriClk(uint32_t *PeripheralAddress);
void DisablePeriClk(uint32_t *PeripheralAddress);
void NVIC_EnableIRQ(uint8_t IRQNum);
void NVIC_DisableIRQ(uint8_t IRQNum);
void NVIC_IRQSetPending(uint8_t IRQNum);
void NVIC_IRQClearPending(uint8_t IRQNum);

/*
Use the macro NVIC_PRIO_REG_BASE as the value to be passed for the first argument of NVIC_SetPriority and NVIC_GetPriority functions
*/
void NVIC_SetPriority(uint32_t *NVICPriorityRegisterBaseAddress, uint8_t IRQNum, uint8_t Priority);
uint8_t NVIC_GetPriority(uint32_t *NVICPriorityRegisterBaseAddress, uint8_t IRQNum);

#endif /* INC_STM32F4XX_H_ */
