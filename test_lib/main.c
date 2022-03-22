#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker, "/HEAP:2000000")
#include <stdio.h>
#include <stdlib.h>
#include "ccvector.h"
#include "ccstack.h"
#include "cchashtable.h"
#include "ccheap.h"
#include "cctree.h"
#include "list.h"
#include <time.h>

int TestVector();
int TestStack();
int TestHashTable();
int TestHeap();
int TestTree();
void RunTests();

#define TREE_NR_ELEMENTS 6
#define NR_TESTS 2
#define NR_ELEM 10000000
#define MAX_KEY_LEN 2 // not inclusive
#define MIN_KEY_LEN 1 // inclusive
#define MIN_VALUE 1
#define MAX_VALUE 99


int GetRandomValue(int MinValue, int MaxValue)
{
    int value = rand() % (MaxValue - MinValue + 1) + MinValue;
    //if (rand() % 2 == 0)
    //{
    //    value = value * (-1);
    //}
    return value;
}

int main(void)
{
    RunTests();
}

void RunTests()
{
    /// NOTE: The tests provided here are by no means exhaustive and are only
    /// provided as a starting point (not all functions are tested, not all use cases
    /// and failure scenarios are covered). You are encouraged to expand these tests
    /// to include missing scenarios.
    if (0 == TestVector())
    {
        printf("Vector test passed\n\n");
    }
    else
    {
        printf("Vector test failed\n\n");
    }

    if (0 == TestStack())
    {
        printf("Stack test passed\n\n");
    }
    else
    {
        printf("Stack test failed\n\n");
    }

    if (0 == TestHashTable())
    {
        printf("HashTable test passed\n\n");
    }
    else
    {
        printf("HashTable test failed\n\n");
    }

    if (0 == TestHeap())
    {
        printf("Heap test passed\n\n");
    }
    else
    {
        printf("Heap test failed\n\n");
    }

    if (0 == TestTree())
    {
        printf("Tree test passed\n\n");
    }
    else
    {
        printf("Tree test failed\n\n");
    }
}

int TestTree()
{
    /*
    srand((unsigned)time(0));

    CC_TREE* tree = NULL;
    int value = 0;
    int isRedBlack = 0;

    if (0 != TreeCreate(&tree))
    {
        printf("TreeCreate failed\n");
        goto cleanup;
    }

    for (int i = 0; i < TREE_NR_ELEMENTS; i++)
    {
        value = GetRandomValue(MIN_VALUE, MAX_VALUE);
        if (0 != TreeInsert(tree, value))
        {
            printf("TreeInsert failed\n");
            goto cleanup;
        }
    }

    _TreePrintPreorder(tree, tree->Root);

    isRedBlack = _IsRedBlack(tree);

    if (-1 == isRedBlack)
    {
        printf("_IsRedBlack failed\n");
        goto cleanup;
    }
    else if (1 == isRedBlack)
    {
        printf("Tree is red-black\n");
    }
    else
    {
        printf("Tree is not red-black\n");
    }

    if (0 != TreeClear(tree))
    {
        printf("TreeClear failed\n");
        goto cleanup;
    }

cleanup:
    if (0 != TreeDestroy(&tree))
    {
        printf("Tree destroy failed\n");
        return -1;
    }
    if (NULL != tree)
    {
        printf("Pointer to tree not NULL after tree has been destoryed");
        return -1;
    }

    RunTests();

    return 0;
    */

    /*// RANDOM TEST

    srand((unsigned)time(0));
    CC_TREE* Tree = NULL;
    CC_VECTOR* vector = NULL;
    int status = 0;
    /*
    int value = 0;
    int index = -1;
    int operation = 0;
    int const NR_TESTS = 10000;
    int const MAX_VALUE = 100000;

    status = TreeCreate(&Tree);
    if (0 != status)
    {
        printf("Tree creation failed\n");
        goto cleanup;
    }

    status = VecCreate(&vector);
    if (0 != status)
    {
        printf("Vector creation failed\n");
        goto cleanup;
    }

    for (int i = 0; i < NR_TESTS; i++)
    {
        operation = rand() % 5;

        // Remove
        if (i > 10 && operation % 5 == 0)
        {
            index = rand() % vector->Count;

            if (0 != VecGetValueByIndex(vector, index, &value))
            {
                goto cleanup;
            }
            while (0 == _VecGetIndexByValue(vector, &index, value))
            {
                if (0 != VecRemoveByIndex(vector, index))
                {
                    goto cleanup;
                }
            }
            if (0 != TreeRemove(Tree, value))
            {
                goto cleanup;
            }
        }
        // Insert
        else
        {
            value = rand() % MAX_VALUE + 1;
            if (0 != VecInsertHead(vector, value))
            {
                goto cleanup;
            }
            if (0 != TreeInsert(Tree, value))
            {
                goto cleanup;
            }
        }

        if (i % 1000 == 0)
        {
            _TreePrintPreorder(Tree, Tree->Root);
            putchar('\n');
            putchar('\n');
            putchar('\n');
        }

        if (1 != _IsRedBlack(Tree))
        {
            printf("Bad luck mate\nLoop iteration: %d\n\n\n", i);
            _TreePrintPreorder(Tree, Tree->Root);
        }
    }

    while (vector->Count > 0)
    {
        index = 0;
        if (0 != VecGetValueByIndex(vector, index, &value))
        {
            goto cleanup;
        }
        while (0 == _VecGetIndexByValue(vector, &index, value))
        {
            if (0 != VecRemoveByIndex(vector, index))
            {
                goto cleanup;
            }
        }

        if (0 != TreeRemove(Tree, value))
        {
            goto cleanup;
        }

        if (Tree->Count > 0)
        {
            if (1 != _IsRedBlack(Tree))
            {
                printf("Bad luck mate\nVector count: %d\n\n\n", vector->Count);
            }
        }
        else
        {
            printf("Tree is empty\n");
        }
    }
    */

    // STATIC TEST
    /*
    int value = 0;
    int preorderPos = 0;
    int inorderPos = 0;
    int postorderPos = 0;

    _TreePrintPreorder(Tree, Tree->Root);
    printf("Is red black: %d\n\n", _IsRedBlack(Tree));
    printf("The tree has %d elements\n", TreeGetCount(Tree));
    printf("Tree contains element 1: %d\n\n", TreeContains(Tree, 1));

    TreeInsert(Tree, 7);
    TreeInsert(Tree, 3);
    TreeInsert(Tree, 9);
    TreeInsert(Tree, 1);
    TreeInsert(Tree, 2);
    _TreePrintPreorder(Tree, Tree->Root);
    printf("Is red black: %d\n\n", _IsRedBlack(Tree));

    printf("The tree has %d elements\n", TreeGetCount(Tree));
    printf("Tree contains element 1: %d\n", TreeContains(Tree, 1));
    printf("Tree contains element 4: %d\n", TreeContains(Tree, 4));
    printf("Tree contains element 10: %d\n", TreeContains(Tree, 10));

    TreeInsert(Tree, 11);
    _TreePrintPreorder(Tree, Tree->Root);
    printf("Is red black: %d\n\n", _IsRedBlack(Tree));

    TreeInsert(Tree, 10);
    _TreePrintPreorder(Tree, Tree->Root);
    printf("Is red black: %d\n\n", _IsRedBlack(Tree));

    TreeInsert(Tree, 4);
    _TreePrintPreorder(Tree, Tree->Root);
    printf("Is red black: %d\n\n", _IsRedBlack(Tree));

    TreeInsert(Tree, 8);
    _TreePrintPreorder(Tree, Tree->Root);
    printf("Is red black: %d\n\n", _IsRedBlack(Tree));

    preorderPos = 7;
    TreeGetNthPreorder(Tree, preorderPos, &value);
    printf("The %dth element in preorder is %d\n", preorderPos, value);

    inorderPos = 9;
    TreeGetNthInorder(Tree, inorderPos, &value);
    printf("The %dth element in inorder is %d\n", inorderPos,value);

    postorderPos = 7;
    TreeGetNthPostorder(Tree, postorderPos, &value);
    printf("The %dth element in postorder is %d\n\n", postorderPos, value);

    TreeRemove(Tree, 7);
    _TreePrintPreorder(Tree, Tree->Root);
    printf("Is red black: %d\n\n", _IsRedBlack(Tree));

    TreeRemove(Tree, 2);
    _TreePrintPreorder(Tree, Tree->Root);
    printf("Is red black: %d\n\n", _IsRedBlack(Tree));
    */

    // INTERACTIVE TEST
    /*
    char a;
    int x;

    TreeInsert(Tree, 7);
    TreeInsert(Tree, 3);
    TreeInsert(Tree, 9);
    TreeInsert(Tree, 1);
    TreeInsert(Tree, 2);

    do
    {
        if (1 != scanf("%c", &a))
        {
            printf("scanf didn't work\n");
            return -1;
        }

        if (a == 'i')
        {
            if (1 != scanf("%d", &x))
            {
                printf("scanf didn't work\n");
                return -1;
            }
            TreeInsert(Tree, x);
            _TreePrintPreorder(Tree, Tree->Root);
            printf("Is red black: %d\n\n", _IsRedBlack(Tree));
        }
        else if (a == 'd')
        {
            if (1 != scanf("%d", &x))
            {
                printf("scanf didn't work\n");
                return -1;
            }
            TreeRemove(Tree, x);
            _TreePrintPreorder(Tree, Tree->Root);
            printf("Is red black: %d\n\n", _IsRedBlack(Tree));

        }
    } while (a != 'e');


    goto cleanup;

cleanup:
    status = TreeDestroy(&Tree);
    if (0 != status || NULL != Tree)
    {
        printf("Cleanup of tree failed failed\n");
        goto cleanup2;
    }
    goto cleanup2;

cleanup2:
    status = VecDestroy(&vector);
    if (0 != status)
    {
        printf("Cleanup of vector failed \n");
        return -1;
    }

    return 0;
    */

    // THEIR TEST
    
    int retVal = -1;
    CC_TREE* usedTree = NULL;

    retVal = TreeCreate(&usedTree);
    if (0 != retVal)
    {
        printf("TreeCreate failed!\n");
        goto cleanup;
    }

    retVal = TreeInsert(usedTree, 20);
    if (0 != retVal)
    {
        printf("TreeInsert failed!\n");
        goto cleanup;
    }

    if (1 != TreeContains(usedTree, 20))
    {
        printf("TreeContains invalid return value!\n");
        retVal = -1;
        goto cleanup;
    }

    retVal = TreeRemove(usedTree, 20);
    if (0 != retVal)
    {
        printf("TreeRemove failed!\n");
        goto cleanup;
    }

    if (0 != TreeContains(usedTree, 20))
    {
        printf("TreeContains invalid return value after remove!\n");
        retVal = -1;
        goto cleanup;
    }

    if (0 != TreeGetCount(usedTree))
    {
        printf("TreeGetCount invalid return value!\n");
        retVal = -1;
        goto cleanup;
    }

cleanup:
    if (NULL != usedTree)
    {
        if (0 != TreeDestroy(&usedTree))
        {
            printf("TreeDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;
 
}
              
int TestHeap()
{
    /*
    // TODO: Create empty heap, perform operations on empty heap, chose random flow of functions, loads of duplicates
    
    srand((unsigned int)time(0));

    int value = 0;
    const unsigned int INITIAL_COUNT = 10;
    const unsigned int INSERT_COUNT = 5;
    const unsigned int MAX_NUMBER = 100;  // not inclusive
    const unsigned int NEG_NR_FREQ = 25;    // in percents
    CC_VECTOR* vector = NULL;
    CC_HEAP* maxHeap = NULL;
    CC_HEAP* minHeap = NULL;
    
    
    // Create vector
    VecCreate(&vector);

    for (unsigned int i = 0; i < INITIAL_COUNT; i++)
    {
        value = rand() % MAX_NUMBER;
        if (rand() % (100 / NEG_NR_FREQ) == 0)
        {
            value = value * (-1);
        }
        VecInsertTail(vector, value);
    }

    // Print the vector
    printf("Vector:\n");
    _VecPrint(vector);
    putchar('\n');
    putchar('\n');

    // Create the heaps
    HpCreateMaxHeap(&maxHeap, vector);
    HpCreateMinHeap(&minHeap, vector);

    // Print the heaps
    printf("Heaps after creation:\n");
    HpPrint(maxHeap);
    putchar('\n');
    HpPrint(minHeap);
    putchar('\n');

    // Check if the heaps are heaps
    if (HpIsHeap(maxHeap) == 1 && HpIsHeap(minHeap) == 1)
    {
        printf("After creation: both are heaps\n\n");
    }


    // Insert in both heaps
    for (unsigned int i = 0; i < INSERT_COUNT; i++)
    {
        value = rand() % MAX_NUMBER;
        if (rand() % (100 / NEG_NR_FREQ) == 0)
        {
            value = value * (-1);
        }
        HpInsert(maxHeap, value);
        HpInsert(minHeap, value);
    }

    // Print them again
    printf("Heaps after insertion:\n");
    HpPrint(maxHeap);
    putchar('\n');
    HpPrint(minHeap);
    putchar('\n');

    // Check if the heaps remained heaps
    if (HpIsHeap(maxHeap) == 1 && HpIsHeap(minHeap) == 1)
    {
        printf("After insertion: both are heaps\n\n");
    }

    // Heap Sort
    printf("Heap sort:\n");
    VecClear(vector);
    HpSortToVector(minHeap, vector);
    _VecPrint(vector);
    if (1 == _VecIsSorted(vector))
    {
        putchar('\n');
        printf("Vector is sorted\n");
    }
    putchar('\n');
    VecClear(vector);
    HpSortToVector(maxHeap, vector);
    _VecPrint(vector);
    if (1 == _VecIsSorted(vector))
    {
        putchar('\n');
        printf("Vector is sorted\n");
    }
    putchar('\n');

    if (HpIsHeap(maxHeap) == 1 && HpIsHeap(minHeap) == 1)
    {
        printf("After heap sort: both are heaps\n\n");
    }

    // Get extreme and pop extreme tests
    printf("Heaps before get/pop extreme:\n\n");
    HpPrint(maxHeap);
    putchar('\n');
    HpPrint(minHeap);
    putchar('\n');

    HpGetExtreme(maxHeap, &value);
    printf("maxHeap extreme = %d\n", value);
    HpGetExtreme(minHeap, &value);
    printf("minHeap extreme = %d\n", value);
    HpPopExtreme(maxHeap, &value);
    printf("\nmaxHeap after popping:\n");
    HpPrint(maxHeap);
    printf("maxHeap popped = %d\n", value);
    HpPopExtreme(minHeap, &value);
    printf("\nminHeap after popping:\n");
    HpPrint(minHeap);
    printf("minHeap popped = %d\n", value);
    if (HpIsHeap(maxHeap) == 1 && HpIsHeap(minHeap) == 1)
    {
        printf("\nAfter popping: both are heaps\n\n");
    }


    printf("\n\nCleanup:\n");
    VecDestroy(&vector);
    HpDestroy(&maxHeap);
    HpDestroy(&minHeap);
    printf("%p\n", maxHeap);
    printf("%p\n", minHeap);
    printf("%p\n", vector);



    return 0;
    */
    int retVal = -1;
    int foundVal = -1;
    CC_HEAP* usedHeap = NULL;

    retVal = HpCreateMinHeap(&usedHeap, NULL);
    if (0 != retVal)
    {
        printf("HpCreateMinHeap failed!\n");
        goto cleanup;
    }

    retVal = HpInsert(usedHeap, 20);
    if (0 != retVal)
    {
        printf("HpInsert failed!\n");
        goto cleanup;
    }

    retVal = HpInsert(usedHeap, 10);
    if (0 != retVal)
    {
        printf("HpInsert failed!\n");
        goto cleanup;
    }

    if (2 != HpGetElementCount(usedHeap))
    {
        printf("Invalid element count!\n");
        retVal = -1;
        goto cleanup;
    }

    retVal = HpGetExtreme(usedHeap, &foundVal);
    if (0 != retVal)
    {
        printf("HpGetExtreme failed!\n");
        goto cleanup;
    }

    if (10 != foundVal)
    {
        printf("Invalid minimum value returned!");
        retVal = -1;
        goto cleanup;
    }

cleanup:
    if (NULL != usedHeap)
    {
        if (0 != HpDestroy(&usedHeap))
        {
            printf("HpDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;
}

int TestHashTable()
{
    /*
    // MY TESTS
    /*
    * srand((unsigned)time(0));
    CC_HASH_TABLE* hashTable = NULL;
    CC_HASH_TABLE_ITERATOR* iterator = NULL;
    char* itKey = NULL;
    char key[MAX_KEY_LEN];
    int keyNumber = 0;
    int value = 0;
    int const NR_TESTS = 50;
    int keyLength = 0;

    if (0 != HtCreate(&hashTable))
    {
        printf("HtCreate failed\n");
        goto cleanup;
    }

    for (int i = 0; i < NR_TESTS * 2; i++)
    {
        // Generate the key 
        keyLength = rand() % (MAX_KEY_LEN - MIN_KEY_LEN) + MIN_KEY_LEN;
        for (int j = 0; j < keyLength; j++)
        {
            //key[j] = rand() % (127 - 33 + 1) + 33;  // random printable character except space
            key[j] = rand() % (122 - 97 + 1) + 97;  // small letters
        }
        key[keyLength] = '\0';
        
        // Insert the key
        if (1 != HtHasKey(hashTable, key))
        {
            // Generate the value
            value = rand() % (MAX_VALUE - MIN_VALUE) + MIN_VALUE;

            // Insert the (key, value) tuple in the hash table
            if (0 != HtSetKeyValue(hashTable, key, value))
            {
                printf("Insertion failed\n");
                goto cleanup;
            }
        }
    }

    printf("\n\n*** Hash table at the beginning ***\n");
    if (0 != _HtPrint(hashTable))
    {
        printf("HtPrint failed\n");
        goto cleanup;
    }

    // ITERATOR
    if (0 != HtGetFirstKey(hashTable, &iterator, &itKey))
    {
        printf("HtGetFirstKey failed\n");
        goto cleanup;
    }

    for (int i = 0; i < NR_TESTS; i++)
    {
        // Print the next key
        keyNumber++;
        printf("\n\nKey %d: %s\n", keyNumber, itKey);
        if (-1 == HtGetNextKey(iterator, &itKey))
        {
            printf("HtGetNextKey failed\n");
            goto cleanup;
        }

        // Generate a key
        keyLength = rand() % (MAX_KEY_LEN - MIN_KEY_LEN) + MIN_KEY_LEN;
        for (int j = 0; j < keyLength; j++)
        {
            //key[j] = rand() % (127 - 33 + 1) + 33;  // random printable character except space
            key[j] = rand() % (122 - 97 + 1) + 97;  // small letters
        }
        key[keyLength] = '\0';

        // Remove the key
        if (1 == HtHasKey(hashTable, key))
        {
            printf("\n\n*** Removing key %s ***\n", key);
            if (0 != HtRemoveKey(hashTable, key))
            {
                printf("HtRemoveKey failed\n");
                goto cleanup;
            }
        }
    }

    printf("\n\n*** Hash table at the end ***\n");
    if (0 != _HtPrint(hashTable))
    {
        printf("HtPrint failed\n");
        goto cleanup;
    }

cleanup:

    if (0 != HtDestroy(&hashTable))
    {
        printf("Hash table cleanup failed\n");
        return -1;
    }

    if (NULL != hashTable)
    {
        printf("Pointer to hash table not NULL\n");
        return -1;
    }

    if (0 != HtReleaseIterator(&iterator))
    {
        printf("Iterator cleanup failed\n");
        return -1;
    }

    if (NULL != iterator)
    {
        printf("Pointer to iterator not NULL\n");
        return -1;
    }

    */

    int retVal = -1;
    int foundVal = -1;
    CC_HASH_TABLE* usedTable = NULL;

    retVal = HtCreate(&usedTable);
    if (0 != retVal)
    {
        printf("HtCreate failed!\n");
        goto cleanup;
    }

    retVal = HtSetKeyValue(usedTable, "mere", 20);
    if (0 != retVal)
    {
        printf("HtSetKeyValue failed!\n");
        goto cleanup;
    }

    if (1 != HtHasKey(usedTable, "mere"))
    {
        printf("Invalid answer to HtHasKey!\n");
        retVal = -1;
        goto cleanup;
    }

    retVal = HtGetKeyValue(usedTable, "mere", &foundVal);
    if (0 != retVal)
    {
        printf("HtGetKeyValue failed!\n");
        goto cleanup;
    }

    if (foundVal != 20)
    {
        printf("Invalid value after get!\n");
        retVal = -1;
        goto cleanup;
    }

cleanup:
    if (NULL != usedTable)
    {
        if (0 != HtDestroy(&usedTable))
        {
            printf("HtDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;
}

int TestStack()
{
    /*
     CC_STACK* stack = NULL;
    int value = 0;

    StCreate(&stack);

    // Test 1:
    printf("Test 1:\n");

    // Should print error and then a 0
    StPeek(stack, &value);
    printf("top: %d\n", value);

    // Should print 3
    StPush(stack, 3);
    StPeek(stack, &value);
    printf("top: %d\n", value);

    // Should print 2
    StPush(stack, 2);
    StPeek(stack, &value);
    printf("top: %d\n", value);

    // Should print 1
    StPush(stack, 1);
    StPeek(stack, &value);
    printf("top: %d\n", value);

    // Test 2:
    putchar('\n');
    printf("Test 2:\n");

    // Should print 1 and then a 2
    StPop(stack, &value);
    printf("popped: %d\n", value);
    StPeek(stack, &value);
    printf("top: %d\n", value);

    // Should print 2 and then a 3
    StPop(stack, &value);
    printf("popped: %d\n", value);
    StPeek(stack, &value);
    printf("top: %d\n", value);

    // Should print 3 and then an error and then 0
    StPop(stack, &value);
    printf("popped: %d\n", value);
    StPeek(stack, &value);
    printf("top: %d\n", value);

    // Test 3:
    putchar('\n');
    printf("Test 3:\n");

    // Should print: -1 -2 -3 ... -10
    // And then: -1 -2 -3 ... -10, an error and then 0
    for (int i = -10; i < 0; i++)
    {
        StPush(stack, i);
    }
    value = StPrint(stack);
    for (int i = 0; i < 11; i++)
    {
        StPop(stack, &value);
        printf("%d ", value);
    }

    // Test 4:
    putchar('\n');
    printf("Test 4:\n");

    // Should print: 4 3 2 1
    // And then: 7 6 5
    // And then: 4 3 2 1 7 6 5
    CC_STACK* stackToPush = NULL;
    StCreate(&stackToPush);

    for (int i = 1; i < 5; i++)
    {
        StPush(stack, i);
    }
    StPrint(stack);
    for (int i = 5; i < 8; i++)
    {
        StPush(stackToPush, i);
    }
    StPrint(stackToPush);
    StPushStack(stack, stackToPush);
    StPrint(stack);

    // Cleanup:
    putchar('\n');
    printf("Cleanup:\n");
    StDestroy(&stack);
    printf("%p\n", stack);
    */

    int retVal = -1;
    int foundVal = -1;
    CC_STACK* usedStack = NULL;

    retVal = StCreate(&usedStack);
    if (0 != retVal)
    {
        printf("StCreate failed!\n");
        goto cleanup;
    }

    retVal = StPush(usedStack, 10);
    if (0 != retVal)
    {
        printf("StPush failed!\n");
        goto cleanup;
    }

    if (0 != StIsEmpty(usedStack))
    {
        printf("Invalid answer to StIsEmpty!\n");
        retVal = -1;
        goto cleanup;
    }

    retVal = StPop(usedStack, &foundVal);
    if (0 != retVal)
    {
        printf("StPop failed!\n");
        goto cleanup;
    }

    if (foundVal != 10)
    {
        printf("Invalid value after pop!\n");
        retVal = -1;
        goto cleanup;
    }

cleanup:
    if (NULL != usedStack)
    {
        if (0 != StDestroy(&usedStack))
        {
            printf("StDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;
}

int TestVector()
{
    // SORTING TEST
/*
srand((unsigned)time(0));
CC_VECTOR* vector = NULL;
int value = 0;

for (int tests = 0; tests < NR_TESTS; tests++)
{
    printf("\n\n\nTest #%d\n", tests + 1);
    if (0 != VecCreate(&vector))
    {
        printf("VecCreate failed\n");
        goto cleanup;
    }
    printf("Vector created\n");

    for (int i = 0; i < NR_ELEM; i++)
    {
        value = rand() % (MAX_VALUE - MIN_VALUE + 1) + MIN_VALUE;
        if (rand() % 2 == 0)
        {
            value = value * (-1);
        }
        if (0 != VecInsertAfterIndex(vector, vector->Count, value))
        {
            printf("VecInsertHead failed\n");
            goto cleanup;
        }
        if (i % 100000 == 0 && i != 0)
        {
            printf("i: %d\n", i);
        }
    }

    printf("Elements inserted\n");

    printf("Sorting vector\n");

    if (0 != VecSort(vector))
    {
        printf("VecSort failed\n");
        goto cleanup;
    }

    if (1 != _VecIsSorted(vector))
    {
        printf("Bad luck mate\n");
        goto cleanup;
    }
    else
    {
        printf("Vector sorted\n");
    }

    printf("Clearing vector\n");

    if (0 != VecClear(vector))
    {
        printf("VecClear failed\n");
        goto cleanup;
    }

    printf("Vector cleared\n");
}

cleanup:
    if (0 != VecDestroy(&vector))
    {
        printf("VecDestroy failed\n");
        return -1;
    }
    if (NULL != vector)
    {
        printf("Vector not NULL after cleanup\n");
        return -1;
    }

    return 0;
    */
    /*
        CC_VECTOR* vector = NULL;

    VecCreate(&vector);
    VecPrint(vector);
    VecInsertTail(vector, 1);
    VecPrint(vector);
    VecInsertHead(vector, 2);
    VecPrint(vector);
    VecInsertAfterIndex(vector, 1, 3);
    VecPrint(vector);
    VecInsertAfterIndex(vector, 3, 4);
    VecPrint(vector);
    VecRemoveByIndex(vector, 1);
    VecPrint(vector);
    printf("Count: %d\n\n", VecGetCount(vector));

    int value = 0;
    VecGetValueByIndex(vector, 2, &value);
    printf("Value at index %d is: %d\n\n", 2, value);

    CC_VECTOR* aux = NULL;
    
    VecCreate(&aux);
    VecInsertAfterIndex(aux, 0, -1);
    VecInsertHead(aux, -2);
    VecInsertHead(aux, -3);
    VecPrint(aux);
    VecAppend(aux, vector);
    VecPrint(vector);
    printf("Count: %d\n\n", VecGetCount(vector));

    VecClear(vector);
    VecClear(aux);

    VecInsertTail(vector, 100);

    VecAppend(vector, aux);
    VecPrint(aux);

    VecAppend(aux, vector);
    VecPrint(vector);

    

    CC_VECTOR* aux2 = NULL;
    CC_VECTOR* aux3 = NULL;
    VecCreate(&aux2);
    VecCreate(&aux3);

    VecInsertAfterIndex(aux2, 0, -1);
    VecInsertAfterIndex(aux3, 0, -1);

    VecDestroy(&aux3);
    VecDestroy(&aux2);
    VecDestroy(&aux);
    VecDestroy(&vector);

    printf("%p\n", aux);
    */
    int retVal = -1;
    int foundVal = 0;
    CC_VECTOR* usedVector = NULL;
    
    retVal = VecCreate(&usedVector);
    if (0 != retVal)
    {
        printf("VecCreate failed!\n");
        goto cleanup;
    }

    retVal = VecInsertTail(usedVector, 10);
    if (0 != retVal)
    {
        printf("VecInsertTail failed!\n");
        goto cleanup;
    }

    retVal = VecInsertHead(usedVector, 16);
    if (0 != retVal)
    {
        printf("VecInsertHead failed!\n");
        goto cleanup;
    }

    if (VecGetCount(usedVector) != 2)
    {
        printf("Invalid count returned!\n");
        retVal = -1;
        goto cleanup;
    }

    _VecPrint(usedVector);

    retVal = VecInsertAfterIndex(usedVector, 0, 20);
    if (0 != retVal)
    {
        printf("VecInsertAfterIndex failed!\n");
        goto cleanup;
    }

    _VecPrint(usedVector);

    retVal = VecRemoveByIndex(usedVector, 0);
    if (0 != retVal)
    {
        printf("VecRemoveByIndex failed!\n");
        goto cleanup;
    }



    retVal = VecGetValueByIndex(usedVector, 0, &foundVal);
    if (0 != retVal)
    {
        printf("VecGetValueByIndex failed!\n");
        goto cleanup;
    }

    _VecPrint(usedVector);

    if (foundVal != 20)
    {
        printf("Invalid value found at position 0\n");
        retVal = -1;
        goto cleanup;
    }

    retVal = VecClear(usedVector);
    if (0 != retVal)
    {
        printf("VecClear failed!\n");
        goto cleanup;
    }

    if (0 != VecGetCount(usedVector))
    {
        printf("Invalid count after clear\n");
        retVal = -1;
        goto cleanup;
    }

cleanup:
    if (NULL != usedVector)
    {
        if (0 != VecDestroy(&usedVector))
        {
            printf("VecDestroy failed!\n");
            retVal = -1;
        }
    }
    return retVal;
}
