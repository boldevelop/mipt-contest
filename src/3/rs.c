/* поразрядная сортировка */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int *alloc_arr(const int s);
int read_int();
void dump_arr(const int *arr, const int s);

int ipow10(int r)
{
    int res = 1;
    while (r-- > 0) {
        res *= 10;
    }
    return res;
}

void radix_sort(int *arr, const int s, int r)
{
    int *digits = alloc_arr(10);
    int *temp = alloc_arr(s);
    r = ipow10(r);

    for (int i = 0; i < s; ++i) {
        digits[(arr[i] / r) % 10] += 1;
    }

    for (int i = 1; i < 10; ++i) {
        digits[i] += digits[i - 1];
    }

    for (int i = s - 1; i >= 0; --i) {
        int d = (arr[i] / r) % 10;
        digits[d]--;
        temp[digits[d]] = arr[i];
    }

    memcpy(arr, temp, sizeof(int) * s);
    free(temp);
}

int main()
{
    int s, r;
    int *arr;

    s = read_int();
    arr = alloc_arr(s);
    for (int i = 0; i < s; ++i) {
        arr[i] = read_int();
    }
    r = read_int();

    radix_sort(arr, s, r);
    dump_arr(arr, s);

    free(arr);
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
