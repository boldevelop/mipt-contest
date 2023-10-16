#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long ull;

ull ifibm(ull x, ull m)
{
    ull t, f = 0, s = 1;
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

ull pizm(ull m)
{
    ull t, f = 0, s = 1, p = 0;
    for (;;) {
        t = s;
        s = (f + s) % m;
        f = t;
        p++;
        if (s == 1 && f == 0) {
            return p;
        }
    }
}

void pp(ull x, ull m)
{
    ull p = pizm(m);
    x = x % p;
    printf("%lld %lld\n", ifibm(x, m), p);
}

int main()
{
    int res;
    ull x, m;
    res = scanf("%lld%lld", &x, &m);
    if (res != 2)
        abort();
    pp(x, m);
    return 0;
}
