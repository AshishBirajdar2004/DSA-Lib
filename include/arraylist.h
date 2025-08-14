/**
 * @file arraylist.h
 * @brief A generic, dynamic array (ArrayList) implementation in C.
 *
 * This file defines the public API for a generic ArrayList data structure.
 * It supports storing elements of any data type, and automatically handles
 * memory management for resizing the internal array as elements are added
 * or removed.
 */

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include "common.h"

/**
 * @struct ArrayList
 * @brief An opaque struct representing the ArrayList data structure.
 *
 * The internal details are hidden to encapsulate the implementation.
 * Users should interact with the ArrayList only through the public API
 * functions defined in this file.
 */
typedef struct ArrayList ArrayList;

/**
 * @brief Initializes a new array list.
 * @details Creates and allocates memory for a new array list instance. It can be
 * initialized with a capacity of 0, in which case memory for elements
 * will be allocated upon the first insertion.
 * @param capacity The initial storage capacity of the array list (number of elements).
 * @param dataSize The size in bytes of each element to be stored (e.g., sizeof(int)).
 * @return A pointer to the newly created array list, or NULL on allocation failure or invalid arguments.
 */
ArrayList* ArrayList_init(size_t capacity, size_t dataSize);

/**
 * @brief Frees all memory associated with the array list.
 * @details Deallocates the internal data array and the array list struct itself.
 * The pointer to the array list becomes invalid after this call.
 * @param arrayList A pointer to the array list to be destroyed.
 */
void ArrayList_destroy(ArrayList* arrayList);

/**
 * @brief Appends an element to the end of the array list.
 * @details Copies the provided element data to the end of the list. If the list
 * is at full capacity, it will automatically expand its internal storage.
 * @param arrayList A pointer to the array list.
 * @param element A pointer to the element data to be copied into the array list.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if array list or element is NULL.
 * @return `STATUS_ERR_ALLOC` if memory allocation fails.
 */
STATUS ArrayList_insert(ArrayList* arrayList, void* element);

/**
 * @brief Deletes an element at a specific index.
 * @details Removes the element at the given index and shifts all subsequent elements
 * to the left. The list may shrink its capacity if the size becomes
 * sufficiently small.
 * @param arrayList A pointer to the array list.
 * @param index The zero-based index of the element to delete.
 * @return `STATUS_OK` on success.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if the index is out of bounds.
 * @return `STATUS_ERR_UNDERFLOW` if the array list is empty.
 */
STATUS ArrayList_delete(ArrayList* arrayList, size_t index);

/**
 * @brief Searches for an element using a custom comparison function.
 * @details Performs a linear search on the array list.
 * @param arr A pointer to the array list.
 * @param key A pointer to the key to search for. The data it points to will be compared against list elements.
 * @param index A pointer to a size_t variable where the index of the first found element will be stored.
 * @param cmp A function pointer used to compare elements. It should return 0 if they are considered a match.
 * @return `STATUS_OK` if the key is found.
 * @return `STATUS_ERR_KEY_NOT_FOUND` if the key is not found.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if any pointer arguments are NULL.
 */
STATUS ArrayList_search(ArrayList* arr, void* key, size_t* index, int (*cmp)(const void*, const void*));

/**
 * @brief Retrieves a copy of an element from a specific index.
 * @param arrayList A pointer to the array list.
 * @param index The zero-based index of the element to retrieve.
 * @param dataOut A pointer to a memory location where the element's data will be copied.
 * @return `STATUS_OK` on success, or `STATUS_ERR_INVALID_ARGUMENT` on failure or if the index is out of bounds.
 */
STATUS ArrayList_get(ArrayList* arrayList, size_t index, void* dataOut);

/**
 * @brief Updates the element at a specific index with new data.
 * @param arrayList A pointer to the array list.
 * @param index The zero-based index of the element to update.
 * @param element A pointer to the new element data that will overwrite the existing data at the index.
 * @return `STATUS_OK` on success, or `STATUS_ERR_INVALID_ARGUMENT` on failure or if the index is out of bounds.
 */
STATUS ArrayList_set(ArrayList* arrayList, size_t index, void* element);

/**
 * @brief Iterates over each element and applies a callback function.
 * @param arrayList A pointer to the array list.
 * @param callBack A function pointer that will be called for each element. The callback
 * receives a pointer to the element's data within the array list.
 * @return `STATUS_OK` on success, or `STATUS_ERR_INVALID_ARGUMENT` if array list or callback is NULL.
 */
STATUS ArrayList_forEach(ArrayList* arrayList, void (*callBack)(void*));

/**
 * @brief Returns the number of elements currently in the array list.
 * @param arrayList A pointer to the array list.
 * @return The number of elements as a `size_t`. Returns 0 if the array list is NULL.
 */
size_t ArrayList_size(const ArrayList* arrayList);

/**
 * @brief Returns the total number of elements the array list can hold before resizing.
 * @param arrayList A pointer to the array list.
 * @return The current capacity of the array list as a `size_t`. Returns 0 if the array list is NULL.
 */
size_t ArrayList_capacity(const ArrayList* arrayList);

#endif // ARRAYLIST_H