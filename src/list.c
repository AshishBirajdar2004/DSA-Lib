#include "../include/list.h"

List* List_init(size_t dataSize)
{
    List* list = (List*)malloc(sizeof(List));
    list->head = NULL;
    list->dataSize = dataSize;
    list->length = 0;
    return list; 
}

void List_destroy(List* list)
{
    if (!list) return;

    ListNode* current;
    while (list->head != NULL)
    {
        current = list->head;
        list->head = list->head->next;
        free(current->data);
        free(current);
    }
    free(list);
}

static ListNode* _List_getNewNode(List* list, void* data)
{
    ListNode* newNode = (ListNode* )malloc(sizeof(ListNode));
    if (!newNode) return NULL;

    newNode->data = malloc(list->dataSize);
    if (!newNode->data)
    {
        free(newNode);
        return NULL;
    }

    memcpy(newNode->data, data, list->dataSize);
    newNode->next = NULL;
    return newNode;
}

STATUS List_insertAtHead(List* list, void* data)
{
    if (!list || !data) return STATUS_ERR_INVALID_ARGUMENT;

    ListNode* newNode = _List_getNewNode(list, data);
    if (!newNode) return STATUS_ERR_ALLOC;

    newNode->next = list->head;
    list->head = newNode;
    list->length++;
    return STATUS_OK;
}

STATUS List_removeAt(List* list, size_t index)
{
    if (!list || !list->head || index < 1 || index > list->length)
        return STATUS_ERR_INVALID_ARGUMENT;

    ListNode* temp1 = list->head;

    if (index == 1)
    {
        list->head = temp1->next;
        free(temp1->data);
        free(temp1);
        list->length--;
        return STATUS_OK;
    }

    for (size_t i = 1; i < index - 1; i++)
        temp1 = temp1->next;

    ListNode* temp2 = temp1->next;
    temp1->next = temp2->next;
    free(temp2->data);
    free(temp2);
    list->length--;
    return STATUS_OK;
}

size_t List_search(List* list, void* key, int (*cmp)(void*, void*)) {
    if (!list || !list->head || !key || !cmp) return 0;

    ListNode* current = list->head;
    size_t index = 1;

    while (current) {
        if (cmp(current->data, key) == 0) {
            return index; // Found!
        }
        current = current->next;
        index++;
    }

    return 0; // Not found
}

STATUS List_getAt(List* list, size_t index, void* dataOut) {
    if (!list || !list->head || !dataOut) 
        return STATUS_ERR_INVALID_ARGUMENT;
    if (index < 1 || index > list->length) 
        return STATUS_ERR_INVALID_ARGUMENT;

    ListNode* temp = list->head;
    for (size_t i = 1; i < index; i++)
        temp = temp->next;

    memcpy(dataOut, temp->data, list->dataSize);
    return STATUS_OK;
}

STATUS List_forEach(List* list, void (*callback)(void*)) 
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