#include <stdio.h>
#include <stdlib.h>

unsigned rfib(unsigned x)
{
    if (x == 0)
        return 0;
    if (x == 1)
        return 1;
    return rfib(x - 1) + rfib(x - 2);
}

unsigned ifib(unsigned x)
{
    unsigned t, f = 0, s = 1;
    if (x == 0)
        return 0;
    if (x == 1)
        return 1;
    while (x > 1) {
        t = s;
        s = f + s;
        f = t;
        x--;
    }
    return s;
}

unsigned ifibm(unsigned x, unsigned m)
{
    unsigned t, f = 0, s = 1;
    if (x == 0)
        return 0;
    if (x == 1)
        return 1;
    while (x > 1) {
        t = s;
        s = (f + s) % m;
        f = t;
        x--;
    }
    return s;
}


void fm(unsigned x, unsigned m)
{
#ifdef LOCAL_TEST
    for (int i = 0; i < 11; ++i) {
        printf("r: %d;\ni: %d;\n\n", rfib(i), ifib(i));
    }
#else
    printf("%d\n", ifibm(x, m));
#endif
}

int main()
{
    int res;
    unsigned x, m;
    res = scanf("%d%d", &x, &m);
    if (res != 2)
        abort();
    fm(x, m);
    return 0;
}
