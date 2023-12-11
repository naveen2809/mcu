/*
 * rgb_led.c
 *
 *  Created on: 02-Dec-2023
 *      Author: naveen
 */

#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "gpio_driver.h"
#include "general_purpose_timer.h"

#define TIM2_CH2_GPIO		1
#define TIM2_CH3_GPIO		2
#define TIM2_CH4_GPIO		3

#define SLEEP_COUNT			50000

struct General_Purpose_Timer_Handle_t TIM2_Handle;

void delay(void)
{
	uint32_t j;
	for(j=0;j<SLEEP_COUNT;j++);
}

int main(void)
{
	TIM2_Handle.pGeneral_Purpose_Timer = (struct General_Purpose_Timer_RegDef_t *) TIM2;

	memset(&TIM2_Handle.General_Purpose_Timer_Config,0,sizeof(TIM2_Handle.General_Purpose_Timer_Config));
	TIM2_Handle.General_Purpose_Timer_Config.Timer_PreScalerValue = 0x1;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_AutoReloadValue = 0xFA0;

	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_1_Status = TIMER_CHANNEL_DISABLED;

	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_2_Status = TIMER_CHANNEL_ENABLED;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_2_Mode = TIMER_CHANNEL_MODE_OUTPUT_TOGGLE;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_2_OutputCompareRegisterValue = 0xFA0;

	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_3_Status = TIMER_CHANNEL_ENABLED;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_3_Mode = TIMER_CHANNEL_MODE_OUTPUT_TOGGLE;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_3_OutputCompareRegisterValue = 0xFA0;

	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_4_Status = TIMER_CHANNEL_ENABLED;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_4_Mode = TIMER_CHANNEL_MODE_OUTPUT_TOGGLE;
	TIM2_Handle.General_Purpose_Timer_Config.Timer_Channel_4_OutputCompareRegisterValue = 0xFA0;

	//GPIO Configuration
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, TIM2_CH2_GPIO, GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,TIM2_CH2_GPIO,GPIO_ALTFN_1);
	GPIOSetMode(GPIOA, TIM2_CH3_GPIO, GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,TIM2_CH3_GPIO,GPIO_ALTFN_1);
	GPIOSetMode(GPIOA, TIM2_CH4_GPIO, GPIO_MODE_ALTFN);
	GPIOSetAltFn(GPIOA,TIM2_CH4_GPIO,GPIO_ALTFN_1);

	// Timer 2 Configuration
	EnablePeriClk(TIM2);
	General_Purpose_Timer_Init(&TIM2_Handle);
	General_Purpose_Timer_PeripheralEnable(&TIM2_Handle);

	while(1)
	{

	}

	return 0;
}
