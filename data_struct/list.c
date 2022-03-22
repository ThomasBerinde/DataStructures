#include "list.h"
#include "common.h"

int ListCreate(LIST** List)
{
    if (NULL == List)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    
    (*List) = (LIST*)malloc(sizeof(LIST));

    if (NULL == *List)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    (*List)->Head.Key = NULL;
    (*List)->Head.Next = NULL;
    (*List)->Head.Prev = NULL;
    (*List)->Head.Value = 0;
    (*List)->Index = 0;
    (*List)->Count = 0;

    return 0;
}

int ListDestroy(LIST** List)
{
    if (NULL == List)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == *List)
    {
        GENERIC_LOG("Program attempted to destroy a list which was NULL\n");
        return -1;
    }

    if (0 != ListClear(*List))
    {
        return -1;
    }

    free(*List);

    *List = NULL;

    return 0;
}

int ListClear(LIST* List)
{
    if (NULL == List)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    // Initialize current node with the first element of the list
    LSTNODE* currentNode = List->Head.Next;
    LSTNODE* temporaryNode = NULL;

    // If the program tries to clear an empty list, it will not return an error

    while (NULL != currentNode)
    {
        temporaryNode = currentNode->Next;
        if (0 != ListNodeDestroy(&currentNode))
        {
            return -1;
        }
        currentNode = temporaryNode;
    }

    List->Head.Next = NULL;
    List->Count = 0;

    return 0;
}

static int ListNodeCreate(LSTNODE** Node, char* Key, int Value)
{
    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    *Node = (LSTNODE*)malloc(sizeof(LSTNODE));

    if (NULL == *Node)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    if (NULL == Key)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    int keyLength = 0;

    while ('\0' != Key[keyLength])
    {
        keyLength++;
    }

    (*Node)->Key = (char*)malloc((keyLength + 1) * sizeof(char));

    if (NULL == (*Node)->Key)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    for (int i = 0; i <= keyLength; i++)
    {
        (*Node)->Key[i] = Key[i];
    }

    if ('\0' != (*Node)->Key[keyLength])
    {
        GENERIC_LOG("String terminator character not assigned\n");
        free((*Node)->Key);
        return -1;
    }

    // Initialize the other fields of the Node
    (*Node)->Value = Value;
    (*Node)->Next = NULL;
    (*Node)->Prev = NULL;

    return 0; 
}

static int ListNodeDestroy(LSTNODE** Node)
{
    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == *Node)
    {
        GENERIC_LOG("Program tried to free NULL pointer\n");
        return -1;
    }

    if (NULL != (*Node)->Key)
    {
        free((*Node)->Key);
    }

    free(*Node);

    *Node = NULL;

    return 0;
}

int ListInsertHead(LIST* List, char* Key, int Value)
{
    if (NULL == List)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    LSTNODE* newNode = NULL;

    if (0 != ListNodeCreate(&newNode, Key, Value))
    {
        return -1;
    }
    
    // List is empty
    if (NULL != List->Head.Next)
    {
        List->Head.Next->Prev = newNode;    // next to node
    }
    newNode->Next = List->Head.Next;        // node to next
    List->Head.Next = newNode;              // head to node
    newNode->Prev = &(List->Head);          // node to head

    List->Count = List->Count + 1;

    return 0;
}

int ListRemoveHead(LIST* List)
{
    if (NULL == List)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == List->Head.Next)
    {
        GENERIC_LOG("Cannot perform this operation on an empty list\n");
        return -1;
    }

    // Create a pointer to the first element of the list
    LSTNODE* firstNode = List->Head.Next;

    List->Head.Next = firstNode->Next;          // head to second

    // List has more than one node
    if (NULL != firstNode->Next)
    {
        firstNode->Next->Prev = &(List->Head);  // second to head
    }

    if (0 != ListNodeDestroy(&firstNode))
    {
        return -1;
    }

    List->Count = List->Count - 1;

    return 0;
}

int ListRemoveNode(LIST* List, LSTNODE* Node)
{
    if (NULL == List)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == List->Head.Next)
    {
        GENERIC_LOG("Cannot perform this operation on an empty list\n");
        return -1;
    }

    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    // Node is the first in the list
    if (Node->Prev == &(List->Head))
    {
        return ListRemoveHead(List);
    }

    // Node is an internal node or the last in the list
    Node->Prev->Next = Node->Next;

    // Node is an internal node
    if (Node->Next != NULL)
    {
        Node->Next->Prev = Node->Prev;
    }

    free(Node);
    List->Count = List->Count - 1;

    return 0;
}

int ListPrint(LIST* List)
{
    if (NULL == List)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return - 1;
    }

    if (NULL == List->Head.Next)
    {
        printf("\n");
        return 0;
    }

    LSTNODE* currentNode = List->Head.Next;

    while (NULL != currentNode)
    {
        printf("%s ", currentNode->Key);
        currentNode = currentNode->Next;
    }

    putchar('\n');

    return 0;
}

int ListGetNodeByKey(LIST* List, char* Key, LSTNODE** Node)
{
    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_3);
        return -1;
    }

    (*Node) = NULL;

    if (NULL == List)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Key)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    // Search for the key in the list
    LSTNODE* currentNode = List->Head.Next;
    while (NULL != currentNode && 1 != isEqual(currentNode->Key, Key))
    {
        currentNode = currentNode->Next;
    }

    if (NULL == currentNode)
    {
        return 0;
    }

    (*Node) = currentNode;

    return 1;
}

int static isEqual(char* Key1, char* Key2)
{
    if (NULL == Key1)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Key2)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    unsigned int i = 0;

    while (Key1[i] != '\0' && Key2[i] != '\0' && Key1[i] == Key2[i])
    {
        i++;
    }
    
    if (Key1[i] == '\0' && Key2[i] == '\0')
    {
        return 1;
    }

    return 0;
}