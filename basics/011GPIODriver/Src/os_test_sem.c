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

#define SLEEP_COUNT_PRODUCER	2000
#define SLEEP_COUNT_CONSUMER	6000

extern uint32_t tick_count;

uint32_t volatile glo_val = 0xFFFF;
Semaphore w;
Semaphore r;

static void producer(void);
static void consumer(void);
static void delay(uint32_t count);

int main(void)
{
	configure_uart();

	os_sem_init(&w,1,1);
	os_sem_init(&r,0,1);

	os_create_task(producer,0,1024);
	os_create_task(consumer,0,1024);
	os_start();

	while(1);

	return 0;
}

static void producer(void)
{
	static uint32_t volatile producer_count = 0;

	while(1)
	{
		producer_count++;
		os_sem_wait(&w);
		glo_val = producer_count;
		printf("Global Value From Producer: %ld\r\n",glo_val);
		os_sem_signal(&r);
		delay(SLEEP_COUNT_PRODUCER);
	}
}

static void consumer(void)
{
	while(1)
	{
		os_sem_wait(&r);
		printf("Global Value From Consumer: %ld\r\n",glo_val);
		os_sem_signal(&w);
		delay(SLEEP_COUNT_CONSUMER);
	}
}

static void delay(uint32_t count)
{
	uint32_t j;
	for(j=0;j<count;j++);
}
