/* найти позицию старшего и младшего установленных битов в числе. */
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>

void bprintf(int v)
{
    unsigned int mask = 1 << (sizeof(int) * CHAR_BIT - 1);
    while (mask) {
        printf("%d", (v & mask ? 1 : 0));
        mask >>= 1;
    }
    printf("\n");
}

int main()
{
    int n;
    int mask = 1;
    int f_b = -1, l_b = -1, l = 0;

    if (scanf("%d", &n) != 1) {
        abort();
    }

    while (n) {
        if (n & mask) {
            if (f_b == -1)
                f_b = l;
            l_b = l;
        }
        n >>= 1;
        l++;
    }

    if (f_b == -1) {
        printf("-1\n");
    } else {
        printf("%d %d\n", l_b, f_b);
    }

    return 0;
}
