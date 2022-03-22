#pragma once

#include "ccvector.h"

#define EMPTY_HEAP "cannot perform this operation on an empty heap\n"

typedef struct _CC_HEAP{
    // Members
    int* Array;
    int Size;
    int Type;  // 0 if MinHeap, 1 if MaxHeap
} CC_HEAP;

// HpCreateMaxHeap and HpCreateMinHeap should create a max heap or a min heap,
// respectively. InitialElements is an optional parameter and, if it is not null, the constructed
// heap should initially contain all the elements in the provided vector.
int HpCreateMaxHeap(CC_HEAP **MaxHeap, CC_VECTOR* InitialElements);
int HpCreateMinHeap(CC_HEAP **MinHeap, CC_VECTOR* InitialElements);
int HpDestroy(CC_HEAP **Heap);

int HpInsert(CC_HEAP *Heap, int Value);

// HpRemove should remove all elements with the value Value in the heap
int HpRemove(CC_HEAP *Heap, int Value);

// HpGetExtreme should return the maximum/minimum value in the heap, depending on the
// type of heap constructed
int HpGetExtreme(CC_HEAP *Heap, int* ExtremeValue);

// HpPopExtreme should return the maximum/minimum value in the heap, and remove all
// instances of said value from the heap
int HpPopExtreme(CC_HEAP *Heap, int* ExtremeValue);

// Returns the number of elements in Heap or -1 in case of error or invalid parameter
int HpGetElementCount(CC_HEAP *Heap);

// HpSortToVector should construct and return (in the SortedVector parameter) a vector
// sorted in increasing order containing all the elements present in the heap
int HpSortToVector(CC_HEAP *Heap, CC_VECTOR* SortedVector);




// Aditional functions declarations

static int HpSwapValues(int* FirstValue, int* SecondValue);

// 0 if FirstValue should come before SecondValue
// 1 if FirstValue should come after SecondValue
static int HpCompareValues(int FirstValue, int SecondValue, int Type);

// Takes the node at Index and pushes it upward in the heap as long as the heap property isn't met
static int HpSwim(CC_HEAP* Heap, int Index);

// Takes the node at Index and drops it downward in the heap as long as the heap property isn't met
static int HpHeapify(CC_HEAP* Heap, int Index);

// 1 if yes
// 0 if no
// -1 if any error occurs
int HpIsHeap(CC_HEAP* Heap);

int HpPrint(CC_HEAP* Heap);
