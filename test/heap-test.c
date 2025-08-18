#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dsa-lib/heap.h>

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

typedef struct {
    int id;
    int score;
} Player;

// Comparison function for a MIN-HEAP of integers.
int compare_int_min(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    return int_a - int_b; // For min-heap
}

// Comparison function for a MAX-HEAP of Players by score.
int compare_player_score_max(const void* a, const void* b) {
    const Player* p_a = (const Player*)a;
    const Player* p_b = (const Player*)b;
    return p_b->score - p_a->score; // For max-heap
}


// =============================================================================
// 3. Test Groups
// =============================================================================

/**
 * @brief Tests the heap configured as a min-heap with integers.
 */
void test_min_heap() {
    printf("\n--- Testing Min-Heap with Integers ---\n");
    Heap* h = Heap_init(5, sizeof(int), compare_int_min);
    ASSERT_TRUE(h != NULL, "Min-heap initialization");
    ASSERT_EQUAL_INT(0, Heap_size(h), "Initial size is 0");

    // Push unsorted values
    int values[] = {10, 4, 15, 20, 0, 3, 30};
    for (size_t i = 0; i < 7; ++i) {
        Heap_push(h, &values[i]);
    }
    ASSERT_EQUAL_INT(7, Heap_size(h), "Size is 7 after pushing 7 elements");

    int peek_val;
    Heap_peek(h, &peek_val);
    ASSERT_EQUAL_INT(0, peek_val, "Peek returns the minimum value (0)");

    // Pop elements and ensure they come out in ascending order
    int expected_order[] = {0, 3, 4, 10, 15, 20, 30};
    for (size_t i = 0; i < 7; ++i) {
        int popped_val;
        char msg[100];
        sprintf(msg, "Popped value %zu should be %d", i + 1, expected_order[i]);
        Heap_pop(h, &popped_val);
        ASSERT_EQUAL_INT(expected_order[i], popped_val, msg);
    }

    ASSERT_EQUAL_INT(0, Heap_size(h), "Heap is empty after popping all elements");
    Heap_destroy(h);
}

/**
 * @brief Tests the heap configured as a max-heap with a custom struct.
 */
void test_max_heap() {
    printf("\n--- Testing Max-Heap with Custom Structs ---\n");
    Heap* h = Heap_init(5, sizeof(Player), compare_player_score_max);
    ASSERT_TRUE(h != NULL, "Max-heap initialization");

    Player players[] = {
        {1, 95}, {2, 88}, {3, 100}, {4, 76}, {5, 95}
    };
    for (size_t i = 0; i < 5; ++i) {
        Heap_push(h, &players[i]);
    }
    ASSERT_EQUAL_INT(5, Heap_size(h), "Size is 5 after pushing 5 players");

    Player peek_player;
    Heap_peek(h, &peek_player);
    ASSERT_EQUAL_INT(100, peek_player.score, "Peek returns player with max score (100)");

    // Pop players and ensure they come out in descending order of score
    int expected_scores[] = {100, 95, 95, 88, 76};
    for (size_t i = 0; i < 5; ++i) {
        Player popped_player;
        char msg[100];
        sprintf(msg, "Popped player %zu should have score %d", i + 1, expected_scores[i]);
        Heap_pop(h, &popped_player);
        ASSERT_EQUAL_INT(expected_scores[i], popped_player.score, msg);
    }

    ASSERT_EQUAL_INT(0, Heap_size(h), "Heap is empty after popping all players");
    Heap_destroy(h);
}

/**
 * @brief Tests edge cases and invalid inputs.
 */
void test_edge_cases() {
    printf("\n--- Testing Edge Cases ---\n");
    Heap* h = Heap_init(1, sizeof(int), compare_int_min);
    int val = 100;

    // NULL checks
    ASSERT_TRUE(Heap_init(1, 0, compare_int_min) == NULL, "Init with dataSize 0 fails");
    ASSERT_TRUE(Heap_init(1, sizeof(int), NULL) == NULL, "Init with NULL compare function fails");
    ASSERT_TRUE(Heap_push(NULL, &val) == STATUS_ERR_INVALID_ARGUMENT, "Push with NULL heap fails");
    ASSERT_TRUE(Heap_push(h, NULL) == STATUS_ERR_INVALID_ARGUMENT, "Push with NULL element fails");
    ASSERT_TRUE(Heap_pop(NULL, &val) == STATUS_ERR_INVALID_ARGUMENT, "Pop with NULL heap fails");
    ASSERT_TRUE(Heap_pop(h, NULL) == STATUS_ERR_INVALID_ARGUMENT, "Pop with NULL output pointer fails");
    ASSERT_TRUE(Heap_peek(NULL, &val) == STATUS_ERR_INVALID_ARGUMENT, "Peek with NULL heap fails");

    // Empty/Underflow checks
    ASSERT_TRUE(Heap_pop(h, &val) == STATUS_ERR_UNDERFLOW, "Pop from empty heap fails");
    ASSERT_TRUE(Heap_peek(h, &val) == STATUS_ERR_EMPTY, "Peek from empty heap fails");

    Heap_destroy(h);
}


// =============================================================================
// 4. Main Test Runner
// =============================================================================

int main() {
    printf("========================================\n");
    printf("          Testing Heap Module\n");
    printf("========================================\n");

    test_min_heap();
    test_max_heap();
    test_edge_cases();

    printf("\n----------------------------------------\n");
    printf("Test Summary:\n");
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("========================================\n");

    return (tests_failed == 0) ? 0 : 1;
}
