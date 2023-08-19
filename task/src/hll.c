#include "hll.h"
#include <stddef.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

// returns the first b bits of x
size_t first_b_bits(size_t x, size_t b)
{
    return x >> (sizeof(size_t) * 8 - b);
}

int find_msb(size_t x)
{
    int c = 0;
    while (x >>= 1) {
        c++;
    }
    return c;
}

hll hll_new(size_t size)
{
    size_t *arr = malloc(size * sizeof(size_t));
    hll sketch = {
        arr,
        size,
        ceil(log2(size)),
    };
    return sketch;
}

void hll_destroy(hll *sketch)
{
    free(sketch->arr);
}

void hll_add(hll *sketch, long long id)
{
    size_t x = hll_hash(sketch->size, id);

    // get 1 + first b bits of x. this will be the index of the counter to
    // modify
    size_t i = 1 + first_b_bits(x, sketch->b);

    // get the position of the leftmost 1, after removing the first b bits
    //
    size_t mask = (1UL << (sizeof(size_t) * 8 - sketch->b));
    size_t rest = mask & x;
    int msb = find_msb(rest);

    // update sketch
    sketch->arr[i] = max(sketch->arr[i], msb);
}

size_t hll_hash(size_t size, long long id)
{
    return id % size;
}
