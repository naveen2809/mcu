/*
 * os_kernel.h
 *
 *  Created on: 08-Feb-2024
 *      Author: naveen
 */

#ifndef INC_OS_KERNEL_H_
#define INC_OS_KERNEL_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>

struct TCB
{
	uint32_t current_sp;
	uint8_t priority;
	uint32_t sleep_count;
	uint32_t *task_stack;
	struct TCB *next;
};

#define SYT_RELOAD_VALUE 		0x0FFFFFU
#define SYT_CONTROL_VALUE 		0x6U

#define IDLE_TASK_STACK_SIZE	1024
#define IDLE_TASK_PRIORITY		254

typedef struct
{
	uint32_t cur_value;
	uint32_t max_value;
}Semaphore;

typedef uint8_t Mutex;

void configure_systick_timer(void);
void enable_systick_timer(void);
void SysTick_Handler(void);
void PendSV_Handler(void);
void idletask(void);
void os_create_task(void (*task)(void),uint8_t task_priority,uint32_t task_stack_size);
void os_start(void);
void update_sleep_status(void);
void task_sleep(uint32_t count);
void update_next_task(void);
void os_disable_interrupts(void);
void os_enable_interrupts(void);
void os_mutex_init(Mutex *m);
void os_mutex_wait(Mutex *m);
void os_mutex_signal(Mutex *m);
void os_sem_init(Semaphore *s,uint32_t init_value,uint32_t maximum_value);
void os_sem_wait(Semaphore *s);
void os_sem_signal(Semaphore *s);
void os_yield(void);

#endif /* INC_OS_KERNEL_H_ */
