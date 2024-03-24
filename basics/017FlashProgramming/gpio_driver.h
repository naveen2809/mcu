/*
 * gpio_driver.h
 *
 *  Created on: Jun 26, 2023
 *      Author: naveen
 */

#ifndef INC_GPIO_DRIVER_H_
#define INC_GPIO_DRIVER_H_


#include "stm32f4xx.h"

#define GPIO_PIN_0		0
#define GPIO_PIN_1		1
#define GPIO_PIN_2		2
#define GPIO_PIN_3		3
#define GPIO_PIN_4		4
#define GPIO_PIN_5		5
#define GPIO_PIN_6		6
#define GPIO_PIN_7		7
#define GPIO_PIN_8		8
#define GPIO_PIN_9		9
#define GPIO_PIN_10		10
#define GPIO_PIN_11		11
#define GPIO_PIN_12		12
#define GPIO_PIN_13		13
#define GPIO_PIN_14		14
#define GPIO_PIN_15		15

#define GPIO_MODE_INPUT		0x0
#define GPIO_MODE_OUTPUT	0x1
#define GPIO_MODE_ALTFN		0x2
#define GPIO_MODE_ANALOG	0x3

struct GPIO_RegDef_t
{
	uint32_t MODER;
	uint32_t OTYPER;
	uint32_t OSPEEDR;
	uint32_t PUPDR;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSSR;
	uint32_t LCKR;
	uint32_t AFR[2];

};

#define GPIOx_MODER_OFFSET			0x00UL
#define GPIOx_OTYPER_OFFSET			0x04UL
#define GPIOx_OSPEEDR_OFFSET		0x08UL
#define GPIOx_PUPDR_OFFSET			0x0CUL
#define GPIOx_IDR_OFFSET			0x10UL
#define GPIOx_ODR_OFFSET			0x14UL
#define GPIOx_BSSR_OFFSET			0x18UL
#define GPIOx_LCKR_OFFSET			0x1CUL
#define GPIOx_AFRL_OFFSET			0x20UL
#define GPIOx_AFRH_OFFSET			0x24UL

#define GPIO_ALTFN_0		0
#define GPIO_ALTFN_1		1
#define GPIO_ALTFN_2		2
#define GPIO_ALTFN_3		3
#define GPIO_ALTFN_4		4
#define GPIO_ALTFN_5		5
#define GPIO_ALTFN_6		6
#define GPIO_ALTFN_7		7
#define GPIO_ALTFN_8		8
#define GPIO_ALTFN_9		9
#define GPIO_ALTFN_10		10
#define GPIO_ALTFN_11		11
#define GPIO_ALTFN_12		12
#define GPIO_ALTFN_13		13
#define GPIO_ALTFN_14		14
#define GPIO_ALTFN_15		15

#define GPIO_OPTYPE_PP		0
#define GPIO_OPTYPE_OD		1

#define GPIO_NO_PULL_UP_DOWN	0
#define GPIO_PULL_UP			1
#define GPIO_PULL_DOWN			2

#define GPIO_OPSPEED_LOW			0
#define GPIO_OPSPEED_MEDIUM			1
#define GPIO_OPSPEED_HIGH			2
#define GPIO_OPSPEED_VERY_HIGH		3

#define GPIO_HIGH	1
#define GPIO_LOW	0

void GPIOSetMode(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t GPIOMode);
uint8_t GPIOReadPin(uint32_t *GPIOAddress, uint8_t GPIOPinNum);
void GPIOWritePin(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t Value);
void GPIOTogglePin(uint32_t *GPIOAddress, uint8_t GPIOPinNum);
void GPIOWritePort(uint32_t *GPIOAddress, uint32_t GPIOPortValue);
uint32_t GPIOReadIDR(uint32_t *GPIOAddress);
uint32_t GPIOReadODR(uint32_t *GPIOAddress);
void GPIOSetAltFn(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t GPIOAltFn);
void GPIOSetOutputType(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t GPIOOPType);
void GPIOSetOutputSpeed(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t GPIOOPSpeed);
void GPIOSetPullUpDownConfig(uint32_t *GPIOAddress, uint8_t GPIOPinNum, uint8_t GPIOPullUpDown);

#endif /* INC_GPIO_DRIVER_H_ */
