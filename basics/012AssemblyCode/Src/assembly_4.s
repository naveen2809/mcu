/*
 * assembly_4.s
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
.equ ICSR_ADDR, 0xE000ED04U
.equ SYT_RLV_REG_ADDR, 0xE000E014U
.equ SYT_CNT_REG_ADDR, 0xE000E010U
.equ GPIOD_PERI_CLK_ENB_VAL, 0x8U
.equ GPIOD_MODE_SET_VAL, 0x40000000U
.equ GPIOD_MODE_CLR_VAL, 0x3FFFFFFFU
.equ GPIOD_ODR_TOG_VAL, 0x00008000U
.equ SYT_RELOAD_VALUE, 0x0FFFFFU
.equ SYT_CONTROL_VALUE, 0x6U
.equ ICSR_VALUE, 0x10000000U

.global main
.global SysTick_Handler
.global PendSV_Handler

.section .text
.align 4
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
	/* Configure Systick Timer */
	LDR R0,=SYT_RLV_REG_ADDR
	LDR R1,=SYT_RELOAD_VALUE
	STR R1, [R0]
	LDR R0,=SYT_CNT_REG_ADDR
	LDR R1,=SYT_CONTROL_VALUE
	LDR R2, [R0]
	ORR R2, R1
	STR R2, [R0]
	LDR R2, [R0]
	ORR.W R2, #0x1
	STR R2, [R0]
Loop_INF:
	MOVS R0, #0x1
	BNE Loop_INF

Return:
	/* Restore LR */
	POP {LR}
	BX LR

.align 4
.section .text.SysTick_Handler
.weak SysTick_Handler
.type SysTick_Handler, %function
SysTick_Handler:
	/* Set the PendSV Exception */
	LDR R0,=ICSR_ADDR
	LDR R1,=ICSR_VALUE
	LDR R2, [R0]
	ORR R2, R1
	STR R2, [R0]
	BX LR

.align 4
.section .text.PendSV_Handler
.weak PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
		/* Toggle GPIO */
		LDR R0,=GPIOD_ODR_ADDR
		LDR R1,=GPIOD_ODR_TOG_VAL
		LDR R2, [R0]
		EOR R2, R1
		STR R2, [R0]
		BX LR
