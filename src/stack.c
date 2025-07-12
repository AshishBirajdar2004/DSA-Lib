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

int Stack_isEmpty(Stack* stack) {
    if (!stack) return 1;
    return stack->list->length ==  0;
}

int Stack_isFull(Stack* stack) {
    if (!stack) return STACK_ERR;
    return stack->list->length == stack->stackSize;
}

int Stack_push(Stack* stack, void* data) {
    if (!stack || !data) return STACK_ERR;
    if (Stack_isFull(stack)) return STACK_ERR;
    return List_insertAtHead(stack->list, data);
}

int Stack_pop(Stack* stack) {
    if (!stack) return STACK_ERR;
    if (Stack_isEmpty(stack)) return STACK_ERR;
    return List_removeAt(stack->list, 1);
}

int Stack_peek(Stack* stack, void* dataOut) {
    if (!stack || !dataOut) return STACK_ERR;
    if (Stack_isEmpty(stack)) return STACK_ERR;
    return List_getAt(stack->list, 1, dataOut);
}