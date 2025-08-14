#include "../include/stack.h"

/**
 * @internal
 * @struct Stack
 * @brief Defines the internal structure of the Stack.
 */
struct Stack {
    LinkedList* list;   // A pointer to the underlying LinkedList used for storage.   
    size_t stackSize;   // The maximum capacity of the stack.
};

Stack* Stack_init(size_t dataSize, size_t stackSize) 
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) return NULL;

    stack->list = LinkedList_init(dataSize);
    if (!stack->list) {
        free(stack);
        return NULL;
    }
    stack->stackSize = stackSize;
    return stack;
}

void Stack_destroy(Stack* stack) 
{
    if (!stack) return;
    LinkedList_destroy(stack->list);
    stack->list = NULL;
    free(stack);
}

bool Stack_isEmpty(Stack* stack) 
{
    if (!stack) return true;
    return LinkedList_size(stack->list) ==  0;
}

bool Stack_isFull(Stack* stack) 
{
    if (!stack) return false;
    return LinkedList_size(stack->list) == stack->stackSize;
}

STATUS Stack_push(Stack* stack, void* element) 
{
    if (!stack || !element) return STATUS_ERR_INVALID_ARGUMENT;
    if (Stack_isFull(stack)) return STATUS_ERR_OVERFLOW;

    // Pushing onto the stack is equivalent to inserting at the head of the list.
    return LinkedList_insert(stack->list, element);
}

STATUS Stack_pop(Stack* stack) 
{
    if (!stack) return STATUS_ERR_INVALID_ARGUMENT;
    if (Stack_isEmpty(stack)) return STATUS_ERR_UNDERFLOW;

    // Popping from the stack is equivalent to deleting the head of the list (index 1).
    return LinkedList_delete(stack->list, 1);
}

STATUS Stack_peek(Stack* stack, void* elementOut) 
{
    if (!stack || !elementOut) return STATUS_ERR_INVALID_ARGUMENT;
    if (Stack_isEmpty(stack)) return STATUS_ERR_EMPTY;

    // Peeking the stack is equivalent to getting the head of the list (index 1).
    return LinkedList_get(stack->list, 1, elementOut);
}