#pragma once

typedef struct _LSTNODE
{
    char* Key;
    int Value;
    struct _LSTNODE* Next;
    struct _LSTNODE* Prev;
} LSTNODE;

typedef struct _LIST
{
    LSTNODE Head;
    int Index;  // the index at which this list is located in the hash table
    int Count;  // number of nodes in the list
} LIST;

int ListCreate(LIST** List);
int ListDestroy(LIST** List);
int ListClear(LIST* List);
static int ListNodeCreate(LSTNODE** Node, char* Key, int Value);
static int ListNodeDestroy(LSTNODE** Node);
int ListInsertHead(LIST* List, char* Key, int Value);
int ListRemoveHead(LIST* List);
int ListRemoveNode(LIST* List, LSTNODE* Node);
int ListPrint(LIST* List);

// Returns:
//  1   -   key is in the list
//  0   -   key is not in the list
// -1   -   error or invalid parameter
// Also returns, through parameter Node, a pointer to the node that has the key or NULL if the key is not in the list
int ListGetNodeByKey(LIST* List, char* Key, LSTNODE** Node);

//  Returns:
//       1  - Keys are equal
//       0  - Keys are not equal
//      -1  - Error or invalid parameter
static int isEqual(char* Key1, char* Key2);
