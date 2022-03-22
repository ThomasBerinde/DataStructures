#include <string.h>
#include "cchashtable.h"
#include "common.h"

// *** IMPORTANT ***
// HtRemoveKey can interfere with hash table iterator if
// the removed key is exactly the next key in the iterator.
// I realized this too late and didn't have time to fix it
// Also, the iterator doesn't work if the hash table
// resizes (upon insertion of other keys) while the iterator
// iterates through it.

// Iterative O(101)
int HtCreate(CC_HASH_TABLE **HashTable)
{
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    return 0 != _HtCreate(HashTable, 101);
}

// Iterative O(Count)
int HtDestroy(CC_HASH_TABLE **HashTable)
{
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == (*HashTable))
    {
        GENERIC_LOG("Program tried to destroy a NULL hash table\n");
        return -1;
    }

    if (NULL != (*HashTable)->Array)
    {
        // Destroy every list of the hash table
        for (int i = 0; i < (*HashTable)->Size; i++)
        {
            if (NULL == (*HashTable)->Array[i])
            {
                continue;
            }
            if (0 != ListDestroy( &((*HashTable)->Array[i]) ) )
            {
                return -1;
            }
            if (NULL != (*HashTable)->Array[i])
            {
                GENERIC_LOG("Hash table list not NULL\n");
                return -1;
            }
        }

        // Destory the array of the hash table
        free((*HashTable)->Array);
        (*HashTable)->Array = NULL;
    }

    // Destory the hash table
    free(*HashTable);
    *HashTable = NULL;

    return 0;
}

// Iterative O(1)
// Takes O(newSize) if resizing is needed
int HtSetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int Value)
{
    // Validate the inputs
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    if (NULL == Key)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }
    if (NULL == HashTable->Array)
    {
        GENERIC_LOG("Hash table Array is NULL\n");
        return -1;
    }

    // Check if key is in the hash table or not
    int status = HtHasKey(HashTable, Key);
    if (1 == status)
    {
        GENERIC_LOG("Program attempted to insert a key which was already in the hash table\n");
        return -1;
    }
    if (-1 == status)
    {
        return -1;
    }

    // If execution arrives here it means the key is not in the hash table
    // Hash the key
    int hash = _HashKey(HashTable, Key);
    if (hash < 0)
    {
        GENERIC_LOG("Hash function returned negative number\n");
        return -1;
    }
    
    // Insert the entry into the hash table
    if (0 != ListInsertHead(HashTable->Array[hash], Key, Value))
    {
        return -1;
    }

    // Update the count of the hash table
    HashTable->Count = HashTable->Count + 1;

    // Resize if fill factor greater 7
    if (HashTable->Count / HashTable->Size >= 7)
    {
        // Print the hash table before resizing
        //if (0 != _HtPrint(HashTable))
        //{
        //    printf("HtPrint failed\n");
        //    return -1;
        //}
        // Print the first 10 keys of the hash table 
        //int keyNumber = 0;
        //char* itKey = NULL;
        //CC_HASH_TABLE_ITERATOR* iterator = NULL;
        //printf("\n\n\nITERATOR:\n");
        //if (0 != HtGetFirstKey(HashTable, &iterator, &itKey))
        //{
        //    printf("HtGetFirstKey failed\n");
        //    return -1;
        //}
        //do
        //{
        //    keyNumber++;
        //    printf("Key %d: %s\n", keyNumber, itKey);
        //} while (keyNumber < 10 && 0 == HtGetNextKey(iterator, &itKey));
        //printf("\n\n\n\n Hash table size: %d.  Resizing hash table...\n", HashTable->Size);
        //
        //// Destroy hash table iterator
        //if (0 != HtReleaseIterator(&iterator))
        //{
        //    printf("Iterator cleanup failed\n");
        //    return -1;
        //}
        //if (NULL != iterator)
        //{
        //    printf("Pointer to iterator not NULL\n");
        //    return -1;
        //}

        // Resize hash table
        if (0 != _HtResize(HashTable))
        {
            GENERIC_LOG("Resizing failed\n");
            return -1;
        }

        //printf("Exited resizing function!\n");
    }

    return 0;
}

// Iterative O(1)
int HtGetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int *Value)
{
    // Validate the inputs
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    if (NULL == Key)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }
    if (NULL == Value)
    {
        GENERIC_LOG(INVALID_PARAM_3);
        return -1;
    }
    if (NULL == HashTable->Array)
    {
        GENERIC_LOG("Hash table Array is NULL\n");
        return -1;
    }

    // Get the hash of the key
    int hash = _HashKey(HashTable, Key);
    if (hash < 0)
    {
        return -1;
    }

    // Get the node with the appropriate key
    LSTNODE* node = NULL;
    int status = ListGetNodeByKey(HashTable->Array[hash], Key, &node);
    if (1 != status)
    {
        if (0 == status)
        {
            GENERIC_LOG("Program tried to get the value of a key which was not in the hash table\n");
        }
        return -1;
    }
    if (NULL == node)
    {
        GENERIC_LOG("Key found but node is NULL\n");
        return -1;
    }

    *Value = node->Value;

    return 0;
}

// Iterative O(1)
// Takes O(Size) if resizing is needed
int HtRemoveKey(CC_HASH_TABLE *HashTable, char *Key)
{
    // Validate inputs
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    if (NULL == Key)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }
    if (NULL == HashTable->Array)
    {
        GENERIC_LOG("Hash table Array is NULL\n");
        return -1;
    }

    // Get the hash of the key
    int hash = _HashKey(HashTable, Key);
    if (hash < 0)
    {
        return -1;
    }

    // Get the node with the appropriate key
    LSTNODE* node = NULL;
    int status = ListGetNodeByKey(HashTable->Array[hash], Key, &node);
    if (1 != status)
    {
        if (0 == status)
        {
            GENERIC_LOG("Program tried to remove a key which was not in the hash table\n");
        }
        return -1;
    }
    if (NULL == node)
    {
        GENERIC_LOG("Key found but node is NULL\n");
        return -1;
    }

    // Remove the node from the hash table
    if (0 != ListRemoveNode(HashTable->Array[hash], node))
    {
        return -1;
    }

    // Update the count of the hash table
    HashTable->Count = HashTable->Count - 1;

    return 0;
}

// Iterative O(1)
int HtHasKey(CC_HASH_TABLE *HashTable, char *Key)
{
    // Validate the inputs
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    if (NULL == Key)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    // Get the hash of the key
    int hash = _HashKey(HashTable, Key);
    if (hash < 0)
    {
        return -1;
    }

    LSTNODE* node = NULL;

    return ListGetNodeByKey(HashTable->Array[hash], Key, &node);
}

// Iterative O(Size)
int HtGetFirstKey(CC_HASH_TABLE* HashTable, CC_HASH_TABLE_ITERATOR **Iterator, char **Key)
{
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    if (NULL == Iterator)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }
    if (NULL == Key)
    {
        GENERIC_LOG(INVALID_PARAM_3);
        return -1;
    }
    if (NULL == HashTable->Array)
    {
        GENERIC_LOG("Hash table Array is NULL\n");
        return -1;
    }

    CC_HASH_TABLE_ITERATOR *iterator = NULL;

    (*Key) = NULL;
    (*Iterator) = NULL;

    // Allocate memory for the iterator
    iterator = (CC_HASH_TABLE_ITERATOR*)malloc(sizeof(CC_HASH_TABLE_ITERATOR));
    if (NULL == iterator)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    memset(iterator, 0, sizeof(*iterator));

    // Initialize iterator fields
    iterator->HashTable = HashTable;
    iterator->CurrentNode = NULL;
    iterator->ListNumber = 0;

    // Assign iterator to Iterator
    *Iterator = iterator;

    // FIND THE FIRST KEY AND SET Key
    if (0 == HashTable->Count || NULL == HashTable->Array)
    {
        GENERIC_LOG("No keys in the hash table\n");
        return -2;
    }

    int i = 0;
    unsigned int found = 0;

    while (i < HashTable->Size && 0 == found)
    {
        if (NULL == HashTable->Array[i])
        {
            GENERIC_LOG("Hash table list is NULL\n");
            return -1;
        }
        if (NULL != HashTable->Array[i]->Head.Next)
        {
            // Make Key point to the first key of the hash table
            *Key = HashTable->Array[i]->Head.Next->Key;
            // Initialize the CurrentNode and the ListNumber of the iterator
            (*Iterator)->CurrentNode = HashTable->Array[i]->Head.Next;
            (*Iterator)->ListNumber = i;
            found = 1;
        }
        i++;
    }

    if (0 == found)
    {
        GENERIC_LOG("No keys in hash table but Count is not 0\n");
        return -1;
    }

    return 0;
}

// Iterative O(Size)
int HtGetNextKey(CC_HASH_TABLE_ITERATOR *Iterator, char **Key)
{
    // Validate inputs
    if (NULL == Key)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }
    (*Key) = NULL;
    if (NULL == Iterator)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    if (NULL == Iterator->HashTable)
    {
        GENERIC_LOG("Hash table contained in iterator is NULL\n");
        return -1;
    }
    if (NULL == Iterator->HashTable->Array)
    {
        GENERIC_LOG("Array of hash table contained in iterator is NULL\0");
        return -1;
    }
    if (NULL == Iterator->CurrentNode)
    {
        GENERIC_LOG("Iterator state error\n");
        return -1;
    }

    // Get the next node of the hash table
    if (NULL != Iterator->CurrentNode->Next)
    {
        Iterator->CurrentNode = Iterator->CurrentNode->Next;
    }
    else
    {
        int i = 0;
        unsigned int found = 0;

        i = Iterator->ListNumber + 1;

        while (i < Iterator->HashTable->Size && 0 == found)
        {
            if (NULL == Iterator->HashTable->Array[i])
            {
                GENERIC_LOG("List of hash table contained in array is NUL\0");
                return -1;
            }
            if (NULL != Iterator->HashTable->Array[i]->Head.Next)
            {
                // Initialize the CurrentNode and the ListNumber of the iterator
                Iterator->CurrentNode = Iterator->HashTable->Array[i]->Head.Next;
                Iterator->ListNumber = i;
                found = 1;
            }
            i++;
        }

        if (0 == found)
        {
            GENERIC_LOG("No more keys in the hash table\n");
            return -2;
        }
    }
    
    // Make the key point to the next key in the hash table
    (*Key) = Iterator->CurrentNode->Key;

    return 0;
}

// Iterative O(1)
int HtReleaseIterator(CC_HASH_TABLE_ITERATOR **Iterator)
{
    if (NULL == Iterator)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == (*Iterator))
    {
        GENERIC_LOG("Program tried to free NULL memory\n");
        return -1;
    }

    free((*Iterator));

    *Iterator = NULL;
    
    return 0;
}

// Iterative O(Count)
int HtClear(CC_HASH_TABLE *HashTable)
{
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == HashTable->Array)
    {
        GENERIC_LOG("Program tried to clear a hash table with a NULL Array\n");
        return -1;
    }

    // Clear every list of the hash table
    // This will only delete the elements in the lists
    // The actual lists will continue to exist on each position of the hash table
    for (int i = 0; i < HashTable->Size; i++)
    {
        if (NULL == HashTable->Array[i])
        {
            GENERIC_LOG("Hash table list is NULL\n");
            return -1;
        }
        if ( 0 != ListClear(HashTable->Array[i]) )
        {
            return -1;
        }
    }

    return 0;
}

// Iterative O(1)
int HtGetKeyCount(CC_HASH_TABLE *HashTable)
{
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    
    return HashTable->Count;
}






// Additional functions

// Returns:
//  >=0     -   Representing the hash of the key for this particular hash table
//  -1      -   Error or invalid parameter
int _HashKey(CC_HASH_TABLE* HashTable, char* Key)
{
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Key)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    int hash = 5381;
    int c;

    while (c = *Key++)
    {
        hash = ((hash << 5) + hash) + c;
    }
    
    if (hash < 0)
    {
        hash = hash * (-1);
    }

    hash = hash % HashTable->Size;

    if (hash < 0 || hash >= HashTable->Size)
    {
        GENERIC_LOG("Error: Invalid hash\n");
        return -1;
    }

    return hash;
}
int _HtPrint(CC_HASH_TABLE* HashTable)
{
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == HashTable->Array)
    {
        GENERIC_LOG("Hash table Array is NULL\n");
        return -1;
    }

    for (int i = 0; i < HashTable->Size; i++)                                                                         ///// HEREEEEEEEEEEEEEEEEEE /////// 
    {
        printf("%d:  ", i);
        if (NULL == HashTable->Array[i])
        {
            GENERIC_LOG("Hash table list is NULL\n");
            return -1;
        }
        if (0 != ListPrint(HashTable->Array[i]))
        {
            return -1;
        }
    }

    return 0;
}
static int _HtCreate(CC_HASH_TABLE** HashTable, int Size)
{
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    // Create the hash table
    (*HashTable) = (CC_HASH_TABLE*)malloc(sizeof(CC_HASH_TABLE));
    if (NULL == (*HashTable))
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    // Create an array of the appropriate number of lists
    (*HashTable)->Array = (LIST**)malloc(Size * sizeof(LIST*));
    if (NULL == (*HashTable)->Array)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    // Create the appropriate number of lists
    for (int i = 0; i < Size; i++)
    {
        if (0 != ListCreate(&((*HashTable)->Array[i])))
        {
            return -1;
        }
        if (NULL == (*HashTable)->Array[i])
        {
            GENERIC_LOG("Newly created list is NULL\n");
            return -1;
        }
        // Set the appropriate index for each list
        (*HashTable)->Array[i]->Index = i;
    }

    // Initialize the other fields of the hash table
    (*HashTable)->Size = Size;
    (*HashTable)->Count = 0;

    return 0;
}
static int _HtResize(CC_HASH_TABLE* HashTable)
{
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == HashTable->Array)
    {
        GENERIC_LOG("Hash table array is NULL\n");
        return -1;
    }

    static int Sizes[] = { 11, 101, 1009, 10007, 100003, 1000003, 10000019, 100000007, 1000000007 };
    static unsigned int const SIZES_LENGTH = 8;

    unsigned int sizePos = 0;

    // Get the new size of the hash table
    while (HashTable->Size > Sizes[sizePos])
    {
        sizePos++;
    }
    if (sizePos > SIZES_LENGTH || HashTable->Size != Sizes[sizePos])
    {
        GENERIC_LOG("Hash table size not in Sizes array\n");
        return -1;
    }

    if (sizePos < 0 || sizePos > SIZES_LENGTH)
    {
        GENERIC_LOG("This size of the hashtable is not acceptable\n");
        return -1;
    }

    // Create a new hash table with the appropriate size
    CC_HASH_TABLE* newHashTable = NULL;

    printf("Creating new hash table\n");
    
    if (0 != _HtCreate(&newHashTable, Sizes[sizePos]))
    {
        return -1;
    }

    printf("Finished!\n");

    // Insert the elements of the old hash table into the new hash table
    LSTNODE* currentNode = NULL;
    for (int i = 0; i < HashTable->Size; i++)
    {
        if (NULL == HashTable->Array[i])
        {
            GENERIC_LOG("Hash table list is NULL\n");
            return -1;
        }

        currentNode = HashTable->Array[i]->Head.Next;

        while (NULL != currentNode)
        {
            // Insert the key and its value in the new hash table
            if (0 != HtSetKeyValue(newHashTable, currentNode->Key, currentNode->Value))
            {
                GENERIC_LOG("Rehashing failed\n");
                return -1;
            }
            currentNode = currentNode->Next;
        }

        // Clear the list we just reinserted to free up space as we move 
        if (0 != ListDestroy(&(HashTable->Array[i])))
        {
            return -1;
        }
        if (NULL != HashTable->Array[i])
        {
            GENERIC_LOG("List not NULL after calling ListDestroy\n");
            return -1;
        }

        if (i % (HashTable->Size / 10) == 0)
        {
            printf("*");
        }
        
        if (i > HashTable->Size / 10 * 10)
        {
            printf("\nri: %d", i);
        }
    }

    printf(" Finished rehasing!\n");

    // Destroy the hash table array
    free(HashTable->Array);
    HashTable->Array = NULL;

    // Finally, assign the new hash table to the initial hash table
    *HashTable = *newHashTable;

    return 0;
}
int _HtGetDispersion(CC_HASH_TABLE* HashTable, double *Dispersion)
{
    if (NULL == HashTable)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    if (NULL == Dispersion)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    // Calculate the ideal number of elements per hash table slot
    double ideal = (double)HashTable->Count / HashTable->Size;
    long double difference = 0;

    for (int i = 0; i < HashTable->Size; i++)
    {
        if (NULL == HashTable->Array[i])
        {
            GENERIC_LOG("Hash table list is NULL\n");
            return -1;
        }
        
        if (HashTable->Array[i]->Count >= ideal)
        {
            difference = difference + (HashTable->Array[i]->Count - ideal);
        }
        else
        {
            difference = difference - HashTable->Array[i]->Count + ideal;
        }
    }

    difference = difference / HashTable->Size;

    (*Dispersion) = (double)difference;
    
    return 0;
}