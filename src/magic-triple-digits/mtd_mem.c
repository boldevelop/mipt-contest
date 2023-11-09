#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "mtd_mem.h"

mtd_arr mtd_alloc(const int capacity)
{
    mtd_arr res = { 0 };
    int *buf = malloc(capacity * sizeof(int));

    if (buf == NULL) {
        abort();
    }

    res.buf = buf;
    res.c = capacity;
    return res;
}

void mtd_push(mtd_arr * arr, const int v)
{
    assert(arr->s != arr->c);
    arr->buf[arr->s++] = v;
}

void mtd_free(mtd_arr arr)
{
    free(arr.buf);
}
