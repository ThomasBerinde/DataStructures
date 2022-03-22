# DataStructures
C libraries implementing the following data structures: vector, stack, heap, hash table, red-black tree

# Components
The data_struct folder contains libraries for the following data structrues:
  - vector
  - stack
  - heap
  - hash table
  - red-black tree 
  - list (used by hash table to resolve collisions by chaining)

It also contains a header "common.h" which defines some macros used by the other libraries.

The test_lib folder contains a main function which implements some tests for the data structures implemented.

# Usage
The libraries can be copied and used as needed. They work on integer numbers, and the hash table works for tuples of type (char*, int), which represent the key and, respectively, the value of the key.
The project can be cloned and the test lib can be used to see how the structures perform when subjected to different tests (the ones already implemented, or the user can create their own tests).

# Future improvements
The main improvement that could be added is for the data structures to work on any type of data, not only integer values.
