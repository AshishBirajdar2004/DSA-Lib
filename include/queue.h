#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>
#include <stdlib.h>
#include "list.h"

#define QUEUE_OK 1
#define QUEUE_ERR 0

/**
 * Queue - Generic FIFO queue built on top of linked list.
 * Holds underlying linked list.
 */
typedef struct {
    List* list;
} Queue;

/**
 * Queue_init - Initialize a new Queue.
 * @param dataSize: Size of each element in bytes.
 * @returns Pointer to new Queue or NULL on failure.
 */
Queue* Queue_init(size_t dataSize);

/**
 * Queue_destroy - Free the Queue and its internal data.
 * @param queue: Pointer to the Queue.
 */
void Queue_destroy(Queue* queue);

/**
 * Queue_enqueue - Add an element to the rear of the Queue.
 * @param queue: Pointer to the Queue.
 * @param data: Pointer to the element to add.
 * @returns QUEUE_OK or QUEUE_ERR.
 */
int Queue_enqueue(Queue* queue, void* data);

/**
 * Queue_dequeue - Remove the front element from the Queue.
 * @param queue: Pointer to the Queue.
 * @returns QUEUE_OK or QUEUE_ERR.
 */
int Queue_dequeue(Queue* queue);

/**
 * Queue_peek - Get the front element without removing it.
 * @param queue: Pointer to the Queue.
 * @param dataOut: Pointer to store the front element.
 * @returns QUEUE_OK or QUEUE_ERR.
 */
int Queue_peek(Queue* queue, void* dataOut);

/**
 * Queue_isEmpty - Check if the Queue is empty.
 * @param queue: Pointer to the Queue.
 * @returns non-zero if empty, zero if not.
 */
int Queue_isEmpty(Queue* queue);

#endif /* QUEUE_H */
