/* система факториалов */
#include <stdio.h>
#include <stdlib.h>

unsigned f[13] = {
    1,
    1,
    2,
    6,
    24,
    120,
    720,
    5040,
    40320,
    362880,
    3628800,
    39916800,
    479001600,
};

/* 463=3*5! + 4 * 4! + 1 * 3! + 0 * 2! + 1 * 1! = 34101 */
void fs(unsigned x)
{
    unsigned i = 12;
    while (i > 1) {
        if (f[i] <= x) {
            break;
        }
        i--;
    }

    while (i > 1) {
        printf("%d.", x / f[i]);
        x = x % f[i];
        i--;
    }
    printf("%d.\n", x);
}

int main()
{
    unsigned x;
    int res = scanf("%d", &x);
    if (res < 1) {
        abort();
    }
    fs(x);
}
