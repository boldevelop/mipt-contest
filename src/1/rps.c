/* сверхстепень a^^b */

#include <stdlib.h>
#include <stdio.h>

/* n^k mod m */
unsigned pow_mod(unsigned n, unsigned k, unsigned m)
{
    unsigned mult = n % m;
    unsigned prod = 1;
    while (k > 0) {
        if ((k % 2) == 1) {
            prod = (prod * mult) % m;
            k -= 1;
        }
        mult = (mult * mult) % m;
        k /= 2;
    }
    return prod;
}

unsigned powu(unsigned b, unsigned e)
{
    unsigned prod = 1;
    while (e > 0) {
        if ((e % 2) == 1) {
            prod = (prod * b);
            e -= 1;
        }
        b = b * b;
        e /= 2;
    }
    return prod;
}

unsigned dm[32] = { 0 };

unsigned sp(unsigned b, unsigned h)
{
    if (h < 2)
        return dm[h];
    if (dm[h] != 0)
        return dm[h];
    dm[h] = powu(b, sp(b, h - 1));
    return dm[h];
}

unsigned spmod(unsigned b, unsigned h, unsigned m)
{
    if (h < 2)
        return dm[h];
    return pow_mod(b, sp(b, h - 1), m);
}

int main()
{
    unsigned int a, b, n;
    int ret = scanf("%d%d%d", &a, &b, &n);
    if (ret < 3) {
        abort();
    }
    dm[0] = 1;
    dm[1] = a % n;
    printf("%d\n", spmod(a, b, n));
    return 0;
}
