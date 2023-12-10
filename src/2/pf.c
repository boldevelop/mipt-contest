/*
Простые числа, подобные числам Фибоначчи
Seqi = k * Seq(i - 1) + n * Seq(i - 2)
вычислить самое большое простое число P, такое, что
P < 2^60 и P входит в данную последовательность
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

typedef unsigned long long ull;
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
const ull P_MAX = 1ull << 60;

/* BEGIN - - - seive - - -*/
struct sieve_t {
    int n;
    unsigned char *s;
};

void set_bit(struct sieve_t *sv, ull bit)
{
    int byte_n = bit / CHAR_BIT;
    sv->s[byte_n] = sv->s[byte_n] | (1 << bit % CHAR_BIT);
}

unsigned char get_bit(struct sieve_t *sv, ull bit)
{
    return sv->s[bit / CHAR_BIT] & (1 << bit % CHAR_BIT);
}

void fill_sieve(struct sieve_t *sv)
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

int sieve_bound(int num)
{
    double dnum, dres;
    assert(num > 20);
    dnum = num;
    dres = dnum * (log(dnum) + log(log(dnum)));
    return (int) ceil(dres / CHAR_BIT);
}

struct sieve_t init_sieve(int num)
{
    struct sieve_t s;
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

void dump_bits(unsigned char *buf, int n)
{
    unsigned char byte;
    for (int i = 0; i < n; ++i) {
        for (int j = CHAR_BIT - 1; j >= 0; j--) {
            byte = (buf[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    printf("\n");
}

/* END - - - seive - - -*/

/* a*b mod m */
ull mul_mod(ull a, ull b, ull m)
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
ull pow_mod(ull n, ull k, ull m)
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

int fermats_test(ull p)
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

int is_prime(struct sieve_t *sv, ull probe)
{
    /* 16441304 */
    ull max_sv_num = sv->n * CHAR_BIT;
    if (probe < 2) {
        return 1;
    }
    if (probe < max_sv_num) {
        return get_bit(sv, probe - 2) == 0;
    }
    return fermats_test(probe);
}

ull seq(ull k, ull n, ull seqim1, ull seqim2)
{
    return k * seqim1 + n * seqim2;
}

ull max_prime(ull k, ull n, struct sieve_t *sv)
{
    ull seqim1 = 1, seqim2 = 0, prime = 0, tmp;
    for (;;) {
        tmp = seqim1;
        seqim1 = seq(k, n, seqim1, seqim2);
        if (seqim1 >= P_MAX) {
            return prime;
        }
        if (is_prime(sv, seqim1)) {
            prime = seqim1;
        }
        seqim2 = tmp;
    }
}

int main()
{
    ull k, n, p;
    struct sieve_t sv;
    srand(time(NULL));

    if (scanf("%llu%llu", &k, &n) != 2) {
        abort();
    }
    sv = init_sieve(1000000);
    p = max_prime(k, n, &sv);
    printf("%llu\n", p);
    return 0;
}
