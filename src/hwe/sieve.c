#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#ifdef DEBUG
#include "sieve.h"
#endif

typedef unsigned char uchar;
typedef unsigned long long ull;

static int is_mod1(unsigned n, unsigned *bit)
{
    *bit = (n - 1) / 6;
    return (n - 1) % 6 == 0;
}

static int is_mod5(unsigned n, unsigned *bit)
{
    *bit = (n - 5) / 6;
    return (n - 5) % 6 == 0;
}

static void set_bit(uchar * bytes, ull bit)
{
    assert(bit < UINT_MAX);
    bytes[bit / CHAR_BIT] |= (1 << bit % CHAR_BIT);
}

static uchar get_bit(uchar * bytes, ull bit)
{
    uchar res;
    uchar offset;
    assert(bit < UINT_MAX);
    offset = bit % CHAR_BIT;
    res = bytes[bit / CHAR_BIT] & (1 << offset);
    return res >> offset;
}

/* Проверять предыдущие */
void fill_sieve(struct sieve_t *sv)
{
    ull size = sv->n * CHAR_BIT;
    ull bit = 0, mod1, mod5, num;
    while (bit < size) {
        if (bit < 2) {
            set_bit(sv->mod1, bit);
            set_bit(sv->mod5, bit);
        } else {
            num = bit * 6;
            mod1 = num + 1;
            mod5 = num + 5;
            if (mod1 % 5 != 0 && mod1 % 7 != 0) {
                set_bit(sv->mod1, bit);
            }
            if (mod5 % 5 != 0 && mod5 % 7 != 0) {
                set_bit(sv->mod5, bit);
            }
        }
        bit++;
    }
}

int is_prime(struct sieve_t *sv, unsigned n)
{
    unsigned bit;
    if (n == 0) {
        return 0;
    }
    if (n > 0 && n < 6) {
        return n != 4;
    }
    if (is_mod1(n, &bit)) {
        return get_bit(sv->mod1, bit) == 1;
    }
    if (is_mod5(n, &bit)) {
        return get_bit(sv->mod5, bit) == 1;
    }
    return 0;
}

struct sieve_t init_sieve(int num)
{
    struct sieve_t s;
    uchar *buf;
    s.n = num / CHAR_BIT + 1;
    buf = calloc(s.n, sizeof(uchar));
    if (buf == NULL) {
        abort();
    }
    s.mod1 = buf;
    buf = calloc(s.n, sizeof(uchar));
    if (buf == NULL) {
        abort();
    }
    s.mod5 = buf;
    fill_sieve(&s);

    return s;
}
