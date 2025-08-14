#include "../include/queue.h"

/**
 * @internal
 * @struct Queue
 * @brief Defines the internal structure of the Queue.
 */
struct Queue{
    LinkedList* list;   // A pointer to the underlying LinkedList used for storage.
};

Queue* Queue_init(size_t dataSize) 
{
    Queue* queue = malloc(sizeof(Queue));
    if (!queue) return NULL;

    queue->list = LinkedList_init(dataSize);
    if (!queue->list) {
        free(queue);
        return NULL;
    }

    return queue;
}

void Queue_destroy(Queue* queue) 
{
    if (!queue) return;
    LinkedList_destroy(queue->list);
    queue->list = NULL;
    free(queue);
}

bool Queue_isEmpty(Queue* queue) 
{
    if (!queue) return true;
    return LinkedList_size(queue->list) == 0;
}

STATUS Queue_enqueue(Queue* queue, void* element) 
{
    if (!queue || !element) return STATUS_ERR_INVALID_ARGUMENT;
    return LinkedList_insert(queue->list, element);
}

STATUS Queue_dequeue(Queue* queue) 
{
    if (!queue) return STATUS_ERR_INVALID_ARGUMENT;
    if (Queue_isEmpty(queue)) return STATUS_ERR_UNDERFLOW;

    // PERFORMANCE NOTE: Dequeue removes from the tail of the list.
    // In a singly-linked list without a tail pointer, this is an O(n) operation
    // as it requires traversing the entire list to find the last element.
    return LinkedList_delete(queue->list, LinkedList_size(queue->list));
}

STATUS Queue_peek(Queue* queue, void* elementOut) 
{
    if (!queue || !elementOut) return STATUS_ERR_INVALID_ARGUMENT;
    if (Queue_isEmpty(queue)) return STATUS_ERR_EMPTY;

    // PERFORMANCE NOTE: Peeking the front of the queue requires getting the tail
    // of the list. This is an O(n) operation in this implementation.
    return List_getAt(queue->list, LinkedList_size(queue->list), elementOut);
}
