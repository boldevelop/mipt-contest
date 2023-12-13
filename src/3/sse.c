/* сортировка выбором */

#include <stdlib.h>
#include <stdio.h>

void dump_arr(int *arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
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

void swap(int *arr, const int i, const int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void selection_sort_step(int *arr, const int s, int pos)
{
    int min = arr[pos];
    int j = pos;
    for (int i = pos; i < s; ++i) {
        if (arr[i] < min) {
            min = arr[i];
            j = i;
        }
    }
    swap(arr, pos, j);
}

int main()
{
    int s, pos;
    int *arr;
    s = read_int();
    arr = alloc_arr(s);

    for (int i = 0; i < s; ++i) {
        arr[i] = read_int();
    }

    pos = read_int();
    selection_sort_step(arr, s, pos);
    dump_arr(arr, s);
}
