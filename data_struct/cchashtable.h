#pragma once
#include "list.h"

/*
* This module implements a hash table that works as follows:
*   - it holds tuples (key, value), where key is a character string and value is an integer
*   - it stores these tuples in doubly-linked lists, each position in the hash table representing a pointer to one of these lists
*   - because lists are used, naturally, collisions are solved by chaining
*   - also, the hash table increases its size by aprox x10 and rehashes all elements when it reaches a fill factor of 7 number of elements 
*/

// *** IMPORTANT ***
// HtRemoveKey can interfere with hash table iterator if
// the removed key is exactly the next key in the iterator.
// I realized this too late and didn't have time to fix it
// Also, the iterator doesn't work if the hash table
// resizes (upon insertion of other keys) while the iterator
// iterates through it.

typedef struct _CC_HASH_TABLE {
    LIST** Array;                      // array of pointers to lists
    int Size;           // size of the Array 
    int Count;          // number of elements in the Array 
} CC_HASH_TABLE;

typedef struct _CC_HASH_TABLE_ITERATOR
{
    CC_HASH_TABLE *HashTable;
    LSTNODE* CurrentNode;
    int ListNumber;  // the position of the list in the hash table (this is an index in the hash table)
} CC_HASH_TABLE_ITERATOR;

int HtCreate(CC_HASH_TABLE **HashTable);
int HtDestroy(CC_HASH_TABLE **HashTable);

// Returns -1 if Key already exist in HashTable or the parameters are invalid
int HtSetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int Value);

// Returns -1 if Key does not exist in HashTable or the parameters are invalid
int HtGetKeyValue(CC_HASH_TABLE *HashTable, char *Key, int *Value);

// Returns -1 if Key does not exist in HashTable or the parameters are invalid
int HtRemoveKey(CC_HASH_TABLE *HashTable, char *Key);

//  Returns:
//       1  - HashTable contains Key
//       0  - HashTable does not contain Key
//      -1  - Error or invalid parameter
int HtHasKey(CC_HASH_TABLE *HashTable, char *Key);

// Initializes the iterator and gets the first key in the hash table
// Returns:
//       -1 - Error or invalid parameter
//       -2 - No keys in the hash table
//      >=0 - Success
int HtGetFirstKey(CC_HASH_TABLE *HashTable, CC_HASH_TABLE_ITERATOR **Iterator, char **Key);

// Returns the next key in the hash table contained in the iterator
// Iterator saves the state of the iteration
// Returns:
//       -1 - Error or invalid parameter
//       -2 - No more keys in the hash table
//      >=0 - Success
int HtGetNextKey(CC_HASH_TABLE_ITERATOR *Iterator, char **Key);

int HtReleaseIterator(CC_HASH_TABLE_ITERATOR **Iterator);

// Removes every element in the hash table
int HtClear(CC_HASH_TABLE *HashTable);

// Returns the number of keys in the HashTable, or -1 in case of error
int HtGetKeyCount(CC_HASH_TABLE *HashTable);






// Additional functions

// Returns:
//  >=0     -   Representing the hash of the key for this particular hash table
//  -1      -   Error or invalid parameter
int _HashKey(CC_HASH_TABLE* HashTable, char* Key);
int _HtPrint(CC_HASH_TABLE* HashTable);
static int _HtCreate(CC_HASH_TABLE** HashTable, int Size);
static int _HtResize(CC_HASH_TABLE* HashTable);
int _HtGetDispersion(CC_HASH_TABLE* HashTable, double* Dispersion);