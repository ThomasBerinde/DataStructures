#include "ccheap.h"
#include "common.h"

int HpCreateMaxHeap(CC_HEAP **MaxHeap, CC_VECTOR* InitialElements)
{
    if (NULL == MaxHeap)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    *MaxHeap = (CC_HEAP*)malloc(sizeof(CC_HEAP));

    if (NULL == *MaxHeap)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }
    
    (*MaxHeap)->Array = NULL;
    (*MaxHeap)->Size = 0;
    (*MaxHeap)->Type = 1;

    if (NULL == InitialElements || NULL == InitialElements->Array || 0 == InitialElements->Count)
    {
        return 0;
    }

    (*MaxHeap)->Array = (int*)malloc(InitialElements->Count * sizeof(int));

    if (NULL == (*MaxHeap)->Array)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    (*MaxHeap)->Size = InitialElements->Count;

    // Copy the elements from the vector in the heap's array
    for (int i = 0; i < InitialElements->Count; i++)
    {
        (*MaxHeap)->Array[i] = InitialElements->Array[i];
    }

    for (int i = (*MaxHeap)->Size / 2 - 1; i >= 0; i--)
    {
        if (0 != HpHeapify(*MaxHeap, i))
        {
            return -1;
        }
        
    }
   
    return 0;
}

int HpCreateMinHeap(CC_HEAP **MinHeap, CC_VECTOR* InitialElements)
{
    if (NULL == MinHeap)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    *MinHeap = (CC_HEAP*)malloc(sizeof(CC_HEAP));

    if (NULL == *MinHeap)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    (*MinHeap)->Array = NULL;
    (*MinHeap)->Size = 0;
    (*MinHeap)->Type = 0;

    if (NULL == InitialElements || NULL == InitialElements->Array || 0 == InitialElements->Count)
    {
        return 0;
    }

    (*MinHeap)->Array = (int*)malloc(InitialElements->Count * sizeof(int));

    if (NULL == (*MinHeap)->Array)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    (*MinHeap)->Size = InitialElements->Count;

    // Copy the elements from the vector in the heap's array
    for (int i = 0; i < InitialElements->Count; i++)
    {
        (*MinHeap)->Array[i] = InitialElements->Array[i];
    }

    for (int i = (*MinHeap)->Size / 2 - 1; i >= 0; i--)
    {
        if (0 != HpHeapify(*MinHeap, i))
        {
            return -1;
        }
    }

    return 0;
}

int HpDestroy(CC_HEAP **Heap)
{
    if (NULL == Heap || NULL == *Heap)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL != (*Heap)->Array)
    {
        free((*Heap)->Array);
    }

    free(*Heap);

    *Heap = NULL;

    return 0;
}

int HpInsert(CC_HEAP *Heap, int Value)
{
    if (NULL == Heap || (Heap->Type != 1 && Heap->Type != 0) || Heap->Size < 0)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    int* arrayCopy = Heap->Array;

    Heap->Array = (int*)realloc(Heap->Array, (Heap->Size + 1) * sizeof(int));  // check for overflow

    if (NULL == Heap->Array)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        Heap->Array = arrayCopy;
        return -1;
    }

    Heap->Size = Heap->Size + 1;
    Heap->Array[Heap->Size - 1] = Value;

    if (0 != HpSwim(Heap, Heap->Size - 1))
    {
        return -1;
    }

    return 0;
}

// Implement this when you have the stack and the queue
int HpRemove(CC_HEAP *Heap, int Value)
{
    CC_UNREFERENCED_PARAMETER(Value);

    if (NULL == Heap)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    return -1;
    // Search for the element, swap it with the last element
    // Heapify the last element, which is now in the place of the element which was deleted
}

int HpGetExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    if (NULL == Heap || (Heap->Type != 1 && Heap->Type != 0) || Heap->Size < 0)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == ExtremeValue)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }
    
    if (NULL == Heap->Array)
    {
        GENERIC_LOG(EMPTY_HEAP);
        return -1;
    }

    *ExtremeValue = Heap->Array[0];

    return 0;
}

int HpPopExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    if (NULL == Heap || (Heap->Type != 1 && Heap->Type != 0) || Heap->Size < 0)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == ExtremeValue)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (NULL == Heap->Array)
    {
        GENERIC_LOG(EMPTY_HEAP);
        return -1;
    }

    *ExtremeValue = Heap->Array[0];

    HpSwapValues(&(Heap->Array[0]), &(Heap->Array[Heap->Size - 1]));

    Heap->Array[Heap->Size - 1] = 0;
    Heap->Size = Heap->Size - 1;

    HpHeapify(Heap, 0);

    return 0;
    
}

int HpGetElementCount(CC_HEAP *Heap)
{
    if (NULL == Heap || (Heap->Type != 1 && Heap->Type != 0) || Heap->Size < 0)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    
    return Heap->Size;
}

int HpSortToVector(CC_HEAP *Heap, CC_VECTOR* SortedVector)
{
    // Validate parameters
    if (NULL == Heap || (Heap->Type != 1 && Heap->Type != 0) || Heap->Size < 0)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == SortedVector)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }
 
    // Declarations of local variables
    CC_HEAP* heapCopy = NULL;
    int (*vecInsertFunction)(CC_VECTOR*, int) = NULL;
    int (*hpCreateFunction)(CC_HEAP**, CC_VECTOR*) = NULL;

    // Choose the appropriate functions based on the type of heap
    if (Heap->Type == 0)
    {
        hpCreateFunction = &HpCreateMinHeap;
        vecInsertFunction = &VecInsertTail;
    }
    else
    {
        hpCreateFunction = &HpCreateMaxHeap;
        vecInsertFunction = &VecInsertHead;
    }

    // Create a copy of the heap
    if (0 != hpCreateFunction(&heapCopy, NULL))
    {
        return -1;
    }

    heapCopy->Size = Heap->Size;

    heapCopy->Array = (int*)malloc(heapCopy->Size * sizeof(int));

    if (NULL == heapCopy->Array)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        goto cleanup;
    }
    
    for (int i = 0; i < heapCopy->Size; i++)
    {
        heapCopy->Array[i] = Heap->Array[i];
    }

    // Heap sort
    while (heapCopy->Size > 0)
    {
        if (0 != vecInsertFunction(SortedVector, heapCopy->Array[0]))
        {
            goto cleanup;
        }

        heapCopy->Size = heapCopy->Size - 1;
        heapCopy->Array[0] = heapCopy->Array[heapCopy->Size];
        heapCopy->Array[heapCopy->Size] = 0;

        if (heapCopy->Size > 0)
        {
            HpHeapify(heapCopy, 0);
        } 
    }

    if (0 != HpDestroy(&heapCopy))
    {
        return -1;
    }
    return 0;

cleanup:
    HpDestroy(&heapCopy);
    return -1;
}


// Aditional functions definitions
static int HpSwapValues(int* FirstValue, int* SecondValue)
{
    if (NULL == FirstValue)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == SecondValue)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    int aux = 0;

    aux = *FirstValue;
    *FirstValue = *SecondValue;
    *SecondValue = aux;

    return 0;
}

static int HpCompareValues(int FirstValue, int SecondValue, int Type)
{
    if (FirstValue == SecondValue)
    {
        return 1;
    }

    unsigned int T = Type;
    unsigned int F = (FirstValue > SecondValue);

    return T ^ F;
}

static int HpSwim(CC_HEAP* Heap, int Index)
{
    if (NULL == Heap || (Heap->Type != 1 && Heap->Type != 0) || Heap->Size < 0)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Index >= Heap->Size || Index < 0)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (NULL == Heap->Array)
    {
        GENERIC_LOG(EMPTY_HEAP);
        return -1;
    }

    int compareResult = 0;
    int parentIndex = (Index - 1) / 2;
    
    if (Index > 0)
    {
        compareResult = HpCompareValues(Heap->Array[Index], Heap->Array[parentIndex], Heap->Type);
    }
   
    while (compareResult != 1 && Index > 0)
    {
        HpSwapValues(&(Heap->Array[Index]), &(Heap->Array[parentIndex]));
        Index = parentIndex;
        parentIndex = (Index - 1) / 2;
        compareResult = HpCompareValues(Heap->Array[Index], Heap->Array[parentIndex], Heap->Type);
    }
    
    return 0;
}

static int HpHeapify(CC_HEAP* Heap, int Index)
{
    if (NULL == Heap || (Heap->Type != 1 && Heap->Type != 0) || Heap->Size < 0)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Index >= Heap->Size || Index < 0)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (NULL == Heap->Array)
    {
        GENERIC_LOG(EMPTY_HEAP);
        return -1;
    }

    int compareResult = 0;
    // Index of the child with which the node must be swapped
    // If the heap is a min heap, it must be swapped with the smallest of its children
    // If the heap is a max heap, it must be swapped with the largest of its children
    int swapChildIndex = 0;  
    int leftChildIndex = 2 * Index + 1;
    int rightChildIndex = 2 * Index + 2;
    
    do
    {
        if (leftChildIndex < Heap->Size)
        {
            if (rightChildIndex < Heap->Size)
            {
                // If the node has both children, find the one with which the node must be swapped
                if (HpCompareValues(Heap->Array[leftChildIndex], Heap->Array[rightChildIndex], Heap->Type) == 0)
                {
                    swapChildIndex = leftChildIndex;
                }
                else
                {
                    swapChildIndex = rightChildIndex;
                }
            }
            else
            {
                // If the node has only the left child, then this is the one with which the node must be swapped
                swapChildIndex = leftChildIndex;
            }
        }

        compareResult = HpCompareValues(Heap->Array[Index], Heap->Array[swapChildIndex], Heap->Type);
        if (compareResult == 1)
        {
            // If the heap property is not met, swap the node with the selected child
            HpSwapValues(&(Heap->Array[Index]), &(Heap->Array[swapChildIndex]));
            Index = swapChildIndex;
            leftChildIndex = 2 * Index + 1;
            rightChildIndex = 2 * Index + 2;
        }
    } while (compareResult == 1 && leftChildIndex < Heap->Size);
    
    return 0;
}

int HpIsHeap(CC_HEAP* Heap)
{
    if (NULL == Heap || (Heap->Type != 1 && Heap->Type != 0) || Heap->Size < 0)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Heap->Array)
    {
        GENERIC_LOG(EMPTY_HEAP);
        return -1;
    }

    for (int i = 0; i < Heap->Size / 2; i++)
    {
        if (Heap->Array[i] != Heap->Array[2 * i + 1] && HpCompareValues(Heap->Array[i], Heap->Array[2 * i + 1], Heap->Type) == 1)
        {
            return 0;
        }
        if (Heap->Array[i] != Heap->Array[2 * i + 2] && 2 * i + 2 < Heap->Size && HpCompareValues(Heap->Array[i], Heap->Array[2 * i + 2], Heap->Type) == 1)
        {
            return 0;
        }
    }

    return 1;
}

int HpPrint(CC_HEAP* Heap)
{
    if (NULL == Heap || (Heap->Type != 1 && Heap->Type != 0) || Heap->Size < 0)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Heap->Array)
    {
        printf("Heap is empty\n");
        return 0;
    }

    for (int i = 0; i < Heap->Size; i++)
    {
        printf("%d ", Heap->Array[i]);
    }

    putchar('\n');

    return 0;
}