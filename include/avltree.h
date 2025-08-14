/**
 * @file avltree.h
 * @brief Public API for a generic, self-balancing AVL Tree.
 *
 * This file defines the interface for a generic AVL Tree data structure.
 * An AVL Tree is a binary search tree that automatically maintains a balanced
 * state by performing rotations upon insertion and deletion, ensuring that
 * operations like search, insert, and delete have a worst-case time
 * complexity of O(log n).
 */
#ifndef AVLTREE_H
#define AVLTREE_H

#include "common.h"

/**
 * @struct AVLTree
 * @brief An opaque struct representing the AVL Tree data structure.
 *
 * The internal details are hidden to encapsulate the implementation.
 * Users should interact with the AVLTree only through the public API functions.
 */
typedef struct AVLTree AVLTree;

/**
 * @brief Initializes a new, empty AVL tree.
 * @param dataSize The size in bytes of each element to be stored (e.g., `sizeof(int)`).
 * @param cmp A function pointer for comparing two elements, which defines the
 * ordering of elements in the tree. It should return:
 * - A negative value if the first element is less than the second.
 * - Zero if the elements are equal.
 * - A positive value if the first element is greater than the second.
 * @return A pointer to the newly created AVLTree, or `NULL` on allocation failure or invalid arguments.
 */
AVLTree* AVLTree_init(size_t dataSize, int (*cmp)(const void *, const void *));

/**
 * @brief Frees all memory associated with the AVL tree.
 * @details Recursively deallocates each node, the data within each node, and the
 * tree structure itself. The tree pointer becomes invalid after this call.
 * @param tree A pointer to the AVL tree to be destroyed.
 */
void AVLTree_destroy(AVLTree* bst);

/**
 * @brief Inserts an element into the AVL tree, maintaining the balance property.
 * @details If the insertion causes an imbalance, the tree will perform the
 * necessary rotations (LL, RR, LR, or RL) to rebalance itself.
 * @param tree A pointer to the AVL tree.
 * @param element A pointer to the element data to be copied into the tree.
 * @return `STATUS_OK` on successful insertion.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if tree or data is NULL.
 * @return `STATUS_ERR_DUPLICATE_KEY` if an element with the same key already exists.
 * @return `STATUS_ERR_ALLOC` if memory allocation for the new node fails.
 */
STATUS AVLTree_insert(AVLTree* avl, void* element);

/**
 * @brief Deletes an element with a specific key from the AVL tree.
 * @details After deletion, the tree may become unbalanced. This function will
 * perform the necessary rotations to restore the AVL property.
 * @param tree A pointer to the AVL tree.
 * @param key A pointer to the key of the element to delete.
 * @return `STATUS_OK` on successful deletion.
 * @return `STATUS_ERR_INVALID_ARGUMENT` if tree or key is NULL.
 * @return `STATUS_ERR_KEY_NOT_FOUND` if no element with the given key is found.
 */
STATUS AVLTree_delete(AVLTree* bst, void* key);

/**
 * @brief Searches for an element with a specific key in the AVL tree.
 * @details The search operation has a time complexity of O(log n).
 * @param tree A pointer to the AVL tree.
 * @param key A pointer to the key of the element to search for.
 * @return A pointer to the data of the found element within the tree.
 * @return `NULL` if the key is not found or if arguments are invalid.
 */
void* AVLTree_search(AVLTree* bst, void* key);

/**
 * @brief Traverses the tree in-order (Left, Root, Right) and applies a callback.
 * @details For a binary search tree, an in-order traversal visits nodes in ascending sorted order.
 * @param tree A pointer to the AVL tree.
 * @param callback A function to be called for each node's data.
 * @return `STATUS_OK` on success, or `STATUS_ERR_INVALID_ARGUMENT`.
 */
STATUS AVLTree_traverseInorder(AVLTree* bst, void (*callback)(void *));

/**
 * @brief Traverses the tree in pre-order (Root, Left, Right) and applies a callback.
 * @param tree A pointer to the AVL tree.
 * @param callback A function to be called for each node's data.
 * @return `STATUS_OK` on success, or `STATUS_ERR_INVALID_ARGUMENT`.
 */
STATUS AVLTree_traversePreorder(AVLTree* bst, void (*callback)(void *));

/**
 * @brief Traverses the tree in post-order (Left, Right, Root) and applies a callback.
 * @param tree A pointer to the AVL tree.
 * @param callback A function to be called for each node's data.
 * @return `STATUS_OK` on success, or `STATUS_ERR_INVALID_ARGUMENT`.
 */
STATUS AVLTree_traversePostorder(AVLTree* bst, void (*callback)(void *));

#endif // AVLTREE_H