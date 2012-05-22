/****************************************************************************
* heap.c
*
* XXX: This relies HEAVILY on there being a (reasonable) max number of games
*
***************************************************************************/

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

#include "heap.h"

// |map| index i to the position of the ith item in |buffer|

static 
struct elt
{
    int map_idx;
    int val;
};

struct heap
{
    struct elt *buffer;
    int *map
    int size;
};

// XXX error checking
struct heap *
create_heap(int n)
{
    struct heap *h = malloc(sizeof(struct heap));
    h->buffer = malloc((sizeof struct elt) * n);
    h->map = malloc((sizeof int) * n);
    for (int i = 0; i < n; i++)
        h->map[i] = -1;
    h->size = 0;
    return h;
}

static inline int
child_idx(int n)
{
    return 2 * n + 1;
}

static inline int
parent_idx(int n)
{
    return (n-1)/2;
}


static void
prop_down(struct heap *heap, int n)
{
    while(child_idx(n) < heap->size)
    {
        int child_pos = child_idx(n);
        if (child_pos + 1 != heap->size
            && heap->buffer[child_pos] < heap->buffer[child_pos + 1])
            child_pos += 1;
        if (heap->buffer[child_pos] <= heap->buffer[n])
            break;
        uint64_t tmp = heap->buffer[n];
        heap->buffer[n] = heap->buffer[child_pos];
        heap->buffer[child_pos] = tmp;
        n = child_pos;
    }
}

struct elt
delete_min(struct heap *heap)
{
    struct elt ret = heap->buffer[0];

    if (--heap->size > 0)
    {
        heap->buffer[0] = heap->buffer[heap->size];
        prop_down(heap);
    }
    return ret;
}

// returns the index of the inserted ele
int
insert_heap(struct heap *heap, int val, int game_idx)
{
    int n;
    if (heap->map[game_idx] == -1)
    {
        n = heap->size++;
        heap->map[game_idx] = n;
    }
    else
        n = heap->map[game_idx];

    heap->buffer[n] = { game_idx, val };
    while (n > 0)
    {
        int parent = parent_idx(n);
        if (heap->buffer[n].val >= heap->buffer[parent].val)
            break;
        struct elt tmp = heap->buffer[parent];
        heap->map[tmp.idx] = n;
        heap->map[heap->buffer[n].idx] = parent;
        heap->buffer[parent] = heap->buffer[n];
        heap->buffer[n] = tmp;
        n = parent;
    }
    prop_down(heap, n);
}
