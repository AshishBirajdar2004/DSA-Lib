/**
 * @file heap.h
 * @brief Public API for a generic binary heap data structure.
 *
 * This file defines the interface for a generic heap. The implementation is
 * built upon a dynamic ArrayList, allowing it to grow as needed. The heap's
 * behavior (min-heap or max-heap) is determined by the comparison function
 * provided during initialization.
 */
#ifndef HEAP_H
#define HEAP_H

#include "common.h"
#include "arraylist.h"

/**
 * @struct Heap
 * @brief An opaque struct representing the Heap data structure.
 *
 * The internal details are hidden to encapsulate the implementation.
 * Users should interact with the Heap only through the public API functions
 * defined in this file.
 */
typedef struct Heap Heap;

/**
 * @brief Initializes a new heap instance.
 * @details This function allocates and sets up a new heap. The ordering of the
 * heap is controlled by the `cmp` function.
 *
 * @param capacity The initial storage capacity of the heap (number of elements).
 * Can be 0, in which case storage is allocated on the first push.
 * @param dataSize The size in bytes of each element to be stored (e.g., `sizeof(int)`).
 * @param cmp A function pointer for comparing two elements.
 * - For a **min-heap**, it should return a negative value if the first element is smaller.
 * - For a **max-heap**, it should return a negative value if the second element is smaller.
 * Example (min-heap for ints): `return *(int*)a - *(int*)b;`
 * @return A pointer to the newly created Heap, or `NULL` on allocation failure or invalid arguments.
 */
Heap* Heap_init(size_t capacity, size_t dataSize, int (*cmp)(const void* a, const void* b));

/**
 * @brief Frees all memory associated with the heap.
 * @details Deallocates the underlying ArrayList and the Heap struct itself.
 * The heap pointer becomes invalid after this call.
 * @param heap A pointer to the heap to be destroyed.
 */
void Heap_destroy(Heap* heap);

/**
 * @brief Adds a new element to the heap, maintaining the heap property.
 * @details The element is added to the end and then "sifted up" to its correct
 * position to ensure the heap order is preserved.
 *
 * @param heap A pointer to the heap.
 * @param element A pointer to the element data to be copied into the heap.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if heap or element is NULL.
 * @return `STATUS_ERR_ALLOC` if memory allocation fails.
 */
STATUS Heap_push(Heap* heap, void* element);

/**
 * @brief Removes the root element from the heap.
 * @details The root element (the minimum for a min-heap, or maximum for a max-heap)
 * is copied to `elementOut`. The heap is then re-ordered to maintain the heap property.
 *
 * @param heap A pointer to the heap.
 * @param elementOut A pointer to a memory location where the popped element's data will be copied.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if heap or elementOut is NULL.
 * @return `STATUS_ERR_UNDERFLOW` if the heap is empty.
 */
STATUS Heap_pop(Heap* heap, void* elementOut);

/**
 * @brief Retrieves a copy of the root element without removing it.
 * @param heap A constant pointer to the heap.
 * @param elementOut A pointer to a memory location where the root element's data will be copied.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if heap or elementOut is NULL.
 * @return `STATUS_ERR_EMPTY` if the heap is empty.
 */
STATUS Heap_peek(const Heap* heap, void* elementOut);

/**
 * @brief Returns the current number of elements in the heap.
 * @param heap A constant pointer to the heap.
 * @return The number of elements as a `size_t`. Returns 0 if the heap is NULL.
 */
size_t Heap_size(const Heap* heap);

#endif