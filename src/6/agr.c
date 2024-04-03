#include <stdio.h>

void swp(int *x, int i, int j)
{
    int tmp = *(x + i);
    *(x + i) = *(x + j);
    *(x + j) = tmp;
}

int foo(int *x, int st, int last)
{
    int j = last, first = *x;

    for (int i = last; i > st; --i) {
        if (x[i] > first) {
            swp(x, i, j);
            j--;
        }
    }

    swp(x, st, j);
    return j;
}

void d_arr(int *x, int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", x[i]);
    }
    printf("\n");
}

void test(int *x, int s, int l)
{
    int res = foo(x, s, l);
    printf("res: %d\n", res);
    d_arr(x, l + 1);
}

int main()
{
    {
        int arr[] = { 9, 2, 5, 3, 8, 7, 8, 7, 5, 8 };
        test(arr, 0, 9);
    }
    {
        int arr[] = { 9, 7, 10, 8 };
        test(arr, 0, 3);
    }
    {
        int arr[] = { 9, 10, 7, 8 };
        test(arr, 0, 3);
    }
    {
        int arr[] = { 13, 15, 19 };
        test(arr, 0, 2);
    }
    {
        int arr[] = { 7, 8, 13 };
        test(arr, 2, 2);
    }
    {
        int arr[] = { 7, 8, 13 };
        test(arr, 0, 0);
    }
}
