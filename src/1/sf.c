#include <stdio.h>
#include <stdlib.h>

#define N 47
unsigned f[N];

unsigned init_fib(unsigned x)
{
    unsigned t, i = 2, first = 0, s = 1;
    f[0] = 0;
    f[1] = 1;
    while (s < x) {
        t = s;
        s = (first + s);
        first = t;
        f[i++] = s;
    }
    return i - 1;
}

void sf(unsigned x)
{
    unsigned max = init_fib(x);

    if (max - 1 <= 1) {
        printf("%d\n", x);
        return;
    }

    while (max - 1 > 1) {
        printf("%d", x / f[max - 1]);
        x = x % f[max - 1];
        max--;
    }
    printf("\n");
}

int main()
{
    int res;
    unsigned x;
    res = scanf("%d", &x);
    if (res != 1)
        abort();
    sf(x);
    return 0;
}
