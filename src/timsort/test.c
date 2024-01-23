#include <time.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "timsort.h"

typedef unsigned char uchar;

#define RED     "\033[31m"
#define RESET   "\033[0m"
#define SWP(arr, i, j) tmp = (arr)[(i)];\
    (arr)[(i)] = (arr)[(j)];\
    (arr)[(j)] = tmp;

static int *alloc_arri(const int n)
{
    int *buf;
    buf = calloc(n, sizeof(int));
    if (!buf)
        abort();
    return buf;
}

void fy_shuffle(int *arr, const int s)
{
    int i = s, j, tmp;
    assert(arr);
    while (i-- > 1) {
        j = rand() % (i + 1);
        SWP(arr, j, i);
    }
}

void iota(int *a, const int s, int val)
{
    assert(a);
    for (int i = 0; i < s; ++i) {
        a[i] = val++;
    }
}

int is_equal(const int *a, const int *b, int s)
{
    assert(a);
    assert(b);
    for (int i = 0; i < s; ++i)
        if (a[i] != b[i])
            return 0;
    return 1;
}

int intcmp(const void *a, const void *b)
{
    const uintptr_t *x = a;
    const uintptr_t *y = b;
    return *x - *y;
}

int lessint(const void *a, const void *b)
{
    const uintptr_t *x = a;
    const uintptr_t *y = b;
    return x < y;
}

int lessintp(const void *a, const void *b)
{
    const uintptr_t *x = a;
    const uintptr_t *y = b;
    return x[0] < y[0];
}

typedef void (*Sort)(void *pbase, int total_elems, int size,
                     Comparator cmp);


float measure_sort(void *arr, const int s, int size, Comparator cmp,
                   Sort sort)
{
    clock_t start, end;
    float seconds;
    start = clock();
    sort(arr, s, size, cmp);
    end = clock();
    seconds = (float) (end - start) / CLOCKS_PER_SEC;
    return seconds;
}

float measure_ts(int *arr, const int s)
{
    clock_t start, end;
    float seconds;
    start = clock();
    timsort(arr, s, sizeof(int), intcmp);
    end = clock();
    seconds = (float) (end - start) / CLOCKS_PER_SEC;
    return seconds;
}

float measure_qs(int *arr, const int s)
{
    clock_t start, end;
    float seconds;
    start = clock();
    qsort(arr, s, sizeof(int), intcmp);
    end = clock();
    seconds = (float) (end - start) / CLOCKS_PER_SEC;
    return seconds;
}

float measure_merge(void *arr, int s, int es, void *tmp, void *tmp_swap)
{
    clock_t start, end;
    float seconds;
    start = clock();
    galop_merge(arr, s, s, es, intcmp, tmp, tmp_swap);
    end = clock();
    seconds = (float) (end - start) / CLOCKS_PER_SEC;
    return seconds;
}

void merge_test(void *arr, void *r_arr, int s, int es, void *tmp,
                int measure, int *t_num, int *passed)
{
    int hs = s / 2;
    int ok = 1;
    float secs = 0;
    void *tmp_swap = calloc(1, es);
    memcpy(r_arr, arr, s * es);
    secs = measure_merge(arr, hs, es, tmp, tmp_swap);

    for (int i = 0; i < s; ++i) {
        if (*((int *) arr + i) != (i + 1)) {
            ok = 0;
            break;
        }
    }

    if (measure) {
        printf("Stack %d. ", (*t_num)++);
        if (ok) {
            (*passed)++;
            printf("Ok. %fs ", secs);
        } else {
            printf(RED "FF. %fs " RESET, secs);
        }
        printf("\n");
    } else {
        if (ok) {
            (*passed)++;
        } else {
            printf("Stack %d. " RED "FF. %fs " RESET "\n", (*t_num), secs);
        }
        (*t_num)++;
    }

    secs = measure_merge(r_arr, hs, es, tmp, tmp_swap);

    for (int i = 0; i < s; ++i) {
        if (*((int *) r_arr + i) != (i + 1)) {
            ok = 0;
            break;
        }
    }

    if (measure) {
        printf("Stack %d. ", (*t_num)++);
        if (ok) {
            (*passed)++;
            printf("Ok. %fs ", secs);
        } else {
            printf(RED "FF. %fs " RESET, secs);
        }
        printf("\n");
    } else {
        if (ok) {
            (*passed)++;
        } else {
            printf("Stack %d. " RED "FF. %fs " RESET "\n", (*t_num), secs);
        }
        (*t_num)++;
    }
    free(tmp_swap);
}


void gen_half_iota(int *arr, const int s)
{
    int half = s / 2;
    iota(arr, half, 1);
    iota(arr + half, half, 1);
}

void gen_fourth_incr_swapped_parts(int *arr, const int s)
{
    int fourth = s / 4;

    iota(arr, fourth, fourth);

    arr += fourth;
    iota(arr, fourth, fourth * 2);

    arr += fourth;
    iota(arr, fourth, fourth * 3);

    arr += fourth;
    iota(arr, fourth, 1);
}

/* same as gen_fourth_incr_swapped_parts but adding shuffled */
void gen_fisp_and_eight_shuffled(int *arr, const int s)
{
    int fourth = s / 4;
    int eighth = fourth / 2;

    iota(arr, fourth, fourth);
    fy_shuffle(arr + eighth, eighth);

    arr += fourth;
    iota(arr, fourth, fourth * 2);
    fy_shuffle(arr + eighth, eighth);

    arr += fourth;
    iota(arr, fourth, fourth * 3);
    fy_shuffle(arr + eighth, eighth);

    arr += fourth;
    iota(arr, fourth, 1);
    fy_shuffle(arr + eighth, eighth);
}
#if 0
/* perf */
int main()
{
    {
        int full_size = 1000000;
        int *ts_arr = alloc_arri(full_size);
        int s = full_size;
        float ts_s;
        iota(ts_arr, full_size, 1);
        while (s <= full_size) {
            fy_shuffle(ts_arr, s);
            ts_s = measure_sort(ts_arr, s, sizeof(int), intcmp, timsort);
            printf("%fs\n", ts_s);
            s *= 10;
        }
        free(ts_arr);
    }
}
#endif
#if 1
int main()
{
    int merge_t_num = 1;
    int merge_t_passed = 0;
    {
        int fs = 100;
        int hs = fs / 2;
        int *arr = alloc_arri(fs);
        /* for test with swap l to r */
        int *arr_r = alloc_arri(fs);
        int *tmp = alloc_arri(fs);


        for (int k = 1; k < 10; ++k) {
            iota(arr, fs, 1);

            for (int i = k, j = 0; i < hs; ++i, ++j) {
                int tmp;
                SWP(arr, i, hs + j);
            }
            assert(arr[0] == 1);
            assert(arr[k] == hs + 1);
            assert(arr[hs] == 1 + k);
            assert(arr[fs - k] == fs - k + 1);
            merge_test(arr, arr_r, fs, sizeof(int), tmp, 1, &merge_t_num,
                       &merge_t_passed);
            // merge_test(arr, arr_r, fs, 0, &merge_t_num, &merge_t_passed);
        }
        free(arr);
        free(arr_r);
        free(tmp);
    }
    {
        /* full size */
        int fs = 1000000;
        int hs = fs / 2;
        int hhs = hs / 2;
        int *arr = alloc_arri(fs);
        /* for test with swap l to r */
        int *arr_r = alloc_arri(fs);
        int *tmp = alloc_arri(fs);

        iota(arr, fs, 1);
        assert(arr[0] == 1);
        assert(arr[hhs] == hhs + 1);
        assert(arr[hs] == hs + 1);
        assert(arr[hs + hhs] == hs + hhs + 1);
        merge_test(arr, arr_r, fs, sizeof(int), tmp, 1, &merge_t_num,
                   &merge_t_passed);

        for (int k = 1; k < 10; ++k) {
            iota(arr, fs, 1);

            for (int i = k, j = 0; i < hs; ++i, ++j) {
                int tmp;
                SWP(arr, i, hs + j);
            }
            assert(arr[0] == 1);
            assert(arr[k] == hs + 1);
            assert(arr[hs] == 1 + k);
            assert(arr[fs - k] == fs - k + 1);
            merge_test(arr, arr_r, fs, sizeof(int), tmp, 1, &merge_t_num,
                       &merge_t_passed);
        }

        for (int k = hs - 10; k < hs; ++k) {
            iota(arr, fs, 1);

            for (int i = k, j = 0; i < hs; ++i, ++j) {
                int tmp;
                SWP(arr, i, hs + j);
            }
            assert(arr[0] == 1);
            assert(arr[k] == hs + 1);
            assert(arr[hs] == 1 + k);
            assert(arr[fs - k] == fs - k + 1);
            merge_test(arr, arr_r, fs, sizeof(int), tmp, 1, &merge_t_num,
                       &merge_t_passed);
        }

        iota(arr, fs, 1);
        for (int i = hhs; i < hs; ++i) {
            int tmp;
            SWP(arr, i, i + hhs);
        }
        assert(arr[0] == 1);
        assert(arr[hhs] == hs + 1);
        assert(arr[hs] == hhs + 1);
        assert(arr[hs + hhs] == hs + hhs + 1);
        merge_test(arr, arr_r, fs, sizeof(int), tmp, 1, &merge_t_num,
                   &merge_t_passed);

        merge_t_num--;
        printf("%d merge tests passed.\n", merge_t_passed);
        if (merge_t_num - merge_t_passed) {
            printf(RED "%d" RESET " merge tests FAILED.\n",
                   merge_t_num - merge_t_passed);
        }

        free(arr);
        free(arr_r);
        free(tmp);
    }
    // return 0;
#if 0
    /* unit for stability */
    {
        /* group_size */
        uintptr_t gs = 2;
        /* list size */
        int ls = 2;
        int t_num = 1;
        int okTests = 0;

        for (; gs < 100; ++gs) {
            /* groups */
            uintptr_t g = 2;
            for (; g < 100; ++g) {
                int ok = 1;
                uintptr_t s = g * gs;
                uintptr_t **arr = (uintptr_t **) alloc_arruip(s);
                uintptr_t c = 1;

                for (uintptr_t i = 0; i < gs; ++i) {
                    for (uintptr_t j = 0; j < g; ++j) {
                        arr[i + j * gs] = alloc_arruip(ls);
                        arr[i + j * gs][0] = i + 1;
                        arr[i + j * gs][1] = c++;
                    }
                }

                timsort((uintptr_t *) arr, s, lessintp);

                for (uintptr_t i = 0; i < s; ++i) {
                    if (arr[i][1] != (uintptr_t) (i + 1))
                        ok = 0;
                }

                if (ok) {
                    okTests++;
                } else {
                    printf("Stable %d." RED "FF." RESET "\n", t_num);
                }
                t_num++;
                for (uintptr_t i = 0; i < s; ++i) {
                    free(arr[i]);
                }
                free(arr);
            }
        }
        t_num--;

        printf("%d stability tests passed.\n", okTests);
        if (t_num - okTests > 1) {
            printf(RED "%d" RESET " stability tests FAILED.\n",
                   t_num - okTests - 1);
        }
    }
#endif
    /* sorting unit */
    /* make stress for 1000000 */
    {
        int s = 10;
        int *arr = calloc(s, sizeof(int));
        for (int i = 0; i < s; ++i) {
            arr[i] = s - i;
        }
        measure_sort(arr, s, sizeof(int), intcmp, timsort);
        for (int i = 0; i < s; ++i) {
            printf("%d ", arr[i]);
        }
        printf("\n");
        free(arr);
    }
    // return 0;
    {
        int full_size = 1000000;
        int *ts_arr = alloc_arri(full_size);
        int *qs_arr = alloc_arri(full_size);
        int s = 10;
        float ts_s, qs_s;

        iota(ts_arr, full_size, 1);
        iota(qs_arr, full_size, 1);

        while (s <= full_size) {
            printf("size: %d.\n", s);
            fy_shuffle(ts_arr, s);
            fy_shuffle(qs_arr, s);

            // ts_s = measure_ts(ts_arr, s);
            ts_s = measure_sort(ts_arr, s, sizeof(int), intcmp, timsort);
            // qs_s = measure_sort(qs_arr, s, sizeof(int), intcmp, qsort);
            qs_s = measure_qs(qs_arr, s);

            printf("Shuffled all. ");
            if (!is_equal(ts_arr, qs_arr, s)) {
                printf(RED "FF\nNot equal\n" RESET);
            } else {
                printf("OK\n");
                printf("ts: %fs\n", ts_s);
                printf("qs: %fs\n", qs_s);
            }

            gen_half_iota(ts_arr, s);
            gen_half_iota(qs_arr, s);

            ts_s = measure_ts(ts_arr, s);
            qs_s = measure_qs(qs_arr, s);

            printf("Half equal. ");
            if (!is_equal(ts_arr, qs_arr, s)) {
                printf(RED "FF\nNot equal\n" RESET);
            } else {
                printf("OK\n");
                printf("ts: %fs\n", ts_s);
                printf("qs: %fs\n", qs_s);
            }

            if (s > 10) {
                gen_fourth_incr_swapped_parts(ts_arr, s);
                gen_fourth_incr_swapped_parts(qs_arr, s);

                ts_s = measure_ts(ts_arr, s);
                qs_s = measure_qs(qs_arr, s);

                printf("Increased 4 parts. ");
                if (!is_equal(ts_arr, qs_arr, s)) {
                    printf(RED "FF\nNot equal\n" RESET);
                } else {
                    printf("OK\n");
                    printf("ts: %fs\n", ts_s);
                    printf("qs: %fs\n", qs_s);
                }

                gen_fisp_and_eight_shuffled(ts_arr, s);
                gen_fisp_and_eight_shuffled(qs_arr, s);

                ts_s = measure_ts(ts_arr, s);
                qs_s = measure_qs(qs_arr, s);

                printf("Increased 4 parts and half of them shuffled. ");
                if (!is_equal(ts_arr, qs_arr, s)) {
                    printf(RED "FF\nNot equal\n" RESET);
                } else {
                    printf("OK\n");
                    printf("ts: %fs\n", ts_s);
                    printf("qs: %fs\n", qs_s);
                }
            }

            s *= 10;
        }

        free(ts_arr);
        free(qs_arr);
    }
}
#endif
