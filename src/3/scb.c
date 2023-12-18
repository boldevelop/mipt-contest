/* обобщённый бинарный поиск */

#include <stdio.h>
#include <stdlib.h>

/* отрицательное число, если lhs логически меньше, чем rhs,
   0 если они равны
   и положительное число, если lhs логически больше, чем rhs */
typedef int (*cmp_t)(const void *lhs, const void *rhs);
typedef unsigned char uchar;

void *cbsearch(const void *key, const void *base, int num, int size,
               cmp_t cmp)
{
    int l = 0;
    int r = num;

    while (l < r) {
        int half = (r - l) / 2 + l;
        const void *cur = (uchar *) base + size * half;
        int res = cmp(key, cur);
        if (res == 0) {
            return (void *) cur;
        }
        if (res < 0) {
            r = half;
            continue;
        }
        l = half + 1;
    }

    return NULL;
}

int cmprt(const void *lhs, const void *rhs)
{
    const int *ilhs = lhs;
    const int *irhs = rhs;
    return *ilhs - *irhs;
}

int bin_search(int *arr, int length, int key)
{
    int l = 0;
    int r = length;
    while (l < r) {
        int half = (r - l) / 2 + l;
        int res = cmprt(&key, &arr[half]);
        if (res == 0) {
            return half;
        }
        if (res < 0) {
            r = half;
            continue;
        }
        l = half + 1;
    }

    return -1;
}

void test_normal(int *arr, int length, int key, int expected)
{
    int actual = bin_search(arr, length, key);
    if (actual == expected) {
        return;
    } else {
        printf("Try: %d. Found: %d. Expected: %d", key, actual, expected);
    }
    printf("\n");
}

void dump_arr(int *arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void cases(int *arr, const int size)
{
    dump_arr(arr, size);
    for (int i = 0; i < size; ++i) {
        test_normal(arr, size, arr[i], i);
        test_normal(arr, size, arr[i] - 1, -1);
        test_normal(arr, size, arr[i] + 1, -1);
    }
}

void ctest_normal(int *arr, int length, int key, int *expected)
{
    int *actual = cbsearch(&key, arr, length, sizeof(int), cmprt);
    if (actual == expected) {
        return;
    } else {
        int found = actual == NULL ? -1 : *actual;
        int exp_val = expected == NULL ? -1 : *expected;
        printf("Try: %d. Found: %d. Expected: %d", key, found, exp_val);
    }
    printf("\n");
}

void ccases(int *arr, const int size)
{
    dump_arr(arr, size);
    for (int i = 0; i < size; ++i) {
        ctest_normal(arr, size, arr[i], arr + i);
        ctest_normal(arr, size, arr[i] - 1, NULL);
        ctest_normal(arr, size, arr[i] + 1, NULL);
    }
}

int main()
{
    int arr1[1] = { 1 };
    int arr2[2] = { 1, 3 };
    int arr3[3] = { 1, 3, 5 };
    int arr7[7] = { 1, 3, 5, 7, 9, 11, 13 };
    int arr8[8] = { 1, 3, 5, 7, 9, 11, 13, 15 };

    cases(arr1, 1);
    cases(arr2, 2);
    cases(arr3, 3);
    cases(arr7, 7);
    cases(arr8, 8);

    ccases(arr1, 1);
    ccases(arr2, 2);
    ccases(arr3, 3);
    ccases(arr7, 7);
    ccases(arr8, 8);
}
