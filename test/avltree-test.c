#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dsa-lib/avltree.h>    
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


// =============================================================================
// 2. Custom Data Type and Helpers for Testing
// =============================================================================

// Comparison function for integers.
int compare_int(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return int_a - int_b;
}

// Helper to add an element to an ArrayList, used as a callback for traversals.
static ArrayList* traversal_result;
void add_to_list_callback(void* data) {
    if (traversal_result) {
        ArrayList_insert(traversal_result, data);
    }
}

// Helper function to verify the in-order traversal of the tree.
void verify_inorder_traversal(AVLTree* tree, int expected[], int expected_size) {
    traversal_result = ArrayList_init(expected_size, sizeof(int));
    AVLTree_traverseInorder(tree, add_to_list_callback);

    ASSERT_EQUAL_INT(expected_size, ArrayList_size(traversal_result), "Traversal should have correct number of elements");

    bool order_correct = true;
    for (int i = 0; i < expected_size; ++i) {
        int val;
        ArrayList_get(traversal_result, i, &val);
        if (val != expected[i]) {
            order_correct = false;
            break;
        }
    }
    ASSERT_TRUE(order_correct, "In-order traversal produces sorted elements");

    ArrayList_destroy(traversal_result);
    traversal_result = NULL;
}


// =============================================================================
// 3. Test Groups
// =============================================================================

/**
 * @brief Tests basic AVL tree operations including all rotation cases.
 */
void test_avl_rotations_and_operations() {
    printf("\n--- Testing AVL Tree Insertions, Rotations, and Deletions ---\n");
    AVLTree* tree = AVLTree_init(sizeof(int), compare_int);
    ASSERT_TRUE(tree != NULL, "AVL Tree initialization");

    // Test Right-Right (RR) Case -> Left Rotation
    int val10 = 10, val20 = 20, val30 = 30;
    AVLTree_insert(tree, &val10);
    AVLTree_insert(tree, &val20);
    AVLTree_insert(tree, &val30); // Triggers RR case
    int expected1[] = {10, 20, 30};
    verify_inorder_traversal(tree, expected1, 3);
    
    // Test Left-Left (LL) Case -> Right Rotation
    int val5 = 5, val3 = 3;
    AVLTree_insert(tree, &val5);
    AVLTree_insert(tree, &val3); // Triggers LL case
    int expected2[] = {3, 5, 10, 20, 30};
    verify_inorder_traversal(tree, expected2, 5);

    // Test Right-Left (RL) Case -> Right-Left Rotation
    int val25 = 25, val28 = 28;
    AVLTree_insert(tree, &val28);
    AVLTree_insert(tree, &val25); // Triggers RL case
    int expected3[] = {3, 5, 10, 20, 25, 28, 30};
    verify_inorder_traversal(tree, expected3, 7);

    // Test Left-Right (LR) Case -> Left-Right Rotation
    int val8 = 8, val6 = 6;
    AVLTree_insert(tree, &val8);
    AVLTree_insert(tree, &val6); // Triggers LR case
    int expected4[] = {3, 5, 6, 8, 10, 20, 25, 28, 30};
    verify_inorder_traversal(tree, expected4, 9);
    
    // Test Search
    int* found = (int*)AVLTree_search(tree, &val25);
    ASSERT_TRUE(found != NULL, "Search for existing key (25) succeeds");
    ASSERT_EQUAL_INT(25, *found, "Found key is correct");
    int not_found_key = 999;
    found = (int*)AVLTree_search(tree, &not_found_key);
    ASSERT_TRUE(found == NULL, "Search for non-existent key (999) fails");

    // Test Deletion
    AVLTree_delete(tree, &val3); // Delete leaf
    AVLTree_delete(tree, &val30); // Delete node with one child
    AVLTree_delete(tree, &val10); // Delete node with two children
    int expected5[] = {5, 6, 8, 20, 25, 28};
    verify_inorder_traversal(tree, expected5, 6);

    AVLTree_destroy(tree);
}

/**
 * @brief Tests edge cases and invalid inputs.
 */
void test_edge_cases() {
    printf("\n--- Testing Edge Cases ---\n");
    AVLTree* tree = AVLTree_init(sizeof(int), compare_int);
    int val = 100;

    // NULL checks
    ASSERT_TRUE(AVLTree_init(sizeof(int), NULL) == NULL, "Init with NULL compare function fails");
    ASSERT_TRUE(AVLTree_insert(NULL, &val) == STATUS_ERR_INVALID_ARGUMENT, "Insert with NULL tree fails");
    ASSERT_TRUE(AVLTree_insert(tree, NULL) == STATUS_ERR_INVALID_ARGUMENT, "Insert with NULL data fails");
    ASSERT_TRUE(AVLTree_delete(NULL, &val) == STATUS_ERR_INVALID_ARGUMENT, "Delete with NULL tree fails");
    ASSERT_TRUE(AVLTree_search(NULL, &val) == NULL, "Search with NULL tree fails");
    ASSERT_TRUE(AVLTree_traverseInorder(NULL, add_to_list_callback) == STATUS_ERR_INVALID_ARGUMENT, "Traversal with NULL tree fails");

    // Operation status checks
    ASSERT_TRUE(AVLTree_delete(tree, &val) == STATUS_ERR_KEY_NOT_FOUND, "Delete on non-existent key fails");
    AVLTree_insert(tree, &val);
    ASSERT_TRUE(AVLTree_insert(tree, &val) == STATUS_ERR_DUPLICATE_KEY, "Insert of duplicate key fails");

    AVLTree_destroy(tree);
}


// =============================================================================
// 4. Main Test Runner
// =============================================================================

int main() {
    printf("========================================\n");
    printf("        Testing AVL Tree Module\n");
    printf("========================================\n");

    test_avl_rotations_and_operations();
    test_edge_cases();

    printf("\n----------------------------------------\n");
    printf("Test Summary:\n");
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("========================================\n");

    return (tests_failed == 0) ? 0 : 1;
}
