#include "ccstack.h"
#include "common.h"


// Mandatory definitions

int StCreate(CC_STACK **Stack)
{
    if (NULL == Stack)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    *Stack = (CC_STACK*)malloc(sizeof(CC_STACK));

    if (NULL == *Stack)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    (*Stack)->Top = NULL;
    (*Stack)->Bottom = NULL;
    (*Stack)->Count = 0;

    return 0;
}

int StDestroy(CC_STACK **Stack)
{
    if (NULL == Stack)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    
    if (NULL == *Stack)
    {
        return 0;
    }

    STACK_NODE* current = (*Stack)->Top;
    STACK_NODE* next = (*Stack)->Top;

    while (current != NULL)
    {
        next = current->Next;
        
        free(current);

        current = next;
    }

    free(*Stack);

    *Stack = NULL;

    return 0;
}

int StPush(CC_STACK *Stack, int Value)
{
    if (NULL == Stack)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    STACK_NODE* newStNode = NULL;

    if (StNodeCreate(&newStNode, Value) == -1)
    {
        return -1;
    }

    newStNode->Next = Stack->Top;
    Stack->Top = newStNode;

    if (NULL == Stack->Bottom)
    {
        Stack->Bottom = newStNode;
    }

    Stack->Count = Stack->Count + 1;

    return 0;
    
}

int StPop(CC_STACK *Stack, int *Value)
{
    STACK_NODE* nextStNode = NULL;

    if (NULL == Value)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (NULL == Stack)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        *Value = 0;
        return -1;
    }

    if (NULL == Stack->Top)
    {
        GENERIC_LOG(EMPTY_STACK);
        *Value = 0;
        return -1;
    }

    *Value = Stack->Top->Value;
    nextStNode = Stack->Top->Next;

    if (StNodeDestroy(&(Stack->Top)) == -1)
    {
        return -1;
    }

    Stack->Top = nextStNode;

    if (NULL == nextStNode)
    {
        Stack->Bottom = NULL;
    }

    Stack->Count = Stack->Count - 1;

    return 0;
}

int StPeek(CC_STACK *Stack, int *Value)
{
    if (NULL == Value)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (NULL == Stack)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        *Value = 0;
        return -1;
    }

    if (NULL == Stack->Top)
    {
        GENERIC_LOG(EMPTY_STACK);
        *Value = 0;
        return -1;
    }
    
    *Value = Stack->Top->Value;

    return 0;
}

int StIsEmpty(CC_STACK *Stack)
{
    if (NULL == Stack)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Stack->Top)
    {
        return 1;
    }

    return 0;
}

int StGetCount(CC_STACK *Stack)
{
    if(NULL == Stack)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    return Stack->Count;
}

int StClear(CC_STACK *Stack)
{
    if (NULL == Stack)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    STACK_NODE* current = Stack->Top;
    STACK_NODE* next = NULL;

    while (current != NULL)
    {
        next = current->Next;
        free(current);
        current = next;
    }

    Stack->Top = NULL;
    Stack->Bottom = NULL;
    Stack->Count = 0;

    return 0;
}

int StPushStack(CC_STACK *Stack, CC_STACK *StackToPush)
{
    if (NULL == Stack)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == StackToPush)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    int value = 0;

    while (StIsEmpty(StackToPush) == 0)
    {
        if (StPop(StackToPush, &value) == -1)
        {
            return -1;
        }
        if (StPushAtEnd(Stack, value) == -1)
        {
            return -1;
        }
    }

    return 0;
}



// Aditional definitons

static int StNodeCreate(STACK_NODE** Node, int Value)
{
    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    *Node = (STACK_NODE*)malloc(sizeof(STACK_NODE));

    if (NULL == *Node)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    (*Node)->Value = Value;
    (*Node)->Next = NULL;

    return 0;
}

static int StNodeDestroy(STACK_NODE** Node)
{
    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL != *Node)
    {
        free(*Node);
    }

    *Node = NULL;

    return 0;
}

static int StPushAtEnd(CC_STACK* Stack, int Value)
{
    if (NULL == Stack)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    STACK_NODE* newStackNode = NULL;

    if (StNodeCreate(&newStackNode, Value) == -1)
    {
        return -1;
    }

    newStackNode->Next = NULL;
    
    // The stack is empty
    if (NULL == Stack->Top)
    {
        Stack->Top = newStackNode;
        Stack->Bottom = newStackNode;
        Stack->Count = 1;

        return 0;
    }

    // The stack has exaclty one node
    if (Stack->Top == Stack->Bottom)
    {
        Stack->Top->Next = newStackNode;
        Stack->Bottom->Next = newStackNode;
        Stack->Count = 2;

        return 0;
    }

    // The stack has more than one node
    Stack->Bottom->Next = newStackNode;
    Stack->Bottom = newStackNode;
    Stack->Count = Stack->Count + 1;

    return 0;
}

int StPrint(CC_STACK* Stack)
{
    if (NULL == Stack)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Stack->Top)
    {
        GENERIC_LOG(EMPTY_STACK);
        return -1;
    }

    STACK_NODE* current = Stack->Top;

    do
    {
        printf("%d ", current->Value);
        current = current->Next;
    } while (current != NULL);

    putchar('\n');

    return 0;
}