/*
 * queue.h
 *
 *  Created on: Aug 7, 2024
 *      Author: Nejc
 */

#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
	void *data;
	struct Node *nextNode;
} Node;

typedef struct {
	Node *front, *end;
	size_t queueSize;
} Queue;

Queue *createQueue(void);
void enqueue(Queue *queue, void *data, size_t dataSize);
void *dequeue(Queue *queue);
void *peek(Queue *queue);
void *rear(Queue *queue);
void deleteQueue(Queue *queue);
int isEmpty(Queue *queue);

#endif /* INC_QUEUE_H_ */
