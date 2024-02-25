/* Задача про рюкзак */

#include <stdio.h>
#include <stdlib.h>

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
    for (int i = 0; i < s; ++i) {
        arr[i] = readint();
    }
}

void dumparr(int *arr, int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main()
{
    int w, is;
    int *items;
    w = readint();
    is = readint();
    items = my_calloc(is, sizeof(int));
    readarr(items, is);

#ifdef DEBUG
    printf("%d\n", w);
    dumparr(items, is);
#endif

    free(items);
}
