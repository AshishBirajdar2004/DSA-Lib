/**
 * @file queue.h
 * @brief Public API for a generic queue data structure.
 *
 * This file defines the interface for a generic queue data structure.
 * It is implemented as an adapter over the existing LinkedList module,
 * providing classic FIFO (First-In, First-Out) operations.
 */
#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"
#include "linkedlist.h"

/**
 * @struct Queue
 * @brief An opaque struct representing the Queue data structure.
 *
 * The internal details are hidden to encapsulate the implementation.
 * Users should interact with the Queue only through the public API functions defined in this file.
 */
typedef struct Queue Queue;

/**
 * @brief Initializes a new, empty queue.
 * @param dataSize The size in bytes of each element to be stored (e.g., `sizeof(int)`).
 * @return A pointer to the newly created Queue, or `NULL` on allocation failure.
 */
Queue* Queue_init(size_t dataSize);

/**
 * @brief Frees all memory associated with the queue.
 * @details Deallocates the underlying LinkedList and the Queue struct itself.
 * The queue pointer becomes invalid after this call.
 * @param queue A pointer to the queue to be destroyed.
 */
void Queue_destroy(Queue* queue);

/**
 * @brief Adds an element to the back of the queue.
 * @param queue A pointer to the queue.
 * @param data A pointer to the element data to be copied into the queue.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if queue or data is NULL.
 * @return `STATUS_ERR_ALLOC` if memory allocation fails.
 */
STATUS Queue_enqueue(Queue* queue, void* data);

/**
 * @brief Removes the element from the front of the queue.
 * @param queue A pointer to the queue.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if the queue is NULL.
 * @return `STATUS_ERR_UNDERFLOW` if the queue is empty.
 */
STATUS Queue_dequeue(Queue* queue);

/**
 * @brief Retrieves a copy of the front element without removing it.
 * @param queue A pointer to the queue.
 * @param dataOut A pointer to a memory location where the front element's data will be copied.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if queue or dataOut is NULL.
 * @return `STATUS_ERR_EMPTY` if the queue is empty.
 */
STATUS Queue_peek(Queue* queue, void* dataOut);

/**
 * @brief Checks if the queue is empty.
 * @param queue A pointer to the queue.
 * @return `true` if the queue has no elements, `false` otherwise. Returns `true` if queue is NULL.
 */
bool Queue_isEmpty(Queue* queue);

#endif /* QUEUE_H */
