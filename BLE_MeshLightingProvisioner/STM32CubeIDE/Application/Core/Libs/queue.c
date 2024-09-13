/*
 * queue.c
 *
 *  Created on: Aug 7, 2024
 *      Author: Nejc
 */

#include "queue.h"

Queue *createQueue(void) {

	Queue *queue;
	if (!(queue = (Queue *) malloc(sizeof(Queue)))) {
		// raise error
	}
	queue->front = NULL;
	queue->end = NULL;
	queue->queueSize = 0;

	return queue;

}

void enqueue(Queue *queue, void *data, size_t dataSize) {

	Node *tmp;
	if (!(tmp = (Node *) malloc(sizeof(Node)))) {
		// raise error
	}
	if (!(tmp->data = malloc(dataSize))) {
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
	free(tmp);
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
		free(dequeue(queue));
	}
	free(queue);

}
