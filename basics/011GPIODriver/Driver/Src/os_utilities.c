/*
 * os_utilities.c
 *
 *  Created on: 09-Feb-2024
 *      Author: naveen
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "os_utilities.h"

void os_queue_init(struct Queue *q)
{
	q->length = 0;
	q->first = NULL;
	q->last = NULL;

	return;
}

void os_enqueue(struct Queue *q,uint32_t value)
{

	struct QueueNode *p;

	p = (struct QueueNode *) malloc(sizeof(struct QueueNode));
	p->member = value;

	if(q->length == 0)					//First member of queue
	{
		p->prev = NULL;
		p->next = NULL;
		q->first = p;
		q->last = p;
		q->length++;
	}
	else
	{
		p->prev = NULL;
		p->next = q->first;
		q->first->prev = p;
		q->first = p;
		q->length++;
	}

	return;
}

uint32_t os_dequeue(struct Queue *q)
{
	uint32_t value = 0;
	struct QueueNode *p;

	if(q->last != NULL)
	{
		p = q->last;
		value = p->member;
		p->prev->next = NULL;

		q->length--;
		q->last = p->prev;

		free(p);
	}

	return value;
}

