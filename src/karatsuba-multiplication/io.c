#include "io.h"

#include <stdio.h>
#include <stdlib.h>

void dump_arr(ull * arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%lld ", arr[i]);
    }
    printf("\n");
}

int read_int()
{
    int n;
    if (scanf("%d", &n) != 1) {
        abort();
    }
    return n;
}
