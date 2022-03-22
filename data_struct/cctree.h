#pragma once
// NEXT TASKS:
// - make functions that print the pre/in/post-orders to vectors and check for the nth element
// - make TreeClear iterative
// - make tests for all the functions

// *** IMPORTANT ***
// Tree rebalancing might put elements equal to an element to its left
// However, the tree is designed to put them to the right

typedef enum _COLOR {RED, BLACK, DBLACK} COLOR;
typedef enum _DIRECTION {NE, NW, SE, SW, E, W} DIRECTION;

typedef struct _TREE_NODE {
    int Value;
    COLOR Color;
    struct _TREE_NODE* LeftChild;
    struct _TREE_NODE* RightChild;
    struct _TREE_NODE* Parent;
} TREE_NODE, * PTREE_NODE;

typedef struct _CC_TREE {
    TREE_NODE* Root;
    TREE_NODE* Sentinel;  // Represents all leaves of the red-black tree
    int Count;
} CC_TREE;

int TreeCreate(CC_TREE** Tree);
int TreeDestroy(CC_TREE **Tree);

// Duplicates are allowed
int TreeInsert(CC_TREE *Tree, int Value);

// Removes all elements equal to Value
int TreeRemove(CC_TREE *Tree, int Value);

//  Returns:
//       1  - Tree contains Value
//       0  - Tree does not contain Value
//      -1  - Error or invalid parameter
int TreeContains(CC_TREE *Tree, int Value);

// Returns the number of elements in Tree or -1 in case of error or invalid parameter
int TreeGetCount(CC_TREE *Tree);

// Returns the height of Tree or -1 in case of error or invalid parameter
int TreeGetHeight(CC_TREE *Tree);

// Removes every element of the tree
int TreeClear(CC_TREE *Tree);

int TreeGetNthPreorder(CC_TREE *Tree, int Index, int *Value);
int TreeGetNthInorder(CC_TREE *Tree, int Index, int *Value);
int TreeGetNthPostorder(CC_TREE *Tree, int Index, int *Value);


// Aditional functions declarations
static int _TreeNodeCreate(TREE_NODE** Node, TREE_NODE* Sentinel, int Value);
static int _TreeNodeDestroy(TREE_NODE** Node);
static int _LeftRotate(CC_TREE* Tree, TREE_NODE* Node);
static int _RightRotate(CC_TREE* Tree, TREE_NODE* Node);
static int _InsertFix(CC_TREE* Tree, TREE_NODE* Node);
static int _RemoveFix(CC_TREE* Tree, TREE_NODE* Node);
static int _GetBlackHeight(TREE_NODE* Root, int* IsRedBlack);
// Returns:
//  1   -  Tree contains the Node
//  0   -  Tree doens't contain the Node
//  -1  -  Error or invalid parameter
static int _FindNode(CC_TREE* Tree, int Value, TREE_NODE** Node);
static int _TreeRemove(CC_TREE* Tree, TREE_NODE* Node);
static int _SwapValues(int* a, int* b);

void _TreePrintPreorder(CC_TREE* Tree, TREE_NODE* Root);
//void _TreePrintInorder(TREE_NODE* Root);
//void _TreePrintPostorder(TREE_NODE* Root);
int _IsRedBlack(CC_TREE* Tree);