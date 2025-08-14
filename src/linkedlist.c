#include "../include/linkedlist.h"

/**
 * @internal
 * @struct ListNode
 * @brief Defines the internal structure of a single node in the LinkedList.
 */
typedef struct ListNode
{
    void* data;             // Pointer to the stored data, which is heap-allocated.
    struct ListNode* next;  // Pointer to the next node in the list.
} ListNode;

/**
 * @internal
 * @struct LinkedList
 * @brief Defines the internal structure of the LinkedList.
 */
struct LinkedList
{
    ListNode* head;     // Pointer to the first node in the list.
    size_t dataSize;    // The size in bytes of the data stored in each node.
    size_t size;        // The current number of nodes in the list.
};

/* --------------------------- Private Helper Functions --------------------------- */

/**
 * @internal
 * @brief Creates and allocates a new linked list node.
 * @details This function allocates memory for the node structure itself and for
 * the data it contains, then copies the provided element data into the new node.
 * @param list The list instance, used to determine the data size.
 * @param element A pointer to the element data to be copied.
 * @return A pointer to the newly created ListNode on success, or `NULL` on memory allocation failure.
 */
static ListNode* _LinkedList_getNewNode(LinkedList* list, void* element)
{
    ListNode* newNode = (ListNode* )malloc(sizeof(ListNode));
    if (!newNode) return NULL;

    newNode->data = malloc(list->dataSize);
    if (!newNode->data)
    {
        free(newNode);
        return NULL;
    }

    memcpy(newNode->data, element, list->dataSize);
    newNode->next = NULL;
    return newNode;
}

/* ----------------------------- Public API Functions ----------------------------- */

LinkedList* LinkedList_init(size_t dataSize)
{
    if (dataSize == 0) return NULL;

    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    if (!list) return NULL;

    list->head = NULL;
    list->dataSize = dataSize;
    list->size = 0;
    return list;
}

void LinkedList_destroy(LinkedList* list)
{
    if (!list) return;

    ListNode* current;
    while (list->head != NULL)
    {
        current = list->head;
        list->head = list->head->next;
        free(current->data);
        current->data = NULL;
        free(current);
        current = NULL;
    }
    free(list);
}

STATUS LinkedList_insert(LinkedList* list, void* element)
{
    if (!list || !element) return STATUS_ERR_INVALID_ARGUMENT;

    if (list->size > SIZE_MAX / 2)
        return STATUS_ERR_OVERFLOW; // Cannot grow further.

    ListNode* newNode = _LinkedList_getNewNode(list, element);
    if (!newNode) return STATUS_ERR_ALLOC;

    // New node becomes the new head.
    newNode->next = list->head;
    list->head = newNode;
    list->size++;
    return STATUS_OK;
}

STATUS LinkedList_delete(LinkedList* list, size_t index)
{
    if (!list || index < 1 || index > list->size)
        return STATUS_ERR_INVALID_ARGUMENT;

    if (list->size == 0) return STATUS_ERR_UNDERFLOW;

    ListNode* temp1 = list->head;
    ListNode* temp2 = NULL;

    // Case for deleting the head node.
    if (index == 1)
    {
        list->head = temp1->next;
        free(temp1->data);
        free(temp1);
        list->size--;
        return STATUS_OK;
    }

    // Traverse to the node just before the one to be deleted.
    for (size_t i = 1; i < index - 1; i++)
        temp1 = temp1->next;

    // temp2 is the node to be deleted.
    temp2 = temp1->next;
    temp1->next = temp2->next;
    free(temp2->data);
    free(temp2);
    list->size--;
    return STATUS_OK;
}

STATUS LinkedList_search(LinkedList* list, void* key, size_t* index, int (*cmp)(void*, void*)){
    if (!list || !key || !index || !cmp)
        return STATUS_ERR_INVALID_ARGUMENT;

    ListNode* current = list->head;
    size_t i = 1;

    while (current) {
        if (cmp(current->data, key) == 0) {
            *index = i;
            return STATUS_OK;
        }
        current = current->next;
        i++;
    }

    return STATUS_ERR_KEY_NOT_FOUND;
}

STATUS LinkedList_get(LinkedList* list, size_t index, void* elementOut)
{
    if (!list || !elementOut || index < 1 || index > list->size)
        return STATUS_ERR_INVALID_ARGUMENT;

    ListNode* temp = list->head;
    for (size_t i = 1; i < index; i++)
        temp = temp->next;

    memcpy(elementOut, temp->data, list->dataSize);
    return STATUS_OK;
}

STATUS LinkedList_set(LinkedList* list, size_t index, void* element)
{
    if (!list || !element || index < 1 || index > list->size)
        return STATUS_ERR_INVALID_ARGUMENT;

    ListNode* temp = list->head;
    for (size_t i = 1; i < index; i++)
        temp = temp->next;

    memcpy(temp->data, element, list->dataSize);
    return STATUS_OK;
}

STATUS LinkedList_forEach(LinkedList* list, void (*callback)(void*))
{
    if (!list || !callback) return STATUS_ERR_INVALID_ARGUMENT;

    ListNode* current = list->head;
    while (current != NULL)
    {
        callback(current->data);
        current = current->next;
    }

    return STATUS_OK;
}

size_t LinkedList_size(const LinkedList* list)
{
    if (!list) {
        return 0;
    }
    return list->size;
}