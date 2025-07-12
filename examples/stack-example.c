#include <stdio.h>
#include <dsa-lib/stack.h>

// A custom complex struct for testing
typedef struct {
    int id;
    char name[50];
} Person;

void printPerson(Person* p) {
    printf("{ id: %d, name: %s }\n", p->id, p->name);
}

int main() {
    // === Test 1: int stack ===
    printf("=== INT STACK ===\n");
    Stack* intStack = Stack_init(sizeof(int), 5);

    for (int i = 1; i <= 5; i++) {
        if (Stack_push(intStack, &i) == STACK_OK) {
            printf("Pushed: %d\n", i);
        } else {
            printf("Push failed at %d\n", i);
        }
    }

    int topInt = 0;
    Stack_peek(intStack, &topInt);
    printf("Top element: %d\n", topInt);

    while (!Stack_isEmpty(intStack)) {
        Stack_peek(intStack, &topInt);
        printf("Popped: %d\n", topInt);
        Stack_pop(intStack);
    }
    Stack_destroy(intStack);

    printf("\n");

    // === Test 2: char stack ===
    printf("=== CHAR STACK ===\n");
    Stack* charStack = Stack_init(sizeof(char), 3);

    char a = 'A', b = 'B', c = 'C';
    Stack_push(charStack, &a);
    Stack_push(charStack, &b);
    Stack_push(charStack, &c);

    char topChar;
    Stack_peek(charStack, &topChar);
    printf("Top char: %c\n", topChar);

    while (!Stack_isEmpty(charStack)) {
        Stack_peek(charStack, &topChar);
        printf("Popped: %c\n", topChar);
        Stack_pop(charStack);
    }
    Stack_destroy(charStack);

    printf("\n");

    // === Test 3: complex struct stack ===
    printf("=== PERSON STACK ===\n");
    Stack* personStack = Stack_init(sizeof(Person), 2);

    Person p1 = {1, "Alice"};
    Person p2 = {2, "Bob"};

    Stack_push(personStack, &p1);
    Stack_push(personStack, &p2);

    Person topPerson;
    Stack_peek(personStack, &topPerson);
    printf("Top person: ");
    printPerson(&topPerson);

    while (!Stack_isEmpty(personStack)) {
        Stack_peek(personStack, &topPerson);
        printPerson(&topPerson);
        Stack_pop(personStack);
    }
    Stack_destroy(personStack);

    return 0;
}
