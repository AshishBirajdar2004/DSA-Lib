#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dsa-lib/queue.h>

// =============================================================================
// 1. Simple Assertion Framework
// =============================================================================

static int tests_passed = 0;
static int tests_failed = 0;

#define ASSERT_TRUE(condition, message) \
    do { \
        if (condition) { \
            printf("[PASS] %s\n", message); \
            tests_passed++; \
        } else { \
            printf("[FAIL] %s\n", message); \
            tests_failed++; \
        } \
    } while (0)

#define ASSERT_EQUAL_INT(expected, actual, message) \
    do { \
        if ((expected) == (actual)) { \
            printf("[PASS] %s\n", message); \
            tests_passed++; \
        } else { \
            printf("[FAIL] %s (Expected: %d, Got: %d)\n", message, (int)(expected), (int)(actual)); \
            tests_failed++; \
        } \
    } while (0)

#define ASSERT_EQUAL_STRING(expected, actual, message) \
    do { \
        if (strcmp((expected), (actual)) == 0) { \
            printf("[PASS] %s\n", message); \
            tests_passed++; \
        } else { \
            printf("[FAIL] %s (Expected: \"%s\", Got: \"%s\")\n", message, expected, actual); \
            tests_failed++; \
        } \
    } while (0)


// =============================================================================
// 2. Custom Data Type for Testing
// =============================================================================

typedef struct {
    int id;
    char name[50];
} Person;


// =============================================================================
// 3. Test Groups
// =============================================================================

/**
 * @brief Tests the queue with basic integer operations.
 */
void test_int_queue() {
    printf("\n--- Testing Queue with Integers ---\n");
    Queue* q = Queue_init(sizeof(int));
    ASSERT_TRUE(q != NULL, "Integer queue initialization");
    ASSERT_TRUE(Queue_isEmpty(q), "Queue is empty after init");

    // Enqueue 10, 20, 30. The front of the queue should be 10.
    int val1 = 10, val2 = 20, val3 = 30;
    Queue_enqueue(q, &val1);
    Queue_enqueue(q, &val2);
    Queue_enqueue(q, &val3);

    ASSERT_TRUE(!Queue_isEmpty(q), "Queue is not empty after enqueuing");

    // Test peek to see the front element
    int peek_val = 0;
    Queue_peek(q, &peek_val);
    ASSERT_EQUAL_INT(10, peek_val, "Peek returns the front value (10)");

    // Test that peek doesn't remove the element
    Queue_peek(q, &peek_val);
    ASSERT_EQUAL_INT(10, peek_val, "Peek again returns the same front value (10)");

    // Test dequeue
    Queue_dequeue(q); // Dequeue 10
    Queue_peek(q, &peek_val);
    ASSERT_EQUAL_INT(20, peek_val, "Peek returns new front value (20) after dequeue");

    Queue_dequeue(q); // Dequeue 20
    Queue_dequeue(q); // Dequeue 30
    ASSERT_TRUE(Queue_isEmpty(q), "Queue is empty after dequeuing all elements");
    ASSERT_TRUE(Queue_dequeue(q) == STATUS_ERR_UNDERFLOW, "Dequeue from empty queue fails (underflow)");

    Queue_destroy(q);
}

/**
 * @brief Tests the queue with C-style strings (char**).
 */
void test_string_queue() {
    printf("\n--- Testing Queue with Strings ---\n");
    Queue* q = Queue_init(sizeof(char*));
    ASSERT_TRUE(q != NULL, "String queue initialization");

    char* str1 = "first";
    char* str2 = "second";
    Queue_enqueue(q, &str1);
    Queue_enqueue(q, &str2);

    char* peek_str = NULL;
    Queue_peek(q, &peek_str);
    ASSERT_EQUAL_STRING("first", peek_str, "Peek returns the front string ('first')");

    Queue_dequeue(q);
    Queue_peek(q, &peek_str);
    ASSERT_EQUAL_STRING("second", peek_str, "Peek returns new front string ('second')");

    Queue_destroy(q);
}

/**
 * @brief Tests the queue with a custom Person struct.
 */
void test_struct_queue() {
    printf("\n--- Testing Queue with Custom Structs ---\n");
    Queue* q = Queue_init(sizeof(Person));
    ASSERT_TRUE(q != NULL, "Struct queue initialization");

    Person p1 = {101, "Alice"};
    Person p2 = {102, "Bob"};
    Queue_enqueue(q, &p1);
    Queue_enqueue(q, &p2);

    Person peek_person = {0};
    Queue_peek(q, &peek_person);
    ASSERT_EQUAL_INT(101, peek_person.id, "Peek returns correct struct ID (front of queue)");
    ASSERT_EQUAL_STRING("Alice", peek_person.name, "Peek returns correct struct name (front of queue)");

    Queue_destroy(q);
}

/**
 * @brief Tests edge cases and invalid inputs.
 */
void test_edge_cases() {
    printf("\n--- Testing Edge Cases ---\n");
    Queue* q = Queue_init(sizeof(int));
    int val = 1;

    // NULL checks
    ASSERT_TRUE(Queue_enqueue(NULL, &val) == STATUS_ERR_INVALID_ARGUMENT, "Enqueue with NULL queue fails");
    ASSERT_TRUE(Queue_enqueue(q, NULL) == STATUS_ERR_INVALID_ARGUMENT, "Enqueue with NULL element fails");
    ASSERT_TRUE(Queue_dequeue(NULL) == STATUS_ERR_INVALID_ARGUMENT, "Dequeue with NULL queue fails");
    ASSERT_TRUE(Queue_peek(q, NULL) == STATUS_ERR_INVALID_ARGUMENT, "Peek with NULL output pointer fails");
    ASSERT_TRUE(Queue_peek(NULL, &val) == STATUS_ERR_INVALID_ARGUMENT, "Peek with NULL queue fails");

    // Empty checks
    ASSERT_TRUE(Queue_isEmpty(q), "Queue is initially empty");
    ASSERT_TRUE(Queue_peek(q, &val) == STATUS_ERR_EMPTY, "Peek from empty queue fails");
    ASSERT_TRUE(Queue_dequeue(q) == STATUS_ERR_UNDERFLOW, "Dequeue from empty queue fails");

    Queue_destroy(q);
    q = NULL;
    ASSERT_TRUE(Queue_isEmpty(NULL), "isEmpty on NULL queue returns true");
}


// =============================================================================
// 4. Main Test Runner
// =============================================================================

int main() {
    printf("========================================\n");
    printf("         Testing Queue Module\n");
    printf("========================================\n");

    test_int_queue();
    test_string_queue();
    test_struct_queue();
    test_edge_cases();

    printf("\n----------------------------------------\n");
    printf("Test Summary:\n");
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("========================================\n");

    return (tests_failed == 0) ? 0 : 1;
}
