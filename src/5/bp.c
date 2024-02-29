/* Задача про рюкзак */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INV -1

int readint()
{
    int b;
    if (scanf("%d", &b) != 1) {
        abort();
    }
    return b;
}

void *my_calloc(size_t n, size_t size)
{
    void *b = calloc(n, size);
    if (!b) {
        abort();
    }
    return b;
}

void readarr(int *arr, int s)
{
    for (int i = 1; i < s; ++i) {
        arr[i] = readint();
    }
}

void dumparr(int *arr, int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%2d ", arr[i]);
    }
    printf("\n");
}

int calc_items(int w, int is, int row_size, int *items, int *d)
{
    int left, right, cur_i;
    if (w < 0) {
        return INV;
    }
    cur_i = is * row_size + w;
    if (d[cur_i] != INV) {
        return d[cur_i];
    }
    left = calc_items(w - items[is], is - 1, row_size, items, d);
    left = left == INV ? 0 : left + 1;
    right = calc_items(w, is - 1, row_size, items, d);

    d[cur_i] = right > left ? right : left;
    return d[cur_i];
}

int *init_d(int s, int w, int is)
{
    int *d = my_calloc(s, sizeof(int));
    for (int i = 1; i < is; ++i) {
        for (int j = 1; j < w; ++j) {
            d[i * w + j] = INV;
        }
    }
    return d;
}

void dump_d(int *d, int w, int is)
{
    if (w < 20) {
        for (int i = 0; i < is; ++i) {
            dumparr(d + i * w, w);
        }
    }
}

int main()
{
    int w, is, max_items, ds;
    int *items, *d;
    w = readint();
    is = readint();
    is++;
    w++;
    ds = w * is;
    items = my_calloc(is, sizeof(int));

    readarr(items, is);
    d = init_d(ds, w, is);

#ifdef DEBUG
    dump_d(d, w, is);
    printf("%d\n", w - 1);
    dumparr(items, is);
#endif

    max_items = calc_items(w - 1, is - 1, w, items, d);
    printf("%d\n", max_items);

#ifdef DEBUG
    dump_d(d, w, is);
#endif

    free(items);
    free(d);
}
