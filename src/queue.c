#include <stdio.h>
#include "../include/queue.h"

Queue* Queue_init(size_t dataSize) {
    Queue* queue = malloc(sizeof(Queue));
    if (!queue) return NULL;

    queue->list = List_init(dataSize);
    if (!queue->list) {
        free(queue);
        return NULL;
    }

    return queue;
}

void Queue_destroy(Queue* queue) {
    if (!queue) return;
    List_destroy(queue->list);
    free(queue);
}

int Queue_isEmpty(Queue* queue) {
    if (!queue) return 1;
    return queue->list->length == 0;
}

int Queue_enqueue(Queue* queue, void* data) {
    if (!queue || !data) return QUEUE_ERR;
    return List_insertAtHead(queue->list, data);
}

int Queue_dequeue(Queue* queue) {
    if (!queue) return QUEUE_ERR;
    if (Queue_isEmpty(queue)) return QUEUE_ERR;
    return List_removeAt(queue->list, queue->list->length);
}

int Queue_peek(Queue* queue, void* dataOut) {
    if (!queue || !dataOut) return QUEUE_ERR;
    if (Queue_isEmpty(queue)) return QUEUE_ERR;
    return List_getAt(queue->list, queue->list->length, dataOut);
}
