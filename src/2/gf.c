/* генерирующие формулы */
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

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
    int n = num < 0 ? -num : num;
    assert(n <= sv.n && "num is more then sieve size");
    return sv.s[n] == 0;
}

int eq(int n, int a, int b)
{
    return n * n + a * n + b;
}

void find_max_prime_sequance(int max)
{
    struct sieve_t s = init_sieve(max * 1000);
    int max_seq = 0;
    int max_seq_a = 0, max_seq_b = 0;
    for (int a = -max + 1; a < max; a++) {
        for (int b = -max + 1; b < max; b++) {
            int n = 0, prime;
            if (!is_prime(s, b)) {
                continue;
            }
            while ((prime = eq(n, a, b)) && prime > 0
                   && is_prime(s, prime)) {
                n++;
            }
            if (max_seq < n) {
                max_seq_a = a;
                max_seq_b = b;
                max_seq = n;
            }
        }
    }

    printf("%d %d %d\n", max_seq_a, max_seq_b, max_seq);
}

int main()
{
    int N;
    if (scanf("%d", &N) != 1) {
        abort();
    }
    find_max_prime_sequance(N);
}
