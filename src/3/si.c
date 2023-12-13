/* сортировка вставками */

#include <stdio.h>

int moveright(int *arr, int key, int last)
{
    if (last == 0) {
        return 0;
    }

    while (last-- != 0) {
        if (arr[last] > key) {
            arr[last + 1] = arr[last];
        } else {
            break;
        }
    }

    return last + 1;
}

#ifdef DEBUG
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
        int arr[6] = { 1, 2, 4, 5, 3, 2 };
        int pos = moveright(arr, 3, 4);
        printf("pos: %d\n", pos);
        dump_arr(arr, 6);
    }
    {
        int arr[6] = { 5, 2, 4, 1, 3, 2 };
        int pos = moveright(arr, 5, 0);
        printf("pos: %d\n", pos);
        dump_arr(arr, 6);
    }
    {
        int arr[6] = { 4, 1, 5, 6, 7, 8 };
        int pos = moveright(arr, 1, 1);
        printf("pos: %d\n", pos);
        dump_arr(arr, 6);
    }
    {
        int arr[10] = { 1, 1, 3, 3, 4, 1, 9, 4, 9, 4 };
        int pos = moveright(arr, 1, 5);
        printf("pos: %d\n", pos);
        dump_arr(arr, 10);
    }
}
#endif
