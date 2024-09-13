/*
 * queue.c
 *
 *  Created on: Aug 7, 2024
 *      Author: Nejc
 */

#include "fsm_queue.h"
#include "freertos_os2.h"

Queue *createQueue(void) {

	Queue *queue;
	if (!(queue = (Queue *) pvPortMalloc(sizeof(Queue)))) {
		// raise error
	}
	queue->front = NULL;
	queue->end = NULL;
	queue->queueSize = 0;

	return queue;

}

void enqueue(Queue *queue, void *data, size_t dataSize) {

	Node *tmp;
	if (!(tmp = (Node *) pvPortMalloc(sizeof(Node)))) {
		// raise error
	}
	if (!(tmp->data = pvPortMalloc(dataSize))) {
		// raise error
	}
	memcpy(tmp->data, data, dataSize);
	tmp->nextNode = NULL;
	if (queue->end == NULL) {
		queue->front = queue->end = tmp;
	} else {
		queue->end->nextNode = tmp;
		queue->end = tmp;
	}
	queue->queueSize++;

}

void *dequeue(Queue *queue) {

	if (isEmpty(queue)) {
		return NULL;
	}
	Node *tmp = queue->front;
	void *data = tmp->data;
	queue->front = queue->front->nextNode;
	if (queue->front == NULL) {
		queue->end = NULL;
	}
	vPortFree(tmp);
	queue->queueSize--;

	return data;

}

void *peek(Queue *queue) {

	if (isEmpty(queue)) {
		return NULL;
	}

	return queue->front->data;

}

void *rear(Queue *queue) {

	if (isEmpty(queue)) {
		return NULL;
	}

	return queue->end->data;

}

int isEmpty(Queue *queue) {

	return queue->front == NULL;

}

void deleteQueue(Queue *queue) {

	while (!isEmpty(queue)) {
		vPortFree(dequeue(queue));
	}
	vPortFree(queue);

}
