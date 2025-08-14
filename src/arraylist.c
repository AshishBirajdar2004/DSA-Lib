#include "../include/arraylist.h"

/**
 * @brief The internal structure of the ArrayList.
 * @details This struct holds all the necessary state for the list, including
 * its size, capacity, the size of the data type it holds, and a void pointer
 * to the actual block of memory where elements are stored.
 */
struct ArrayList
{
    size_t capacity; // The total number of elements the list can currently hold.
    size_t dataSize; // The size of a single element in bytes (e.g., sizeof(int)).
    size_t size;     // The current number of elements in the list.
    void* data;      // A void pointer to the contiguous block of memory for the elements.
};

/** @internal Constants */

/**
 * @brief The default initial capacity for a list created with a capacity of 0.
 * This prevents frequent reallocations when starting with an empty list.
 */
#define DEFAULT_CAPACITY 8

/**
 * @brief The factor by which the list's capacity will grow when it's full.
 * A factor of 2 is a common choice that provides good amortized performance.
 */
#define DEFAULT_EXPANSION_FACTOR 2

/* --------------------------- Private Helper Functions --------------------------- */

/**
 * @internal
 * @brief Internal helper to reallocate the data buffer of the ArrayList.
 * @param arrayList A pointer to the ArrayList.
 * @param newCapacity The desired new capacity.
 * @return `true` on successful reallocation, `false` otherwise.
 */
static bool _ArrayList_realloc(ArrayList* arrayList, size_t newCapacity)
{
    void* newData = realloc(arrayList->data, newCapacity * arrayList->dataSize);
    if (!newData)
        return false; // Reallocation failed; the original block is still valid.

    // On success, update the data pointer and the capacity.
    arrayList->data = newData;
    arrayList->capacity = newCapacity;
    return true;
}

/* ----------------------------- Public API Functions ----------------------------- */

ArrayList* ArrayList_init(size_t capacity, size_t dataSize)
{
    if (dataSize == 0)
        return NULL;

    ArrayList* arrayList = (ArrayList* )malloc(sizeof(ArrayList));
    if (!arrayList)
        return NULL; // Allocation failed.

    arrayList->capacity = capacity;
    arrayList->dataSize = dataSize;
    arrayList->size = 0;
    arrayList->data = NULL; // Initialize data pointer to NULL.

    // If the user requests an initial capacity, allocate the data block now.
    if (capacity > 0) {
        arrayList->data = malloc(arrayList->dataSize * capacity);
        if (!arrayList->data)
        {
            // If data allocation fails, we must clean up the struct we just allocated.
            free(arrayList);
            arrayList = NULL;
            return NULL;
        }
    }
    // If capacity is 0, data remains NULL. Allocation is deferred until the first insert.

    return arrayList;
}

void ArrayList_destroy(ArrayList* arrayList)
{
    if (!arrayList)
        return;
    
    free(arrayList->data);
    arrayList->data = NULL;

    free(arrayList);
}

STATUS ArrayList_insert(ArrayList* arrayList, void* element)
{
    if (!arrayList || !element)
        return STATUS_ERR_INVALID_ARGUMENT;

    // Check if the list is full and needs to be expanded.
    if (arrayList->size >= arrayList->capacity)
    {
        // Check for potential integer overflow before multiplication.
        if (arrayList->capacity > SIZE_MAX / DEFAULT_EXPANSION_FACTOR)
            return STATUS_ERR_OVERFLOW; // Cannot grow further.

        // Determine the new capacity. If current capacity is 0, use the default.
        size_t newCapacity = arrayList->capacity == 0
            ? DEFAULT_CAPACITY
            : arrayList->capacity * DEFAULT_EXPANSION_FACTOR;

        // Attempt to reallocate with the new capacity.
        if (!_ArrayList_realloc(arrayList, newCapacity))
            return STATUS_ERR_ALLOC;
    }

    // Calculate the memory address for the new element at the end of the array.
    // Casting `data` to `char*` allows for byte-level pointer arithmetic.
    void* target = (char* )arrayList->data + arrayList->size * arrayList->dataSize;
    
    // Copy the user's element data into the calculated position.
    memcpy(target, element, arrayList->dataSize);
    arrayList->size++;

    return STATUS_OK;
}

STATUS ArrayList_delete(ArrayList* arrayList, size_t index)
{
    if (!arrayList)
        return STATUS_ERR_INVALID_ARGUMENT;

    if (arrayList->size == 0)
        return STATUS_ERR_UNDERFLOW;

    if (index >= arrayList->size)
        return STATUS_ERR_INVALID_ARGUMENT;

    // To "delete" an element, we shift all subsequent elements one position to the left.
    // `memmove` is used because the source and destination memory regions overlap.
    memmove(
        (char* )arrayList->data + index * arrayList->dataSize, // Destination
        (char* )arrayList->data + (index + 1) * arrayList->dataSize, // Source
        (arrayList->size - index - 1) * arrayList->dataSize); // Total bytes to move

    arrayList->size--;

    // Check if the array should be shrunk to conserve memory.
    // We shrink if size is 1/4 of capacity, but not below the default capacity.
    if (arrayList->capacity > DEFAULT_CAPACITY && arrayList->size <= arrayList->capacity / 4)
    {
        size_t newCapacity = arrayList->capacity / 2;
        
        // Ensure we don't shrink below our defined minimum.
        if (newCapacity < DEFAULT_CAPACITY) newCapacity = DEFAULT_CAPACITY;

        // Only reallocate if the calculated new capacity is different.
        if (newCapacity != arrayList->capacity) {
             _ArrayList_realloc(arrayList, newCapacity);
             // Note: We don't strictly need to handle allocation failure here,
             // as shrinking is an optimization. The list remains valid even if it fails.
        }
    }

    return STATUS_OK;
}

STATUS ArrayList_get(ArrayList* arrayList, size_t index, void* dataOut)
{
    if (!arrayList || !dataOut || index >= arrayList->size)
        return STATUS_ERR_INVALID_ARGUMENT;

    // Calculate the address of the desired element.
    void* element = (char*)arrayList->data + index * arrayList->dataSize;

    // Copy the data from the list into the user-provided output buffer.
    memcpy(dataOut, element, arrayList->dataSize);
    return STATUS_OK;
}

STATUS ArrayList_set(ArrayList* arrayList, size_t index, void* element)
{
    if (!arrayList || !element || index >= arrayList->size)
        return STATUS_ERR_INVALID_ARGUMENT;

    // Calculate the address of the element to be updated.
    void* target = (char* )arrayList->data + index * arrayList->dataSize;
    
    // Overwrite the existing data with the new element data.
    memcpy(target, element, arrayList->dataSize);
    return STATUS_OK;
}

STATUS ArrayList_search(ArrayList* arr, void* key, size_t* index, int (*cmp)(const void* , const void* )) 
{
    if (!arr || !key || !cmp || !index)
        return STATUS_ERR_INVALID_ARGUMENT;

    // Perform a linear scan of the array.
    for (size_t i = 0; i < arr->size; i++) {
        void* element = (char*)arr->data + i * arr->dataSize;
        // Use the user-provided comparison function to check for a match.
        if (cmp(element, key) == 0) {
            *index = i; // Store the found index in the output parameter.
            return STATUS_OK;
        }
    }

    // If the loop completes without a match, the key was not found.
    return STATUS_ERR_KEY_NOT_FOUND;
}

STATUS ArrayList_forEach(ArrayList* arrayList, void (*callBack)(void* ))
{
    if (!arrayList || !callBack)
        return STATUS_ERR_INVALID_ARGUMENT;

    // Iterate through each element.
    for (size_t i = 0; i < arrayList->size; i++)
    {
        // Calculate the address of the current element.
        void* element = (char* )arrayList->data + i * arrayList->dataSize;
        // Execute the user-provided callback function on the element.
        callBack(element);
    }
    return STATUS_OK;
}

size_t ArrayList_size(const ArrayList* arrayList)
{
    if (!arrayList) {
        return 0;
    }
    return arrayList->size;
}

size_t ArrayList_capacity(const ArrayList* arrayList)
{
    if (!arrayList) {
        return 0;
    }
    return arrayList->capacity;
}