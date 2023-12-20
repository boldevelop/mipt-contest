/* алгоритм Фишера-Йетса тасование массива */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

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

void dump_arr(int *arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void swap(int *arr, const int i, const int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void fy_shuffle(int *arr, const int s)
{
    int i = s;
    int j;
    while (i-- > 1) {
        j = rand() % (i + 1);
        swap(arr, j, i);
    }
}

int main()
{
    int s;
    int *arr;
    srand(time(NULL));

    s = read_int();
    if (s == 0) {
        printf("0\n");
        return 0;
    }
    if (s == 1) {
        printf("%d\n", read_int());
        return 0;
    }
    arr = alloc_arr(s);

    for (int i = 0; i < s; ++i) {
        arr[i] = read_int();
    }

    fy_shuffle(arr, s);
    dump_arr(arr, s);
}
