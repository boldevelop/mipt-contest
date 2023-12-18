/* слияние для сортировки слиянием */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *alloc_arr(const int s)
{
    int *buf;
    buf = calloc(sizeof(int), s);
    if (buf == NULL) {
        abort();
    }
    return buf;
}

void merge(int *arr, int l, int m, int r)
{
    int size = r - l + 1, l1 = l, l2 = m + 1, out = 0;
    int *buf;
    if (size < 2) {
        return;
    }
    if (size == 2) {
        if (arr[l1] > arr[l2]) {
            int tmp = arr[l1];
            arr[l1] = arr[l2];
            arr[l2] = tmp;
        }
        return;
    }

    buf = alloc_arr(size);
    while (l1 < m + 1 || l2 < r + 1) {
        if (l1 == m + 1) {
            buf[out++] = arr[l2++];
            continue;
        }
        if (l2 == r + 1) {
            buf[out++] = arr[l1++];
            continue;
        }
        if (arr[l1] < arr[l2]) {
            buf[out++] = arr[l1++];
        } else {
            buf[out++] = arr[l2++];
        }
    }
    memcpy(arr + l, buf, sizeof(int) * size);
    free(buf);
}

void dump_arr(int *arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main()
{
    {
        int arr[6] = { 1, 2, 3, 4, 5, 6 };
        merge(arr, 0, 2, 5);
        dump_arr(arr, 6);
    }
    {
        int arr[6] = { 4, 5, 6, 1, 2, 3 };
        merge(arr, 0, 2, 5);
        dump_arr(arr, 6);
    }
    {
        int arr[1] = { 4 };
        merge(arr, 0, 0, 0);
        dump_arr(arr, 1);
    }
    {
        int arr[2] = { 1, 3 };
        merge(arr, 0, 0, 1);
        dump_arr(arr, 2);
    }
    {
        int arr[2] = { 3, 1 };
        merge(arr, 0, 0, 1);
        dump_arr(arr, 2);
    }
}
