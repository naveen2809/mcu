/*
 * assembly_1.s
 *
 *  Created on: 10-Sep-2023
 *      Author: naveen
 */


.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.global main

.section .text
main:
	LDR R0,=my_data
	LDRSB R4,[R0],#1
	LDRSB R5,[R0],#1
	LDRSB R6,[R0],#1
	LDRSB R7,[R0],#1
	LDRSB R8,[R0],#1
Return:
	BX LR

.section .data
my_data:
	.byte	0xAA,0xBB,0xCC,0xDD,0xEE
