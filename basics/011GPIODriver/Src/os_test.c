/*
 * task_scheduler.c
 *
 *  Created on: 11-Sep-2023
 *      Author: naveen
 */

#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "usart_driver.h"
#include "gpio_driver.h"
#include <stdio.h>
#include "common_utils.h"
#include "os_kernel.h"

#define GPIO_PIN_LED_1		GPIO_PIN_12
#define GPIO_PIN_LED_2		GPIO_PIN_13
#define GPIO_PIN_LED_3		GPIO_PIN_14
#define GPIO_PIN_LED_4		GPIO_PIN_15

#define TICK_COUNT_TASK_1	0x8U
#define TICK_COUNT_TASK_2	0x16U
#define TICK_COUNT_TASK_3   0x32U
#define TICK_COUNT_TASK_4   0x64U

extern uint32_t tick_count;

void configure_leds(void);
void task1(void);
void task2(void);
void task3(void);
void task4(void);

int main(void)
{
	configure_leds();
	configure_uart();

	os_create_task(task1,0,1024);
	os_create_task(task2,1,1024);
	os_create_task(task3,2,1024);
	os_create_task(task4,3,1024);
	os_start();

	while(1);

	return 0;
}

void task1(void)
{
	static uint32_t target_tick_count_1 = TICK_COUNT_TASK_1;

	while(1)
	{
		printf("From Task 1...\r\n");
		//printf("Task 1 Going to Sleep...\r\n");
		//task_sleep(0xFFFFFFFF);
		if(target_tick_count_1 <= tick_count)
		{
			target_tick_count_1 = tick_count + TICK_COUNT_TASK_1;
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_1);
		}
	}
}

void task2(void)
{
	static uint32_t target_tick_count_2 = TICK_COUNT_TASK_2;

	while(1)
	{
		printf("From Task 2...\r\n");
		//printf("Task 2 Going to Sleep...\r\n");
		//task_sleep(0xFFFFFFFF);
		if(target_tick_count_2 <= tick_count)
		{
			target_tick_count_2 = tick_count + TICK_COUNT_TASK_2;
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_2);
		}
	}
}

void task3(void)
{
	static uint32_t target_tick_count_3 = TICK_COUNT_TASK_3;

	/*
	static int count = 0;

	if(count == 0)
	{
		printf("Task 3 Going to Sleep...\r\n");
		count++;
		task_sleep(0x1FF);
	}
	*/

	while(1)
	{
		printf("From Task 3...\r\n");
		//printf("Task 3 Going to Sleep...\r\n");
		//task_sleep(0xFFFFFFFF);
		if(target_tick_count_3 <= tick_count)
		{
			target_tick_count_3 = tick_count + TICK_COUNT_TASK_3;
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_3);
		}
	}
}

void task4(void)
{
	static uint32_t target_tick_count_4 = TICK_COUNT_TASK_4;

	while(1)
	{
		printf("From Task 4...\r\n");
		//printf("Task 4 Going to Sleep...\r\n");
		//task_sleep(0xFFFFFFFF);
		if(target_tick_count_4 <= tick_count)
		{
			target_tick_count_4 = tick_count + TICK_COUNT_TASK_4;
			GPIOTogglePin(GPIOD, GPIO_PIN_LED_4);
		}
	}
}

void configure_leds(void)
{
	EnablePeriClk(GPIOD);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_1, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_2, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_3, GPIO_MODE_OUTPUT);
	GPIOSetMode(GPIOD, GPIO_PIN_LED_4, GPIO_MODE_OUTPUT);
}
