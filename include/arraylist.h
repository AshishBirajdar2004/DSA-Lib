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
 * @brief An opaque struct representing the ArrayList.
 *
 * The internal details are hidden to encapsulate the implementation.
 * Users should interact with it only through the provided API functions.
 */
typedef struct ArrayList ArrayList;

/**
 * @brief Initializes a new ArrayList.
 * @details Creates and allocates memory for a new ArrayList instance. It can be
 * initialized with a capacity of 0, in which case memory for elements
 * will be allocated upon the first insertion.
 * @param capacity The initial storage capacity of the list (number of elements).
 * @param dataSize The size in bytes of each element to be stored (e.g., sizeof(int)). Must be greater than 0.
 * @return A pointer to the newly created ArrayList, or NULL on allocation failure or invalid arguments.
 */
ArrayList* ArrayList_init(size_t capacity, size_t dataSize);

/**
 * @brief Frees all memory associated with the ArrayList.
 * @details Deallocates the internal data array and the ArrayList struct itself.
 * The pointer to the ArrayList becomes invalid after this call.
 * @param arrayList A pointer to the ArrayList to be destroyed.
 */
void ArrayList_destroy(ArrayList* arrayList);

/**
 * @brief Appends an element to the end of the ArrayList.
 * @details Copies the provided element data to the end of the list. If the list
 * is at full capacity, it will automatically expand its internal storage.
 * @param arrayList A pointer to the ArrayList.
 * @param element A pointer to the element data to be copied into the list.
 * @return STATUS_OK on success, or an error status on failure (e.g., invalid argument, allocation error).
 */
STATUS ArrayList_insert(ArrayList* arrayList, void* element);

/**
 * @brief Deletes an element at a specific index.
 * @details Removes the element at the given index and shifts all subsequent elements
 * to the left. The list may shrink its capacity if the size becomes
 * sufficiently small.
 * @param arrayList A pointer to the ArrayList.
 * @param index The zero-based index of the element to delete.
 * @return STATUS_OK on success, or an error status on failure (e.g., invalid index, underflow).
 */
STATUS ArrayList_delete(ArrayList* arrayList, size_t index);

/**
 * @brief Retrieves a copy of an element from a specific index.
 * @param arrayList A pointer to the ArrayList.
 * @param index The zero-based index of the element to retrieve.
 * @param dataOut A pointer to a memory location where the element's data will be copied.
 * The caller is responsible for ensuring this location is large enough.
 * @return STATUS_OK on success, or STATUS_ERR_INVALID_ARGUMENT on failure.
 */
STATUS ArrayList_get(ArrayList* arrayList, size_t index, void* dataOut);

/**
 * @brief Updates the element at a specific index with new data.
 * @param arrayList A pointer to the ArrayList.
 * @param index The zero-based index of the element to update.
 * @param element A pointer to the new element data that will overwrite the existing data at the index.
 * @return STATUS_OK on success, or STATUS_ERR_INVALID_ARGUMENT on failure.
 */
STATUS ArrayList_set(ArrayList* arrayList, size_t index, void* element);

/**
 * @brief Searches for an element in the list using a custom comparison function.
 * @details Performs a linear search on the list.
 * @param arr A pointer to the ArrayList.
 * @param key A pointer to the key to search for. The data it points to will be compared against list elements.
 * @param index A pointer to a size_t variable where the index of the first found element will be stored.
 * @param cmp A function pointer used to compare elements. It should take two const void* pointers
 * and return 0 if they are considered a match.
 * @return STATUS_OK if the key is found, STATUS_ERR_KEY_NOT_FOUND if not found, or STATUS_ERR_INVALID_ARGUMENT.
 */
STATUS ArrayList_search(ArrayList* arr, void* key, size_t* index, int (*cmp)(const void*, const void*));

/**
 * @brief Iterates over each element in the list and applies a callback function.
 * @param arrayList A pointer to the ArrayList.
 * @param callBack A function pointer that will be called for each element in the list. The callback
 * receives a pointer to the element's data within the list.
 * @return STATUS_OK on success, or STATUS_ERR_INVALID_ARGUMENT.
 */
STATUS ArrayList_forEach(ArrayList* arrayList, void (*callBack)(void*));

#endif