#include <stdio.h>
#include <dsa-lib/queue.h>

// A custom complex struct for testing
typedef struct {
    int id;
    char name[50];
} Person;

void printPerson(Person* p) {
    printf("{ id: %d, name: %s }\n", p->id, p->name);
}

int main() {
    // === Test 1: int queue ===
    printf("=== INT QUEUE ===\n");
    Queue* intQueue = Queue_init(sizeof(int));

    int nums[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        Queue_enqueue(intQueue, &nums[i]);
        printf("Enqueued: %d\n", nums[i]);
    }

    int frontInt = 0;
    while (!Queue_isEmpty(intQueue)) {
        Queue_peek(intQueue, &frontInt);
        printf("Dequeued: %d\n", frontInt);
        Queue_dequeue(intQueue);
    }
    Queue_destroy(intQueue);

    printf("\n");

    // === Test 2: char queue ===
    printf("=== CHAR QUEUE ===\n");
    Queue* charQueue = Queue_init(sizeof(char));

    char chars[] = {'A', 'B', 'C'};
    for (int i = 0; i < 3; i++) {
        Queue_enqueue(charQueue, &chars[i]);
        printf("Enqueued: %c\n", chars[i]);
    }

    char frontChar;
    while (!Queue_isEmpty(charQueue)) {
        Queue_peek(charQueue, &frontChar);
        printf("Dequeued: %c\n", frontChar);
        Queue_dequeue(charQueue);
    }
    Queue_destroy(charQueue);

    printf("\n");

    // === Test 3: struct queue ===
    printf("=== PERSON QUEUE ===\n");
    Queue* personQueue = Queue_init(sizeof(Person));

    Person p1 = {1, "Alice"};
    Person p2 = {2, "Bob"};
    Person p3 = {3, "Charlie"};

    Queue_enqueue(personQueue, &p1);
    Queue_enqueue(personQueue, &p2);
    Queue_enqueue(personQueue, &p3);

    Person frontPerson;
    while (!Queue_isEmpty(personQueue)) {
        Queue_peek(personQueue, &frontPerson);
        printPerson(&frontPerson);
        Queue_dequeue(personQueue);
    }
    Queue_destroy(personQueue);

    return 0;
}
