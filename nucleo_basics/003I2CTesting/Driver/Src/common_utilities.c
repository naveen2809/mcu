/*
 * common_utilities.c
 *
 *  Created on: 12-Dec-2023
 *      Author: naveen
 */

#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "general_purpose_timer.h"
#include "common_utilities.h"

struct General_Purpose_Timer_Handle_t TIM2_Handle;

void delay_us(uint32_t delay)
{
	TIM2_Handle.pGeneral_Purpose_Timer->TIMx_CNT = 0;
	while(TIM2_Handle.pGeneral_Purpose_Timer->TIMx_CNT < 16*delay);
}

void configure_delay_timer(void)
{
	//TIM2 Configuration
	TIM2_Handle.pGeneral_Purpose_Timer = (struct General_Purpose_Timer_RegDef_t *) TIM2;
	memset(&TIM2_Handle.General_Purpose_Timer_Config,0,sizeof(TIM2_Handle.General_Purpose_Timer_Config));
	TIM2_Handle.General_Purpose_Timer_Config.Timer_PreScalerValue = 0x1;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_AutoReloadValue = 0xFFFFFFFF;

	EnablePeriClk(TIM2);
	General_Purpose_Timer_Init(&TIM2_Handle);
	General_Purpose_Timer_PeripheralEnable(&TIM2_Handle);
}
