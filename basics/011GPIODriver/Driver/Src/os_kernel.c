/*
 * task_scheduler.c
 *
 *  Created on: 11-Sep-2023
 *      Author: naveen
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "common_utils.h"
#include "os_kernel.h"

uint32_t tick_count = 0;
struct TCB *current_task = NULL;
struct TCB *next_task = NULL;

void idletask(void)
{
	while(1)
	{
		printf("From Idle Task...\r\n");
	}
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

void SysTick_Handler(void)
{
	uint32_t *pICSR = (uint32_t *) 0xE000ED04U;

	tick_count++;
	update_sleep_status();

	update_next_task();

	// Enable PendSV Exception
	*pICSR |= 0x10000000U;
}

__attribute ((naked)) void PendSV_Handler(void)
{
	/* Save Context of Current Task */
	__asm volatile ("MRS R0, PSP");
	__asm volatile ("STMDB R0!, {R4-R11}");

	/* Save the current SP value */
	__asm volatile ("LDR R1, =current_task");
	__asm volatile ("LDR R1, [R1]");
	__asm volatile ("STR R0, [R1]");

	/* Restore Context of Next Task */

	/* Retrieve the PSP of next task */
	__asm volatile ("LDR R1, =next_task");
	__asm volatile ("LDR R1, [R1]");
	__asm volatile ("LDR R0, [R1]");

	/* Restore Context of Next Task */
	__asm volatile ("LDMIA	R0!, {R4-R11}");
	__asm volatile ("MSR PSP, R0");

	/* Update Current Task */
	current_task = next_task;

	/* Exit PendSV_Handler Handler */
	__asm volatile ("BX LR");
}

void os_create_task(void (*task)(void),uint8_t task_priority,uint32_t task_stack_size)
{
	uint32_t *p;
	struct TCB *last_task, *new_task;

	if(current_task == NULL)						//Adding the idle task
	{
		current_task = (struct TCB *) malloc(sizeof(struct TCB));

		current_task->priority = IDLE_TASK_PRIORITY;
		current_task->sleep_count = 0;

		current_task->task_stack = (uint32_t *) malloc(IDLE_TASK_STACK_SIZE*4);
		current_task->current_sp = ((uint32_t) current_task->task_stack + 4*IDLE_TASK_STACK_SIZE - 4*16);
		p = (uint32_t*) ((uint32_t)current_task->current_sp+14*4);
		*p = (uint32_t) idletask;
		p = (uint32_t*) ((uint32_t)current_task->current_sp+15*4);
		*p = (uint32_t) 0x01000000U;

		current_task->next = current_task;
	}

	//Go to end of list
	last_task = current_task;
	while(last_task->next != current_task)
	{
		last_task = last_task->next;
	}

	//Add new task
	new_task = (struct TCB *) malloc(sizeof(struct TCB));
	last_task->next = new_task;

	new_task->priority = task_priority;
	new_task->sleep_count = 0;

	new_task->task_stack = (uint32_t *) malloc(task_stack_size*4);
	new_task->current_sp = ((uint32_t) new_task->task_stack + 4*task_stack_size - 4*16);
	p = (uint32_t*) ((uint32_t) new_task->current_sp+14*4);
	*p = (uint32_t) task;
	p = (uint32_t*) ((uint32_t) new_task->current_sp+15*4);
	*p = (uint32_t) 0x01000000U;

	new_task->next = current_task;

	return;
}

void os_start(void)
{
	configure_systick_timer();
	enable_systick_timer();

	//Initialize PSP
	__asm volatile ("LDR R1, =current_task");
	__asm volatile ("LDR R0, [R1]");
	__asm volatile ("LDR R0, [R0]");
	__asm volatile ("MSR PSP, R0");

	//Change SP from MSP to PSP
	__asm volatile ("MRS R0, CONTROL");
	__asm volatile ("ORR R0, #0x2");
	__asm volatile ("MSR CONTROL, R0");
	__asm volatile ("ISB");

	idletask();

	return;
}

void task_sleep(uint32_t count)
{
	uint32_t *pICSR = (uint32_t *) 0xE000ED04U;

	current_task->sleep_count = count;

	//Pending the SysTick Exception
	*pICSR |= 0x4000000U;

	return;
}

void os_yield(void)
{
	uint32_t *pICSR = (uint32_t *) 0xE000ED04U;

	//Pending the SysTick Exception
	*pICSR |= 0x4000000U;

	return;
}

void update_sleep_status(void)
{
	struct TCB *present_task;

	present_task = current_task;

	do
	{
		if(present_task->sleep_count > 0)
		{
			present_task->sleep_count--;
		}
		present_task = present_task->next;
	}
	while(present_task != current_task);

	return;
}

void update_next_task(void)
{
	uint8_t max_priority = 255;
	struct TCB *best_task;

	next_task = current_task;

	do
	{
		next_task = next_task->next;

		if(next_task->priority < max_priority && next_task->sleep_count==0)
		{
			max_priority = next_task->priority;
			best_task = next_task;
		}
	}
	while(next_task != current_task);

	next_task = best_task;

	return;
}

void os_disable_interrupts(void)
{
	__asm volatile ("CPSID I");

	return;
}

void os_enable_interrupts(void)
{
	__asm volatile ("CPSIE I");

	return;
}

void os_mutex_init(Mutex *m)
{
	*m = 1;

	return;
}

void os_mutex_wait(Mutex *m)
{
	os_disable_interrupts();

	while((*m) == 0)
	{
		os_enable_interrupts();
		os_yield();
		os_disable_interrupts();
	}

	*m = 0;

	os_enable_interrupts();

	return;
}

void os_mutex_signal(Mutex *m)
{
	os_disable_interrupts();

	*m = 1;

	os_enable_interrupts();

	return;
}

void os_sem_init(Semaphore *s,uint32_t init_value,uint32_t maximum_value)
{
	s->cur_value = init_value;
	s->max_value = maximum_value;

	return;
}

void os_sem_wait(Semaphore *s)
{
	os_disable_interrupts();

	while((s->cur_value) == 0)
	{
		os_enable_interrupts();
		os_yield();
		os_disable_interrupts();
	}

	s->cur_value--;

	os_enable_interrupts();

	return;
}

void os_sem_signal(Semaphore *s)
{
	os_disable_interrupts();

	if(s->cur_value < s->max_value)
	{
		s->cur_value++;
	}

	os_enable_interrupts();

	return;
}