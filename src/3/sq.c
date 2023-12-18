/* partition for quick sort */

#include <stdio.h>
#include <stdlib.h>

void swap(int *arr, const int i, const int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

unsigned partition(int *arr, unsigned low, unsigned high)
{
    int pivot_v = arr[low];
    unsigned pivot = low;
    unsigned res;
    if (low - high == 0) {
        return low;
    }

    low++;
    while (low != high) {
        if (arr[high] <= pivot_v) {
            if (arr[low] <= pivot_v) {
                low++;
            } else {
                swap(arr, low, high);
                high--;
            }
        } else {
            high--;
        }
    }

    res = arr[low] <= pivot_v ? low : low - 1;
    swap(arr, res, pivot);
    return res;
}

void dump_arr(int *arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", arr[i]);
    }
}

int main()
{
    {
        int arr[3] = { 2, 1, 3 };
        int res = partition(arr, 0, 2);
        dump_arr(arr, 3);
        printf("Res: %d\n", res);
    }
    {
        int arr[3] = { 1, 1, 1 };
        int res = partition(arr, 0, 2);
        dump_arr(arr, 3);
        printf("Res: %d\n", res);
    }
    {
        int arr[4] = { 7, 8, 9, 10 };
        int res = partition(arr, 0, 3);
        dump_arr(arr, 4);
        printf("Res: %d\n", res);
    }
    {
        int arr[4] = { 7, 1, 2, 3 };
        int res = partition(arr, 0, 3);
        dump_arr(arr, 4);
        printf("Res: %d\n", res);
    }
    {
        int arr[6] = { 7, 4, 10, 2, 8, 5 };
        int res = partition(arr, 0, 5);
        dump_arr(arr, 6);
        printf("Res: %d\n", res);
    }
    {
        int arr[7] = { 7, 4, 10, 2, 8, 5, 13 };
        int res = partition(arr, 0, 6);
        dump_arr(arr, 7);
        printf("Res: %d\n", res);
    }
    {
        int arr[10] = { 5, 6, 8, 4, 5, 6, 3, 8, 1, 2 };
        int res = partition(arr, 0, 9);
        dump_arr(arr, 10);
        printf("Res: %d\n", res);
    }
    {
        int arr[10] = { 9, 2, 5, 3, 8, 7, 8, 7, 5, 8 };
        int res = partition(arr, 0, 9);
        dump_arr(arr, 10);
        printf("Res: %d\n", res);
    }
}
