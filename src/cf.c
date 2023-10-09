/* цепные дроби */
#include <assert.h>
#include <stdio.h>

typedef long long ll;

ll llabs(ll x) { return (x < 0) ? -x : x; }
ll mod(ll x, ll y) { return llabs(x) % llabs(y); }

void cf(ll n, ll d) {
    ll r, q;
    assert(d != 0);
    while (d != 0) {
        r = mod(n ,d);
        q = (n - r) / d;
        printf("%lli ", q);
        n = d;
        d = r;
    }
}

int main() {
    ll num, den;

    int res = scanf("%lli %lli", &num, &den);
    assert(res == 2);
    cf(num, den);
    printf("\n");

    return 0;
}
