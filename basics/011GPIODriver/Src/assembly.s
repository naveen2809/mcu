/*
 * assembly.s
 *
 *  Created on: 11-Sep-2023
 *      Author: naveen
 */


.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.extern current_task
.extern next_task
.extern current_sp

.global PendSV_Handler

.align 4
.section .text.PendSV_Handler
.weak PendSV_Handler
.type PendSV_Handler, %function
PendSV_Handler:
		/* Save Context of Previous Task */
		MRS R0, PSP
		STMDB R0!, {R4-R11}
		/* Save the current SP value */
		LDR R1, =current_task
		LDR R2, [R1]
		LDR R1, =current_sp
		MOV R3, #4
		MUL R2, R2, R3
		ADD R1, R1, R2
		STR R0, [R1]

		/* Retrieve the PSP of next task */
		LDR R1, =next_task
		LDR R2, [R1]
		LDR R1, =current_sp
		MOV R3, #4
		MUL R2, R2, R3
		ADD R1, R1, R2
		LDR R0, [R1]

		/* Restore Context of Next Task */
		LDMIA	R0!, {R4-R11}
		MSR PSP, R0

		/* Make current task equal to next task */
		LDR R1, =next_task
		LDR R2, [R1]
		LDR R1, =current_task
		STR R2, [R1]

		/* Exit Exception Handler */
		BX LR
