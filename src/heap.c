#include "../include/heap.h"

/**
 * @struct Heap
 * @brief Defines the internal structure of the heap.
 * @details This is a private structure definition and should not be accessed directly.
 * All interactions should be through the public API.
 */
struct Heap{
    ArrayList* arr;                             // A pointer to the underlying ArrayList that stores heap elements.
    size_t dataSize;                            // The size in bytes of a single element in the heap (e.g., sizeof(int)).
    int (*cmp)(const void* a, const void* b);   // A function pointer to compare two elements, defining the heap order.
};

/** @internal Macros for calculating parent and child indices in the heap array. */
#define PARENT(i) (((i) - 1) / 2)
#define LEFT_CHILD(i) (2 * (i) + 1)
#define RIGHT_CHILD(i) (2 * (i) + 2)

/* --------------------------- Private Helper Functions --------------------------- */

/**
 * @internal
 * @brief Swaps two elements within the heap's underlying array.
 * @param heap The heap instance.
 * @param i The index of the first element.
 * @param j The index of the second element.
 * @return `STATUS_OK` on success, or `STATUS_ERR_ALLOC` on memory allocation failure.
 */
static STATUS _Heap_swap(Heap* heap, size_t i, size_t j) {
    void* elem1 = malloc(heap->dataSize);
    void* elem2 = malloc(heap->dataSize);
    if (!elem1 || !elem2) {
        free(elem1);
        free(elem2);
        return STATUS_ERR_ALLOC;
    }

    ArrayList_get(heap->arr, i, elem1);
    ArrayList_get(heap->arr, j, elem2);

    ArrayList_set(heap->arr, i, elem2);
    ArrayList_set(heap->arr, j, elem1);

    free(elem1);
    free(elem2);
    return STATUS_OK;
}

/**
 * @internal
 * @brief Restores the heap property by sifting an element upwards from a given index.
 * @details This is a recursive function that compares an element with its parent and
 * swaps them if they are in the wrong order, continuing until the heap property is
 * satisfied or the root is reached. Typically used after a push operation.
 * @param heap The heap instance.
 * @param index The index of the element to sift up.
 * @return `STATUS_OK` on success, or `STATUS_ERR_ALLOC` on memory allocation failure.
 */
static STATUS _Heap_heapify_up(Heap* heap, size_t index) {
    STATUS status = STATUS_OK;

    if (index == 0) return status;

    size_t parentIndex = PARENT(index);
    void* parent = malloc(heap->dataSize);
    void* child = malloc(heap->dataSize);

    if (!parent || !child) {
        status = STATUS_ERR_ALLOC;
    } else {
        ArrayList_get(heap->arr, parentIndex, parent);
        ArrayList_get(heap->arr, index, child);
        if (heap->cmp(child, parent) < 0) {
            if ((status = _Heap_swap(heap, parentIndex, index)) == STATUS_OK)
                status = _Heap_heapify_up(heap, parentIndex);
        }
    }
    
    free(parent);
    free(child);
    return status;
}

/**
 * @internal
 * @brief Restores the heap property by sifting an element downwards from a given index.
 * @details This is a recursive function that compares an element with its children,
 * swapping it with the smallest/largest child (as per the cmp function) if they are
 * in the wrong order. This continues until the element is in its correct place.
 * Typically used after a pop operation.
 * @param heap The heap instance.
 * @param index The index of the element to sift down.
 * @return `STATUS_OK` on success, or `STATUS_ERR_ALLOC` on memory allocation failure.
 */
static STATUS _Heap_heapify_down(Heap* heap, size_t index) {
    STATUS status = STATUS_OK;

    size_t left = LEFT_CHILD(index);
    size_t right = RIGHT_CHILD(index);
    size_t target = index;

    void* targetElem = malloc(heap->dataSize);
    void* leftElem = malloc(heap->dataSize);
    void* rightElem = malloc(heap->dataSize);

    if (!targetElem || !leftElem || !rightElem) {
        status = STATUS_ERR_ALLOC;
    } else {
        ArrayList_get(heap->arr, target, targetElem);

        // Find the "smallest" or "largest" among the parent and its children.
        if (left < Heap_size(heap)) {
            ArrayList_get(heap->arr, left, leftElem);
            if (heap->cmp(leftElem, targetElem) < 0) {
                target = left;
                memcpy(targetElem, leftElem, heap->dataSize);
            }
        }
        if (right < Heap_size(heap)) {
            ArrayList_get(heap->arr, right, rightElem);
            if (heap->cmp(rightElem, targetElem) < 0) {
                target = right;
            }
        }
        // If the target is not the original element, swap and recurse down.
        if (target != index) {
            if ((status = _Heap_swap(heap, index, target)) == STATUS_OK) {
                status = _Heap_heapify_down(heap, target);
            }
        }
    }

    free(targetElem);
    free(leftElem);
    free(rightElem);
    return status;
}


/* ----------------------------- Public API Functions ----------------------------- */

Heap* Heap_init(size_t capacity, size_t dataSize, int (*cmp)(const void* a, const void* b)) {
    if (dataSize == 0 || !cmp) return NULL;

    Heap* heap = (Heap*)malloc(sizeof(Heap));
    if (!heap) return NULL;

    ArrayList* arr = ArrayList_init(capacity, dataSize);
    if (!arr) {
        free(heap);
        return NULL;
    }

    heap->arr = arr;
    heap->dataSize = dataSize;
    heap->cmp = cmp;
    return heap;
}

void Heap_destroy(Heap* heap) {
    if (!heap) return;
    ArrayList_destroy(heap->arr); 
    heap->arr = NULL;
    free(heap);
}

STATUS Heap_push(Heap* heap, void* element) {
    if (!heap || !element) return STATUS_ERR_INVALID_ARGUMENT;

    STATUS status = ArrayList_insert(heap->arr, element);
    if (status != STATUS_OK) return status;

    return _Heap_heapify_up(heap, Heap_size(heap) - 1);
}

STATUS Heap_pop(Heap* heap, void* elementOut) {
    if (!heap || !elementOut) return STATUS_ERR_INVALID_ARGUMENT;

    size_t size = Heap_size(heap);
    if (size == 0) return STATUS_ERR_UNDERFLOW;

    STATUS status = ArrayList_get(heap->arr, 0, elementOut);
    if (status != STATUS_OK) return status;

    if (size == 1) {
        return ArrayList_delete(heap->arr, 0);
    }
    
    // The root is replaced by the last element, which is then sifted down.
    if ((status = _Heap_swap(heap, 0, size - 1)) != STATUS_OK) return status;
    if ((status = ArrayList_delete(heap->arr, size - 1)) != STATUS_OK) return status;

    return _Heap_heapify_down(heap, 0);
}

STATUS Heap_peek(const Heap* heap, void* elementOut) {
    if (!heap || !elementOut) return STATUS_ERR_INVALID_ARGUMENT;
    if (Heap_size(heap) == 0) return STATUS_ERR_EMPTY;
    
    return ArrayList_get(heap->arr, 0, elementOut);
}

size_t Heap_size(const Heap* heap) {
    return ArrayList_size(heap->arr);
}