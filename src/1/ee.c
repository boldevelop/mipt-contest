/* расширенный алгоритм Евклида */
#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

/* ax + by = d */
void extended_euclid(ll x, ll y) {
    ll r, q, temp_s, temp_t;
    ll s0 = 1, t0 = 0;
    ll s1 = 0, t1 = 1;

    for (;;) {
        q = x / y;

        r = x - q * y;

        temp_s = s1;
        s1 = s0 - q * s1;
        s0 = temp_s;

        temp_t = t1;
        t1 = t0 - q * t1;
        t0 = temp_t;

        x = y;
        y = r;

        if (r == 0) {
            // x = gcd, coef Bezu = s0, t0
            s0 = x < 0 ? -s0 : s0;
            t0 = x < 0 ? -t0 : t0;
            x = x < 0 ? -x : x;
            printf("%lli %lli %lli\n", s0, t0, x);
            return;
        }
    }
}

int main() {
    ll x, y;
    int res = scanf("%lli %lli", &x, &y);
    if (res != 2) {
        abort();
    }

    extended_euclid(x, y);
}
