#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <errno.h>

#include "sieve.h"
#include "bit-sieve.h"

#define MAX_NUM 16441304
#define DATA_PATH "./data/prime.bit-s.data"

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

#ifdef DEBUG
int main()
{
#if 0
    struct bit_sieve_t bsv;
    srand(time(NULL));

    bsv = init_bsieve(1000000);
    for (unsigned num = 1; num < MAX_NUM; ++num) {
        if (is_bprime(&bsv, num)) {
            printf("%d\n", num);
        }
    }
#else
    struct sieve_t sv;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    int read;
    int i = 0;

    fp = fopen(DATA_PATH, "r");
    if (fp == NULL) {
        perror("fopen() failed");
        abort();
    }
    sv = init_sieve(MAX_NUM);

    while ((read = getline(&line, &len, fp)) != -1) {
        ull num = strtoull(line, NULL, 0);
        if (!is_prime(&sv, num)) {
            printf("%lld\n", num);
            abort();
        }
    }

    fclose(fp);
#endif

}
#endif
