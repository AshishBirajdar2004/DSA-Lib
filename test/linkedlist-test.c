#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dsa-lib/linkedlist.h>

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
// 2. Custom Data Type and Helpers for Testing
// =============================================================================

typedef struct {
    int id;
    char name[50];
} Person;

// Comparison function for searching a Person by ID
int compare_person_id(void* a, void* b) {
    Person* person_a = (Person*)a;
    Person* key = (Person*)b;
    return person_a->id - key->id;
}

// forEach callback to increment integer values
void increment_int(void* data) {
    int* value = (int*)data;
    (*value)++;
}


// =============================================================================
// 3. Test Groups
// =============================================================================

/**
 * @brief Tests the LinkedList with basic integer operations.
 */
void test_int_list() {
    printf("\n--- Testing LinkedList with Integers ---\n");
    LinkedList* list = LinkedList_init(sizeof(int));
    ASSERT_TRUE(list != NULL, "Integer list initialization");
    ASSERT_EQUAL_INT(0, LinkedList_size(list), "Initial size is 0");

    // Insertions are at the head, so the list will be in reverse order: 30 -> 20 -> 10
    int val1 = 10, val2 = 20, val3 = 30;
    LinkedList_insert(list, &val1);
    LinkedList_insert(list, &val2);
    LinkedList_insert(list, &val3);
    ASSERT_EQUAL_INT(3, LinkedList_size(list), "Size is 3 after three inserts");

    // Test get (1-based index)
    int retrieved_val;
    LinkedList_get(list, 1, &retrieved_val);
    ASSERT_EQUAL_INT(30, retrieved_val, "Get element at index 1 (head)");
    LinkedList_get(list, 3, &retrieved_val);
    ASSERT_EQUAL_INT(10, retrieved_val, "Get element at index 3 (tail)");

    // Test set (1-based index)
    int new_val = 25;
    LinkedList_set(list, 2, &new_val); // Change 20 to 25
    LinkedList_get(list, 2, &retrieved_val);
    ASSERT_EQUAL_INT(25, retrieved_val, "Set element at index 2");

    // Test delete (1-based index). List: 30 -> 25 -> 10
    LinkedList_delete(list, 1); // Delete head (30). List: 25 -> 10
    ASSERT_EQUAL_INT(2, LinkedList_size(list), "Size is 2 after deleting head");
    LinkedList_get(list, 1, &retrieved_val);
    ASSERT_EQUAL_INT(25, retrieved_val, "New head is 25");

    // Test forEach
    LinkedList_forEach(list, increment_int); // List becomes 26 -> 11
    LinkedList_get(list, 1, &retrieved_val);
    ASSERT_EQUAL_INT(26, retrieved_val, "forEach correctly increments first element");
    LinkedList_get(list, 2, &retrieved_val);
    ASSERT_EQUAL_INT(11, retrieved_val, "forEach correctly increments second element");

    LinkedList_destroy(list);
}

/**
 * @brief Tests the LinkedList with C-style strings (char*).
 */
void test_string_list() {
    printf("\n--- Testing LinkedList with Strings ---\n");
    LinkedList* list = LinkedList_init(sizeof(char*));
    ASSERT_TRUE(list != NULL, "String list initialization");

    char* str1 = "hello";
    char* str2 = "world";
    LinkedList_insert(list, &str1);
    LinkedList_insert(list, &str2);
    ASSERT_EQUAL_INT(2, LinkedList_size(list), "Size is 2 after inserting two strings");

    char* retrieved_str;
    LinkedList_get(list, 1, &retrieved_str); // Get head
    ASSERT_EQUAL_STRING("world", retrieved_str, "Get first string ('world')");

    LinkedList_destroy(list);
}

/**
 * @brief Tests the LinkedList with a custom Person struct.
 */
void test_struct_list() {
    printf("\n--- Testing LinkedList with Custom Structs ---\n");
    LinkedList* list = LinkedList_init(sizeof(Person));
    ASSERT_TRUE(list != NULL, "Struct list initialization");

    // List: p2 -> p1
    Person p1 = {101, "Alice"};
    Person p2 = {102, "Bob"};
    LinkedList_insert(list, &p1);
    LinkedList_insert(list, &p2);

    // Test search (1-based index)
    size_t found_index;
    Person search_key = {101, ""}; // Search by ID
    STATUS status = LinkedList_search(list, &search_key, &found_index, compare_person_id);
    ASSERT_TRUE(status == STATUS_OK, "Search for Person with ID 101 succeeds");
    ASSERT_EQUAL_INT(2, found_index, "Found index for Person with ID 101 is 2");

    // Test get
    Person retrieved_person;
    LinkedList_get(list, 1, &retrieved_person);
    ASSERT_EQUAL_INT(102, retrieved_person.id, "Get struct at index 1 (ID)");
    ASSERT_EQUAL_STRING("Bob", retrieved_person.name, "Get struct at index 1 (Name)");

    LinkedList_destroy(list);
}

/**
 * @brief Tests edge cases and invalid inputs.
 */
void test_edge_cases() {
    printf("\n--- Testing Edge Cases ---\n");
    LinkedList* list = LinkedList_init(sizeof(int));
    int val = 100;

    // NULL checks
    ASSERT_TRUE(LinkedList_init(0) == NULL, "Init with dataSize 0 fails");
    ASSERT_TRUE(LinkedList_insert(NULL, &val) == STATUS_ERR_INVALID_ARGUMENT, "Insert with NULL list fails");
    ASSERT_TRUE(LinkedList_delete(NULL, 1) == STATUS_ERR_INVALID_ARGUMENT, "Delete with NULL list fails");
    ASSERT_TRUE(LinkedList_get(NULL, 1, &val) == STATUS_ERR_INVALID_ARGUMENT, "Get with NULL list fails");
    ASSERT_TRUE(LinkedList_set(list, 1, NULL) == STATUS_ERR_INVALID_ARGUMENT, "Set with NULL element fails");
    ASSERT_TRUE(LinkedList_search(list, NULL, NULL, NULL) == STATUS_ERR_INVALID_ARGUMENT, "Search with NULL args fails");

    // Boundary checks for an empty list
    ASSERT_TRUE(LinkedList_delete(list, 1) == STATUS_ERR_UNDERFLOW, "Delete on empty list fails");
    
    // Boundary checks for a non-empty list
    LinkedList_insert(list, &val);
    ASSERT_TRUE(LinkedList_get(list, 0, &val) == STATUS_ERR_INVALID_ARGUMENT, "Get with 0 index fails");
    ASSERT_TRUE(LinkedList_get(list, 2, &val) == STATUS_ERR_INVALID_ARGUMENT, "Get with out-of-bounds index fails");
    ASSERT_TRUE(LinkedList_set(list, 2, &val) == STATUS_ERR_INVALID_ARGUMENT, "Set with out-of-bounds index fails");
    ASSERT_TRUE(LinkedList_delete(list, 2) == STATUS_ERR_INVALID_ARGUMENT, "Delete with out-of-bounds index fails");

    // Search for non-existent key
    int non_existent_key = 999;
    size_t found_index;
    ASSERT_TRUE(LinkedList_search(list, &non_existent_key, &found_index, (void*)memcmp) == STATUS_ERR_KEY_NOT_FOUND, "Search for non-existent key fails");

    LinkedList_destroy(list);
}


// =============================================================================
// 4. Main Test Runner
// =============================================================================

int main() {
    printf("========================================\n");
    printf("       Testing LinkedList Module\n");
    printf("========================================\n");

    test_int_list();
    test_string_list();
    test_struct_list();
    test_edge_cases();

    printf("\n----------------------------------------\n");
    printf("Test Summary:\n");
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("========================================\n");

    return (tests_failed == 0) ? 0 : 1;
}
