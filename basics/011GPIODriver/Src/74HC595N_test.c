/*
 * basic_timer_test.c
 *
 *  Created on: 29-Aug-2023
 *      Author: naveen
 */

#include "timer_driver.h"
#include "gpio_driver.h"

#define CLK_PIN_SERIAL		GPIO_PIN_1
#define CLK_PIN_PARALLEL	GPIO_PIN_3
#define DATA_PIN			GPIO_PIN_5
#define OE_PIN				GPIO_PIN_7
#define TIM6_IRQ_NUM		54
#define TIM7_IRQ_NUM		55

struct Basic_Timer_Handle_t TIM6_Handle;
struct Basic_Timer_Handle_t TIM7_Handle;

uint8_t bit_pattern[8] = {1,0,0,0,0,0,0,0};
uint8_t data_count = 0;
uint32_t clock_count = 0;

void TIM6_DAC_IRQHandler(void);
void TIM7_IRQHandler(void);
void Basic_Timer_Interrupt_Callback(struct Basic_Timer_Handle_t *pBasic_Timer_Handle);

int main(void)
{

	//GPIO Configuration for on-board LED
	EnablePeriClk(GPIOA);
	GPIOSetMode(GPIOA, CLK_PIN_SERIAL, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, CLK_PIN_PARALLEL, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, DATA_PIN, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOA, OE_PIN, GPIO_MODE_OUTPUT);

	//Enabling the Interrupt for TIM6 and TIM7 Peripherals in NVIC
	NVIC_EnableIRQ(TIM6_IRQ_NUM);
	NVIC_EnableIRQ(TIM7_IRQ_NUM);

	//TIM6 Configuration
	TIM6_Handle.pBasic_Timer = (struct Basic_Timer_RegDef_t *)TIM6;
	TIM6_Handle.Basic_Timer_Config.Timer_PreScalerValue = 0xffff;
	TIM6_Handle.Basic_Timer_Config.Timer_AutoReloadValue = 0x7f;
	TIM6_Handle.Basic_Timer_Config.Timer_UpdateInterruptConfig = TIMER_UPDATE_INTERRUPT_ENABLE;

	EnablePeriClk(TIM6);
	Basic_Timer_Init(&TIM6_Handle);
	Basic_Timer_PeripheralEnable(&TIM6_Handle);

	//TIM7 Configuration
	TIM7_Handle.pBasic_Timer = (struct Basic_Timer_RegDef_t *)TIM7;
	TIM7_Handle.Basic_Timer_Config.Timer_PreScalerValue = 0xffff;
	TIM7_Handle.Basic_Timer_Config.Timer_AutoReloadValue = 0x3f;
	TIM7_Handle.Basic_Timer_Config.Timer_UpdateInterruptConfig = TIMER_UPDATE_INTERRUPT_ENABLE;

	EnablePeriClk(TIM7);
	Basic_Timer_Init(&TIM7_Handle);
	//Basic_Timer_PeripheralEnable(&TIM7_Handle);


	GPIOWritePin(GPIOA,CLK_PIN_SERIAL,GPIO_LOW);
	GPIOWritePin(GPIOA,CLK_PIN_PARALLEL,GPIO_LOW);
	GPIOWritePin(GPIOA,OE_PIN,GPIO_LOW);

	//Starting the data stream
	if(bit_pattern[data_count] == 1)
	{
		GPIOWritePin(GPIOA,DATA_PIN,GPIO_HIGH);
		data_count = (data_count+1)%8;
	}
	else
	{
		GPIOWritePin(GPIOA,DATA_PIN,GPIO_LOW);
		data_count = (data_count+1)%8;
	}

	while(1);

	return 0;
}

void TIM6_DAC_IRQHandler(void)
{
	Basic_Timer_IRQHandling(&TIM6_Handle);
}

void TIM7_IRQHandler(void)
{
	Basic_Timer_IRQHandling(&TIM7_Handle);
}

void Basic_Timer_Interrupt_Callback(struct Basic_Timer_Handle_t *pBasic_Timer_Handle)
{
	Basic_Timer_ClearUpdateEventFlag(pBasic_Timer_Handle);

	if(pBasic_Timer_Handle->pBasic_Timer == (struct Basic_Timer_RegDef_t *)TIM6)
	{
		GPIOTogglePin(GPIOA, CLK_PIN_SERIAL);
		Basic_Timer_PeripheralEnable(&TIM7_Handle);
	}
	else if(pBasic_Timer_Handle->pBasic_Timer == (struct Basic_Timer_RegDef_t *)TIM7)
	{
		GPIOTogglePin(GPIOA, CLK_PIN_PARALLEL);
		Basic_Timer_PeripheralDisable(&TIM7_Handle);

		clock_count++;

		if(clock_count%2 == 0)
		{
			if(bit_pattern[data_count] == 1)
			{
				GPIOWritePin(GPIOA,DATA_PIN,GPIO_HIGH);
				data_count = (data_count+1)%8;
			}
			else
			{
				GPIOWritePin(GPIOA,DATA_PIN,GPIO_LOW);
				data_count = (data_count+1)%8;
			}
		}
	}
}
