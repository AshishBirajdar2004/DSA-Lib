#include <stdio.h>
#include <dsa-lib/list.h>

// Callback to print an int node
void printInt(void* data) {
    printf("%d ", *(int*)data);
}

// Callback to sum ints using a context (demonstrates forEachNode + external sum)
int sum = 0;
void sumInts(void* data) {
    sum += *(int*)data;
}

// Callback to compare two ints using a context (returns 0 if are equal)
int cmp(void* a, void* b) {
    return (*(int*)(a) - *(int*)(b));
}

int main() {
    List* myList = List_init(sizeof(int));

    // Insert integers at head
    for (int i = 1; i <= 5; i++) {
        List_insertAtHead(myList, &i);  // Inserts 1,2,3,4,5 at head => list is 5->4->3->2->1
    }

    printf("List elements: ");
    List_forEach(myList, printInt);
    printf("\n");

    // Sum them up
    List_forEach(myList, sumInts);
    printf("Sum of elements: %d\n", sum);

    // Delete the 3rd element (position = 3)
    List_removeAt(myList, 3);
    printf("After deleting position 3: ");
    List_forEach(myList, printInt);
    printf("\n");

    // Search for an element and retrieve it
    int key = 4;
    int pos = List_search(myList, &key, cmp);
    printf("Key found at index %d\n", pos);
    int element = 0;
    List_getAt(myList, pos, &element);
    printf("Element at index - %d: %d\n", pos, element);

    List_destroy(myList);
    myList = NULL;
    return 0;
}
