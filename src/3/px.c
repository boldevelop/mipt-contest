/*  возведение в степень произвольных матриц
    по модулю */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifdef DEBUG
#define N 3
#endif                          /* DEBUG */

typedef unsigned Mat[N];
typedef unsigned long long ull;

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

void identityNxN(Mat * A)
{
    for (int i = 0; i < N; ++i) {
        unsigned *A_row = *(A + i);
        for (int j = 0; j < N; ++j) {
            A_row[j] = i == j;
        }
    }
}

void copyNxN(Mat * src, Mat * dst)
{
    for (int i = 0; i < N; ++i) {
        unsigned *src_row = *(src + i);
        unsigned *dst_row = *(dst + i);
        memcpy(dst_row, src_row, sizeof(unsigned) * N);
    }
}

void mulNxN(Mat * A, Mat * B, unsigned m)
{
    unsigned row_tmp[N];

    for (int i = 0; i < N; ++i) {
        unsigned *A_row = *(A + i);
        memcpy(row_tmp, A_row, sizeof(unsigned) * N);

        for (int j = 0; j < N; ++j) {
            unsigned res = 0;

            for (int k = 0; k < N; ++k) {
                unsigned row_val = row_tmp[k];
                unsigned col_val = (*(B + k))[j];

                res += mul_mod(row_val, col_val, m);
                res %= m;
            }
            A_row[j] = res;
        }
    }
}

/* A^k mod m */
void powNxN(unsigned (*A)[N], unsigned k, unsigned m)
{
    unsigned mult[N][N] = { 0 };
    unsigned prod[N][N] = { 0 };
    unsigned temp[N][N] = { 0 };
    copyNxN(A, mult);
    identityNxN(prod);

    while (k > 0) {
        if ((k % 2) == 1) {
            mulNxN(prod, mult, m);
            k -= 1;
        }
        copyNxN(mult, temp);
        mulNxN(mult, temp, m);
        k >>= 1;
    }
    copyNxN(prod, A);
}

#ifdef DEBUG
void dump_matr(Mat * A);
int equal(Mat * l, Mat * r);
void test(Mat * val1, Mat * val2, Mat * val3, unsigned k);

int main()
{
    // for (int k = 1; k < 100000; k += 987) {
    //     unsigned val1[][N] = {{1, 1}, {1, 0}};
    //     unsigned val2[][N] = {{1, 1}, {1, 0}};
    //     unsigned val3[][N] = {{1, 1}, {1, 0}};
    //     test(val1, val2, val3, k);
    // }
    for (int k = 1; k < 100000; k += 987) {
        unsigned val1[][N] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 0 } };
        unsigned val2[][N] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 0 } };
        unsigned val3[][N] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 0 } };
        test(val1, val2, val3, k);
    }

    return 0;
}

void test(Mat * val1, Mat * val2, Mat * val3, unsigned k)
{
    unsigned i = 1;
    unsigned m = 10;

    while (i < k) {
        mulNxN(val2, val3, m);
        i++;
    }
    powNxN(val1, k, m);

    if (!equal(val1, val2)) {
        printf("%d NOT EQUAL \n", k);
        dump_matr(val1);

        printf("\nNaive\n");
        dump_matr(val2);
        printf("- - -\n\n");
    } else {
        printf("%d OK\n", k);
    }

}

int equal(Mat * l, Mat * r)
{
    for (int i = 0; i < N; ++i) {
        unsigned *row_l = *(l + i);
        unsigned *row_r = *(r + i);
        for (int j = 0; j < N; ++j) {
            if (row_l[j] != row_r[j]) {
                return 0;
            }
        }
    }
    return 1;
}

void dump_matr(Mat * A)
{
    for (int i = 0; i < N; ++i) {
        unsigned *row = *(A + i);
        for (int j = 0; j < N; ++j) {
            printf("%d ", row[j]);
        }
        printf("\n");
    }
}

#endif
