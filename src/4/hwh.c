/* 
h(S)  =  S[0]  +  S[1] * P  +  S[2] * P^2  +  S[3] * P^3  +  ...  +  S[N] * P^N

hi(x) = ((ax + b) % p) % m
a, b - любые
m: мощность хэша
p: простое число
Строки
h(c1..cl) = hi( SUM(i, l, ci * r^(l-i)) % p)

unsigned hashint(unsigned a, unsigned b, unsigned x) {
    return (a*x + b) >> (w - M);
}

djb2
unsigned long
hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; // hash * 33 + c 

    return hash;
}

K&R2
unsigned hash(char *s)
{
    unsigned hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31*hashval;
    return hashval % HASHSIZE;
}

https://raw.githubusercontent.com/dwyl/english-words/master/words.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
/* typedef long long i64; size_t (unsigned long) */

// #define HASHSIZE 65536
#define HASHSIZE 466550

int readint() {
    int n;
    if (scanf("%d", &n) != 1) {
        abort();
    }
    return n;
}

void filter_words();
unsigned pow_mod(unsigned n, unsigned k);
unsigned a;
unsigned b;

struct bucket_t {
    struct bucket_t* next;
    char* key;
    int d;
};
typedef struct bucket_t Bucket; 

typedef struct {
    unsigned a;
    unsigned b;
    unsigned p;
    unsigned s;
    Bucket** buckets;
} HashMap;

HashMap* init_hm(int strl);
int find_hm(HashMap* hm, char* key);
void insert_hm(HashMap* hm, char* key);
void destroy_hm(HashMap* hm);

int main() {
    char* wrds[10][512];
    char word[512];
    int i = 0;
    int c;
    // srand(738547485u);
    // a = rand();
    // b = rand();
    // printf("%d %d\n", a, b);

    #if 0
    int a, l;
    a = readint();
    l = readint();
    #endif

    char buf[1024];
    int bi = 0; 
    do {
        c = getchar();
        if (isspace(c) || c == EOF || c == '\n') {
            buf[bi++] = '\0';
            strcpy(wrds[i], buf);
            i++;
            bi = 0;
            if (c == EOF || i == 3) {
                break;
            }
            continue;
        }
        buf[bi++] = c;
    } while (1);
    // while ((scanf("%s", &word)) == 0) {
    //     wrds[i++] = word;
    // }
    for (int j = 0; j < i; ++j) {
        printf("%s\n", wrds[j]);
    }
    return;

    // filter_words();
}

unsigned hash(char *s)
{
    unsigned h;

    for (h = 0; *s != '\0'; s++) {
        h = *s + 31*h;
    }

    return h % HASHSIZE;
}


#define MAX(l, r) (l) < (r) ? (r) : (l);

void collision(int* b) {
    int c = 0, max = 0;
    for (int i = 0; i < HASHSIZE; ++i) {
        c += b[i];
        max = MAX(max, b[i]);
    }
    printf("strhash\n");
    printf("HS: %d\n", HASHSIZE);
    printf("c: %d\n", c/HASHSIZE);
    printf("max: %d\n", max);
}
unsigned strhash(char* str);
void filter_words() {
    char line[256];
    int *buf = calloc(HASHSIZE, sizeof(int));
    if (!buf) abort();
    while(fgets(line, sizeof(line), stdin) != NULL) {
        unsigned h;
        h = strhash(line);
        buf[h]++;
    }
    
    collision(buf);
    free(buf);
}

/* n^k mod sizeof(unsigned) */
unsigned pow_mod(unsigned n, unsigned k)
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

unsigned strhash(char* str) {
    unsigned p = 10000019;
    // unsigned p = 2 * 1000000000 + 11;
    unsigned r = 1;
    unsigned sum = 0;

    char *c = str;
    while (*c != '\0') {
        sum += (*c) * r;
        r *= 17;
        c++;
    }
    sum %= p;

    sum = (a * sum + b) % p;
    return sum % HASHSIZE;
}

#define MAX_POW_2 10

unsigned rand_range(unsigned l, unsigned r) {
    return l + rand() / (RAND_MAX / (r - l + 1u) + 1u);
}

HashMap* alloc_hm(unsigned p, unsigned s);
Bucket* alloc_bucket(char* key);

HashMap* init_hm(int strl) {
    static unsigned pow_2[MAX_POW_2];
    /*  words in str max 'strl / 2',
        so take 'words*2' */
    unsigned posible_count_words = strl;
    unsigned s = 0;
    unsigned p = 10000019;
    // unsigned p = 2 * 1000000000 + 11;

    if (!pow_2[0]) {
        for (int i = MAX_POW_2; i < 20; ++i) {
            pow_2[i - MAX_POW_2] = pow_mod(2, i);
        }
    }

    for (int i = 0; i < MAX_POW_2; ++i) {
        if (posible_count_words < pow_2[i]) {
            s = pow_2[i];
        }
    }


    return alloc_hm(p, s);
}

unsigned hs(HashMap* hm, char* key) {
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

int find_hm(HashMap* hm, char* key) {
    unsigned ind;
    Bucket* cur;

    assert(hm);

    ind = hs(hm, key) % hm->s;
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

void insert_hm(HashMap* hm, char* key) {
    unsigned ind;
    Bucket* cur, *prev;

    assert(hm);
    
    ind = hs(hm, key) % hm->s;
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

HashMap* alloc_hm(unsigned p, unsigned s) {
    HashMap* hm = calloc(1, sizeof(HashMap));
    if (!hm) abort();
    hm->p = p;
    hm->a = rand_range(1, p - 1);
    hm->b = rand_range(0, p - 1);
    hm->s = s;
    hm->buckets = calloc(s, sizeof(Bucket*));
    if (!hm->buckets) abort();
    return hm;
}

Bucket* alloc_bucket(char* key) {
    Bucket* b = calloc(1, sizeof(Bucket));
    if (!b) abort();
    b->key = key;
    b->d = 1;
    return b;
}

void free_buckets(Bucket* cur) {
    while (cur) {
        Bucket* tmp = cur->next;
        free(cur);
        cur = tmp;
    }
}

void destroy_hm(HashMap* hm) {
    for (unsigned i = 0; i < hm->s; ++i) {
        Bucket* cur = hm->buckets[i];
        free_buckets(cur);
    }
    free(hm->buckets);
    free(hm);
}