/*
 * os_kernel.h
 *
 *  Created on: 08-Feb-2024
 *      Author: naveen
 */

#ifndef INC_OS_KERNEL_H_
#define INC_OS_KERNEL_H_

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

#endif /* INC_OS_KERNEL_H_ */
