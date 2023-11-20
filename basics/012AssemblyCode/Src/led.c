/*
 * led.c
 *
 *  Created on: Sep 5, 2023
 *      Author: naveen
 */

#include <stdint.h>

#define SLEEP_COUNT						500000

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{

	//0. Store the addresses AHB1ENR, GPIOD_MODE_REG, GPIOD_ODR
	__asm volatile ("LDR R0, =0x40023830");
	__asm volatile ("LDR R1, =0x40020C00");
	__asm volatile ("LDR R2, =0x40020C14");


	//1. Enable peripheral clock for GPIOD
	__asm volatile ("LDR R5, [R0]");
	__asm volatile ("MOV R4, #0x8");
	__asm volatile ("ORR R5, R4");
	__asm volatile ("STR R5, [R0]");

	//2. Set GPIOD Pin 12 as Output
	__asm volatile ("LDR R5, [R1]");
	__asm volatile ("MOVW R4, #0xffff");
	__asm volatile ("MOVT R4, #0xfcff");
	__asm volatile ("AND R5, R4");
	__asm volatile ("MOVW R4, #0x0000");
	__asm volatile ("MOVT R4, #0x0100");
	__asm volatile ("ORR R5, R4");
	__asm volatile ("STR R5, [R1]");

	//3. Turn on GPIOD Pin 12

	while(1)
	{
		__asm volatile ("LDR R5, [R2]");
		__asm volatile ("MOVW R4, #0x1000");
		__asm volatile ("MOVT R4, #0x0000");
		__asm volatile ("ORR R5, R4");
		__asm volatile ("STR R5, [R2]");
		__asm volatile ("PUSH {R0-R2}");
		delay();
		__asm volatile ("POP {R0-R2}");
		__asm volatile ("LDR R5, [R2]");
		__asm volatile ("MOVW R4, #0xefff");
		__asm volatile ("MOVT R4, #0xffff");
		__asm volatile ("AND R5, R4");
		__asm volatile ("STR R5, [R2]");
		__asm volatile ("PUSH {R0-R2}");
		delay();
		__asm volatile ("POP {R0-R2}");
	}

    /* Loop forever */
	for(;;);
}
