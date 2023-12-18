/* generalized selection step */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int (*cmp_t)(void const *lhs, void const *rhs);
typedef unsigned char uchar;
void cswap(void *lhs, void *rhs, size_t size)
{
    void *tmp = malloc(size);
    memcpy(tmp, rhs, size);
    memcpy(rhs, lhs, size);
    memcpy(lhs, tmp, size);
    free(tmp);
}

int selstep(void *parr, int eltsize, int numelts, int nsorted, cmp_t cmp)
{
    void *min = NULL;

    if (nsorted >= numelts) {
        return 1;
    }

    min = (uchar *) parr + nsorted * eltsize;
    for (int pos = nsorted; pos < numelts; ++pos) {
        void *cur = (uchar *) parr + pos * eltsize;
        if (cmp(cur, min)) {
            min = cur;
        }
    }
    cswap(min, (uchar *) parr + nsorted * eltsize, eltsize);
    return 0;
}

void dump_arr(int *arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


int cmpr(void const *lhs, void const *rhs)
{
    int const *ilhs = lhs;
    int const *irhs = rhs;
    if ((*ilhs) < (*irhs)) {
        return 1;
    }
    return 0;
    printf("%d ", *ilhs);
    return 0;
}

int main()
{
    int arr[6] = { 1, 2, 3, 6, 5, 4 };
    selstep(arr, sizeof(int), 6, 3, cmpr);
    dump_arr(arr, 6);
    printf("\n");
}
