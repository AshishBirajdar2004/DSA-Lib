#include "../include/bst.h"

BST* BST_init(size_t datasize, int (*cmp)(const void *, const void *))
{
    if (!cmp || datasize == 0) return NULL;

    BST *bst = malloc(sizeof(BST));
    if (!bst) return NULL;

    bst->root = NULL;
    bst->dataSize = datasize;
    bst->cmp = cmp;
    return bst;
}

static void _BST_destroyNode(BSTNode *root)
{
    if (!root) return;
    _BST_destroyNode(root->left);
    _BST_destroyNode(root->right);
    free(root->data);
    free(root);
}

void BST_destroy(BST *bst)
{
    if (!bst) return;
    _BST_destroyNode(bst->root);
    free(bst);
}

// -------------------------------------------------------------------------

static int _BST_getHeight(BSTNode* node)
{
    if (!node) return -1;
    return node->height;
}

static void _BST_updateHeight(BSTNode* root)
{
    if (root) {
        int leftHeight = _BST_getHeight(root->left);
        int rightHeight = _BST_getHeight(root->right);
        root->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }
}

static int _BST_getBalanceFactor(BSTNode *root)
{
    if (!root) return 0;
    return (_BST_getHeight(root->left) - _BST_getHeight(root->right));
}

static BSTNode* _BST_getNewNode(size_t dataSize, void *data)
{
    if (!data) return NULL;

    BSTNode* newNode = malloc(sizeof(BSTNode));
    if (!newNode) return NULL;

    newNode->data = malloc(dataSize);
    if (!newNode->data)
    {
        free(newNode);
        return NULL;
    }

    memcpy(newNode->data, data, dataSize);
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 0;
    return newNode;
}

static BSTNode* _BST_rightRotate(BSTNode* root)
{
    BSTNode* x = root->left;
    BSTNode* y = x->right;

    x->right = root;
    root->left = y;

    _BST_updateHeight(root);
    _BST_updateHeight(x);

    return x;
}

static BSTNode* leftRotate(BSTNode* root)
{
    BSTNode* x = root->right;
    BSTNode* y = x->left;

    x->left = root;
    root->right = y;

    _BST_updateHeight(root);
    _BST_updateHeight(x);

    return x;
}

static BSTNode* _BST_insertNode(BSTNode* root, size_t dataSize, void* data, int (*cmp)(const void *, const void *), InsertStatus* status)
{
    if (!root) {
        root = _BST_getNewNode(dataSize, data);
        if (root) status->isInserted = true;
        else status->isAllocFailed = true;
        return root;
    }
    else if (cmp(data, root->data) < 0)
        root->left = _BST_insertNode(root->left, dataSize, data, cmp, status);
    else if (cmp(data, root->data) > 0)
        root->right = _BST_insertNode(root->right, dataSize, data, cmp, status);
    else {
        status->isDuplicate = true;
        return root;
    }

    _BST_updateHeight(root);

    int balanceFactor = _BST_getBalanceFactor(root);

    // LL -> happens when BF at imbalanced node is 2 and key < root.left.data
    if (balanceFactor > 1 && root->left && cmp(data, root->left->data) < 0)
        return _BST_rightRotate(root);
    // RR -> happens when BF at imbalanced node is -2 and key > root.right.data
    if (balanceFactor < -1 && root->right && cmp(data, root->right->data) > 0)
        return leftRotate(root);
    // LR -> happens when BF at imbalanced node is 2 and key > root.left.data
    if (balanceFactor > 1 && root->left && cmp(data, root->left->data) > 0)
    {
        root->left = leftRotate(root->left);
        return _BST_rightRotate(root);
    }
    // RL -> happens when BF at imbalanced node is -2 and key < root.right.data
    if (balanceFactor < -1 && root->right && cmp(data, root->right->data) < 0)
    {
        root->right = _BST_rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

STATUS BST_insert(BST *bst, void *data)
{
    if (!bst || !data) return STATUS_ERR_INVALID_ARGUMENT;

    InsertStatus status = {0};

    bst->root = _BST_insertNode(bst->root, bst->dataSize, data, bst->cmp, &status);
    
    if (status.isInserted) return STATUS_OK;
    if (status.isAllocFailed) return STATUS_ERR_ALLOC;
    if (status.isDuplicate) return STATUS_ERR_DUPLICATE_KEY;
    
    return STATUS_ERR_UNKNOWN;
}

// -------------------------------------------------------------------------

static BSTNode* _BST_getMin(BSTNode* root) {
    if (!root) return NULL;
    if (!root->left) return root;
    return _BST_getMin(root->left);
}

static BSTNode* _BST_deleteNode(BSTNode* root, void* key, size_t datasize, int (*cmp)(const void *, const void *), DeleteStatus* status) {
    if (!root) {
        status->isKeyNotFound = true;
        return NULL;
    }
    else if (cmp(key, root->data) < 0)
        root->left = _BST_deleteNode(root->left, key, datasize, cmp, status);
    else if (cmp(key, root->data) > 0)
        root->right = _BST_deleteNode(root->right, key, datasize, cmp, status);
    else {
        if (!root->left && !root->right) {
            free(root->data);
            free(root);
            root = NULL;
        }
        else if (!root->left) {
            BSTNode* temp = root;
            root = root->right;
            free(temp->data);
            free(temp);
            temp = NULL;
        }
        else if (!root->right) {
            BSTNode* temp = root;
            root = root->left;
            free(temp->data);
            free(temp);
            temp = NULL;
        }
        else {
            BSTNode* inorderSuccessor = _BST_getMin(root->right);
            if (!inorderSuccessor) return root;
            memcpy(root->data, inorderSuccessor->data, datasize);
            root->right = _BST_deleteNode(root->right, inorderSuccessor->data, datasize, cmp, status);
        }
        status->isDeleted = true;
    }

    if (!root) return NULL;

    _BST_updateHeight(root);

    int balanceFactor = _BST_getBalanceFactor(root);

    // case - 1 deletion in right causes heavy left subtree
    // R0 -> LL R1 -> LL | R-1 -> LR
    if (balanceFactor > 1 && _BST_getBalanceFactor(root->left) >= 0)
        return _BST_rightRotate(root);
    if (balanceFactor > 1 && _BST_getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return _BST_rightRotate(root);
    }

    // case - 2 deletion in left causes heavy right subtree
    // L0 -> RR L-1 -> RR | L1 -> RL
    if (balanceFactor < -1 && _BST_getBalanceFactor(root->right) <= 0)
        return leftRotate(root);
    if (balanceFactor < -1 && _BST_getBalanceFactor(root->right) > 0) {
        root->left = _BST_rightRotate(root->left);
        return leftRotate(root);
    }

    return root;    
}

STATUS BST_delete(BST* bst, void* key) {
    if (!bst || !key) return STATUS_ERR_INVALID_ARGUMENT;

    DeleteStatus status = {0};

    bst->root = _BST_deleteNode(bst->root, key, bst->dataSize, bst->cmp, &status);

    if (status.isDeleted) return STATUS_OK;
    if (status.isKeyNotFound) return STATUS_ERR_KEY_NOT_FOUND;

    return STATUS_ERR_UNKNOWN;
}

// -------------------------------------------------------------------------

static BSTNode* _BST_searchNode(BSTNode* root, void* key, int (*cmp)(const void *, const void *))
{
    if (!root || cmp(key, root->data) == 0)
        return root;
    else if (cmp(key, root->data) < 0)
        return _BST_searchNode(root->left, key, cmp);
    else
        return _BST_searchNode(root->right, key, cmp);
}

void* BST_search(BST* bst, void* key)
{
    if (!bst || !key) return NULL;
    BSTNode* found = _BST_searchNode(bst->root, key, bst->cmp);
    return (found) ? found->data : NULL;
}

// -------------------------------------------------------------------------

typedef enum {
    TRAVERSAL_INORDER,
    TRAVERSAL_PREORDER,
    TRAVERSAL_POSTORDER,
} TraversalOrder;

static void _BST_forEachNode(BSTNode* root, void (*callback)(void *), TraversalOrder order) {
    if (!root || !callback) return;
    if (order == TRAVERSAL_PREORDER) callback(root->data);
    _BST_forEachNode(root->left, callback, order);
    if (order == TRAVERSAL_INORDER) callback(root->data);
    _BST_forEachNode(root->right, callback, order);
    if (order == TRAVERSAL_POSTORDER) callback(root->data);
}

STATUS BST_traverseInorder(BST* bst, void (*callback)(void *)) {
    if (!bst || !callback) return STATUS_ERR_INVALID_ARGUMENT;
    _BST_forEachNode(bst->root, callback, TRAVERSAL_INORDER);
    return STATUS_OK;
}

STATUS BST_traversePreorder(BST* bst, void (*callback)(void *)) {
    if (!bst || !callback) return STATUS_ERR_INVALID_ARGUMENT;
    _BST_forEachNode(bst->root, callback, TRAVERSAL_PREORDER);
    return STATUS_OK;
}

STATUS BST_traversePostorder(BST* bst, void (*callback)(void *)) {
    if (!bst || !callback) return STATUS_ERR_INVALID_ARGUMENT;
    _BST_forEachNode(bst->root, callback, TRAVERSAL_POSTORDER);
    return STATUS_OK;
}