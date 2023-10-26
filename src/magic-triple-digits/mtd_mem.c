#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "mtd_mem.h"

mtd_arr mtd_alloc(int capacity)
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

void mtd_push(mtd_arr * arr, int v)
{
    assert(arr->s != arr->c);
    arr->buf[arr->s++] = v;
}

void mtd_free(mtd_arr arr)
{
    free(arr.buf);
}

#ifdef RESEARCH
mtd_arr_ptr mtd_alloc_ptr(int capacity)
{
    mtd_arr_ptr res = { 0 };
    int **buf = malloc(capacity * sizeof(int *));

    if (buf == NULL) {
        abort();
    }

    res.buf = buf;
    res.c = capacity;
    return res;
}

void mtd_push_ptr(mtd_arr_ptr * arr, mtd_arr val)
{
    int bytes;
    int *buf;
    assert(arr->s != arr->c);
    bytes = val.s * sizeof(int);
    buf = malloc(bytes);
    if (buf == NULL) {
        abort();
    }

    memcpy(buf, val.buf, bytes);
    arr->buf[arr->s++] = buf;
}

void mtd_clear_ptr(mtd_arr_ptr * arr)
{
    for (int i = 0; i < arr->s; ++i) {
        free(arr->buf[i]);
    }
    arr->s = 0;
}

void mtd_free_ptr(mtd_arr_ptr arr)
{
    for (int i = 0; i < arr.s; ++i) {
        free(arr.buf[i]);
    }
    free(arr.buf);
}
#endif
