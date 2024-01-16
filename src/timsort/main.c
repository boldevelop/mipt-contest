#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define RED     "\033[31m"
#define RESET   "\033[0m"

#define MAX_STACK_SIZE 85
#define GALLOP_MODE 7

int get_minrun(int n)
{
    int r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

struct Records {
    int **r;
    int rn;                     /* recorords (r) count */
    int n;                      /* elems in (r) count */
};
typedef struct Records records_t;

int *alloc_arri(int n)
{
    int *buf;
    buf = calloc(n, sizeof(int));
    if (!buf)
        abort();
    return buf;
}

uintptr_t *alloc_arruip(const int n)
{
    uintptr_t *buf;
    buf = calloc(n, sizeof(uintptr_t));
    if (!buf)
        abort();
    return buf;
}

int **alloc_arrpi(int rn)
{
    int **buf;
    buf = calloc(rn, sizeof(int *));
    if (!buf)
        abort();
    return buf;
}

records_t init_records(int rn, int n)
{
    records_t r;
    r.rn = rn;
    r.n = n;
    r.r = alloc_arrpi(r.rn);
    for (int i = 0; i < r.rn; ++i) {
        r.r[i] = alloc_arri(r.n);
    }
    return r;
}

void free_records(const records_t r)
{
    for (int i = 0; i < r.rn; ++i) {
        free(r.r[i]);
    }
    free(r.r);
}

void dump_r(const records_t r)
{
    for (int i = 0; i < r.rn; ++i) {
        for (int j = 0; j < r.n; ++j)
            printf("%d ", r.r[i][j]);
        printf("\n");
    }
}

void fill_r(records_t r)
{
    // int minrun = get_minrun(r.rn);

    for (int i = 0; i < r.rn; ++i) {
        for (int j = 0; j < r.n; ++j) {
            r.r[i][j] = (i + 1) + j * 10;
        }
    }
}

#define IFL(x, y) if (state->less((const void*)(x), (const void*)(y)))
typedef unsigned char uchar;

struct run_t {
    uintptr_t *d;
    int s;
};
typedef struct run_t Run;

struct stack_t {
    Run d[MAX_STACK_SIZE];
    int s;
};
typedef struct stack_t Stack;

typedef int (*Less)(const void *l, const void *r);

struct ts_state_t {
    Less less;
    Stack st;
};
typedef struct ts_state_t TsState;

#define next(voidPtr, es) (voidPtr) = (uchar*)(voidPtr) + (es);
#define nTh(voidPtr, pos) (uchar*)(voidPtr) + state->es * (pos)

void swap(uintptr_t * d, int l, int r)
{
    uintptr_t t = d[l];
    d[l] = d[r];
    d[r] = t;
}

void reverse(uintptr_t * d, int st, int end)
{
    end--;
    while (st < end) {
        swap(d, st, end);
        ++st;
        --end;
    }
}

/**
 * find  a1 <= a2 <= ... OR a1 > a2 > ...
 * b: begin positon
 * end: end of array
 * rn: count run
*/
void find_run(TsState * state, uintptr_t * data, int b, int end, int *rn)
{
    /* desc ordering */
    int desc = 0;
    /* preserve begin position */
    int b_saved = b;

    if (b == end) {
        *rn = 0;
        return;
    }
    *rn = 1;
    b++;

    if (b == end) {
        return;
    }

    IFL(data[b], data[b - 1]) {
        desc = 1;
    }

    b++;
    (*rn)++;

    if (desc) {
        for (; b < end; ++b) {
            IFL(data[b], data[b - 1]) {
                (*rn)++;
            }
            else {
                break;
            }
        }
    } else {
        for (; b < end; ++b) {
            IFL(data[b], data[b - 1]) {
                break;
            }
            else {
                (*rn)++;
            }
        }
    }
    if (desc) {
        reverse(data, b_saved, b);
    }
}

void da(uintptr_t * d, int b, int e)
{
    for (; b < e; ++b)
        printf("%ld ", d[b]);
    printf("\n");
}

/**
 * stup: start unsorted position
 */
void insertion_sort(TsState * state, uintptr_t * data, int b, int end,
                    int stup)
{
    for (int i = stup; i < end; ++i) {
        int p = i;              /* prev j index */

        for (int j = i - 1; j >= b; --j) {
            IFL(data[p], data[j]) {
                swap(data, j, p);
                p--;
            } else {
                break;
            }
        }
    }

}


void dstack(Stack * st)
{
    printf("Stack:\n");
    for (int i = st->s - 1; i >= 0; --i) {
        Run *r = st->d + i;
        printf("%d: ", r->s);
        da(r->d, 0, r->s);
    }
}

int gallop(TsState * state, uintptr_t * data, int s, uintptr_t k)
{
    int i = 0;
    int last = s - 1;

    if (s < 1) {
        return 0;
    }

    IFL(data[i], k) {
        i = 2;
        if (i > last) {
            i = last;
        }
    }
    else {
        /* unlucky gallop */
        return 0;
    }

    for (;;) {
        IFL(data[i], k) {
            if (i == last) {
                break;
            }
            i <<= 1;
            assert(i >= 0);
        }
        else {
            break;
        }
        if (i > last) {
            i = last;
        }
    }

    /* go_back */
    for (;;) {
        IFL(data[i], k) {
            break;
        }
        i--;
    }

    return i + 1;
}

/* merging top 2 stack elements */
void merge(TsState * state)
{
    Run *l, *r;                 /* left stack item (top - 1), right stack item (top) */
    uintptr_t *tmp;
    /* left position, right position, out position */
    int lp = 0, rp = 0, out = 0;
    int ls, rs;                 /* left size, right size */
    int r_copy_count = 0;       /* count copy from the right */
    int l_copy_count = 0;       /* count copy from the left */
    int g = 0;                  /* gallop count */
    assert(state);

    l = state->st.d + state->st.s - 2;
    r = state->st.d + state->st.s - 1;
    ls = l->s;
    rs = r->s;
    tmp = alloc_arruip(ls);
    memcpy(tmp, l->d, sizeof(uintptr_t) * ls);

    while (lp < ls || rp < rs) {
        if (lp == ls) {
            break;
        }

        if (rp == rs) {
            memcpy(l->d + out, tmp + lp, sizeof(uintptr_t) * (ls - lp));
            break;
        }

        IFL(r->d[rp], tmp[lp]) {
            l->d[out++] = r->d[rp++];
            r_copy_count++;
            l_copy_count = 0;
        }
        else {
            l->d[out++] = tmp[lp++];
            l_copy_count++;
            r_copy_count = 0;
        }

        if (r_copy_count > GALLOP_MODE) {
            r_copy_count = 0;
            g = gallop(state, r->d + rp, rs - rp, tmp[lp]);
            if (g > 0) {
                memmove(l->d + out, r->d + rp, sizeof(uintptr_t) * g);
                rp += g;
                out += g;
            }
            continue;
        }

        if (l_copy_count > GALLOP_MODE) {
            l_copy_count = 0;
            g = gallop(state, tmp + lp, ls - lp, r->d[rp]);
            if (g > 0) {
                memcpy(l->d + out, tmp + lp, sizeof(uintptr_t) * g);
                lp += g;
                out += g;
            }
            continue;
        }
    }

    state->st.s--;
    l->s = ls + rs;
    free(tmp);
}

/**
 * Stack:
 *  X
 *  Y
 *  Z
 * size is more 2 Z>=X+Y merge Y with min(X,Z)
 * size is more 1 Y<=X merge X with Y 
 * merging while (Z>X+Y && Y > X) falsy
 */
void merge_stack(TsState * state)
{
    Run *runs;
    assert(state);
    runs = state->st.d;

    if (state->st.s < 2) {
        return;
    }

    while (state->st.s > 1) {
        int s = state->st.s;
        int is_Z_less;
        Run *X = runs + s - 1;
        Run *Y = runs + s - 2;
        Run *Z = runs + s - 3;
        if (s == 2) {
            if (X->s < Y->s) {
                break;
            }
            merge(state);
            continue;
        }

        if (Z->s > X->s + Y->s && Y->s > X->s) {
            break;
        }

        is_Z_less = Z->s < X->s;
        if (is_Z_less) {
            state->st.s--;
        }
        merge(state);
        if (is_Z_less) {
            state->st.s++;
            /* after merge Y with Z. [..., YZ, invalid, X] */
            state->st.d[state->st.s - 1] = state->st.d[state->st.s];
        }

    }
}

void collapse_stack(TsState * state)
{
    while (state->st.s > 1) {
        int n = state->st.s - 2;
        int is_Z_less = n > 0
            && state->st.d[n - 1].s < state->st.d[n + 1].s;
        if (is_Z_less) {
            state->st.s--;
        }
        merge(state);
        if (is_Z_less) {
            state->st.s++;
            /* after merge Y with Z. [..., YZ, invalid, X] */
            state->st.d[state->st.s - 1] = state->st.d[state->st.s];
        }
    }
}

/**
 * d - pointer to begin data
 * s - elems count
 * es - elem size
 * less - funct return 1 if a < b
 */
void ts(uintptr_t * d, const int s, Less less)
{
    int remains = s;
    int st = 0;
    int minrun = get_minrun(s);
    // Stack stack = { 0 };
    TsState state = { 0 };
    state.less = less;

    do {
        int rn = 0;             /* natural run length */
        int rrl;                /* run remains length */
        find_run(&state, d, st, s, &rn);

        if (rn < minrun) {
            rrl = remains > minrun ? minrun : remains;
            insertion_sort(&state, d, st, st + rrl, st + rn);
            rn = rrl;
        }

        assert(state.st.s < MAX_STACK_SIZE);
        state.st.d[state.st.s] = (Run) {
        .d = d + st,.s = rn};
        state.st.s++;

        merge_stack(&state);

        st += rn;
        remains -= rn;

    } while (remains > 0);

    collapse_stack(&state);
}

/* fy */
#define SWP(arr, i, j) tmp = (arr)[(i)];\
    (arr)[(i)] = (arr)[(j)];\
    (arr)[(j)] = tmp;

void fy_shuffle(uintptr_t * arr, const int s)
{
    int i = s;
    uintptr_t j, tmp;
    assert(arr);
    while (i-- > 1) {
        j = rand() % (i + 1);
        SWP(arr, j, i);
    }
}

/* fy end */
void iota(uintptr_t * a, const int s, int val)
{
    assert(a);
    for (int i = 0; i < s; ++i) {
        a[i] = val++;
    }
}

int is_equal(const uintptr_t * a, const uintptr_t * b, int s)
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

float measure_ts(uintptr_t * arr, const int s)
{
    clock_t start, end;
    float seconds;
    start = clock();
    ts(arr, s, lessint);
    end = clock();
    seconds = (float) (end - start) / CLOCKS_PER_SEC;
    return seconds;
}

float measure_qs(uintptr_t * arr, const int s)
{
    clock_t start, end;
    float seconds;
    start = clock();
    qsort(arr, s, sizeof(uintptr_t), intcmp);
    end = clock();
    seconds = (float) (end - start) / CLOCKS_PER_SEC;
    return seconds;
}

float measure_merge(TsState * st)
{
    clock_t start, end;
    float seconds;
    start = clock();
    merge(st);
    end = clock();
    seconds = (float) (end - start) / CLOCKS_PER_SEC;
    return seconds;
}

void stack_test(uintptr_t * arr, uintptr_t * r_arr, const int s,
                int *t_num)
{
    int hs = s / 2;
    TsState state = { 0 };
    Run *res;
    int ok = 1;
    float secs;
    state.less = lessint;

    for (int i = 0; i < s; ++i) {
        r_arr[i] = arr[i];
    }

    state.st.s = 2;
    state.st.d[0] = (Run) {
    .d = arr,.s = hs};
    state.st.d[1] = (Run) {
    .d = arr + hs,.s = hs};
    secs = measure_merge(&state);
    res = state.st.d;
    for (int i = 0; i < res->s; ++i) {
        if (res->d[i] != (uintptr_t) (i + 1))
            ok = 0;
    }

    printf("Stack %d. ", (*t_num)++);
    if (ok) {
        printf("Ok. %fs ", secs);
    } else {
        printf(RED "FF. %fs " RESET, secs);
        printf("\n");
        for (int i = 0; i < res->s; ++i) {
            printf("%ld ", res->d[i]);
        }
        printf("\n");
    }
    printf("\n");

    state.st.s = 2;
    state.st.d[0] = (Run) {
    .d = r_arr,.s = hs};
    state.st.d[1] = (Run) {
    .d = r_arr + hs,.s = hs};
    secs = measure_merge(&state);
    res = state.st.d;
    for (int i = 0; i < res->s; ++i) {
        if (res->d[i] != (uintptr_t) (i + 1))
            ok = 0;
    }

    printf("Stack %d. ", (*t_num)++);
    if (ok) {
        printf("Ok. %fs ", secs);
    } else {
        printf(RED "FF. %fs " RESET, secs);
    }
    printf("(r)\n");
}

int main()
{
    /* stack unit */
    {
        int fs = 100;
        int hs = fs / 2;
        uintptr_t *arr = alloc_arruip(fs);
        /* for test with swap l to r */
        uintptr_t *arr_r = alloc_arruip(fs);
        int t_num = 1;
        // iota(arr, fs, 1);
        // stack_test(arr, arr_r, fs, &t_num);

        for (int k = 1; k < 10; ++k) {
            iota(arr, fs, 1);

            for (int i = k, j = 0; i < hs; ++i, ++j) {
                uintptr_t tmp;
                SWP(arr, i, hs + j);
            }
            assert((int) arr[0] == 1);
            assert((int) arr[k] == hs + 1);
            assert((int) arr[hs] == 1 + k);
            assert((int) arr[fs - k] == fs - k + 1);
            stack_test(arr, arr_r, fs, &t_num);
        }
        free(arr);
        free(arr_r);
    }

    /* unit for stable */
    {
        /* group_size */
        uintptr_t gs = 2;
        /* groups */
        /* list size */
        int ls = 2;
        int t_num = 1;
        for (; gs < 100; ++gs) {
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

                ts((uintptr_t *) arr, s, lessintp);

                for (uintptr_t i = 0; i < s; ++i) {
                    if (arr[i][1] != (uintptr_t) (i + 1))
                        ok = 0;
                }

                printf("Stable %d. ", t_num++);
                if (ok) {
                    printf("Ok.\n");
                } else {
                    printf(RED "FF." RESET "\n");
                }

                for (uintptr_t i = 0; i < s; ++i) {
                    free(arr[i]);
                }
                free(arr);
            }
        }
    }

    {
        /* full size */
        int fs = 1000000;
        int hs = fs / 2;
        int hhs = hs / 2;
        uintptr_t *arr = alloc_arruip(fs);
        /* for test with swap l to r */
        uintptr_t *arr_r = alloc_arruip(fs);
        int t_num = 1;

        iota(arr, fs, 1);
        assert((int) arr[0] == 1);
        assert((int) arr[hhs] == hhs + 1);
        assert((int) arr[hs] == hs + 1);
        assert((int) arr[hs + hhs] == hs + hhs + 1);
        stack_test(arr, arr_r, fs, &t_num);

        for (int k = 1; k < 10; ++k) {
            iota(arr, fs, 1);

            for (int i = k, j = 0; i < hs; ++i, ++j) {
                uintptr_t tmp;
                SWP(arr, i, hs + j);
            }
            assert((int) arr[0] == 1);
            assert((int) arr[k] == hs + 1);
            assert((int) arr[hs] == 1 + k);
            assert((int) arr[fs - k] == fs - k + 1);
            stack_test(arr, arr_r, fs, &t_num);
        }

        for (int k = hs - 10; k < hs; ++k) {
            iota(arr, fs, 1);

            for (int i = k, j = 0; i < hs; ++i, ++j) {
                uintptr_t tmp;
                SWP(arr, i, hs + j);
            }
            assert((int) arr[0] == 1);
            assert((int) arr[k] == hs + 1);
            assert((int) arr[hs] == 1 + k);
            assert((int) arr[fs - k] == fs - k + 1);
            stack_test(arr, arr_r, fs, &t_num);
        }

        iota(arr, fs, 1);
        for (int i = hhs; i < hs; ++i) {
            uintptr_t tmp;
            SWP(arr, i, i + hhs);
        }
        assert((int) arr[0] == 1);
        assert((int) arr[hhs] == hs + 1);
        assert((int) arr[hs] == hhs + 1);
        assert((int) arr[hs + hhs] == hs + hhs + 1);
        stack_test(arr, arr_r, fs, &t_num);

        free(arr);
        free(arr_r);
    }

    /* sorting unit */
    /* make stress for 1000000 */
    {
        int full_size = 1000000;
        uintptr_t *ts_arr = alloc_arruip(full_size);
        uintptr_t *qs_arr = alloc_arruip(full_size);
        int s = 10;
        int fourth;
        int half;
        float ts_s, qs_s;

        iota(ts_arr, full_size, 1);
        iota(qs_arr, full_size, 1);

        while (s <= full_size) {
            fourth = s / 4;
            half = s / 2;
            printf("size: %d.\n", s);
            fy_shuffle(ts_arr, s);
            fy_shuffle(qs_arr, s);

            ts_s = measure_ts(ts_arr, s);
            qs_s = measure_qs(qs_arr, s);

            printf("Shuffled all. ");
            if (!is_equal(ts_arr, qs_arr, s)) {
                printf(RED "FF\nNot equal\n" RESET);
            } else {
                printf("OK\n");
                printf("ts: %fs\n", ts_s);
                printf("qs: %fs\n", qs_s);
            }

            iota(ts_arr, half, 1);
            iota(qs_arr, half, 1);
            iota(ts_arr + half, half, 1);
            iota(qs_arr + half, half, 1);

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
                uintptr_t *b = ts_arr;
                int half_f = fourth / 2;

                iota(b, fourth, fourth);
                b += fourth;
                iota(b, fourth, fourth * 2);
                b += fourth;
                iota(b, fourth, fourth * 3);
                b += fourth;
                iota(b, fourth, 1);

                b = qs_arr;
                iota(b, fourth, fourth);
                b += fourth;
                iota(b, fourth, fourth * 2);
                b += fourth;
                iota(b, fourth, fourth * 3);
                b += fourth;
                iota(b, fourth, 1);

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

                b = ts_arr;
                iota(b, fourth, fourth);
                fy_shuffle(b + half_f, half_f);
                b += fourth;
                iota(b, fourth, fourth * 2);
                fy_shuffle(b + half_f, half_f);
                b += fourth;
                iota(b, fourth, fourth * 3);
                fy_shuffle(b + half_f, half_f);
                b += fourth;
                iota(b, fourth, 1);
                fy_shuffle(b + half_f, half_f);

                b = qs_arr;
                iota(b, fourth, fourth);
                fy_shuffle(b + half_f, half_f);
                b += fourth;
                iota(b, fourth, fourth * 2);
                fy_shuffle(b + half_f, half_f);
                b += fourth;
                iota(b, fourth, fourth * 3);
                fy_shuffle(b + half_f, half_f);
                b += fourth;
                iota(b, fourth, 1);
                fy_shuffle(b + half_f, half_f);

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
