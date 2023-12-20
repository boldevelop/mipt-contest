/* поиск большинства Boyer–Moore majority vote algorithm */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int majority_element(const int *parr, int len)
{
    int num = -1, count = 0;
    for (int i = 0; i < len; ++i) {
        if (count == 0) {
            num = parr[i];
            count = 1;
            continue;
        }
        if (num == parr[i]) {
            count++;
        } else {
            count--;
        }
    }

    if (count == 0) {
        return -1;
    }

    count = 0;
    for (int i = 0; i < len; ++i)
        if (num == parr[i])
            count++;

    return count > len / 2 ? num : -1;
}

#ifdef DEBUG

void dump_arr(int *arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d", arr[i]);
        if (i != s - 1) {
            printf(", ");
        }
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

void test(int *parr, int len, int exp)
{
    int i = 0;
    while (i < 20) {
        int res = majority_element(parr, len);
        if (res != exp) {
            printf("Exp: %d, res: %d\n", exp, res);
            dump_arr(parr, len);
        }
        i++;
    }
}

int main()
{
    srand(time(NULL));
    {
        int arr[5] = { 2, 3, 1, 2, 2 };
        test(arr, 5, 2);
    }
    {
        int arr[6] = { 9, 1, 1, 9, 2, 2 };
        test(arr, 6, -1);
    }
    {
        int arr[9] = { 3, 4, 3, 2, 4, 4, 2, 4, 4 };
        test(arr, 9, 4);
    }
    {
        int arr[8] = { 3, 3, 4, 2, 4, 4, 2, 4 };
        test(arr, 8, -1);
    }
    {
        int arr[4] = { 1, 1, 2, 2 };
        test(arr, 4, -1);
    }
    {
        int arr[10] = { 1, 1, 1, 1, 2, 2, 3, 3, 4, 4 };
        test(arr, 10, -1);
    }
    {
        int arr[10] = { 1, 1, 1, 1, 2, 2, 4, 4, 4, 4 };
        test(arr, 10, -1);
    }
    {
        int arr[10] = { 4, 4, 1, 1, 1, 1, 4, 4, 4, 4 };
        test(arr, 10, 4);
    }
    {
        int arr[10] = { 4, 4, 1, 1, 1, 1, 4, 4, 1, 1 };
        test(arr, 10, 1);
    }
    {
        int arr[2] = { 1, 2 };
        test(arr, 2, -1);
    }
    {
        int arr[2] = { 1, 1 };
        test(arr, 2, 1);
    }
    {
        int arr[2] = { 1, 1 };
        test(arr, 0, -1);
    }
}

#endif
