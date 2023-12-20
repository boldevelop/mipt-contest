/*  сортировка подсчётом
    Печатает на стандартный вывод массив бакетов сортировки подсчётом
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int *alloc_arr(const int s);
int read_int();
void dump_arr(const int *arr, const int s);

int *scp(const int *arr, int s, int *bs)
{
    int *b;

    if (s == 0) {
        return NULL;
    }

    *bs = INT_MIN;
    for (int i = 0; i < s; ++i)
        if (arr[i] > *bs)
            *bs = arr[i];

    (*bs)++;

    b = alloc_arr(*bs);
    for (int i = 0; i < s; ++i)
        b[arr[i]]++;

    return b;
}

int main()
{
    int s, bs;
    int *arr;
    int *b = NULL;

    s = read_int();
    arr = alloc_arr(s);
    for (int i = 0; i < s; ++i) {
        arr[i] = read_int();
    }

    b = scp(arr, s, &bs);

    if (b != NULL)
        dump_arr(b, bs);

    free(arr);
    free(b);
}

int *alloc_arr(const int s)
{
    int *buf;
    buf = calloc(sizeof(int), s);
    if (buf == NULL) {
        abort();
    }
    return buf;
}

int read_int()
{
    int n;
    if (scanf("%d", &n) != 1) {
        abort();
    }
    return n;
}

void dump_arr(const int *arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
