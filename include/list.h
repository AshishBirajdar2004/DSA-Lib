#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define LIST_OK 1
#define LIST_ERR 0

/**
 * ListNode structure for the linked list.
 * Holds a pointer to data and the next node.
 */
typedef struct ListNode
{
    void* data;
    struct ListNode* next;
} ListNode;

/**
 * List structure.
 * Holds pointer to head, size of each data element, and current length.
 */
typedef struct
{
    ListNode* head;
    size_t dataSize;
    size_t length;
} List;

/* === FUNCTION DECLARATIONS === */

/**
 * Initialize a new list.
 * @param dataSize Size of each data element.
 * @return Pointer to the List struct.
 */
List* List_init(size_t dataSize);

/**
 * Destroy the entire list and free memory.
 * @param list Pointer to the List.
 */
void List_destroy(List* list);

/**
 * Insert a new node at the head of the list.
 * @param list Pointer to the List.
 * @param data Pointer to data to insert.
 * @return LIST_OK or LIST_ERR.
 */
int List_insertAtHead(List* list, void* data);

/**
 * Delete a node from a given index (1-based index).
 * @param list Pointer to the List.
 * @param index Position to delete.
 * @return LIST_OK or LIST_ERR.
 */
int List_removeAt(List* list, size_t index);

/**
 * Search for an element in the list (1-based index).
 * @param list Pointer to the List.
 * @param key Pointer to the key/value to search for.
 * @param cmp Comparator function pointer. Should return 0 if two elements are equal.
 * @return The 1-based index of the first matching element if found, else 0.
 * @
 */
size_t List_search(List* list, void* key, int (*cmp)(void*, void*));


/**
 * Retrieve the data stored at a given index in the list (1-based index).
 * @param list Pointer to the List.
 * @param index Position of the element to retrieve.
 * @param dataOut Pointer to a buffer where the element's data will be copied.
 * @return LIST_OK or LIST_ERR.
 */
int List_getAt(List* list, size_t index, void* dataOut);

/**
 * Apply a callback function to each node's data.
 * @param list Pointer to the List.
 * @param callback Function to run for each node's data.
 */
void List_forEach(List* list, void (*callback)(void*));

#endif