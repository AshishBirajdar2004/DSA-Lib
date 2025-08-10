#ifndef STACK_H
#define STACK_H

#include "common.h"
#include "list.h"

/**
 * Stack - Generic stack built on top of linked list.
 * Holds underlying linked list and max stack capacity.
 */
typedef struct {
    List* list;         
    size_t stackSize;
} Stack;

/**
 * Stack_init - Initialize a new Stack.
 * @param dataSize: Size of each element in bytes.
 * @param stackSize: Maximum capacity of the stack.
 * @returns Pointer to new Stack, or NULL on failure.
 */
Stack* Stack_init(size_t dataSize, size_t stackSize);

/**
 * Stack_destroy - Free the Stack and its internal data.
 * @param stack: Pointer to the Stack.
 */
void Stack_destroy(Stack* stack);

/**
 * Stack_push - Push an element onto the stack.
 * @param stack: Pointer to the Stack.
 * @param data: Pointer to the element to push.
 * @returns STACK_OK or STACK_ERR.
 */
STATUS Stack_push(Stack* stack, void* data);

/**
 * Stack_pop - Remove the top element from the stack.
 * @param stack: Pointer to the Stack.
 * @returns STACK_OK or STACK_ERR.
 */
STATUS Stack_pop(Stack* stack);

/**
 * Stack_peek - Get the top element without removing it.
 * @param stack: Pointer to the Stack.
 * @param dataOut: Pointer to store the top element.
 * @returns STACK_OK or STACK_ERR.
 */
STATUS Stack_peek(Stack* stack, void* dataOut);

/**
 * Stack_isEmpty - Check if the stack is empty.
 * @param stack: Pointer to the Stack.
 * @returns non-zero if empty, zero if not.
 */
bool Stack_isEmpty(Stack* stack);

/**
 * Stack_isFull - Check if the stack is full.
 * @param stack: Pointer to the Stack.
 * @returns non-zero if full, zero if not.
 */
bool Stack_isFull(Stack* stack);

#endif /* STACK_H */
