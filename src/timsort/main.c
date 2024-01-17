#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "timsort.h"

int *alloc_arri(int n)
{
    int *buf;
    buf = calloc(n, sizeof(int));
    if (!buf)
        abort();
    return buf;
}

int **alloc_arrpi(int rn)
{
    int **buf;
    buf = calloc(rn, sizeof(int *));
    if (!buf)
        abort();
    return buf;
}

int read_int() {
    int buf;
    if (scanf("%d", &buf) != 1) {
        abort();
    }
    return buf;
}

static int X = 0;

int less_jarr(const void *a, const void *b)
{
    const int *x = a;
    const int *y = b;
    return x[X] < y[X];
}

int main()
{
    int N, K;
    int** j_arr;
    /* количество записей (N), количество полей (K), номер поля для сортировки (X) */
    N = read_int();
    K = read_int();
    X = read_int();
    j_arr = alloc_arrpi(N);
    for (int i = 0; i < N; ++i) {
        j_arr[i] = alloc_arri(K);
        for (int j = 0; j < K; ++j) {
            j_arr[i][j] = read_int();
        }
    }
    timsort((uintptr_t *)j_arr, N, less_jarr);

    /*  для записи под номером n (номера с нуля) записывать её поле с номером (n % N) */
    for (int i = 0; i < N; ++i) {
        printf("%d ", j_arr[i][i % K]);
    }
    printf("\n");

    for (int i = 0; i < N; ++i)
        free(j_arr[i]);
    free(j_arr);
}
