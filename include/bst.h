#ifndef BST_H
#define BST_H

#include "common.h"

/**
 * BSTNode structure for the Binary Search Tree.
 * Holds a pointer to the stored data, left and right child pointers, and the node's height.
 */
typedef struct BSTNode
{
    void* data;
    struct BSTNode* left;
    struct BSTNode* right;
    int height;
} BSTNode;

/**
 * BST structure.
 * Holds a pointer to the root node, size of each data element, and a general comparator function.
 */
typedef struct BST
{
    BSTNode* root;
    size_t dataSize;
    int (*cmp)(const void *, const void *);
} BST;

/* === FUNCTION DECLARATIONS === */

/**
 * Initialize a new Binary Search Tree.
 * @param dataSize Size of each data element in bytes.
 * @param cmp General comparator function for comparing two elements.
 * @return Pointer to the newly created BST structure, or NULL on failure.
 */
BST* BST_init(size_t dataSize, int (*cmp)(const void *, const void *));

/**
 * Destroy the entire Binary Search Tree and free all associated memory.
 * @param bst Pointer to the BST.
 */
void BST_destroy(BST *bst);

/**
 * Insert a new element into the Binary Search Tree.
 * The tree will be self-balanced automatically.
 * @param bst Pointer to the BST.
 * @param data Pointer to the data to insert.
 * @return STATUS_OK if inserted successfully,
 *         STATUS_ERR_ALLOC if memory allocation failed,
 *         STATUS_ERR_DUPLICATE_KEY if a duplicate key is detected,
 *         STATUS_ERR_INVALID_ARGUMENT if arguments are invalid,
 *         STATUS_ERR_UNKNOWN for any other failure.
 */
STATUS BST_insert(BST *bst, void *data);

/**
 * Delete an existing element from the Binary Search Tree.
 * The tree will be self-balanced automatically.
 * @param bst Pointer to the BST.
 * @param key Pointer to the key to delete.
 * @return STATUS_OK if deleted successfully,
 *         STATUS_ERR_KEY_NOT_FOUND if the key does not exist,
 *         STATUS_ERR_INVALID_ARGUMENT if arguments are invalid,
 *         STATUS_ERR_UNKNOWN for any other failure.
 */
STATUS BST_delete(BST* bst, void* key);

/**
 * Search for an element in the Binary Search Tree.
 * @param bst Pointer to the BST.
 * @param key Pointer to the key to search for.
 * @return Pointer to the stored data if found, or NULL if not found.
 * @note The returned pointer must be type-cast to the appropriate data type.
 */
void* BST_search(BST* bst, void* key);

/**
 * Traverse the BST in inorder and apply a callback function to each node's data.
 * @param bst Pointer to the BST.
 * @param callback Function to apply to each node's data.
 * @return STATUS_OK on success, STATUS_ERR_INVALID_ARGUMENT if arguments are invalid.
 */
STATUS BST_traverseInorder(BST* bst, void (*callback)(void *));

/**
 * Traverse the BST in preorder and apply a callback function to each node's data.
 * @param bst Pointer to the BST.
 * @param callback Function to apply to each node's data.
 * @return STATUS_OK on success, STATUS_ERR_INVALID_ARGUMENT if arguments are invalid.
 */
STATUS BST_traversePreorder(BST* bst, void (*callback)(void *));

/**
 * Traverse the BST in postorder and apply a callback function to each node's data.
 * @param bst Pointer to the BST.
 * @param callback Function to apply to each node's data.
 * @return STATUS_OK on success, STATUS_ERR_INVALID_ARGUMENT if arguments are invalid.
 */
STATUS BST_traversePostorder(BST* bst, void (*callback)(void *));


#endif