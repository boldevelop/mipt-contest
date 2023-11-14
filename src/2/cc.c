/* циркулярные простые */
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MAX_N 1000000

struct sieve_t {
    int n;
    char *s;
};

typedef unsigned long long ull;

ull sieve_bound(ull num)
{
    double dnum, dres;
    assert(num > 20);
    dnum = num;
    dres = dnum * (log(dnum) + log(log(dnum)));
    return (ull) round(dres);
}

void fill_sieve(struct sieve_t *sv)
{
    for (int i = 2; i * i <= sv->n; ++i) {
        if (sv->s[i] == 0) {
            for (int j = i * i; j <= sv->n; j += i) {
                sv->s[j] = 1;
            }
        }
    }
}

struct sieve_t init_sieve(ull num)
{
    struct sieve_t s;
    char *buf;
    s.n = sieve_bound(MAX(num, 21));
    buf = calloc(s.n, sizeof(char));
    if (buf == NULL) {
        abort();
    }
    s.s = buf;
    fill_sieve(&s);

    return s;
}

int is_prime(struct sieve_t sv, int num)
{
    assert(num <= sv.n && "num is more then sieve size");
    return sv.s[num] == 0;
}

int count_digits(int n)
{
    int c = 0;
    while (n) {
        c++;
        n /= 10;
    }
    return c;
}

int is_cyclic_permutation_prime(int n, struct sieve_t sv)
{
    int perm = n, r, strip;
    int digits = count_digits(n);
    do {
        if (!is_prime(sv, perm)) {
            return 0;
        }

        r = perm % 10;
        strip = perm / 10;
        perm = (pow(10, digits - 1)) * r + strip;
    } while (perm != n);
    return 1;
}

void fill_sieve_cyclic(struct sieve_t sv)
{
    for (int i = 2; i <= MAX_N; ++i) {
        if (sv.s[i] == 0 && !is_cyclic_permutation_prime(i, sv)) {
            sv.s[i] = 1;
        }
    }
}

int get_nearest_prime_cyclic(struct sieve_t sv, int n)
{
    int left_prime = 2, right_prime = 2, left_s = 0, right_s = 0;
    if (is_prime(sv, n)) {
        return n;
    }

    for (int i = n - 1; i > 1; i--, left_s++) {
        if (is_prime(sv, i)) {
            left_prime = i;
            break;
        }
    }
    for (int i = n + 1; i < MAX_N; ++i, right_s++) {
        if (is_prime(sv, i)) {
            right_prime = i;
            break;
        }
    }
    if (n + 1 + right_s == MAX_N) {
        return left_prime;
    }
    return left_s < right_s ? left_prime : right_prime;
}

int main()
{
    int n;
    struct sieve_t sv;
    if (scanf("%d", &n) != 1) {
        abort();
    }

    sv = init_sieve(MAX_N);
    fill_sieve_cyclic(sv);
    printf("%d\n", get_nearest_prime_cyclic(sv, n));
}
