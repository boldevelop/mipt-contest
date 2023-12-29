/* найти наименьшее число, которое делится без остатка на числа от 2 до N */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ull unsigned long long

ull gcd(ull x, ull y)
{
    ull q;
    if (y > x) {
        ull t = y;
        y = x;
        x = t;
    }

    do {
        assert(y > 0);
        q = x % y;
        x = y;
        y = q;
    } while (y != 0);

    return x;
}

ull lcm(ull a, ull b)
{
    ull res = a, div;
    while (a < b) {
        div = gcd(res, a + 1);
        res = res * (a + 1) / div;
        a++;
    }
    return res;
}

int main()
{
    ull n;
    if (scanf("%llu", &n) != 1) {
        abort();
    }
    printf("%lld \n", lcm(2, n));
    return 0;
}
