/*
 * assembly_5.s
 *
 *  Created on: 11-Sep-2023
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
.equ GPIOD_MODE_SET_VAL, 0x55000000U
.equ GPIOD_MODE_CLR_VAL, 0x00FFFFFFU
.equ GPIOD_ODR_TOG_VAL_1, 0x00001000U
.equ GPIOD_ODR_TOG_VAL_2, 0x00002000U
.equ GPIOD_ODR_TOG_VAL_3, 0x00004000U
.equ GPIOD_ODR_TOG_VAL_4, 0x00008000U
.equ SYT_RELOAD_VALUE, 0x0FFFFFU
.equ SYT_CONTROL_VALUE, 0x6U
.equ ICSR_VALUE, 0x10000000U
.equ DELAY_COUNT, 0xFFFFFU

.global main
.global SVC_Handler

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
Loop_INF:
	SVC #1
	BL Delay
	B Loop_INF

Return:
	/* Restore LR */
	POP {LR}
	BX LR

.align 4
.section .text.SVC_Handler
.weak SVC_Handler
.type SVC_Handler, %function
SVC_Handler:
	/* Toggle GPIO */
		LDR R0,=GPIOD_ODR_ADDR
		MRS R3, MSP
		ADD R3, R3, #24
		LDR R4, [R3]
		SUB R4, R4, #2
		EOR R3, R3, R3
		LDRB R3, [R4]
Try_1:
		SUBS R3, #1
		BNE Try_2
		LDR R1,=GPIOD_ODR_TOG_VAL_1
		B Exec_Toggle
Try_2:
		SUBS R3, #1
		BNE Try_3
		LDR R1,=GPIOD_ODR_TOG_VAL_2
		B Exec_Toggle
Try_3:
		SUBS R3, #1
		BNE Try_4
		LDR R1,=GPIOD_ODR_TOG_VAL_3
		B Exec_Toggle
Try_4:
		LDR R1,=GPIOD_ODR_TOG_VAL_4
Exec_Toggle:
		LDR R2, [R0]
		EOR R2, R1
		STR R2, [R0]
		BX LR

.align 4
.section .text.Delay_Function
.type Delay, %function
Delay:
	LDR R3,=DELAY_COUNT
Delay_Repeat:
	SUBS R3, R3, #1
	BNE Delay_Repeat
	BX LR
