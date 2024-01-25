#include <stdio.h>
#include <stdlib.h>

#include "timsort.h"

int *alloc_arri(int n)
{
    int *buf;
    buf = calloc(n, sizeof(int));
    if (!buf)
        abort();
    return buf;
}

int read_int()
{
    int buf;
    if (scanf("%d", &buf) != 1) {
        abort();
    }
    return buf;
}

static int X = 0;

int cmp(const void *a, const void *b)
{
    const int *x = a;
    const int *y = b;
    return x[X] - y[X];
}

int main()
{
    int N, K;
    int *arr;
    /* количество записей (N), количество полей (K), номер поля для сортировки (X) */
    N = read_int();
    K = read_int();
    X = read_int();
    arr = alloc_arri(N * K);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < K; ++j) {
            arr[i * K + j] = read_int();
        }
    }

    timsort(arr, N, sizeof(int) * K, cmp);

    /*  для записи под номером n (номера с нуля) записывать её поле с номером (n % N) */
    for (int i = 0; i < N; ++i) {
        printf("%d ", arr[i * K + i % K]);
    }
    printf("\n");

    free(arr);
}
