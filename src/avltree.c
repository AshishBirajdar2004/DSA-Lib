#include "../include/avltree.h"

/**
 * @internal
 * @struct AVLNode
 * @brief Defines the structure of a single node in the AVL Tree.
 */
typedef struct AVLNode
{
    void* data;             // Pointer to the stored data, which is heap-allocated.
    struct AVLNode* left;   // Pointer to the left child node.
    struct AVLNode* right;  // Pointer to the right child node.
    int height;             // The height of the subtree rooted at this node.
} AVLNode;

/**
 * @internal
 * @struct AVLTree
 * @brief Defines the internal structure of the AVL Tree.
 */
struct AVLTree
{
    AVLNode* root;                              // Pointer to the root node of the tree.
    size_t dataSize;                            // The size in bytes of the data stored in each node.
    int (*cmp)(const void *, const void *);     // Function to compare two elements.
};

/* --------------------------------------Creation & Destruction-------------------------------------- */

AVLTree* AVLTree_init(size_t datasize, int (*cmp)(const void *, const void *))
{
    if (!cmp || datasize == 0) return NULL;

    AVLTree* avl = malloc(sizeof(AVLTree));
    if (!avl) return NULL;

    avl->root = NULL;
    avl->dataSize = datasize;
    avl->cmp = cmp;
    return avl;
}

/**
 * @internal
 * @brief Recursively frees a node and all its descendants.
 */
static void _AVLTree_destroyNode(AVLNode* root)
{
    if (!root) return;
    _AVLTree_destroyNode(root->left);
    _AVLTree_destroyNode(root->right);
    free(root->data);
    free(root);
}

void AVLTree_destroy(AVLTree* avl)
{
    if (!avl) return;
    _AVLTree_destroyNode(avl->root);
    free(avl);
}

/* -----------------------------------Private Helpers for Balancing----------------------------------- */

/**
 * @internal
 * @brief Gets the height of a node. Returns -1 for a NULL node.
 */
static int _AVLTree_getHeight(AVLNode* node)
{
    if (!node) return -1;
    return node->height;
}

/**
 * @internal
 * @brief Updates the height of a node based on the height of its children.
 */
static void _AVLTree_updateHeight(AVLNode* root)
{
    if (root) {
        int leftHeight = _AVLTree_getHeight(root->left);
        int rightHeight = _AVLTree_getHeight(root->right);
        root->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }
}

/**
 * @internal
 * @brief Calculates the balance factor of a node (height of left subtree - height of right subtree).
 */
static int _AVLTree_getBalanceFactor(AVLNode* root)
{
    if (!root) return 0;
    return (_AVLTree_getHeight(root->left) - _AVLTree_getHeight(root->right));
}

/**
 * @internal
 * @brief Allocates a new node and copies the provided data into it.
 */
static AVLNode* _AVLTree_getNewNode(size_t dataSize, void *element)
{
    if (!element) return NULL;

    AVLNode* newNode = malloc(sizeof(AVLNode));
    if (!newNode) return NULL;

    newNode->data = malloc(dataSize);
    if (!newNode->data)
    {
        free(newNode);
        return NULL;
    }

    memcpy(newNode->data, element, dataSize);
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 0; // Height of a new leaf node is 0
    return newNode;
}

/**
 * @internal
 * @brief Performs a right rotation on the subtree rooted at `root`.
 * Used to fix a left-heavy imbalance (LL case).
 */
static AVLNode* _AVLTree_rightRotate(AVLNode* root)
{
    AVLNode* x = root->left;
    AVLNode* y = x->right;

    // Perform rotation
    x->right = root;
    root->left = y;

    // Update heights
    _AVLTree_updateHeight(root);
    _AVLTree_updateHeight(x);

    return x; // New root of the subtree
}

/**
 * @internal
 * @brief Performs a left rotation on the subtree rooted at `root`.
 * Used to fix a right-heavy imbalance (RR case).
 */
static AVLNode* _AVLTree_leftRotate(AVLNode* root)
{
    AVLNode* x = root->right;
    AVLNode* y = x->left;

    // Perform rotation
    x->left = root;
    root->right = y;

    // Update heights
    _AVLTree_updateHeight(root);
    _AVLTree_updateHeight(x);

    return x; // New root of the subtree
}

/* ------------------------------------------Insertion Logic------------------------------------------ */

/**
 * @internal
 * @brief Recursively inserts a node and then rebalances the tree on the way back up.
 */
static AVLNode* _AVLTree_insertNode(AVLNode* root, size_t dataSize, void* data, int (*cmp)(const void *, const void *), InsertStatus* status)
{
    // 1. Standard BST insertion
    if (!root) {
        root = _AVLTree_getNewNode(dataSize, data);
        if (root) status->isInserted = true;
        else status->isAllocFailed = true;
        return root;
    }
    else if (cmp(data, root->data) < 0)
        root->left = _AVLTree_insertNode(root->left, dataSize, data, cmp, status);
    else if (cmp(data, root->data) > 0)
        root->right = _AVLTree_insertNode(root->right, dataSize, data, cmp, status);
    else {
        status->isDuplicate = true;
        return root;
    }

    // 2. Update height of the current node
    _AVLTree_updateHeight(root);

    // 3. Get balance factor and check for imbalance
    int balanceFactor = _AVLTree_getBalanceFactor(root);

    // 4. Perform rotations if needed
    // Left-Left (LL) Case
    // LL -> happens when BF at imbalanced node is 2 and key < root.left.data
    if (balanceFactor > 1 && root->left && cmp(data, root->left->data) < 0)
        return _AVLTree_rightRotate(root);

    // Right-Right (RR) Case
    // RR -> happens when BF at imbalanced node is -2 and key > root.right.data
    if (balanceFactor < -1 && root->right && cmp(data, root->right->data) > 0)
        return _AVLTree_leftRotate(root);

    // Left-Right (LR) Case
    // LR -> happens when BF at imbalanced node is 2 and key > root.left.data
    if (balanceFactor > 1 && root->left && cmp(data, root->left->data) > 0)
    {
        root->left = _AVLTree_leftRotate(root->left);
        return _AVLTree_rightRotate(root);
    }

    // Right-Left (RL) Case
    // RL -> happens when BF at imbalanced node is -2 and key < root.right.data
    if (balanceFactor < -1 && root->right && cmp(data, root->right->data) < 0)
    {
        root->right = _AVLTree_rightRotate(root->right);
        return _AVLTree_leftRotate(root);
    }

    return root;
}

STATUS AVLTree_insert(AVLTree* avl, void* element)
{
    if (!avl || !element) return STATUS_ERR_INVALID_ARGUMENT;

    InsertStatus status = {0};

    avl->root = _AVLTree_insertNode(avl->root, avl->dataSize, element, avl->cmp, &status);
    
    if (status.isInserted) return STATUS_OK;
    if (status.isAllocFailed) return STATUS_ERR_ALLOC;
    if (status.isDuplicate) return STATUS_ERR_DUPLICATE_KEY;
    
    return STATUS_ERR_UNKNOWN;
}

/* ----------------------------------------------Deletion Logic---------------------------------------------- */

/**
 * @internal
 * @brief Finds the node with the minimum value in a subtree (the leftmost node).
 */
static AVLNode* _AVLTree_getMin(AVLNode* root) {
    if (!root) return NULL;
    if (!root->left) return root;
    return _AVLTree_getMin(root->left);
}

/**
 * @internal
 * @brief Recursively deletes a node and then rebalances the tree on the way back up.
 */
static AVLNode* _AVLTree_deleteNode(AVLNode* root, void* key, size_t datasize, int (*cmp)(const void *, const void *), DeleteStatus* status) {
    // 1. Standard BST deletion
    if (!root) {
        status->isKeyNotFound = true;
        return NULL;
    }
    else if (cmp(key, root->data) < 0)
        root->left = _AVLTree_deleteNode(root->left, key, datasize, cmp, status);
    else if (cmp(key, root->data) > 0)
        root->right = _AVLTree_deleteNode(root->right, key, datasize, cmp, status);
    else { // Node to be deleted found
        // No child
        if (!root->left && !root->right) {
            free(root->data);
            free(root);
            root = NULL;
        }
        // One child
        else if (!root->left) {
            AVLNode* temp = root;
            root = root->right;
            free(temp->data);
            free(temp);
            temp = NULL;
        }
        else if (!root->right) {
            AVLNode* temp = root;
            root = root->left;
            free(temp->data);
            free(temp);
            temp = NULL;
        }
        // Node with 2 children: get in-order successor (smallest in right subtree)
        else {
            AVLNode* inorderSuccessor = _AVLTree_getMin(root->right);
            if (!inorderSuccessor) return root;
            memcpy(root->data, inorderSuccessor->data, datasize);
            // Delete the in-order successor
            root->right = _AVLTree_deleteNode(root->right, inorderSuccessor->data, datasize, cmp, status);
        }
        status->isDeleted = true;
    }

    if (!root) return NULL; // Handle if tree became empty after deletion

    // 2. Update height
    _AVLTree_updateHeight(root);

    // 3. Get balance factor and rebalance if necessary
    int balanceFactor = _AVLTree_getBalanceFactor(root);

    // case - 1 deletion in right causes heavy left subtree
    // R0 -> LL R1 -> LL | R-1 -> LR
    if (balanceFactor > 1 && _AVLTree_getBalanceFactor(root->left) >= 0)
        return _AVLTree_rightRotate(root);
    if (balanceFactor > 1 && _AVLTree_getBalanceFactor(root->left) < 0) {
        root->left = _AVLTree_leftRotate(root->left);
        return _AVLTree_rightRotate(root);
    }

    // case - 2 deletion in left causes heavy right subtree
    // L0 -> RR L-1 -> RR | L1 -> RL
    if (balanceFactor < -1 && _AVLTree_getBalanceFactor(root->right) <= 0)
        return _AVLTree_leftRotate(root);
    if (balanceFactor < -1 && _AVLTree_getBalanceFactor(root->right) > 0) {
        root->left = _AVLTree_rightRotate(root->left);
        return _AVLTree_leftRotate(root);
    }

    return root;    
}

STATUS AVLTree_delete(AVLTree* avl, void* key) {
    if (!avl || !key) return STATUS_ERR_INVALID_ARGUMENT;

    DeleteStatus status = {0};

    avl->root = _AVLTree_deleteNode(avl->root, key, avl->dataSize, avl->cmp, &status);

    if (status.isDeleted) return STATUS_OK;
    if (status.isKeyNotFound) return STATUS_ERR_KEY_NOT_FOUND;

    return STATUS_ERR_UNKNOWN;
}

/* ----------------------------------------------Search Logic---------------------------------------------- */

/**
 * @internal
 * @brief Recursively searches for a node with the given key.
 */
static AVLNode* _AVLTree_searchNode(AVLNode* root, void* key, int (*cmp)(const void *, const void *))
{
    if (!root || cmp(key, root->data) == 0)
        return root;
    else if (cmp(key, root->data) < 0)
        return _AVLTree_searchNode(root->left, key, cmp);
    else
        return _AVLTree_searchNode(root->right, key, cmp);
}

void* AVLTree_search(AVLTree* avl, void* key)
{
    if (!avl || !key) return NULL;
    AVLNode* found = _AVLTree_searchNode(avl->root, key, avl->cmp);
    return (found) ? found->data : NULL;
}

/* --------------------------------------------Traversal Logic---------------------------------------------- */
typedef enum {
    TRAVERSAL_INORDER,
    TRAVERSAL_PREORDER,
    TRAVERSAL_POSTORDER,
} TraversalOrder;

/**
 * @internal
 * @brief A single recursive function to handle all three traversal types.
 */
static void _AVLTree_forEachNode(AVLNode* root, void (*callback)(void *), TraversalOrder order) {
    if (!root || !callback) return;
    if (order == TRAVERSAL_PREORDER) callback(root->data);
    _AVLTree_forEachNode(root->left, callback, order);
    if (order == TRAVERSAL_INORDER) callback(root->data);
    _AVLTree_forEachNode(root->right, callback, order);
    if (order == TRAVERSAL_POSTORDER) callback(root->data);
}

STATUS AVLTree_traverseInorder(AVLTree* avl, void (*callback)(void *)) {
    if (!avl || !callback) return STATUS_ERR_INVALID_ARGUMENT;
    _AVLTree_forEachNode(avl->root, callback, TRAVERSAL_INORDER);
    return STATUS_OK;
}

STATUS AVLTree_traversePreorder(AVLTree* avl, void (*callback)(void *)) {
    if (!avl || !callback) return STATUS_ERR_INVALID_ARGUMENT;
    _AVLTree_forEachNode(avl->root, callback, TRAVERSAL_PREORDER);
    return STATUS_OK;
}

STATUS AVLTree_traversePostorder(AVLTree* avl, void (*callback)(void *)) {
    if (!avl || !callback) return STATUS_ERR_INVALID_ARGUMENT;
    _AVLTree_forEachNode(avl->root, callback, TRAVERSAL_POSTORDER);
    return STATUS_OK;
}