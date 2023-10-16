/* Возведение матрицы 2x2 в степень n по модулю m */
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long ull;
#define At(m, i, j) m[(i * 2) + (j)]

void printm(ull *mat) {
    printf("%lld ",  At(mat, 0, 0));
    printf("%lld ",  At(mat, 0, 1));
    printf("%lld ",  At(mat, 1, 0));
    printf("%lld\n", At(mat, 1, 1));
}

/* n^k mod m */
unsigned pow_mod(unsigned n, unsigned k, unsigned m) {
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

void mswap(ull *l, ull *r) {
    At(l, 0, 0) = At(r, 0, 0);
    At(l, 0, 1) = At(r, 0, 1);
    At(l, 1, 0) = At(r, 1, 0);
    At(l, 1, 1) = At(r, 1, 1);
}

void minit(ull *mat) {
    At(mat, 0, 0) = 1;
    At(mat, 0, 1) = 0;
    At(mat, 1, 0) = 0;
    At(mat, 1, 1) = 1;
}

void mprod(ull *l, ull *r, ull m, int assignToRight) {
    ull t[4];

    At(t, 0, 0) = (At(l, 0, 0) * At(r, 0, 0)) % m +
                  (At(l, 0, 1) * At(r, 1, 0)) % m;
    At(t, 0, 0) %= m;

    At(t, 1, 0) = (At(l, 1, 0) * At(r, 0, 0)) % m +
                  (At(l, 1, 1) * At(r, 1, 0)) % m;
    At(t, 1, 0) %= m;

    At(t, 0, 1) = (At(l, 0, 0) * At(r, 0, 1)) % m +
                  (At(l, 0, 1) * At(r, 1, 1)) % m;
    At(t, 0, 1) %= m;

    At(t, 1, 1) = (At(l, 1, 0) * At(r, 0, 1)) % m +
                  (At(l, 1, 1) * At(r, 1, 1)) % m;
    At(t, 1, 1) %= m;

    if (assignToRight == 1) {
        mswap(r, t);
    } else {
        mswap(l, t);
    }
}

void mpowm(ull *mat, ull n, ull m) {
    ull mult[4];
    ull prod[4];
    At(mult, 0, 0) = At(mat, 0, 0) % m;
    At(mult, 0, 1) = At(mat, 0, 1) % m;
    At(mult, 1, 0) = At(mat, 1, 0) % m;
    At(mult, 1, 1) = At(mat, 1, 1) % m;

    minit(prod);
    while (n > 0) {
        if ((n % 2) == 1) {
            mprod(prod, mult, m, 0);
            n -= 1;
        }
        mprod(mult, mult, m, 0);
        n /= 2;
    }
    printm(prod);
}

void naive_mpowm(ull *mat, ull n, ull m) {
    ull mult[4];
    minit(mult);
    while (n > 0) {
        mprod(mult, mat, m, 0);
        n--;
    }
    printm(mult);
}

int main() {
    int res;
    ull mat[4];
    ull n, m;
    res = scanf("%lld%lld%lld%lld", &At(mat, 0, 0), &At(mat, 0, 1), &At(mat, 1, 0), &At(mat, 1, 1));
    if (res != 4) abort();
    res = scanf("%lld%lld", &n, &m);
    if (res != 2) abort();
#if 0
    naive_mpowm(mat, n, m);
#endif
    mpowm(mat, n , m);
    return 0;
}
