#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

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

int nth_prime(struct sieve_t *sv, int N)
{
    int i = 2;
    while (i < sv->n) {
        if (!sv->s[i]) {
            N--;
        }
        if (N <= 0) {
            break;
        }
        i++;
    }
    return i;
}

int main()
{
    struct sieve_t s;
    int n = 300;
    s.n = sieve_bound(n);
    printf("s.n: %d\n", s.n);
    s.s = calloc(s.n + 1, sizeof(char));
    fill_sieve(&s);
    for (int i = 1; i <= 100; ++i) {
        printf("%d prime: %d\n", i, nth_prime(&s, i));
    }
}
