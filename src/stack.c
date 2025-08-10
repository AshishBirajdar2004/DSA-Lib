#include "../include/stack.h"

Stack* Stack_init(size_t dataSize, size_t stackSize) {
    Stack* stack = malloc(sizeof(Stack));
    if (!stack) return NULL;

    stack->list = List_init(dataSize);
    if (!stack->list) {
        free(stack);
        return NULL;
    }
    stack->stackSize = stackSize;
    return stack;
}

void Stack_destroy(Stack* stack) {
    if (!stack) return;
    List_destroy(stack->list);
    free(stack);
}

bool Stack_isEmpty(Stack* stack) {
    if (!stack) return true;
    return stack->list->length ==  0;
}

bool Stack_isFull(Stack* stack) {
    if (!stack) return false;
    return stack->list->length == stack->stackSize;
}

STATUS Stack_push(Stack* stack, void* data) {
    if (!stack || !data) return STATUS_ERR_INVALID_ARGUMENT;
    if (Stack_isFull(stack)) return STATUS_ERR_OVERFLOW;
    return List_insertAtHead(stack->list, data);
}

STATUS Stack_pop(Stack* stack) {
    if (!stack) return STATUS_ERR_INVALID_ARGUMENT;
    if (Stack_isEmpty(stack)) return STATUS_ERR_UNDERFLOW;
    return List_removeAt(stack->list, 1);
}

STATUS Stack_peek(Stack* stack, void* dataOut) {
    if (!stack || !dataOut) return STATUS_ERR_INVALID_ARGUMENT;
    if (Stack_isEmpty(stack)) return STATUS_ERR_EMPTY;
    return List_getAt(stack->list, 1, dataOut);
}