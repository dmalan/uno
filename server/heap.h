/****************************************************************************
* heap.h
*
* Computer Science 124
* Programming Assignment 3
*
* Declares heap functionality. (from prog1)
***************************************************************************/

#ifndef HEAP_HDR
#define HEAP_HDR

#include <inttypes.h>

struct heap;

int heap_size(struct heap *heap);
struct heap *create_heap(int n);
uint64_t delete_max(struct heap *heap);
void insert_heap(struct heap *heap, uint64_t elt);

#endif
