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
#include "os_utilities.h"

Mutex m;

uint8_t volatile producer_count = 0;
uint8_t volatile consumer_count = 0;

static void producer(void);
static void consumer(void);
static void delay(uint32_t count);

int main(void)
{
	configure_uart();

	os_mutex_init(&m);

	os_create_task(producer,0,1024);
	os_create_task(producer,0,1024);
	os_create_task(consumer,0,1024);
	os_create_task(consumer,0,1024);
	os_create_task(consumer,0,1024);
	os_start();

	while(1);

	return 0;
}

static void producer(void)
{
	uint32_t volatile producer_id;

	os_mutex_wait(&m);
	producer_id = ++producer_count;
	os_mutex_signal(&m);

	while(1)
	{
		os_mutex_wait(&m);
		printf("From Producer %ld\r\n",producer_id);
		os_mutex_signal(&m);

		delay(5000);
	}
}

static void consumer(void)
{
	uint32_t volatile consumer_id;

	os_mutex_wait(&m);
	consumer_id = ++consumer_count;
	os_mutex_signal(&m);

	while(1)
	{
		os_mutex_wait(&m);
		printf("From Consumer %ld\r\n",consumer_id);
		os_mutex_signal(&m);

		delay(5000);
	}
}

static void delay(uint32_t count)
{
	uint32_t j;
	for(j=0;j<count;j++);
}