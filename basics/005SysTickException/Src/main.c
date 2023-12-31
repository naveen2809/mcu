/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#define SYSTICK_CTRL_AND_STATUS_REG_ADDR	0xE000E010UL
#define SYSTICK_RELOAD_VAL_REG_ADDR 		0xE000E014UL
#define SYSTICK_CURRENT_VAL_REG_ADDR 		0xE000E018UL

void SysTick_Handler(void);
int count = 0;

int main(void)
{

	//SysTick Peripheral Configuration
	uint32_t *pSysTickCtrlAndStatusReg = (uint32_t*) SYSTICK_CTRL_AND_STATUS_REG_ADDR;
	uint32_t *pSysTickReloadValReg = (uint32_t *) SYSTICK_RELOAD_VAL_REG_ADDR;
	uint32_t *pSysTickCurrentValReg = (uint32_t *) SYSTICK_CURRENT_VAL_REG_ADDR;

	*pSysTickReloadValReg = 0x00FFFFFF;
	*pSysTickCurrentValReg = 0x00000000;        //Clearing the current value register
	*pSysTickCtrlAndStatusReg |= (7 << 0);

    /* Loop forever */
	for(;;);
}

void SysTick_Handler(void)
{
	count++;
	if(count%10 == 0)
		printf("SysTick Exception Triggered: %d\n", count);
}
