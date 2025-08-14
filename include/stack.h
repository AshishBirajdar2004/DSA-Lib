/**
 * @file stack.h
 * @brief Public API for a generic, bounded stack data structure.
 *
 * This file defines the interface for a generic stack with a fixed capacity.
 * It is implemented as an adapter over the existing LinkedList data structure,
 * providing classic LIFO (Last-In, First-Out) operations.
 */
#ifndef STACK_H
#define STACK_H

#include "common.h"
#include "linkedlist.h"

/**
 * @struct Stack
 * @brief An opaque struct representing the Stack data structure.
 *
 * The internal details are hidden to encapsulate the implementation.
 * Users should interact with the Stack only through the public API functions defined in this file.
 */
typedef struct Stack Stack;

/**
 * @brief Initializes a new, empty stack with a specified maximum size.
 * @param dataSize The size in bytes of each element to be stored (e.g., `sizeof(int)`).
 * @param stackSize The maximum number of elements the stack can hold.
 * @return A pointer to the newly created Stack, or `NULL` on allocation failure.
 */
Stack* Stack_init(size_t dataSize, size_t stackSize);

/**
 * @brief Frees all memory associated with the stack.
 * @details Deallocates the underlying LinkedList and the Stack struct itself.
 * The stack pointer becomes invalid after this call.
 * @param stack A pointer to the stack to be destroyed.
 */
void Stack_destroy(Stack* stack);

/**
 * @brief Adds an element to the top of the stack.
 * @param stack A pointer to the stack.
 * @param element A pointer to the element data to be copied onto the stack.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if stack or data is NULL.
 * @return `STATUS_ERR_ALLOC` if memory allocation for the LinkedList fails.
 * @return `STATUS_ERR_OVERFLOW` if the stack is full.
 */
STATUS Stack_push(Stack* stack, void* element);

/**
 * @brief Removes the top element from the stack.
 * @param stack A pointer to the stack.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if the stack is NULL.
 * @return `STATUS_ERR_UNDERFLOW` if the stack is empty.
 */
STATUS Stack_pop(Stack* stack);

/**
 * @brief Retrieves a copy of the top element without removing it.
 * @param stack A pointer to the stack.
 * @param dataOut A pointer to a memory location where the top element's data will be copied.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if stack or dataOut is NULL.
 * @return `STATUS_ERR_EMPTY` if the stack is empty.
 */
STATUS Stack_peek(Stack* stack, void* dataOut);

/**
 * @brief Checks if the stack is empty.
 * @param stack A pointer to the stack.
 * @return `true` if the stack has no elements, `false` otherwise. Returns `true` if stack is NULL.
 */
bool Stack_isEmpty(Stack* stack);

/**
 * @brief Checks if the stack has reached its maximum capacity.
 * @param stack A pointer to the stack.
 * @return `true` if the number of elements equals the stack's maximum size, `false` otherwise.
 */
bool Stack_isFull(Stack* stack);

#endif /* STACK_H */
