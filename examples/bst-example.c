#include <stdio.h>
#include <string.h>
#include <dsa-lib/bst.h>

// ====== Custom Complex struct ======
typedef struct
{
    int id;
    char name[50];
    double salary;
} Person;

// ====== Comparators for different data types ======
int cmp_int(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

int cmp_double(const void *a, const void *b)
{
    double diff = (*(double *)a - *(double *)b);
    return (diff > 0) - (diff < 0); // 1 if >, -1 if <, 0 if equal
}

int cmp_str(const void *a, const void *b)
{
    return strcmp(*(char **)a, *(char **)b);
}

int cmp_person_by_id(const void *a, const void *b)
{
    const Person *pa = (const Person *)a;
    const Person *pb = (const Person *)b;
    return pa->id - pb->id;
}

// ====== Callbacks for traversals ======
void print_int(void *data)
{
    printf("%d ", *(int *)data);
}

void print_double(void *data)
{
    printf("%.2f ", *(double *)data);
}

void print_str(void *data)
{
    printf("%s ", *(char **)data);
}

void print_person(void *data)
{
    Person *p = (Person *)data;
    printf("[ID=%d, Name=%s, Salary=%.2f] ", p->id, p->name, p->salary);
}

// ====== Test function for integers ======
void test_int_bst()
{
    printf("\n=== Testing Integer BST ===\n");
    BST *bst = BST_init(sizeof(int), cmp_int);

    int nums[] = {10, 20, 5, 15, 25, 3, 7};

    // Insert elements
    for (int i = 0; i < 7; i++)
    {
        STATUS s = BST_insert(bst, &nums[i]);
        printf("Insert %d: %s\n", nums[i], s == STATUS_OK ? "OK" : (s == STATUS_ERR_DUPLICATE_KEY ? "Duplicate" : "Error"));
    }

    // Try duplicate insert
    STATUS dup = BST_insert(bst, &nums[0]);
    printf("Insert duplicate %d: %s\n", nums[0], dup == STATUS_ERR_DUPLICATE_KEY ? "Duplicate Detected" : "Error");

    // Traversals
    printf("Inorder: ");
    BST_traverseInorder(bst, print_int);
    printf("\n");
    printf("Preorder: ");
    BST_traversePreorder(bst, print_int);
    printf("\n");
    printf("Postorder: ");
    BST_traversePostorder(bst, print_int);
    printf("\n");

    // Search existing & non-existing
    int key1 = 15, key2 = 99;
    int *found1 = BST_search(bst, &key1);
    printf("Search %d: %s\n", key1, found1 ? "Found" : "Not Found");
    int *found2 = BST_search(bst, &key2);
    printf("Search %d: %s\n", key2, found2 ? "Found" : "Not Found");

    // Delete existing
    STATUS del1 = BST_delete(bst, &key1);
    printf("Delete %d: %s\n", key1, del1 == STATUS_OK ? "Deleted" : "Error");
    // Delete non-existing
    STATUS del2 = BST_delete(bst, &key2);
    printf("Delete %d: %s\n", key2, del2 == STATUS_ERR_KEY_NOT_FOUND ? "Key Not Found" : "Error");

    printf("Inorder after deletion: ");
    BST_traverseInorder(bst, print_int);
    printf("\n");

    BST_destroy(bst);
}

// ====== Test function for doubles ======
void test_double_bst()
{
    printf("\n=== Testing Double BST ===\n");
    BST *bst = BST_init(sizeof(double), cmp_double);

    double nums[] = {1.5, 3.2, 0.7, 2.8, 4.9};

    for (int i = 0; i < 5; i++)
        BST_insert(bst, &nums[i]);

    printf("Inorder: ");
    BST_traverseInorder(bst, print_double);
    printf("\n");

    double key = 2.8;
    BST_delete(bst, &key);
    printf("After deleting %.2f: ", key);
    BST_traverseInorder(bst, print_double);
    printf("\n");

    BST_destroy(bst);
}

// ====== Test function for strings ======
void test_str_bst()
{
    printf("\n=== Testing String BST ===\n");
    BST *bst = BST_init(sizeof(char *), cmp_str);

    char *words[] = {"apple", "banana", "cherry", "date", "elderberry"};

    for (int i = 0; i < 5; i++)
        BST_insert(bst, &words[i]);

    printf("Inorder: ");
    BST_traverseInorder(bst, print_str);
    printf("\n");

    char *searchKey = "banana";
    char **found = BST_search(bst, &searchKey);
    printf("Search %s: %s\n", searchKey, found ? "Found" : "Not Found");

    char *delKey = "apple";
    BST_delete(bst, &delKey);
    printf("After deleting %s: ", delKey);
    BST_traverseInorder(bst, print_str);
    printf("\n");

    BST_destroy(bst);
}

// ====== Test function for complex struct ======
void test_person_bst()
{
    printf("\n=== Testing Complex Struct BST (Person) ===\n");
    BST *bst = BST_init(sizeof(Person), cmp_person_by_id);

    Person people[] = {
        {3, "Alice", 55000.0},
        {1, "Bob", 45000.0},
        {5, "Charlie", 70000.0},
        {2, "David", 50000.0},
        {4, "Eve", 65000.0}};

    for (int i = 0; i < 5; i++)
    {
        STATUS s = BST_insert(bst, &people[i]);
        printf("Insert %s (ID=%d): %s\n", people[i].name, people[i].id,
               s == STATUS_OK ? "OK" : (s == STATUS_ERR_DUPLICATE_KEY ? "Duplicate" : "Error"));
    }

    // Duplicate insert
    STATUS dup = BST_insert(bst, &people[0]);
    printf("Insert duplicate ID=%d: %s\n", people[0].id,
           dup == STATUS_ERR_DUPLICATE_KEY ? "Duplicate Detected" : "Error");

    // Traversal
    printf("Inorder by ID: ");
    BST_traverseInorder(bst, print_person);
    printf("\n");

    // Search
    Person key = {2};
    Person *found = BST_search(bst, &key);
    printf("Search ID=%d: %s\n", key.id, found ? found->name : "Not Found");

    // Delete existing
    printf("Delete ID=%d: %s\n", key.id,
           BST_delete(bst, &key) == STATUS_OK ? "Deleted" : "Error");

    // Delete non-existing
    Person notExist = {99};
    printf("Delete ID=%d: %s\n", notExist.id,
           BST_delete(bst, &notExist) == STATUS_ERR_KEY_NOT_FOUND ? "Key Not Found" : "Error");

    printf("Inorder after deletions: ");
    BST_traverseInorder(bst, print_person);
    printf("\n");

    BST_destroy(bst);
}

//  Driver Code
int main()
{
    test_int_bst();
    test_double_bst();
    test_str_bst();
    test_person_bst();
    return 0;
}
