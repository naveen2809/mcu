/*
 * task_scheduler.c
 *
 *  Created on: 11-Sep-2023
 *      Author: naveen
 */


#include "gpio_driver.h"

#define GPIO_PIN_LED_0		GPIO_PIN_12
#define GPIO_PIN_LED_1		GPIO_PIN_13
#define GPIO_PIN_LED_2		GPIO_PIN_14
#define GPIO_PIN_LED_3		GPIO_PIN_15
#define MAX_TASKS			4
#define TASK_STACK_SIZE		1024
#define SYT_RELOAD_VALUE 	0x1FFFFFU
#define SYT_CONTROL_VALUE 	0x6U
#define TICK_COUNT_TASK_0	0x4U
#define TICK_COUNT_TASK_1	0x8U
#define TICK_COUNT_TASK_2   0x12U
#define TICK_COUNT_TASK_3   0x16U

uint32_t tick_count = 0;
uint32_t current_task, next_task;
uint32_t task0_stack[TASK_STACK_SIZE];
uint32_t task1_stack[TASK_STACK_SIZE];
uint32_t task2_stack[TASK_STACK_SIZE];
uint32_t task3_stack[TASK_STACK_SIZE];
uint32_t current_sp[MAX_TASKS];

void configure_leds(void);
void configure_systick_timer(void);
void enable_systick_timer(void);
void SysTick_Handler(void);
void task0(void);
void task1(void);
void task2(void);
void task3(void);
void initialize_task_stacks(void);

int main(void)
{

	configure_leds();
	configure_systick_timer();
	initialize_task_stacks();
	enable_systick_timer();

	//Initialize PSP

	__asm volatile ("LDR R1, =current_sp");
	__asm volatile ("LDR R0, [R1]");
	__asm volatile ("MSR PSP, R0");

	//Change SP from MSP to PSP
	__asm volatile ("MRS R0, CONTROL");
	__asm volatile ("ORR R0, #0x3");
	__asm volatile ("MSR CONTROL, R0");
	__asm volatile ("ISB");

	current_task = 0;
	task0();

	while(1);

	return 0;
}

void task0(void)
{
	static uint32_t target_tick_count_0 = TICK_COUNT_TASK_0;

	while(1)
	{
		if(target_tick_count_0 <= tick_count)
		{
			target_tick_count_0 += TICK_COUNT_TASK_0;
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_0);
		}
	}
}

void task1(void)
{
	static uint32_t target_tick_count_1 = TICK_COUNT_TASK_1;

	while(1)
	{
		if(target_tick_count_1 <= tick_count)
		{
			target_tick_count_1 += TICK_COUNT_TASK_1;
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_1);
		}
	}
}

void task2(void)
{
	static uint32_t target_tick_count_2 = TICK_COUNT_TASK_2;

	while(1)
	{
		if(target_tick_count_2 <= tick_count)
		{
			target_tick_count_2 += TICK_COUNT_TASK_2;
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_2);
		}
	}
}

void task3(void)
{
	static uint32_t target_tick_count_3 = TICK_COUNT_TASK_3;

	while(1)
	{
		if(target_tick_count_3 <= tick_count)
		{
			target_tick_count_3 += TICK_COUNT_TASK_3;
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_3);
		}
	}
}

void configure_leds(void)
{
	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_0, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_1, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_2, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_3, GPIO_MODE_OUTPUT);
}

void configure_systick_timer(void)
{
	uint32_t *pSYT_RVR = (uint32_t *) 0xE000E014U;
	uint32_t *pSYT_CFGR = (uint32_t *) 0xE000E010U;

	*pSYT_RVR = SYT_RELOAD_VALUE;
	*pSYT_CFGR |= SYT_CONTROL_VALUE;
}

void enable_systick_timer(void)
{
	uint32_t *pSYT_CFGR = (uint32_t *) 0xE000E010U;

	*pSYT_CFGR |= 0x1;
}

void initialize_task_stacks(void)
{
	uint32_t *p;

	current_sp[0] = ((uint32_t) &task0_stack + 4*TASK_STACK_SIZE);
	/*
	current_sp[0] = ((uint32_t) &task0_stack + 4*TASK_STACK_SIZE - 4*16);
	p = (uint32_t*) ((uint32_t)current_sp[0]+14*4);
	*p = (uint32_t) task0;
	p = (uint32_t*) ((uint32_t)current_sp[0]+15*4);
	*p = (uint32_t) 0x01000000U;
	*/

	current_sp[1] = ((uint32_t) &task1_stack + 4*TASK_STACK_SIZE - 4*16);
	p = (uint32_t*) ((uint32_t)current_sp[1]+14*4);
	*p = (uint32_t) task1;
	p = (uint32_t*) ((uint32_t)current_sp[1]+15*4);
	*p = (uint32_t) 0x01000000U;

	current_sp[2] = ((uint32_t) &task2_stack + 4*TASK_STACK_SIZE - 4*16);
	p = (uint32_t*) ((uint32_t)current_sp[2]+14*4);
	*p = (uint32_t) task2;
	p = (uint32_t*) ((uint32_t)current_sp[2]+15*4);
	*p = (uint32_t) 0x01000000U;

	current_sp[3] = ((uint32_t) &task3_stack + 4*TASK_STACK_SIZE - 4*16);
	p = (uint32_t*) ((uint32_t)current_sp[3]+14*4);
	*p = (uint32_t) task3;
	p = (uint32_t*) ((uint32_t)current_sp[3]+15*4);
	*p = (uint32_t) 0x01000000U;
}

void SysTick_Handler(void)
{
	uint32_t *pICSR = (uint32_t *) 0xE000ED04U;

	tick_count++;
	next_task = (current_task+1)%(MAX_TASKS);

	// Enable PendSV Exception
	*pICSR |= 0x10000000U;
}
