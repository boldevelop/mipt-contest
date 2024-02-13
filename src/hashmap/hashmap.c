#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashmap.h"

#define RANGE_POW_2 10

typedef struct bucket_t {
    struct bucket_t *next;
    char *key;
    int d;
} Bucket;

typedef struct {
    unsigned a;
    unsigned b;
    unsigned p;
    unsigned s;
    Bucket **buckets;
} HashMapImpl;

unsigned PRIME = 10000019;
// unsigned PRIME = 2 * 1000000000 + 11;

static unsigned rand_range(unsigned l, unsigned r)
{
    return l + rand() / (RAND_MAX / (r - l + 1u) + 1u);
}

/* n^k mod sizeof(unsigned) */
static unsigned pow_mod(unsigned n, unsigned k)
{
    unsigned mult = n;
    unsigned prod = 1;
    while (k > 0) {
        if ((k & 1) != 0) {
            prod *= mult;
            k -= 1;
        }
        mult *= mult;
        k >>= 1;
    }
    return prod;
}

static void *hm_calloc(size_t nmemb, size_t size)
{
    void *b = calloc(nmemb, size);
    if (!b) {
        abort();
    }
    return b;
}

static HashMapImpl *hm_alloc(unsigned p, unsigned s)
{
    HashMapImpl *hm = hm_calloc(1, sizeof(HashMapImpl));
    hm->p = p;
    hm->a = rand_range(1, p - 1);
    hm->b = rand_range(0, p - 1);
    hm->s = s;
    hm->buckets = hm_calloc(s, sizeof(Bucket *));
    return hm;
}

static Bucket *alloc_bucket(char *key)
{
    size_t k_l = strlen(key) + 1;

    Bucket *b = hm_calloc(1, sizeof(Bucket));
    b->key = hm_calloc(k_l, sizeof(char));

    strcpy(b->key, key);
    b->d = 1;
    return b;
}

static void free_buckets(Bucket * cur)
{
    while (cur) {
        Bucket *tmp = cur->next;
        free(cur->key);
        free(cur);
        cur = tmp;
    }
}

static unsigned calc_size(unsigned desired_size)
{
    static unsigned pow_2[RANGE_POW_2];

    unsigned s = 0;

    if (!pow_2[0]) {
        for (int i = RANGE_POW_2; i < 20; ++i) {
            pow_2[i - RANGE_POW_2] = pow_mod(2, i);
        }
    }

    for (int i = 0; i < RANGE_POW_2; ++i) {
        if (desired_size < pow_2[i]) {
            s = pow_2[i];
            break;
        }
    }

    if (s == 0) {
        s = pow_2[RANGE_POW_2 - 1];
    }

    return s;
}

/* 
    h(S)  =  S[0]  +  S[1] * P  +  S[2] * P^2  +  S[3] * P^3  +  ...  +  S[N] * P^N

    hi(x) = ((ax + b) % p) % m
    a, b - любые
    m: мощность хэша (степень 2)
    p: простое число
    Строки
    h(c1..cl) = hi( SUM(i, l, ci * r^(i)) % p)
*/
static inline unsigned hash(HashMapImpl * hm, char *key)
{
    unsigned r = 1;
    unsigned sum = 0;

    char *c = key;
    while (*c != '\0') {
        sum += (*c) * r;
        r *= 17;
        c++;
    }
    sum %= hm->p;

    sum = (hm->a * sum + hm->b) % hm->p;
    return sum;
}

static inline unsigned get_index(unsigned h, unsigned s)
{
    return h & (s - 1);
}

HashMap hm_ctor(int strl)
{
    /*  words in str max 'strl / 2',
       so take 'words*2' */
    unsigned s = calc_size(strl);
    return hm_alloc(PRIME, s);
}

int hm_find(HashMap hmp, char *key)
{
    unsigned ind;
    Bucket *cur;
    HashMapImpl *hm = hmp;

    assert(hm);

    ind = get_index(hash(hm, key), hm->s);
    cur = hm->buckets[ind];
    if (!cur) {
        return 0;
    }

    for (; cur != NULL; cur = cur->next) {
        if (strcmp(cur->key, key) == 0) {
            return cur->d;
        }
    }

    return 0;
}

void hm_insert(HashMap hmp, char *key)
{
    unsigned ind;
    Bucket *cur, *prev;
    HashMapImpl *hm = hmp;

    assert(hm);

    ind = get_index(hash(hm, key), hm->s);
    cur = hm->buckets[ind];
    if (!cur) {
        hm->buckets[ind] = alloc_bucket(key);
        return;
    }

    prev = cur;
    for (; cur != NULL; cur = cur->next) {
        prev = cur;
        if (strcmp(cur->key, key) == 0) {
            cur->d++;
            return;
        }
    }

    prev->next = alloc_bucket(key);
}

void hm_dctor(HashMap hmp)
{
    HashMapImpl *hm = hmp;
    for (unsigned i = 0; i < hm->s; ++i) {
        Bucket *cur = hm->buckets[i];
        free_buckets(cur);
    }
    free(hm->buckets);
    free(hm);
}

int hm_rehash(HashMap hmp, int desired_size)
{
    HashMapImpl *hm = hmp;
    Bucket **n_buckets;
    Bucket *cur, *next, *inserted_b;
    unsigned possible_size = calc_size(desired_size);
    unsigned n_ind;

    if (possible_size == hm->s) {
        return 0;
    }

    n_buckets = hm_calloc(possible_size, sizeof(Bucket *));

    for (unsigned i = 0; i < hm->s; ++i) {
        cur = hm->buckets[i];

        while (cur) {
            next = cur->next;
            cur->next = NULL;

            n_ind = get_index(hash(hm, cur->key), possible_size);
            inserted_b = n_buckets[n_ind];

            if (!inserted_b) {
                n_buckets[n_ind] = cur;
                cur = next;
                continue;
            }

            while (inserted_b->next) {
                inserted_b = inserted_b->next;
            }

            inserted_b->next = cur;
            cur = next;
        }
    }

    free(hm->buckets);
    hm->buckets = n_buckets;
    hm->s = possible_size;

    return 1;
}

#undef RANGE_POW_2
