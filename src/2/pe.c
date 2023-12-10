/* побитовое решето */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

#ifdef DEBUG
struct sieve_t {
    int n;
    unsigned char *s;
};
#endif

void set_bit(struct sieve_t *sv, int bit)
{
    int byte_n = bit / CHAR_BIT;
    sv->s[byte_n] = sv->s[byte_n] | (1 << bit % CHAR_BIT);
}

unsigned char get_bit(struct sieve_t *sv, int bit)
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

int is_prime(struct sieve_t *sv, unsigned n)
{
    if (n < 2)
        return 1;
    return get_bit(sv, n - 2) == 0;
}

#ifdef DEBUG
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

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

int main()
{
    struct sieve_t sv;
    sv = init_sieve(1000000);

    dump_bits(sv.s, 2);
    for (unsigned i = 2; i < 25; ++i) {
        int prime = is_prime(&sv, i);
        printf("%d: %d \n", i, prime);
    }
    return 0;
}
#endif
