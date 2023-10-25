#include <stdlib.h>
#include <assert.h>

#define At(arr, i) arr[(i)]

struct mtd_arr {
    int* buf;
    int s;
    int c;
};

struct mtd_arr mtd_alloc(int capacity) {
    struct mtd_arr res = {0};
    int *buf = malloc(capacity * sizeof(int));

    if (buf == NULL) {
        abort();
    }

    res.buf = buf;
    res.c = capacity;
    return res;
}

void mtd_push(struct mtd_arr arr, int v) {
    assert((arr.s + 1) != arr.c);
    arr.buf[arr.s++] = v;
}

void mtd_free(struct mtd_arr arr) {
    free(arr.buf);
}
