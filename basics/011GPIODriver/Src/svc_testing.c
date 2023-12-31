/*
 * svc_testing.c
 *
 *  Created on: 13-Sep-2023
 *      Author: naveen
 */

#include <stdint.h>
#include "gpio_driver.h"

#define GPIO_PIN_LED_1		GPIO_PIN_12
#define GPIO_PIN_LED_2		GPIO_PIN_13
#define GPIO_PIN_LED_3		GPIO_PIN_14
#define GPIO_PIN_LED_4		GPIO_PIN_15
#define SLEEP_COUNT 50000

void SVC_Handler(void);
void configure_leds(void);
void delay(void);
uint32_t dummy_call(uint32_t j);

uint8_t GPIOPin = 255;

int main(void)
{
	configure_leds();

	while(1)
	{
		__asm volatile ("SVC #4");
		delay();
	}
}

__attribute__ ((naked)) void SVC_Handler(void)
{

	/* Save Registers R4 to R11 */
	__asm volatile ("PUSH {LR}");
	__asm volatile ("PUSH {R4-R11}");

	/* Get the argument of SVC instruction */
	__asm volatile ("LDR R4,=GPIOPin");
	__asm volatile ("MRS R6, MSP");
	__asm volatile ("ADD R6, R6, #60");
	__asm volatile ("LDR R7, [R6]");
	__asm volatile ("SUB R7, R7, #2");
	__asm volatile ("LDRB R5, [R7]");
	__asm volatile ("STRB R5,[R4]");

	/* Toggle the appropriate LED */
	switch(GPIOPin)
	{
		case 1:
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_1);
			break;
		case 2:
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_2);
			break;
		case 3:
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_3);
			break;
		case 4:
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_4);
			break;
		default:
			break;
	}

	/* Restore Registers R4 to R11 */
	__asm volatile ("POP {R4-R11}");

	/* Return from exception handler */
	__asm volatile ("POP {LR}");
	__asm volatile ("BX LR");
}

void configure_leds(void)
{
	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_1, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_2, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_3, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_4, GPIO_MODE_OUTPUT);
}

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++)
		dummy_call(j);
}

uint32_t dummy_call(uint32_t j)
{
	j++;
	return j;
}
