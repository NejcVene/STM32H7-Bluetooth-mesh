/*
 * queue.c
 *
 *  Created on: Aug 7, 2024
 *      Author: Nejc
 */

#include "fsm_queue.h"
#include "freertos_os2.h"

/**
  * @brief  Create queue.
  * @param  None
  * @retval	Queue pointer
  */
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

/**
  * @brief  Insert data to queue.
  * @param  queue		Pointer to the used queue data structure.
  * @param  data		Void pointer to data which is to be inserted.
  * @param  dataSize	Size of data to be inserted.
  * @retval	None
  */
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

/**
  * @brief  Delete/get data from queue.
  * @param  queue	Pointer to the used queue data structure.
  * @retval	void pointer
  */
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

/**
  * @brief  Get data at the from of the queue.
  * @param  queue	Pointer to the used queue data structure.
  * @retval	void pointer
  */
void *peek(Queue *queue) {

	if (isEmpty(queue)) {
		return NULL;
	}

	return queue->front->data;

}

/**
  * @brief  Get data at the back of the queue.
  * @param  queue	Pointer to the used queue data structure.
  * @retval	void pointer
  */
void *rear(Queue *queue) {

	if (isEmpty(queue)) {
		return NULL;
	}

	return queue->end->data;

}

/**
  * @brief  Check if the queue is empty.
  * @note	1 -> is empty, 0 -> is not empty
  * @param  queue	Pointer to the used queue data structure.
  * @retval	int value
  */
int isEmpty(Queue *queue) {

	return queue->front == NULL;

}

/**
  * @brief  Delete all elements from the queue, then the queue itself.
  * @param  queue	Pointer to the used queue data structure.
  * @retval	None
  */
void deleteQueue(Queue *queue) {

	while (!isEmpty(queue)) {
		vPortFree(dequeue(queue));
	}
	vPortFree(queue);

}
