/*
 * assembly_2.s
 *
 *  Created on: 10-Sep-2023
 *      Author: naveen
 */


.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.equ AHB1ENR_ADDR, 0x40023830U
.equ GPIOD_MODE_REG_ADDR, 0x40020C00U
.equ GPIOD_ODR_ADDR, 0x40020C14U
.equ GPIOD_PERI_CLK_ENB_VAL, 0x8U
.equ GPIOD_MODE_SET_VAL, 0x40000000U
.equ GPIOD_MODE_CLR_VAL, 0x3FFFFFFFU
.equ GPIOD_ODR_SET_VAL, 0x00008000U
.equ GPIOD_ODR_CLR_VAL, 0xFFFF7FFFU
.equ DELAY_COUNT, 0x1FFFFFU

.global main

.section .text
main:

	/* Save LR */
	PUSH {LR}

	/* 1. Enable peripheral clock for GPIOD */
	LDR R0,=AHB1ENR_ADDR
	LDR R1,=GPIOD_PERI_CLK_ENB_VAL
	LDR R2, [R0]
	ORR R2, R1
	STR R2, [R0]

	/* 2. Set GPIOD Pin 12 as Output */
	LDR R0,=GPIOD_MODE_REG_ADDR
	LDR R2, [R0]
	LDR R1,=GPIOD_MODE_CLR_VAL
	AND R2, R1
	LDR R1,=GPIOD_MODE_SET_VAL
	ORR R2, R1
	STR R2, [R0]

LED_On:
	/* 3. Toggle on GPIOD Pin 12 Periodically */
	LDR R0,=GPIOD_ODR_ADDR
	LDR R1,=GPIOD_ODR_SET_VAL
	LDR R2, [R0]
	ORR R2, R1
	STR R2, [R0]

	/* Delay */
	BL Delay
	/*
	LDR R3,=DELAY_COUNT
Delay_On:
	SUBS R3, R3, #1
	BNE Delay_On
*/

LED_Off:
	LDR R0,=GPIOD_ODR_ADDR
	LDR R1,=GPIOD_ODR_CLR_VAL
	LDR R2, [R0]
	AND R2, R1
	STR R2, [R0]

	/* Delay */
	BL Delay
	/*
	LDR R3,=DELAY_COUNT
Delay_Off:
	SUBS R3, R3, #1
	BNE Delay_Off
*/
	B LED_On

Return:
	/* Restore LR */
	POP {LR}
	BX LR

.align 4
Delay:
	LDR R3,=DELAY_COUNT
Delay_Repeat:
	SUBS R3, R3, #1
	BNE Delay_Repeat
	BX LR
