#pragma once

// *** IMPORTANT ***
// VecInsertAfterIndex actually inserts AT index. I left
// it like this because it was your code that inserts it
// there.
// However, the test given by you in the main function expects
// the value to be inserted AFTER the index.

// Aditional declarations
#define REALLOCATION_CONSTANT_FACTOR 1.618
#define EMPTY_VECTOR "cannot perform this operation on an empty vector\n"


// Mandatory declarations
typedef struct _CC_VECTOR {
    int *Array;  // contiguous array that holds the elements of the vector
    int Size;  // the number of elements for which memory was allocated
    int Count;  // the actual number of elements that are currently in the vector
} CC_VECTOR;

int VecCreate(CC_VECTOR **Vector);
int VecDestroy(CC_VECTOR **Vector);

int VecInsertTail(CC_VECTOR *Vector, int Value);
int VecInsertHead(CC_VECTOR *Vector, int Value);
int VecInsertAfterIndex(CC_VECTOR *Vector, int Index, int Value);
int VecRemoveByIndex(CC_VECTOR *Vector, int Index);
int VecGetValueByIndex(CC_VECTOR *Vector, int Index, int *Value);

// Returns the number of elements in Vector or -1 in case of error or invalid parameters
int VecGetCount(CC_VECTOR *Vector);
int VecClear(CC_VECTOR *Vector);

// Sort the vector in decreasing order
int VecSort(CC_VECTOR *Vector);

// Appends all the elements in DestVector to SrcVector
int VecAppend(CC_VECTOR *DestVector, CC_VECTOR *SrcVector);



// Additional declarations

// 0 if not sorted in ascending order
// 1 if sorted in (non-strictly) ascending order
// -1 if any error occured or param invalid
int _VecIsSorted(CC_VECTOR* Vector);
int _VecPrint(CC_VECTOR* Vector);
int _VecGetIndexByValue(CC_VECTOR* Vector, int* Index, int Value);

// For sorting
static int _SwapValues(int* Number1, int* Number2);
static int _Partition(int* Array, int* PivotPos, int Left, int Right);
static int _QuickSortRecursive(int* Array, int Left, int Right);
static int _QuickSort(int* Array, int Left, int Right);
static int _InsertionSort(int* Array, int Size);