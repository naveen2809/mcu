/*
 * gpio_driver.c
 *
 *  Created on: Jun 26, 2023
 *      Author: naveen
 */

#include <stdint.h>
#include "gpio_driver.h"

void GPIOSetMode(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t GPIOMode)
{

	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOAddress;

	pGPIO->MODER &= ~(0x03 << 2*GPIOPinNum);
	pGPIO->MODER |= (GPIOMode << 2*GPIOPinNum);

}

uint8_t GPIOReadPin(uint32_t *GPIOAddress, uint8_t GPIOPinNum)
{
	uint32_t value, temp;

	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOAddress;

	temp = pGPIO->IDR;
	temp &= (1 << GPIOPinNum);
	value = (temp >> GPIOPinNum);

	return value;
}

void GPIOWritePin(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t GPIOValue)
{
	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOAddress;

	pGPIO->ODR &= ~(1 << GPIOPinNum);
	pGPIO->ODR |= (GPIOValue << GPIOPinNum);
}

void GPIOTogglePin(uint32_t *GPIOAddress, uint8_t GPIOPinNum)
{
	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOAddress;

	pGPIO->ODR ^= (1 << GPIOPinNum);
}

void GPIOWritePort(uint32_t *GPIOAddress, uint32_t GPIOPortValue)
{
	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOAddress;

	pGPIO->ODR = GPIOPortValue;
}

uint32_t GPIOReadIDR(uint32_t *GPIOAddress)
{
	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOAddress;

	return pGPIO->IDR;
}

uint32_t GPIOReadODR(uint32_t *GPIOAddress)
{
	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOAddress;

	return pGPIO->ODR;
}

void GPIOSetAltFn(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t GPIOAltFn)
{
	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOAddress;

	uint8_t afr_reg_no = GPIOPinNum/8;
	uint8_t afr_bit_pos = GPIOPinNum%8;

	pGPIO->AFR[afr_reg_no] &= ~(0xF << 4*afr_bit_pos);
	pGPIO->AFR[afr_reg_no] |= (GPIOAltFn << 4*afr_bit_pos);
}

void GPIOSetOutputType(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t GPIOOPType)
{
	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOAddress;

	pGPIO->OTYPER &= ~(0x1 << GPIOPinNum);
	pGPIO->OTYPER |= (GPIOOPType << GPIOPinNum);
}

void GPIOSetPullUpDownConfig(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t GPIOPullUpDown)
{
	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOAddress;

	pGPIO->PUPDR &= ~(0x3 << 2*GPIOPinNum);
	pGPIO->PUPDR |= (GPIOPullUpDown << 2*GPIOPinNum);
}

void GPIOSetOutputSpeed(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t GPIOOPSpeed)
{
	struct GPIO_RegDef_t *pGPIO = (struct GPIO_RegDef_t *) GPIOAddress;

	pGPIO->OSPEEDR &= ~(0x3 << 2*GPIOPinNum);
	pGPIO->OSPEEDR |= (GPIOOPSpeed << 2*GPIOPinNum);
}
