/*
 * os_utilities.h
 *
 *  Created on: 09-Feb-2024
 *      Author: naveen
 */

#ifndef INC_OS_UTILITIES_H_
#define INC_OS_UTILITIES_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>

struct QueueNode
{
	uint32_t member;
	struct QueueNode *prev;
	struct QueueNode *next;
};

struct Queue
{
	uint32_t length;
	struct QueueNode *first;
	struct QueueNode *last;
};

void os_queue_init(struct Queue *q);
void os_enqueue(struct Queue *q,uint32_t value);
uint32_t os_dequeue(struct Queue *q);

typedef struct Queue os_queue;

#endif /* INC_OS_UTILITIES_H_ */
