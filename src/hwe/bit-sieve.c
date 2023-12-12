#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

#include "bit-sieve.h"

static void set_bit(struct bit_sieve_t *sv, ull bit)
{
    sv->s[bit / CHAR_BIT] |= (1 << bit % CHAR_BIT);
}

static unsigned char get_bit(struct bit_sieve_t *sv, ull bit)
{
    return sv->s[bit / CHAR_BIT] & (1 << bit % CHAR_BIT);
}

static void fill_sieve(struct bit_sieve_t *sv)
{
    int bound = sv->n * CHAR_BIT;
    for (int i = 2; i * i <= bound; ++i) {
        if (get_bit(sv, i - 2) == 0) {
            for (int j = i * i; j <= bound; j += i) {
                set_bit(sv, j - 2);
            }
        }
    }
}

static int sieve_bound(int num)
{
    double dnum, dres;
    assert(num > 20);
    dnum = num;
    dres = dnum * (log(dnum) + log(log(dnum)));
    return (int) ceil(dres / CHAR_BIT);
}

/* a*b mod m */
static ull mul_mod(ull a, ull b, ull m)
{
    ull res = 0;
    while (a != 0) {
        if (a & 1) {
            res = (res + b) % m;
        }
        a >>= 1;
        b = (b << 1) % m;
    }
    return res;
}

/* n^k mod m */
static ull pow_mod(ull n, ull k, ull m)
{
    ull mult = n % m;
    ull prod = 1ull;
    while (k > 0) {
        if ((k % 2) == 1) {
            prod = mul_mod(prod, mult, m) % m;
            k -= 1ull;
        }
        mult = mul_mod(mult, mult, m) % m;
        k >>= 1;
    }
    return prod;
}

static int fermats_test(ull p)
{
    ull a;
    int i = 0;

    while (i < 100) {
        a = ((ull) rand() % p) + 2ull;
        if (a % p == 0 || a == 0 || a > p - 2) {
            continue;
        }
        if (pow_mod(a, p - 1ull, p) != 1ull) {
            return 0;
        }
        i++;
    }
    return 1;
}

struct bit_sieve_t init_bsieve(int num)
{
    struct bit_sieve_t s;
    unsigned char *buf;
    s.n = sieve_bound(MAX(num, 21));
    buf = calloc(s.n, sizeof(unsigned char));
    if (buf == NULL) {
        abort();
    }
    s.s = buf;
    fill_sieve(&s);

    return s;
}

int is_bprime(struct bit_sieve_t *sv, ull probe)
{
    /* 16441304 */
    ull max_sv_num = sv->n * CHAR_BIT;
    if (probe < 2) {
        return 1;
    }
    if (probe < max_sv_num) {
        return get_bit(sv, probe - 2) == 0;
    }
    /* beware THE CARMICHAEL NUMBERS */
    return fermats_test(probe);
}
