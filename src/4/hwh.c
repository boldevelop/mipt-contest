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

int main() {
    srand(738547485u);
    a = rand();
    b = rand();
    printf("%d %d\n", a, b);

    #if 0
    int a, l;
    a = readint();
    l = readint();
    #endif
    filter_words();
    // sizeof(size_t); word
}

unsigned hash(char *s)
{
    unsigned h;

    for (h = 0; *s != '\0'; s++) {
        h = *s + 31*h;
    }

    return h % HASHSIZE;
}

unsigned long
hashdjb2(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; // hash * 33 + c 

    return hash % HASHSIZE;
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

    unsigned *c = str;
    while (*c != '\0') {
        sum += (*c) * r;
        r *= 17;
        c++;
    }
    sum %= p;

    sum = (a * sum + b) % p;
    return sum % HASHSIZE;
}