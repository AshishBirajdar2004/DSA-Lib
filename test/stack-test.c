#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dsa-lib/stack.h>

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
 * @brief Tests the stack with basic integer operations.
 */
void test_int_stack() {
    printf("\n--- Testing Stack with Integers ---\n");
    Stack* s = Stack_init(sizeof(int), 3);
    ASSERT_TRUE(s != NULL, "Integer stack initialization");
    ASSERT_TRUE(Stack_isEmpty(s), "Stack is empty after init");
    ASSERT_TRUE(!Stack_isFull(s), "Stack is not full after init");

    int val1 = 10, val2 = 20, val3 = 30;
    Stack_push(s, &val1);
    Stack_push(s, &val2);
    Stack_push(s, &val3);

    ASSERT_TRUE(Stack_isFull(s), "Stack is full after 3 pushes");
    ASSERT_TRUE(Stack_push(s, &val1) == STATUS_ERR_OVERFLOW, "Push to full stack fails (overflow)");

    int peek_val = 0;
    Stack_peek(s, &peek_val);
    ASSERT_EQUAL_INT(30, peek_val, "Peek returns the top value (30)");

    // Test that peek doesn't remove the element
    Stack_peek(s, &peek_val);
    ASSERT_EQUAL_INT(30, peek_val, "Peek again returns the same top value (30)");

    Stack_pop(s); // Pop 30
    Stack_peek(s, &peek_val);
    ASSERT_EQUAL_INT(20, peek_val, "Peek returns new top value (20) after pop");

    Stack_pop(s); // Pop 20
    Stack_pop(s); // Pop 10
    ASSERT_TRUE(Stack_isEmpty(s), "Stack is empty after popping all elements");
    ASSERT_TRUE(Stack_pop(s) == STATUS_ERR_UNDERFLOW, "Pop from empty stack fails (underflow)");

    Stack_destroy(s);
}

/**
 * @brief Tests the stack with C-style strings (char**).
 */
void test_string_stack() {
    printf("\n--- Testing Stack with Strings ---\n");
    Stack* s = Stack_init(sizeof(char*), 2);
    ASSERT_TRUE(s != NULL, "String stack initialization");

    char* str1 = "hello";
    char* str2 = "world";
    Stack_push(s, &str1);
    Stack_push(s, &str2);

    char* peek_str = NULL;
    Stack_peek(s, &peek_str);
    ASSERT_EQUAL_STRING("world", peek_str, "Peek returns the top string ('world')");

    Stack_pop(s);
    Stack_peek(s, &peek_str);
    ASSERT_EQUAL_STRING("hello", peek_str, "Peek returns new top string ('hello')");

    Stack_destroy(s);
}

/**
 * @brief Tests the stack with a custom Person struct.
 */
void test_struct_stack() {
    printf("\n--- Testing Stack with Custom Structs ---\n");
    Stack* s = Stack_init(sizeof(Person), 2);
    ASSERT_TRUE(s != NULL, "Struct stack initialization");

    Person p1 = {101, "Alice"};
    Person p2 = {102, "Bob"};
    Stack_push(s, &p1);
    Stack_push(s, &p2);

    Person peek_person = {0};
    Stack_peek(s, &peek_person);
    ASSERT_EQUAL_INT(102, peek_person.id, "Peek returns correct struct ID");
    ASSERT_EQUAL_STRING("Bob", peek_person.name, "Peek returns correct struct name");

    Stack_destroy(s);
}

/**
 * @brief Tests edge cases and invalid inputs.
 */
void test_edge_cases() {
    printf("\n--- Testing Edge Cases ---\n");
    Stack* s = Stack_init(sizeof(int), 1);
    int val = 1;

    // NULL checks
    ASSERT_TRUE(Stack_push(NULL, &val) == STATUS_ERR_INVALID_ARGUMENT, "Push with NULL stack fails");
    ASSERT_TRUE(Stack_push(s, NULL) == STATUS_ERR_INVALID_ARGUMENT, "Push with NULL element fails");
    ASSERT_TRUE(Stack_pop(NULL) == STATUS_ERR_INVALID_ARGUMENT, "Pop with NULL stack fails");
    ASSERT_TRUE(Stack_peek(s, NULL) == STATUS_ERR_INVALID_ARGUMENT, "Peek with NULL output pointer fails");
    ASSERT_TRUE(Stack_peek(NULL, &val) == STATUS_ERR_INVALID_ARGUMENT, "Peek with NULL stack fails");

    // Empty/Full checks on a stack of size 1
    ASSERT_TRUE(Stack_isEmpty(s), "Stack of size 1 is initially empty");
    ASSERT_TRUE(!Stack_isFull(s), "Stack of size 1 is not initially full");
    Stack_push(s, &val);
    ASSERT_TRUE(!Stack_isEmpty(s), "Stack is not empty after one push");
    ASSERT_TRUE(Stack_isFull(s), "Stack is full after one push");

    // Peek/Pop from empty stack
    Stack_pop(s);
    ASSERT_TRUE(Stack_peek(s, &val) == STATUS_ERR_EMPTY, "Peek from empty stack fails");

    Stack_destroy(s);
    s = NULL;
    ASSERT_TRUE(Stack_isEmpty(NULL), "isEmpty on NULL stack returns true");
    ASSERT_TRUE(!Stack_isFull(NULL), "isFull on NULL stack returns false");
}


// =============================================================================
// 4. Main Test Runner
// =============================================================================

int main() {
    printf("========================================\n");
    printf("         Testing Stack Module\n");
    printf("========================================\n");

    test_int_stack();
    test_string_stack();
    test_struct_stack();
    test_edge_cases();

    printf("\n----------------------------------------\n");
    printf("Test Summary:\n");
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("========================================\n");

    return (tests_failed == 0) ? 0 : 1;
}
