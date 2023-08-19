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

long double alpha_m(size_t m)
{
    switch (m) {
        case 2:
            return 0.35119395;
        case 4:
            return 0.53243461;
        case 8:
            return 0.62560871;
        case 16:
            return 0.67310202;
        case 32:
            return 0.69712263;
        case 64:
            return 0.70920845;
        default:
            return 0.7213 / (1 + (1.079 / m));
    }
}

hll hll_new(size_t size)
{
    size_t *arr = calloc(size, sizeof(size_t));
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

long double hll_count(hll *sketch)
{
    long double z = 0;
    for (unsigned int i = 0; i < sketch->size; i++)
    {
        z += powl(2, -sketch->arr[i]);
    }
    z = powl(z, -1);

    return alpha_m(sketch->size) * sketch->size * sketch->size * z;
}

size_t hll_hash(size_t size, long long id)
{
    return id % size;
}
