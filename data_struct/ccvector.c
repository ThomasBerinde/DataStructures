#include "ccvector.h"
#include "ccstack.h"
#include "common.h"
#include "string.h"

// *** IMPORTANT ***
// VecInsertAfterIndex actually inserts AT index. I left
// it like this because it was your code that inserts it
// there.
// However, the test given by you in the main function expects
// the value to be inserted AFTER the index.

#define INITIAL_SIZE    100

int VecCreate(CC_VECTOR **Vector)
{
    CC_VECTOR *vec = NULL;

    if (NULL == Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    vec = (CC_VECTOR*)malloc(sizeof(CC_VECTOR));
    if (NULL == vec)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    memset(vec, 0, sizeof(*vec));

    vec->Count = 0;
    vec->Size = INITIAL_SIZE;
    vec->Array = (int*)malloc( sizeof(int) * INITIAL_SIZE );
    if (NULL == vec->Array) 
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        free(vec);
        return -1;
    }

    *Vector = vec;

    return 0;
}

int VecDestroy(CC_VECTOR **Vector)
{
    if (NULL == Vector || NULL == *Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL != (*Vector)->Array)
    {
        free((*Vector)->Array);
        (*Vector)->Array = NULL;
    }
    
    free(*Vector);

    *Vector = NULL;

    return 0;
}

int VecInsertTail(CC_VECTOR *Vector, int Value)
{
    int* arrayCopy;

    if (NULL == Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        if (Vector->Count > Vector->Size)
        {
            GENERIC_LOG("Count > Size\n");
            return -1;
        }

        /// REALLOC

        // Create a copy of the pointer to the array
        // If the reallocation fails, we can restore the pointer
        // If we wouldn't do this, we wouldn't be able to free the memory afterwards
        arrayCopy = Vector->Array;

        // Allocate space for more elements using the reallocation constant factor
        Vector->Size = (int)(Vector->Count * REALLOCATION_CONSTANT_FACTOR) + 1;
        Vector->Array = (int*)realloc(Vector->Array, Vector->Size * sizeof(int));
        
        // Check if the reallocation was successful 
        if (NULL == Vector->Array)
        {
            GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
            Vector->Array = arrayCopy;  // restore the pointer to the array 
            return -1;
        }
    }

    Vector->Array[Vector->Count] = Value;
    Vector->Count++;

    return 0;
}

int VecInsertHead(CC_VECTOR *Vector, int Value)
{
    int* arrayCopy;

    if (NULL == Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        if (Vector->Count > Vector->Size)
        {
            GENERIC_LOG("Count > Size\n");
            return -1;
        }
        /// REALLOC

        // Create a copy of the Array pointer
        arrayCopy = Vector->Array;

        // Allocate space for one more element
        Vector->Size = Vector->Count + 1;
        Vector->Array = (int*)realloc(Vector->Array, Vector->Size * sizeof(int));

        // Check if the reallocation was successful 
        if (NULL == Vector->Array)
        {
            GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
            Vector->Array = arrayCopy;  // restore the pointer to the array 
            return -1;
        }
    }

    for (int i = Vector->Count-1; i >= 0; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i];
    }
    Vector->Array[0] = Value;
    Vector->Count++;

    return 0;
}

int VecInsertAfterIndex(CC_VECTOR *Vector, int Index, int Value)
{
    int* arrayCopy;

    if (NULL == Vector || NULL == Vector->Array)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Index < 0 || Index > Vector->Count)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (Vector->Count >= Vector->Size)
    {
        if (Vector->Count > Vector->Size)
        {
            GENERIC_LOG("Count > Size\n");
            return -1;
        }

        /// REALLOC

        // Create a copy of the Array pointer
        arrayCopy = Vector->Array;

        // Allocate space for one more element
        Vector->Size = Vector->Count + 2;  // increase by two here just because of the warnings
        Vector->Array = (int*)realloc(Vector->Array, Vector->Size * sizeof(int));

        // Check if the reallocation was successful 
        if (NULL == Vector->Array)
        {
            GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
            Vector->Array = arrayCopy;  // restore the pointer to the array 
            return -1;
        }

        Vector->Array[Vector->Count] = 0;
    }

    for (int i = Vector->Count - 1; i >= Index; i--)
    {
        Vector->Array[i + 1] = Vector->Array[i];
    }
    Vector->Array[Index] = Value;
    Vector->Count++;

    return 0;
}

int VecRemoveByIndex(CC_VECTOR *Vector, int Index)
{
    if (NULL == Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Index < 0 || Index >= Vector->Count)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    for (int i = Index; i < Vector->Count - 1; i++)
    {
        Vector->Array[i] = Vector->Array[i + 1];
    }

    Vector->Count = Vector->Count - 1;

    return 0;
}

int VecGetValueByIndex(CC_VECTOR *Vector, int Index, int *Value)
{
    if (NULL == Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Index < 0 || Index >= Vector->Count)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    *Value = Vector->Array[Index];

    return 0;
}

int VecGetCount(CC_VECTOR *Vector)
{
    if (NULL == Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Vector->Count < 0)
    {
        GENERIC_LOG("Count < 0\n");
        return -1;
    }

    if (Vector->Count > Vector->Size)
    {
        GENERIC_LOG("Count > Size\n");
        return -1;
    }

    return Vector->Count;
}

int VecClear(CC_VECTOR *Vector)
{
    if (NULL == Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL != Vector->Array)
    {
        free(Vector->Array);
    }

    Vector->Array = NULL;
    Vector->Size = 0;
    Vector->Count = 0;

    return 0;
}

// Iterative O(n*logn) in 99.9999% of cases
int VecSort(CC_VECTOR *Vector)
{
    if (NULL == Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    if (NULL == Vector->Array)
    {
        GENERIC_LOG("Program tried to sort an empty vector\n");
        return -1;
    }

    if (0 != _QuickSort(Vector->Array, 0, Vector->Count - 1))
    {
        return -1;
    }

    return _InsertionSort(Vector->Array, Vector->Count);
}

int VecAppend(CC_VECTOR *DestVector, CC_VECTOR *SrcVector)
{
    int* arrayCopy;

    if (NULL == DestVector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == SrcVector)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }


    if (SrcVector->Size < SrcVector->Count + DestVector->Count)
    {
        /// REALLOC

        // Make a copy of the array pointer
        arrayCopy = SrcVector->Array;

        // Allocate enough memory for the elements from both arrays
        SrcVector->Size = SrcVector->Count + DestVector->Count;
        SrcVector->Array = (int*)realloc(SrcVector->Array, SrcVector->Size * sizeof(int));

        // Check if the reallocation was successful
        if (NULL == SrcVector->Array)
        {
            GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
            SrcVector->Array = arrayCopy;  // restore the pointer to the array
            return -1;
        }
    }

    for (int i = SrcVector->Count; i < SrcVector->Count + DestVector->Count; i++)
    {
        SrcVector->Array[i] = DestVector->Array[i - SrcVector->Count];
    }

    SrcVector->Count = SrcVector->Count + DestVector->Count;

    return 0;
}



// Aditional functions
int _VecIsSorted(CC_VECTOR* Vector)
{
    if (NULL == Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Vector->Array)
    {
        GENERIC_LOG(EMPTY_VECTOR);
        return -1;
    }

    for (int i = 0; i < Vector->Count - 1; i++)
    {
        if (Vector->Array[i] < Vector->Array[i + 1])
        {
            return 0;
        }
    }

    return 1;
}
int _VecPrint(CC_VECTOR* Vector)
{
    if (NULL == Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Vector->Count == 0 || NULL == Vector->Array)
    {
        printf("Vector is empty\n\n");
        return -1;
    }
    
    for (int i = 0; i < Vector->Count; i++)
    {
        printf("%d ", Vector->Array[i]);
    }

    putchar('\n');

    return 0;
}
int _VecGetIndexByValue(CC_VECTOR* Vector, int* Index, int Value)
{
    if (NULL == Vector)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Index)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    *Index = -1;

    int i = 0, found = 0;

    while (i < Vector->Count && 1 != found)
    {
        if (Value == Vector->Array[i])
        {
            *Index = i;
            found = 1;
        }
        i++;
    }

    if (1 == found)
    {
        return 0;
    }

    return -1;
}
static int _SwapValues(int* Number1, int* Number2)
{
    if (NULL == Number1)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    if (NULL == Number2)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    int auxNumber = *Number1;
    *Number1 = *Number2;
    *Number2 = auxNumber;

    return 0;
}
static int _Partition(int* Array, int* PivotPos, int Left, int Right)
{
    if (NULL == Array)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    if (NULL == PivotPos)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    int pivotPos = 5831;
    int partitionSize = Right - Left + 1;
    int partitionSizeCopy = partitionSize;
    int median1 = 0;
    int median2 = 0;
    int median3 = 0;
    int i = Left - 1;

    // Choose a pseudo random pivot position 
    while (partitionSizeCopy > 0)
    {
        pivotPos = ((pivotPos << 5) + pivotPos) + Array[Left + partitionSizeCopy - 1];
        partitionSizeCopy = partitionSizeCopy / 10;
    }
    if (pivotPos < 0)
    {
        pivotPos = pivotPos * (-1);
    }
    pivotPos = pivotPos % partitionSize + Left;

    // Choose the median of 3
    median1 = pivotPos;
    if (Left == pivotPos)
    {
        median2 = pivotPos + 1;
        median3 = pivotPos + 2;
    }
    else if (Right == pivotPos)
    {
        median2 = pivotPos - 1;
        median3 = pivotPos - 2;
    }
    else
    {
        median2 = pivotPos + 1;
        median3 = pivotPos - 1;
    }
    if ((Array[median1] <= Array[median2] && Array[median2] <= Array[median3]) || (Array[median3] <= Array[median2] && Array[median2] <= Array[median1]))
    {
        pivotPos = median2;
    }
    else if ((Array[median1] <= Array[median3] && Array[median3] <= Array[median2]) || (Array[median2] <= Array[median3] && Array[median3] <= Array[median1]))
    {
        pivotPos = median3;
    }

    // Swap the pivot with the last element of the array
    if (0 != _SwapValues(&Array[pivotPos], &Array[Right]))
    {
        return -1;
    }

    // Begin partition
    for (int j = Left; j <= Right - 1; j++)
    {
        // If current element is smaller than the pivot
        if (Array[j] > Array[Right])
        {
            i++; // increment index of smaller element
            if (0 != _SwapValues(&Array[i], &Array[j]))
            {
                return -1;
            }
        }
    }
    if (0 != _SwapValues(&Array[i + 1], &Array[Right]))
    {
        return -1;
    }
    
    *PivotPos = i + 1;

    return 0;
}
static int _QuickSortRecursive(int* Array, int Left, int Right)
{
    int pivotPos = 0;

    if (Left < Right)
    {
        int status1 = 0;
        int status2 = 0;
        int const minPartitionSize = 4;

        if (0 != _Partition(Array, &pivotPos, Left, Right))
        {
            printf("Partition failed\n");
            return -1;
        }
        
        // Don't call quick sort on small arrays
        if (pivotPos - Left > minPartitionSize)
        {
            status1 = _QuickSort(Array, Left, pivotPos - 1);
        }
        if (Right - pivotPos > minPartitionSize)
        {
            status2 = _QuickSort(Array, pivotPos + 1, Right);
        }

        if (status1 != 0)
        {
            return status1;
        }

        if (status2 != 0)
        {
            return status2;
        }

        return 0;
    }

    return 0;
}
static int _InsertionSort(int* Array, int Size)
{
    if (NULL == Array)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    
    int k = 0;
    int currentValue = 0;

    for (int i = 1; i < Size; i++)
    {
        k = i - 1;
        currentValue = Array[i];

        while (k >= 0 && currentValue > Array[k])
        {
            Array[k + 1] = Array[k];
            k = k - 1;
        }

        Array[k + 1] = currentValue;
    }

    return 0;
}
static int _QuickSort(int* Array, int Left, int Right)
{
    CC_STACK* stack = NULL;
    int pivotPos = 0;
    int status = 0;
    int left = 0;
    int right = 0;
    //int const minPartitionSize = 4;

    // Create stack
    if (0 != StCreate(&stack))
    {
        return -1;
    }

    // Push array margins on stack
    if (0 != StPush(stack, Left))
    {
        return -1;
    }
    if (0 != StPush(stack, Right))
    {
        return -1;
    }

    while (1 != (status = StIsEmpty(stack)))
    {
        // Check for error
        if (-1 == status)
        {
            return -1;
        }

        // Pop margins
        if (0 != StPop(stack, &right))
        {
            return -1;
        }
        if (0 != StPop(stack, &left))
        {
            return -1;
        }

        // Partition and get pivot position
        if (0 != _Partition(Array, &pivotPos, left, right))
        {
            printf("Partition failed\n");
            return -1;
        }

        if (pivotPos - 1 > left)// + minPartitionSize)
        {
            if (0 != StPush(stack, left))
            {
                return -1;
            }
            if (0 != StPush(stack, pivotPos - 1))
            {
                return -1;
            }
        }

        if (pivotPos + 1 < right)// - minPartitionSize)
        {
            if (0 != StPush(stack, pivotPos + 1))
            {
                return -1;
            }
            if (0 != StPush(stack, right))
            {
                return -1;
            }
        }
    }

    if (0 != StDestroy(&stack))
    {
        return -1;
    }

    return 0;
}