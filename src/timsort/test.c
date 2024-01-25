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

static float *alloc_arrf(const int n)
{
    float *buf;
    buf = calloc(n, sizeof(float));
    if (!buf)
        abort();
    return buf;
}

static void fy_shuffle(int *arr, const int s)
{
    int i = s, j, tmp;
    assert(arr);
    while (i-- > 1) {
        j = rand() % (i + 1);
        SWP(arr, j, i);
    }
}

static void iota(int *a, const int s, int val)
{
    assert(a);
    for (int i = 0; i < s; ++i) {
        a[i] = val++;
    }
}

static int is_equal(const int *a, const int *b, int s)
{
    assert(a);
    assert(b);
    for (int i = 0; i < s; ++i)
        if (a[i] != b[i])
            return 0;
    return 1;
}

static int intcmp(const void *a, const void *b)
{
    const int *x = a;
    const int *y = b;
    return *x - *y;
}

static int intpcmp(const void *a, const void *b)
{
    const int *x = a;
    const int *y = b;
    return x[0] - y[0];
}

typedef void (*Sort)(void *pbase, size_t total_elems, size_t size,
                     Comparator cmp);


static float measure_sort(void *arr, size_t s, size_t size, Comparator cmp,
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

static float measure_merge(void *arr, int s, int es, void *tmp,
                           void *tmp_swap)
{
    clock_t start, end;
    float seconds;
    start = clock();
    galop_merge(arr, s, s - 1, es, intcmp, tmp, tmp_swap);
    end = clock();
    seconds = (float) (end - start) / CLOCKS_PER_SEC;
    return seconds;
}

static int is_merge_ok(int *arr, int s)
{
    int ok = 1;
    for (int i = 0; i < s; ++i) {
        if (arr[i] != (i + 1)) {
            ok = 0;
            break;
        }
    }

    return ok;
}

static void print_merge_test_verdict(int ok, int t_num, float secs)
{
    printf("Stack %d. ", t_num);
    if (ok) {
        printf("Ok. %fs ", secs);
    } else {
        printf(RED "FF. %fs " RESET, secs);
    }
    printf("\n");
}

static void merge_test(void *arr, void *r_arr, int s, int es, void *tmp,
                       void *tmp_swap, int is_print, int *t_num,
                       int *passed)
{
    int hs = s / 2;
    int ok = 1;
    float secs = 0;
    memcpy(r_arr, arr, s * es);

    secs = measure_merge(arr, hs, es, tmp, tmp_swap);
    ok = is_merge_ok(arr, s);
    if (ok) {
        (*passed)++;
    }
    if (is_print) {
        print_merge_test_verdict(ok, *t_num, secs);
    }
    (*t_num)++;

    secs = measure_merge(r_arr, hs, es, tmp, tmp_swap);
    ok = is_merge_ok(r_arr, s);
    if (ok) {
        (*passed)++;
    }
    if (is_print) {
        print_merge_test_verdict(ok, *t_num, secs);
    }
    (*t_num)++;
}


static void gen_half_iota(int *arr, const int s)
{
    int half = s / 2;
    iota(arr, half, 1);
    iota(arr + half, half, 1);
}

static void gen_fourth_iota(int *arr, const int s)
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

static void shuffle_end_of_fourth(int *arr, const int s)
{
    int fourth = s / 4;
    int eighth = fourth / 2;

    fy_shuffle(arr + eighth, eighth);

    arr += fourth;
    fy_shuffle(arr + eighth, eighth);

    arr += fourth;
    fy_shuffle(arr + eighth, eighth);

    arr += fourth;
    fy_shuffle(arr + eighth, eighth);
}

static void print_tests_count(int t_passed, int t_count, const char *msg);
static void measure_print(float *measures, int s_count, int t_count,
                          int f_count);
static void measure_sorts(int *ts_arr, int *qs_arr, int s, float *measures,
                          int *m_i, const char *message);

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
    /* merge tests */
    {
        int is_print = 1;
        int t_num = 1;
        int t_passed = 0;

        /* full size */
        int fs = 1000000;
        int hs = fs / 2;
        int hhs = hs / 2;
        int es = sizeof(int);

        int *arr = alloc_arri(fs);
        /* for test with swap l to r */
        int *arr_r = alloc_arri(fs);
        int *tmp = alloc_arri(fs);
        void *tmp_swap = calloc(1, es);

        /* small test */
        int small_fs = 100;
        int small_hs = small_fs / 2;

        if (!tmp_swap)
            abort();

        for (int k = 1; k < 10; ++k) {
            iota(arr, small_fs, 1);

            for (int i = k, j = 0; i < small_hs; ++i, ++j) {
                int tmp;
                SWP(arr, i, small_hs + j);
            }
            assert(arr[0] == 1);
            assert(arr[k] == small_hs + 1);
            assert(arr[small_hs] == 1 + k);
            assert(arr[small_fs - k] == small_fs - k + 1);
            merge_test(arr, arr_r, small_fs, es, tmp, tmp_swap, is_print,
                       &t_num, &t_passed);

        }

        iota(arr, fs, 1);
        assert(arr[0] == 1);
        assert(arr[hhs] == hhs + 1);
        assert(arr[hs] == hs + 1);
        assert(arr[hs + hhs] == hs + hhs + 1);
        merge_test(arr, arr_r, fs, es, tmp, tmp_swap, is_print, &t_num,
                   &t_passed);

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
            merge_test(arr, arr_r, fs, es, tmp, tmp_swap, is_print,
                       &t_num, &t_passed);
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
            merge_test(arr, arr_r, fs, es, tmp, tmp_swap, is_print,
                       &t_num, &t_passed);
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
        merge_test(arr, arr_r, fs, es, tmp, tmp_swap, is_print, &t_num,
                   &t_passed);

        t_num--;

        free(arr);
        free(arr_r);
        free(tmp);
        free(tmp_swap);
        print_tests_count(t_num, t_passed, "merge tests");
    }

    /* unit for stability */
    {
        /* group_size */
        int gs = 2;
        /* list size */
        int ls = 2;
        int t_num = 1;
        int okTests = 0;
        int max = 100;
        int *arr = alloc_arri(max * max * ls);

        for (; gs < max; ++gs) {
            /* groups */
            int g = 2;
            for (; g < max; ++g) {
                int ok = 1;

                int c = 1;
                for (int i = 0; i < gs; ++i) {
                    int i_in_group = i * ls;
                    for (int j = 0; j < g; ++j) {
                        int g_num = j * gs * ls;
                        arr[i_in_group + g_num] = i + 1;
                        arr[i_in_group + g_num + 1] = c++;
                    }
                }

                timsort(arr, g * gs, ls * sizeof(int), intpcmp);

                for (int i = 0; i < gs; ++i) {
                    if (arr[i * ls + 1] != i + 1)
                        ok = 0;
                }

                if (ok) {
                    okTests++;
                } else {
                    printf("Stable %d." RED "FF." RESET "\n", t_num);
                }
                t_num++;
            }
        }
        free(arr);
        t_num--;

        print_tests_count(t_num, okTests, "stability tests");
    }
    /* sorting unit */
    {
        int full_size = 1000000;
        int *ts_arr = alloc_arri(full_size);
        int *qs_arr = alloc_arri(full_size);
        int s = 100;

        /* tests count */
        int t_count = 4;
        /* size tests count */
        int s_count = 0;
        /* function measure count */
        int f_count = 2;
        float *measures;
        /* measure iter */
        int m_i = 0;

        while (s <= full_size) {
            s_count++;
            s *= 10;
        }
        measures = alloc_arrf(t_count * s_count * f_count);
        s = 100;

        while (s <= full_size) {
            gen_fourth_iota(ts_arr, s);
            gen_fourth_iota(qs_arr, s);
            measure_sorts(ts_arr, qs_arr, s, measures, &m_i,
                          "Iota 4 parts. (arr)");

            gen_half_iota(ts_arr, s);
            gen_half_iota(qs_arr, s);
            measure_sorts(ts_arr, qs_arr, s, measures, &m_i,
                          "Iota 2 parts. (arr/)");

            gen_fourth_iota(ts_arr, s);
            shuffle_end_of_fourth(ts_arr, s);
            gen_fourth_iota(qs_arr, s);
            shuffle_end_of_fourth(qs_arr, s);
            measure_sorts(ts_arr, qs_arr, s, measures, &m_i,
                          "Iota 4 parts but end of every parts shuffled. (arr*)");

            fy_shuffle(ts_arr, s);
            fy_shuffle(qs_arr, s);
            measure_sorts(ts_arr, qs_arr, s, measures, &m_i,
                          "All elems shuffled. (arr!)");

            s *= 10;
        }

        free(ts_arr);
        free(qs_arr);

        measure_print(measures, s_count, t_count, f_count);
        free(measures);
    }
}

static void print_tests_count(int t_passed, int t_count, const char *msg)
{
    printf("%d %s passed.\n", t_passed, msg);
    if (t_count - t_passed) {
        printf(RED "%d" RESET " %s FAILED.\n", t_count - t_passed, msg);
    }
}

static void measure_sorts(int *ts_arr, int *qs_arr, int s, float *measures,
                          int *m_i, const char *message)
{
    float ts_s, qs_s;
    int es = sizeof(int);
    ts_s = measure_sort(ts_arr, s, es, intcmp, timsort);
    measures[(*m_i)++] = ts_s;
    qs_s = measure_sort(qs_arr, s, es, intcmp, qsort);
    measures[(*m_i)++] = qs_s;

    if (!is_equal(ts_arr, qs_arr, s)) {
        printf("%s ", message);
        printf(RED "FF\nNot equal\n" RESET);
    }
}

static void measure_print(float *measures, int s_count, int t_count,
                          int f_count)
{
    int size = 100;

    printf("\n");
    printf("arr : 4 equal parts of iota sorts part.\n");
    printf("arr/: 2 equal parts of iota sorts part.\n");
    printf
        ("arr*: 4 equal parts of iota sorts part but end of every parts shuffled.\n");
    printf("arr!: all elems shuffled.\n");
    printf("\n");

    printf("                arr   ");
    printf("     arr/   ");
    printf("     arr*    ");
    printf("     arr!   \n");

    for (int s1 = 0; s1 < s_count; ++s1) {
        int size_index = s1 * t_count * f_count;
        int test_index;
        if (s1 != 0) {
            size *= 10;
        }

        printf("%d\n", size);

        printf("       ts:    ");
        for (int test = 0; test < t_count; ++test) {
            test_index = f_count * test;
            printf("%f    ", measures[size_index + test_index]);
        }
        printf("\n");

        printf("       qs:    ");
        for (int test = 0; test < t_count; ++test) {
            test_index = f_count * test;
            printf("%f    ", measures[size_index + test_index + 1]);
        }
        printf("\n");
    }
}
#endif
