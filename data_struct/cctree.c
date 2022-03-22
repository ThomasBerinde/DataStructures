#include "cctree.h"
#include "common.h"


// *** IMPORTANT ***
// Tree rebalancing might put elements equal to an element to its left
// However, the tree is designed to put them to the right

// Iterative O(1)
int TreeCreate(CC_TREE **Tree)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    *Tree = (CC_TREE*)malloc(sizeof(CC_TREE));

    if (NULL == *Tree)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    (*Tree)->Root = NULL;
    (*Tree)->Count = 0;

    (*Tree)->Sentinel = (TREE_NODE*)malloc(sizeof(TREE_NODE));

    if (NULL == (*Tree)->Sentinel)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    (*Tree)->Sentinel->Color = BLACK;
    (*Tree)->Sentinel->LeftChild = NULL;
    (*Tree)->Sentinel->RightChild = NULL;
    (*Tree)->Sentinel->Parent = (*Tree)->Sentinel;
    (*Tree)->Sentinel->Value = 0;

    return 0;
}

// Iterative O(n)
int TreeDestroy(CC_TREE **Tree)
{
    if (NULL == Tree || NULL == *Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == (*Tree)->Root)
    {
        free(*Tree);
        *Tree = NULL;
        return 0;
    }
    
    int status = TreeClear(*Tree);

    if (0 == status)
    {
        *Tree = NULL;
    }

    return status;
}

// Iterative O(logn)
int TreeInsert(CC_TREE *Tree, int Value)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    TREE_NODE* newNode;
    if (0 != _TreeNodeCreate(&newNode, Tree->Sentinel, Value))
    {
        return -1;
    }

    if (NULL == Tree->Root)
    {
        Tree->Root = newNode;
        Tree->Count = Tree->Count + 1;
        newNode->Color = BLACK; 
        return 0;
    }

    TREE_NODE* currentNode = Tree->Root;
    int done = 0;

    do
    {
        // Check the strict/non-strict inequality
        if (newNode->Value < currentNode->Value)
        {
            if (Tree->Sentinel != currentNode->LeftChild)
            {
                currentNode = currentNode->LeftChild;
            }
            else
            {
                currentNode->LeftChild = newNode;
                newNode->Parent = currentNode;
                done = 1;
            }
        }
        else
        {
            if (Tree->Sentinel != currentNode->RightChild)
            {
                currentNode = currentNode->RightChild;
            }
            else
            {
                currentNode->RightChild = newNode;
                newNode->Parent = currentNode;
                done = 1;
            }
        }
    } while (0 == done);

    Tree->Count = Tree->Count + 1;

    // Balance the red-black tree
    if (0 != _InsertFix(Tree, newNode))
    {
        return -1;
    }

    return 0;
}

// Recursive O(logn) (nr of recursions is guaranteed to be very low - max 2)
int TreeRemove(CC_TREE *Tree, int Value)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Tree->Root)
    {
        GENERIC_LOG("Cannot perform this operation on an emtpy tree\n");
        return -1;
    }
    
    TREE_NODE* Node = Tree->Root;

    if (1 != _FindNode(Tree, Value, &Node))
    {
        if (NULL == Node)
        {
            GENERIC_LOG("Program tried to delete a node which is not in the tree\n");
            return -1;
        }
        else
        {
            GENERIC_LOG("Error, something is wrong\n");
            return -1;
        }
    }

    if (0 != _TreeRemove(Tree, Node))
    {
        return -1;
    }

    int status = 1;

    do
    {
        status = _FindNode(Tree, Value, &Node);

        if (-1 == status)
        {
            return -1;
        }
        else if (0 == status)
        {
            return 0;
        }
        else if (1 == status)
        {
            if (NULL == Node)
            {
                GENERIC_LOG("Value found but Node is NULL\n");
                return -1;
            }

            if (0 != _TreeRemove(Tree, Node))
            {
                return -1;
            }
        }
    } while (1 == status);

    // If we get here then it's a problem
    GENERIC_LOG("Program shouldn't reach this code\n");
    return -1;
}

// Iterative O(logn)
int TreeContains(CC_TREE *Tree, int Value)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    TREE_NODE* Node = NULL;

    return _FindNode(Tree, Value, &Node);
}

// Iterative O(1)
int TreeGetCount(CC_TREE *Tree)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    
    return Tree->Count;
}

// Iterative O(n)
int TreeGetHeight(CC_TREE *Tree)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    TREE_NODE* currentNode = Tree->Root;
    int treeHeight = 0;
    int currentHeight = 0;
    DIRECTION direction = NE;

    do
    {
        if (Tree->Sentinel != currentNode->LeftChild && (NE == direction || NW == direction))
        {
            currentNode = currentNode->LeftChild;
            direction = NE;
            currentHeight++;

            if (currentHeight > treeHeight)
            {
                treeHeight = currentHeight;
            }
        }
        else if (Tree->Sentinel != currentNode->RightChild && (SE != direction))
        {
            currentNode = currentNode->RightChild;
            direction = NW;
            currentHeight++;

            if (currentHeight > treeHeight)
            {
                treeHeight = currentHeight;
            }
        }
        else
        {
            if (NULL == currentNode->Parent)
            {
                GENERIC_LOG("Loop arrived back to root node and didn't exit\n");
                return -1;
            }
            else if (currentNode == currentNode->Parent->LeftChild)
            {
                direction = SW;
            }
            else if (currentNode == currentNode->Parent->RightChild)
            {
                direction = SE;
            }
            currentNode = currentNode->Parent;
            currentHeight--;
        }
    } 
    while (Tree->Root != currentNode);

    return 0;
}

// Iterative O(n)
int TreeClear(CC_TREE *Tree)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    TREE_NODE* currentNode = Tree->Root;
    TREE_NODE* temporaryNode = NULL;
    DIRECTION direction = NE;

    do
    {
        // First time we go to the left child, if there is one
        if (Tree->Sentinel != currentNode->LeftChild)
        {
            currentNode = currentNode->LeftChild;
            direction = NE;
        }
        // If the node has no left child then we go to the right child
        else if (Tree->Sentinel != currentNode->RightChild)
        {
            currentNode = currentNode->RightChild;
            direction = NW;
        }
        // If the node has no children, we delete it and go to its parent
        else
        {
            if (NULL == currentNode->Parent)
            {
                GENERIC_LOG("Error: Node has no parent and is not root node\n");
                return -1;
            }

            // Replace the node with the sentinel
            if (currentNode->Parent->LeftChild == currentNode)
            {
                currentNode->Parent->LeftChild = Tree->Sentinel;
                direction = SW;
            }
            else if (currentNode->Parent->RightChild == currentNode)
            {
                currentNode->Parent->RightChild = Tree->Sentinel;
                direction = SE;
            }
            else
            {
                GENERIC_LOG("Error\n");
                return -1;
            }

            // Delete the node and continue
            temporaryNode = currentNode->Parent;
            free(currentNode);
            currentNode = temporaryNode;
        }
    } while (currentNode != Tree->Root || direction != SE);

    if (Tree->Root != currentNode)
    {
        GENERIC_LOG("Error\n");
        return -1;
    }

    free(Tree->Root);
    Tree->Root = NULL;

    return 0;
}

// Iterative O(n)
int TreeGetNthPreorder(CC_TREE *Tree, int Index, int *Value)
{
    if (NULL == Tree || 0 == Tree->Count)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Index < 1 || Index > Tree->Count)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (NULL == Value)
    {
        GENERIC_LOG(INVALID_PARAM_3);
        return -1;
    }

    TREE_NODE* currentNode = Tree->Root;
    DIRECTION direction = NE;

    while (Index > 1)  // the root node is counted by default
    {
        if (Tree->Sentinel != currentNode->LeftChild && (direction == NE || direction == NW))
        {
            currentNode = currentNode->LeftChild;
            direction = NE;
            Index--;
        }
        else if (Tree->Sentinel != currentNode->RightChild && direction != SE)
        {
            currentNode = currentNode->RightChild;
            direction = NW;
            Index--;
        }
        else
        {
            // Check for the existence of the parent before using it 
            if (NULL == currentNode->Parent)
            {
                GENERIC_LOG("Node has no parent\n");
                return -1;
            }

            // If the node is a left child
            if (currentNode == currentNode->Parent->LeftChild)
            {
                direction = SW;
            }
            // If the node is a right child
            else if (currentNode == currentNode->Parent->RightChild)
            {
                direction = SE;
            }

            currentNode = currentNode->Parent;
        }
    }

    *Value = currentNode->Value;

    return 0;
}

// Iterative O(n)
int TreeGetNthInorder(CC_TREE *Tree, int Index, int *Value)
{
    if (NULL == Tree || 0 == Tree->Count)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Index < 1 || Index > Tree->Count)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (NULL == Value)
    {
        GENERIC_LOG(INVALID_PARAM_3);
        return -1;
    }

    TREE_NODE* currentNode = Tree->Root;
    DIRECTION direction = NE;

    while (Index > 0)
    {
        if (Tree->Sentinel != currentNode->LeftChild && (direction == NE || direction == NW))
        {
            currentNode = currentNode->LeftChild;
            direction = NE;
        }
        else if (Tree->Sentinel == currentNode->LeftChild && (direction == NE || direction == NW))
        {
            // Pretend as if we went to the left child and came back
            // Here is where we count the current node
            direction = SW;
            Index--;
        }
        else if (Tree->Sentinel != currentNode->RightChild && direction == SW)
        {
            // We only go to the right child once we've returned from the left child
            currentNode = currentNode->RightChild;
            direction = NW;
        }
        else
        {
            // Check for the existence of the parent before using it 
            if (NULL == currentNode->Parent)
            {
                GENERIC_LOG("Node has no parent\n");
                return -1;
            }

            // If the node is a left child
            if (currentNode == currentNode->Parent->LeftChild)
            {
                Index--;
                direction = SW;
            }
            // If the node is a right child
            else if (currentNode == currentNode->Parent->RightChild)
            {
                direction = SE;
            }

            currentNode = currentNode->Parent;
        }
    }

    *Value = currentNode->Value;

    return 0;
}

// Iterative O(n)
int TreeGetNthPostorder(CC_TREE *Tree, int Index, int *Value)
{
    if (NULL == Tree || 0 == Tree->Count)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (Index < 1 || Index > Tree->Count)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (NULL == Value)
    {
        GENERIC_LOG(INVALID_PARAM_3);
        return -1;
    }

    TREE_NODE* currentNode = Tree->Root;
    DIRECTION direction = NE;

    while (Index > 0)
    {
        // First time we go to the left child, if there is one
        if (Tree->Sentinel != currentNode->LeftChild && (direction == NE || direction == NW))
        {
            currentNode = currentNode->LeftChild;
            direction = NE;
        }
        // If the node has no left child then we go to the right child
        else if (Tree->Sentinel != currentNode->RightChild && direction != SE)
        {
            currentNode = currentNode->RightChild;
            direction = NW;
        }
        else if (Tree->Sentinel == currentNode->RightChild && (direction != SE))
        {
            // Pretend as if we went to the right child and came back
            // Here is where we count the current node
            direction = SE;
            Index--;
        }
        else
        {
            // Check for the existence of the parent before using it 
            if (NULL == currentNode->Parent)
            {
                GENERIC_LOG("Node has no parent\n");
                return -1;
            }

            // If the node is a left child
            if (currentNode == currentNode->Parent->LeftChild)
            {
                direction = SW;
            }
            // If the node is a right child
            else if (currentNode == currentNode->Parent->RightChild)
            {
                direction = SE;
                Index--;
            }

            currentNode = currentNode->Parent;
        }
    }

    *Value = currentNode->Value;

    return 0;
}











// Additional functions' definitions

// Iterative O(1)
static int _TreeNodeCreate(TREE_NODE** Node, TREE_NODE* Sentinel, int Value)
{
    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    *Node = (TREE_NODE*)malloc(sizeof(TREE_NODE));

    if (NULL == *Node)
    {
        GENERIC_LOG(MEMORY_ALLOCATION_FAIL);
        return -1;
    }

    (*Node)->Value = Value;
    (*Node)->Color = RED;
    (*Node)->LeftChild = Sentinel;
    (*Node)->RightChild = Sentinel;
    (*Node)->Parent = NULL;

    return 0;
}

// Iterative O(1)
static int _TreeNodeDestroy(TREE_NODE** Node)
{
    if (NULL == Node || NULL == *Node)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    free(*Node);

    *Node = NULL;

    return 0;
}

// Iterative O(1)
static int _LeftRotate(CC_TREE* Tree, TREE_NODE* Node)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (Tree->Sentinel == Node->RightChild)
    {
        GENERIC_LOG("Cannot perform left rotation on a node with a NULL right child\n");
        return -1;
    }

    TREE_NODE* temp = NULL;
    TREE_NODE* y = Node->RightChild;

    // Link Node's left child to Node's parent
    if (NULL == Node->Parent)
    {
        Tree->Root = y;
    }
    else if (Node->Parent->LeftChild == Node)
    {
        Node->Parent->LeftChild = y;
    }
    else if (Node->Parent->RightChild == Node)
    {
        Node->Parent->RightChild = y;
    }
    else
    {
        GENERIC_LOG("Error\n");
        return -1;
    }
    y->Parent = Node->Parent;

    // Store a pointer to y's left child
    temp = y->LeftChild;

    // Link the Node as y's left child
    y->LeftChild = Node;
    Node->Parent = y;

    // Link temp as the Node's right child 
    Node->RightChild = temp;
    if (Tree->Sentinel != temp)
    {
        temp->Parent = Node;
    }

    return 0;
}

// Iterative O(1)
static int _RightRotate(CC_TREE* Tree, TREE_NODE* Node)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (Tree->Sentinel == Node->LeftChild)
    {
        GENERIC_LOG("Cannot perform right rotation on a node with a NULL left child\n");
        return -1;
    }

    TREE_NODE* temp = NULL;
    TREE_NODE* y = Node->LeftChild;

    // Link Node's left child to Node's parent
    if (NULL == Node->Parent)
    {
        Tree->Root = y;
    }
    else if (Node->Parent->LeftChild == Node)
    {
        Node->Parent->LeftChild = y;
    }
    else if (Node->Parent->RightChild == Node)
    {
        Node->Parent->RightChild = y;
    }
    else
    {
        GENERIC_LOG("Error\n");
        return -1;
    }
    y->Parent = Node->Parent;

    // Store a pointer to y's right child
    temp = y->RightChild;

    // Link the Node as y's right child
    y->RightChild = Node;
    Node->Parent = y;

    // Link temp as the Node's left child 
    Node->LeftChild = temp;
    if (Tree->Sentinel != temp)
    {
        temp->Parent = Node;
    }

    return 0;
}

// Iterative O(logn)
// Calls: _LeftRotate and _RightRotate
static int _InsertFix(CC_TREE* Tree, TREE_NODE* Node)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (NULL == Node->Parent)
    {
        Node->Color = BLACK;
        return 0;
    }

    TREE_NODE* nodeUncle = NULL;

    // When we get here first time we know NULL != Node and NULL != Node->Parent
    while (NULL != Node && NULL != Node->Parent && RED == Node->Parent->Color && RED == Node->Color)
    {
        if (NULL == Node->Parent->Parent)
        {
            GENERIC_LOG("Node has no grandparent, something is wrong\n");
            return -1;
        }
        // When we get here we know NULL != Node->Parent->Parent
        else
        {
            // Following steps are dependent on the node's uncle's color 
            if (Node->Parent->Parent->LeftChild == Node->Parent)
            {
                nodeUncle = Node->Parent->Parent->RightChild;
            }
            else if (Node->Parent->Parent->RightChild == Node->Parent)
            {
                nodeUncle = Node->Parent->Parent->LeftChild;
            }
            else
            {
                GENERIC_LOG("Error\n");
                return -1;
            }

            if (Tree->Sentinel == nodeUncle || BLACK == nodeUncle->Color)
            {
                if (Node->Parent->Parent->LeftChild == Node->Parent)
                {
                    // LL
                    if (Node->Parent->LeftChild == Node)
                    {
                        _RightRotate(Tree, Node->Parent->Parent);
                        Node->Parent->Color = BLACK;
                        Node->Parent->RightChild->Color = RED;
                    }
                    // LR
                    else if (Node->Parent->RightChild == Node)
                    {
                        // Rotate
                        _LeftRotate(Tree, Node->Parent);
                        _RightRotate(Tree, Node->Parent);
                        // Recolor
                        Node->RightChild->Color = RED;
                        Node->Color = BLACK;
                    }
                    else
                    {
                        GENERIC_LOG("Error\n");
                        return -1;
                    }
                }
                else if (Node->Parent->Parent->RightChild == Node->Parent)
                {
                    // RL
                    if (Node->Parent->LeftChild == Node)
                    {
                        // Rotate
                        _RightRotate(Tree, Node->Parent);
                        _LeftRotate(Tree, Node->Parent);
                        // Recolor 
                        Node->LeftChild->Color = RED;
                        Node->Color = BLACK;
                    }
                    // RR
                    else if (Node->Parent->RightChild == Node)
                    {
                        _LeftRotate(Tree, Node->Parent->Parent);
                        Node->Parent->Color = BLACK;
                        Node->Parent->LeftChild->Color = RED;
                    }
                    else
                    {
                        GENERIC_LOG("Error\n");
                        return -1;
                    }
                }
                else
                {
                    GENERIC_LOG("Error\n");
                    return -1;
                }
            }
            else if (nodeUncle->Color == RED)
            {
                // Recolor parent and uncle
                Node->Parent->Color = BLACK;
                nodeUncle->Color = BLACK;
                // If grandparent is not root, recolor it and recheck
                if (NULL != Node->Parent->Parent->Parent)
                {
                    Node->Parent->Parent->Color = RED;
                    Node = Node->Parent->Parent;
                    // If this assignment happens we are sure that the expression in the contition of the while loop is correct
                }
                // If grandparent is root then stop
                else
                {
                    return 0;
                }
            }
        }
    }

    return 0;
}

// Iterative O(logn)
// Calls: _LeftRotate, _RightRotate and _TreeNodeCreate
static int _RemoveFix(CC_TREE* Tree, TREE_NODE* Node)
{
    /*
    CASE 1: Node is red
        - delete Node
    
    CASE 2: Root is double black
        - remove double black
    
    From here onwards, we remove the Node and replace it with a temporary node that is double black.
    After the insertions, this temporary node will become NULL

    CASE 3: Node is double black and it's sibling and both it's sibling's children are black
        - remove double black
        - add black to Node's parent (if red => black, if black => double black)
        - make sibling red
        - if Node's parent is double black, call again

    CASE 4: Node is double black and it's sibling is red
        - swap colors of Node's parent and Node's sibling 
        - rotate Node's parent in Node's direction
        - we still have double black on the temporary node, so call again

    CASE 5: Node is double black, sibling is black, it's sibling's child which is furthest from the node is black, but the nearest is red
        - swap colors of sibling with it's nearest child
        - rotate sibling in opposite direction from Node (away from the Node)
        - we still have double black on temporary node, so call again

    CASE 6: Node is double black, sibling is black, it's sibling child which is furthest from node is red
        - swap parent and sibling's color
        - parent roatates towards Node
        - remove double black and add black to the red child of its initial sibling
    */
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    if (NULL == Node->Parent)
    {
        GENERIC_LOG("Node to be deleted has no parent\n");
        return -1;
    }

    DIRECTION initDirection = NE;

    // Find the direction of the node in relation to its parent
    if (Node->Parent->LeftChild == Node)
    {
        initDirection = W;
    }
    else if (Node->Parent->RightChild == Node)
    {
        initDirection = E;
    }
    else
    {
        GENERIC_LOG("Error\n");
        return -1;
    }

    // Case 1
    if (RED == Node->Color)
    {
        if (W == initDirection)
        {
            Node->Parent->LeftChild = Tree->Sentinel;
        }
        else if (E == initDirection)
        {
            Node->Parent->RightChild = Tree->Sentinel;
        }
        else
        {
            GENERIC_LOG("Error\n");
            return -1;
        }

        free(Node);

        return 0;
    }

    TREE_NODE* tempSentinel = NULL;
    TREE_NODE* currentNode = NULL;
    TREE_NODE* nodeSibling = NULL;
    DIRECTION nodeDirection = NW;

    // Create a temporary node that will act as the NULL node with which we replace 
    // the black node which we want to be deleted
    if (0 != _TreeNodeCreate(&tempSentinel, NULL, -1))
    {
        return -1;
    }

    // The temporary sentinel's color is double black
    tempSentinel->Color = DBLACK;

#   // Replace the Node with the temporary sentinel
    if (W == initDirection)
    {
        Node->Parent->LeftChild = tempSentinel;
    }
    else if (E == initDirection)
    {
        Node->Parent->RightChild = tempSentinel;
    }
    else
    {
        GENERIC_LOG("Error\n");
        return -1;
    }
    tempSentinel->Parent = Node->Parent;

    // Remove the Node
    free(Node);

    // Fix the red-black properties if needed
    currentNode = tempSentinel;

    // First time we enter this loop we are sure that the current node is double black
    do
    {
        // Case 2
        if (NULL == currentNode->Parent)
        {
            currentNode->Color = BLACK;
            goto cleanup;
        }

        // Get Node's sibling and direction
        if (currentNode == currentNode->Parent->LeftChild)
        {
            nodeSibling = currentNode->Parent->RightChild;
            nodeDirection = W;  // node is to the left of its parent
            
        }
        else if (currentNode == currentNode->Parent->RightChild)
        {
            nodeSibling = currentNode->Parent->LeftChild;
            nodeDirection = E;  // node is to the right of its parent
        }
        else
        {
            GENERIC_LOG("Error\n");
            return -1;
        }

        // We can't have a NULL sibling so return an error if we do
        if (NULL == nodeSibling || Tree->Sentinel == nodeSibling)
        {
            GENERIC_LOG("Invalid sibling\n");
            return -1;
        }

        // Case 4
        if (RED == nodeSibling->Color)
        {
            // Swap the color of the parent with the color of the sibling
            COLOR auxColor = currentNode->Parent->Color;
            currentNode->Parent->Color = nodeSibling->Color;
            nodeSibling->Color = auxColor;

            // Rotate parent in node's direction
            if (W == nodeDirection)
            {
                _LeftRotate(Tree, currentNode->Parent);
            }
            else if (E == nodeDirection)
            {
                _RightRotate(Tree, currentNode->Parent);
            }
            else
            {
                GENERIC_LOG("Invalid direction\n");
                return -1;
            }
        }
        else if (BLACK == nodeSibling->Color)
        {
            if (W == nodeDirection)
            {
                // Case 6
                if (RED == nodeSibling->RightChild->Color)
                {
                    // Change color of far child of node's sibling to black
                    nodeSibling->RightChild->Color = BLACK;
                    
                    // Swap colors of parent and sibling
                    COLOR auxColor = currentNode->Parent->Color;
                    currentNode->Parent->Color = nodeSibling->Color;
                    nodeSibling->Color = auxColor;

                   // Rotate parent in node's direction
                    _LeftRotate(Tree, currentNode->Parent);

                    // Remove double black
                    currentNode->Color = BLACK;   
                }
                else if (BLACK == nodeSibling->RightChild->Color)
                {
                    // Case 3
                    if (BLACK == nodeSibling->LeftChild->Color)
                    {
                        // Remove double black
                        currentNode->Color = BLACK;

                        // Add black to node's parent
                        if (BLACK == currentNode->Parent->Color)
                        {
                            currentNode->Parent->Color = DBLACK;
                        }
                        else if (RED == currentNode->Parent->Color)
                        {
                            currentNode->Parent->Color = BLACK;
                        }
                        else
                        {
                            GENERIC_LOG("Invalid color\n");
                            return -1;
                        }

                        // Make sibling red
                        nodeSibling->Color = RED;

                        // Move on to the next possible double black node
                        currentNode = currentNode->Parent;
                    }
                    // Case 5
                    else if (RED == nodeSibling->LeftChild->Color)
                    {
                        // Swap color of sibling with color of sibling's nearest child to the node
                        COLOR auxColor = nodeSibling->Color;
                        nodeSibling->Color = nodeSibling->LeftChild->Color;
                        nodeSibling->LeftChild->Color = auxColor;

                        // Rotate sibling in opposite direction to node
                        _RightRotate(Tree, nodeSibling);

                        // Apply case 6, so don't do anything, it will be applied when the loop runs again
                    }
                    else
                    {
                        GENERIC_LOG("Invalid color\n");
                        return -1;
                    }
                }
                else
                {
                    GENERIC_LOG("Invalid color\n");
                    return -1;
                }
            }
            else if (E == nodeDirection)
            {
                // Case 6
                if (RED == nodeSibling->LeftChild->Color)
                {
                    // Change color of far child of node's sibling to black
                    nodeSibling->LeftChild->Color = BLACK;

                    // Swap colors of parent and sibling
                    COLOR auxColor = currentNode->Parent->Color;
                    currentNode->Parent->Color = nodeSibling->Color;
                    nodeSibling->Color = auxColor;

                    // Rotate parent in node's direction
                    _RightRotate(Tree, currentNode->Parent);

                    // Remove double black
                    currentNode->Color = BLACK;
                }
                else if (BLACK == nodeSibling->LeftChild->Color)
                {
                    // Case 3
                    if (BLACK == nodeSibling->RightChild->Color)
                    {
                        // Remove double black
                        currentNode->Color = BLACK;

                        // Add black to node's parent
                        if (BLACK == currentNode->Parent->Color)
                        {
                            currentNode->Parent->Color = DBLACK;
                        }
                        else if (RED == currentNode->Parent->Color)
                        {
                            currentNode->Parent->Color = BLACK;
                        }
                        else
                        {
                            GENERIC_LOG("Invalid color\n");
                            return -1;
                        }

                        // Make sibling red
                        nodeSibling->Color = RED;

                        // Move on to the next possible double black node
                        currentNode = currentNode->Parent;
                    }
                    // Case 5
                    else if (RED == nodeSibling->RightChild->Color)
                    {
                        // Swap color of sibling with color of sibling's nearest child to the node
                        COLOR auxColor = nodeSibling->Color;
                        nodeSibling->Color = nodeSibling->RightChild->Color;
                        nodeSibling->RightChild->Color = auxColor;

                        // Rotate sibling in opposite direction to node
                        _LeftRotate(Tree, nodeSibling);

                        // Apply case 6, so don't do anything, it will be applied when the loop runs again
                    }
                    else
                    {
                        GENERIC_LOG("Invalid color\n");
                        return -1;
                    }
                }
                else
                {
                    GENERIC_LOG("Invalid color\n");
                    return -1;
                }
            }
            else
            {
                GENERIC_LOG("Invalid direction\n");
                return -1;
            }
        }
        else
        {
            GENERIC_LOG("Invalid color\n");
            return -1;
        }

    } while (DBLACK == currentNode->Color);


cleanup:
    if (W == initDirection)
    {
        tempSentinel->Parent->LeftChild = Tree->Sentinel;
    }
    else if (E == initDirection)
    {
        tempSentinel->Parent->RightChild = Tree->Sentinel;
    }
    else
    {
        GENERIC_LOG("Invalid direction\n");
        return -1;
    }

    free(tempSentinel);

    return 0;
}

// Recursive O(n)
static int _GetBlackHeight(TREE_NODE* Root, int *IsRedBlack)
{
    int leftBlackHeight = 0;
    int rightBlackHeight = 0;

    if (NULL == Root)
    {
        return 0;
    }
    if (NULL != Root->LeftChild)
    {
        if (RED == Root->Color && BLACK != Root->LeftChild->Color)
        {
            (*IsRedBlack) *= 0;
            return 0;
        }
        leftBlackHeight = _GetBlackHeight(Root->LeftChild, IsRedBlack);
    }
    if (NULL != Root->RightChild)
    {
        if (RED == Root->Color && BLACK != Root->RightChild->Color)
        {
            (*IsRedBlack) *= 0;
            return 0;
        }
        rightBlackHeight = _GetBlackHeight(Root->RightChild, IsRedBlack);
    }

    if (leftBlackHeight != rightBlackHeight)
    {
        (*IsRedBlack) *= 0;
        return 0;
    }
    else
    {
        return leftBlackHeight + (Root->Color == BLACK);
    }
}

// Iterative O(logn)
static int _FindNode(CC_TREE* Tree, int Value, TREE_NODE** Node)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_3);
        return -1;
    }

    if (NULL == Tree->Root)
    {
        (*Node) = NULL;
        return 0;
    }

    TREE_NODE* currentNode = Tree->Root;

    do
    {
        if (Tree->Sentinel == currentNode)
        {
            (*Node) = NULL;
            return 0;
        }
        else if (Value == currentNode->Value)
        {
            (*Node) = currentNode;
            return 1;
        }
        else if (Value < currentNode->Value)
        {
            currentNode = currentNode->LeftChild;
        }
        else if (Value > currentNode->Value)
        {
            currentNode = currentNode->RightChild;
        }
    } while (1);
}

// Recursive O(logn) - max 2 recursions
// Calls: _RemoveFix
static int _TreeRemove(CC_TREE* Tree, TREE_NODE* Node)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Node)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    // If Node has no children
    if (Tree->Sentinel == Node->LeftChild && Tree->Sentinel == Node->RightChild)
    {
        // If Node is root
        if (NULL == Node->Parent)
        {
            free(Node);
            Tree->Root = NULL;
            Tree->Count = 0;
            return 0;
        }
        // If node is not root
        else
        {
            _RemoveFix(Tree, Node);
            
            Tree->Count = Tree->Count - 1;
            
            return 0;
        }
    }
    // If Node has a left child and not a right child
    else if (Tree->Sentinel != Node->LeftChild && Tree->Sentinel == Node->RightChild)
    {
        // Swap the value of the Node with the value of its left child
        if (0 != _SwapValues(&(Node->Value), &(Node->LeftChild->Value)))
        {
            return -1;
        }

        // Since a chain of 3 is not possible in red-black trees, 
        // we are sure that the Node's left child is the last node
        // on this path, after which comes the sentinel leaf.
        // The recursion will only happen once.
        return _TreeRemove(Tree, Node->LeftChild);
    }
    // If Node has no left child but has a right child
    else if (Tree->Sentinel == Node->LeftChild && Tree->Sentinel != Node->RightChild)
    {
        // Swap the value of the Node with the value of its right child
        if (0 != _SwapValues(&(Node->Value), &(Node->RightChild->Value)))
        {
            return -1;
        }

        // Since a chain of 3 is not possible in red-black trees, 
        // we are sure that the Node's right child is the last node
        // on this path, after which comes the sentinel leaf.
        // The recursion will only happen once.
        return _TreeRemove(Tree, Node->RightChild);
    }
    // If the node has both children
    else
    {
        // Find the inorder predecessor of the Node
        TREE_NODE* inorderPredecessor = Node->LeftChild;
        while (Tree->Sentinel != inorderPredecessor->RightChild)
        {
            inorderPredecessor = inorderPredecessor->RightChild;
        }

        // Swap the value of the Node with the value of its inorder predecessor 
        if (0 != _SwapValues(&(Node->Value), &(inorderPredecessor->Value)))
        {
            return -1;
        }
        
        // We are sure that the inorder predecessor of the Node has
        // no right child. At most, it can have a left child.
        // We are sure that the recursion happens at most 2 times.
        return _TreeRemove(Tree, inorderPredecessor);
    }
}

// Iterative O(1)
static int _SwapValues(int* a, int* b)
{
    if (NULL == a)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }
    if (NULL == b)
    {
        GENERIC_LOG(INVALID_PARAM_2);
        return -1;
    }

    int aux = *a;
    *a = *b;
    *b = aux;

    return 0;
}

// Recursive O(n)
void _TreePrintPreorder(CC_TREE* Tree, TREE_NODE* Root)
{
    static int nodesPrinted = 0;

    if (Root == NULL)
    {
        printf("The tree is empty\n");
    }
    else if (Root != Tree->Sentinel)
    {
        char color = 'B';
        if (Root->Color == RED)
        {
            color = 'R';
        }
        printf("%d%c ", Root->Value, color);
        nodesPrinted++;
        if (nodesPrinted == Tree->Count)
        {
            printf("\n");
            nodesPrinted = 0;
        }
        _TreePrintPreorder(Tree, Root->LeftChild);
        _TreePrintPreorder(Tree, Root->RightChild);
    }
}

// Recursive O(n)
// Calls: _GetBlackHeight
int _IsRedBlack(CC_TREE* Tree)
{
    if (NULL == Tree)
    {
        GENERIC_LOG(INVALID_PARAM_1);
        return -1;
    }

    if (NULL == Tree->Root)
    {
        GENERIC_LOG("Cannot perform this operation on an empty tree\n");
        return -1;
    }

    if (BLACK != Tree->Root->Color)
    {
        return 0;
    }

    int isRedBlack = 1;

    _GetBlackHeight(Tree->Root, &isRedBlack);

    return isRedBlack;
}