#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dsa-lib/arraylist.h>

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
int compare_person_id(const void* a, const void* b) {
    const Person* person_a = (const Person*)a;
    const Person* key = (const Person*)b;
    return person_a->id - key->id;
}

// forEach callback to increment integer values
void increment_int(void* data) {
    int* value = (int*)data;
    (*value)++;
}

// Comparison function for searching an integer
int compare_int(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

// =============================================================================
// 3. Test Groups
// =============================================================================

/**
 * @brief Tests the ArrayList with basic integer operations.
 */
void test_int_list() {
    printf("\n--- Testing ArrayList with Integers ---\n");
    ArrayList* list = ArrayList_init(2, sizeof(int));
    ASSERT_TRUE(list != NULL, "Integer list initialization");
    ASSERT_EQUAL_INT(0, ArrayList_size(list), "Initial size is 0");
    ASSERT_EQUAL_INT(2, ArrayList_capacity(list), "Initial capacity is 2");

    int val1 = 10, val2 = 20, val3 = 30;
    ArrayList_insert(list, &val1);
    ArrayList_insert(list, &val2);
    ASSERT_EQUAL_INT(2, ArrayList_size(list), "Size is 2 after two inserts");

    // Test automatic expansion
    ArrayList_insert(list, &val3);
    ASSERT_EQUAL_INT(3, ArrayList_size(list), "Size is 3 after third insert");
    ASSERT_TRUE(ArrayList_capacity(list) > 2, "Capacity expanded after third insert");

    // Test get
    int retrieved_val;
    ArrayList_get(list, 1, &retrieved_val);
    ASSERT_EQUAL_INT(20, retrieved_val, "Get element at index 1");

    // Test set
    int new_val = 25;
    ArrayList_set(list, 1, &new_val);
    ArrayList_get(list, 1, &retrieved_val);
    ASSERT_EQUAL_INT(25, retrieved_val, "Set element at index 1");

    // Test delete
    ArrayList_delete(list, 0); // Delete first element (10)
    ASSERT_EQUAL_INT(2, ArrayList_size(list), "Size is 2 after delete");
    ArrayList_get(list, 0, &retrieved_val);
    ASSERT_EQUAL_INT(25, retrieved_val, "New element at index 0 is 25");

    // Test forEach
    ArrayList_forEach(list, increment_int);
    ArrayList_get(list, 0, &retrieved_val);
    ASSERT_EQUAL_INT(26, retrieved_val, "forEach correctly increments first element");
    ArrayList_get(list, 1, &retrieved_val);
    ASSERT_EQUAL_INT(31, retrieved_val, "forEach correctly increments second element");

    ArrayList_destroy(list);
}

/**
 * @brief Tests the ArrayList with C-style strings (char*).
 */
void test_string_list() {
    printf("\n--- Testing ArrayList with Strings ---\n");
    ArrayList* list = ArrayList_init(2, sizeof(char*));
    ASSERT_TRUE(list != NULL, "String list initialization");

    char* str1 = "hello";
    char* str2 = "world";
    ArrayList_insert(list, &str1);
    ArrayList_insert(list, &str2);
    ASSERT_EQUAL_INT(2, ArrayList_size(list), "Size is 2 after inserting two strings");

    char* retrieved_str;
    ArrayList_get(list, 0, &retrieved_str);
    ASSERT_EQUAL_STRING("hello", retrieved_str, "Get first string");

    ArrayList_destroy(list);
}

/**
 * @brief Tests the ArrayList with a custom Person struct.
 */
void test_struct_list() {
    printf("\n--- Testing ArrayList with Custom Structs ---\n");
    ArrayList* list = ArrayList_init(2, sizeof(Person));
    ASSERT_TRUE(list != NULL, "Struct list initialization");

    Person p1 = {101, "Alice"};
    Person p2 = {102, "Bob"};
    ArrayList_insert(list, &p1);
    ArrayList_insert(list, &p2);

    // Test search
    size_t found_index;
    Person search_key = {102, ""}; // Search by ID
    STATUS status = ArrayList_search(list, &search_key, &found_index, compare_person_id);
    ASSERT_TRUE(status == STATUS_OK, "Search for Person with ID 102 succeeds");
    ASSERT_EQUAL_INT(1, found_index, "Found index for Person with ID 102 is 1");

    // Test get
    Person retrieved_person;
    ArrayList_get(list, 0, &retrieved_person);
    ASSERT_EQUAL_INT(101, retrieved_person.id, "Get struct at index 0 (ID)");
    ASSERT_EQUAL_STRING("Alice", retrieved_person.name, "Get struct at index 0 (Name)");

    ArrayList_destroy(list);
}

/**
 * @brief Tests edge cases and invalid inputs.
 */
void test_edge_cases() {
    printf("\n--- Testing Edge Cases ---\n");
    ArrayList* list = ArrayList_init(1, sizeof(int));
    int val = 100;

    // NULL checks
    ASSERT_TRUE(ArrayList_init(1, 0) == NULL, "Init with dataSize 0 fails");
    ASSERT_TRUE(ArrayList_insert(NULL, &val) == STATUS_ERR_INVALID_ARGUMENT, "Insert with NULL list fails");
    ASSERT_TRUE(ArrayList_delete(NULL, 0) == STATUS_ERR_INVALID_ARGUMENT, "Delete with NULL list fails");
    ASSERT_TRUE(ArrayList_get(NULL, 0, &val) == STATUS_ERR_INVALID_ARGUMENT, "Get with NULL list fails");
    ASSERT_TRUE(ArrayList_set(list, 0, NULL) == STATUS_ERR_INVALID_ARGUMENT, "Set with NULL element fails");
    ASSERT_TRUE(ArrayList_search(list, NULL, NULL, NULL) == STATUS_ERR_INVALID_ARGUMENT, "Search with NULL args fails");

    // Boundary checks
    ASSERT_TRUE(ArrayList_delete(list, 0) == STATUS_ERR_UNDERFLOW, "Delete on empty list fails");
    ArrayList_insert(list, &val);
    ASSERT_TRUE(ArrayList_get(list, 1, &val) == STATUS_ERR_INVALID_ARGUMENT, "Get with out-of-bounds index fails");
    ASSERT_TRUE(ArrayList_set(list, 1, &val) == STATUS_ERR_INVALID_ARGUMENT, "Set with out-of-bounds index fails");
    ASSERT_TRUE(ArrayList_delete(list, 1) == STATUS_ERR_INVALID_ARGUMENT, "Delete with out-of-bounds index fails");

    // Search for non-existent key
    int non_existent_key = 999;
    size_t found_index;
    ASSERT_TRUE(ArrayList_search(list, &non_existent_key, &found_index, compare_int) == STATUS_ERR_KEY_NOT_FOUND, "Search for non-existent key fails");

    ArrayList_destroy(list);
}


// =============================================================================
// 4. Main Test Runner
// =============================================================================

int main() {
    printf("========================================\n");
    printf("       Testing ArrayList Module\n");
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
