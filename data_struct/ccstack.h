#pragma once

// Additional declarations
#define EMPTY_STACK "cannot perform this operation on an empty stack\n"

typedef struct _STACK_NODE {
    int Value;
    struct _STACK_NODE* Next;
} STACK_NODE, * PSTACK_NODE;



// Mandatory declarations
typedef struct _CC_STACK {
    STACK_NODE* Top;  // pointer to the first node of the stack
    STACK_NODE* Bottom;  // pointer to the last node of the stack
    unsigned int Count;  // number of elements in the stack
} CC_STACK;

int StCreate(CC_STACK **Stack);
int StDestroy(CC_STACK **Stack);

int StPush(CC_STACK *Stack, int Value);
int StPop(CC_STACK *Stack, int *Value);

// Gets top of stack without popping the value
int StPeek(CC_STACK *Stack, int *Value);

//  Returns:
//       1  - Stack is empty
//       0  - Stack is not empty
//      -1  - Error or invalid parameter
int StIsEmpty(CC_STACK *Stack);

// Returns the number of elements in the stack
int StGetCount(CC_STACK *Stack);

// Removes all elements from the stack
int StClear(CC_STACK *Stack);

// StPushStack removes all the elements from the StackToPush and appends
// them to the first stack
// ex: Stack1: 1, 2, 3
//     Stack2: 1, 4, 5
// After push: Stack1: 1, 2, 3, 1, 4, 5
//             Stack2: empty
int StPushStack(CC_STACK *Stack, CC_STACK *StackToPush);



// Aditional declarations
static int StNodeCreate(STACK_NODE** Node, int Value);
static int StNodeDestroy(STACK_NODE** Node);
static int StPushAtEnd(CC_STACK* Stack, int Value);
int StPrint(CC_STACK* Stack);

