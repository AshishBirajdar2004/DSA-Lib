/**
 * @file linkedlist.h
 * @brief Public API for a generic, singly-linked list data structure.
 *
 * This file defines the interface for a generic linked list. It supports
 * storing elements of any data type by copying them into new nodes.
 * Note that this implementation uses 1-based indexing for all operations.
 */
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "common.h"

/**
 * @struct LinkedList
 * @brief An opaque struct representing the LinkedList data structure.
 *
 * The internal details are hidden to encapsulate the implementation.
 * Users should interact with the LinkedList only through the public API
 * functions defined in this file.
 */
typedef struct LinkedList LinkedList;

/**
 * @brief Initializes a new, empty linked list.
 * @param dataSize The size in bytes of each element to be stored (e.g., `sizeof(int)`).
 * @return A pointer to the newly created linked list, or `NULL` on allocation failure.
 */
LinkedList* LinkedList_init(size_t dataSize);

/**
 * @brief Frees all memory associated with the linked list.
 * @details Deallocates each node, the data within each node, and the linked list
 * structure itself. The linked list pointer becomes invalid after this call.
 * @param linkedlist A pointer to the linked list to be destroyed.
 */
void LinkedList_destroy(LinkedList* linkedlist);

/**
 * @brief Inserts an element at the beginning of the linked list.
 * @param linkedlist A pointer to the linked list.
 * @param element A pointer to the element data to be copied into the new node.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if linked list or element is NULL.
 * @return `STATUS_ERR_ALLOC` if memory allocation for the new node fails.
 * @return `STATUS_ERR_OVERFLOW` if the linked list is grows upto large extent.
 */
STATUS LinkedList_insert(LinkedList* linkedlist, void* element);

/**
 * @brief Deletes an element at a specific 1-based index.
 * @param linkedlist A pointer to the linked list.
 * @param index The 1-based index of the element to delete.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if the index is out of bounds.
 * @return `STATUS_ERR_UNDERFLOW` if the linked list is empty.
 */
STATUS LinkedList_delete(LinkedList* linkedlist, size_t index);

/**
 * @brief Searches for an element using a custom comparison function.
 * @details Performs a linear search from the head of the linked list.
 * @param linkedlist A pointer to the linked list.
 * @param key A pointer to the key to search for. The data it points to will be compared against list elements.
 * @param index A pointer to a size_t variable where the 1-based index of the first found element will be stored.
 * @param cmp A function pointer to compare elements. It should return 0 if they are considered a match.
 * @return `STATUS_OK` if the key is found.
 * @return `STATUS_ERR_KEY_NOT_FOUND` if the key is not found.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if any pointer arguments are NULL.
 */
STATUS LinkedList_search(LinkedList* linkedlist, void* key, size_t* index, int (*cmp)(void*, void*));

/**
 * @brief Retrieves a copy of an element from a specific 1-based index.
 * @param linkedlist A pointer to the linked list.
 * @param index The 1-based index of the element to retrieve.
 * @param elementOut A pointer to a memory location where the element's data will be copied.
 * @return `STATUS_OK` on success, or `STATUS_ERR_INVALID_ARGUMENT` on failure or if the index is out of bounds.
 */
STATUS LinkedList_get(LinkedList* linkedlist, size_t index, void* elementOut);

/**
 * @brief Updates the element at a specific 1-based index with new data.
 * @param linkedlist A pointer to the linked list.
 * @param index The 1-based index of the element to update.
 * @param element A pointer to the new element data that will overwrite the existing data at the index.
 * @return `STATUS_OK` on success, or `STATUS_ERR_INVALID_ARGUMENT` on failure or if the index is out of bounds.
 */
STATUS LinkedList_set(LinkedList* linkedlist, size_t index, void* element);

/**
 * @brief Iterates over each element and applies a callback function.
 * @param linkedlist A pointer to the linked list.
 * @param callback A function pointer that will be called for each element. The
 * callback receives a pointer to the element's data within the linked list.
 * @return `STATUS_OK` on success, or `STATUS_ERR_INVALID_ARGUMENT` if linked list or callback is NULL.
 */
STATUS LinkedList_forEach(LinkedList* linkedlist, void (*callback)(void*));

/**
 * @brief Returns the number of elements currently in the linked list.
 * @param linkedlist A constant pointer to the linked list.
 * @return The number of elements as a `size_t`. Returns 0 if the linked list is NULL.
 */
size_t LinkedList_size(const LinkedList* linkedlist);

#endif // LINKEDLIST_H