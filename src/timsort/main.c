#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

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

#define IFL(x, y) if ((x) < (y))

void swap(int *d, int l, int r)
{
    int t = d[l];
    d[l] = d[r];
    d[r] = t;
}

void reverse(int *d, int st, int end)
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
void find_run(int *d, int b, int end, int *rn)
{
    int desc = 0;
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

    IFL(d[b], d[b - 1]) {
        desc = 1;
    }
    b++;
    (*rn)++;
    if (desc) {
        for (; b < end; ++b) {
            IFL(d[b], d[b - 1]) {
                (*rn)++;
            }
            else {
                break;
            }
        }
    } else {
        for (; b < end; ++b) {
            IFL(d[b], d[b - 1]) {
                break;
            }
            else {
                (*rn)++;
            }
        }
    }
    if (desc) {
        reverse(d, b_saved, b);
    }
}

void da(int *d, int b, int e)
{
    for (; b < e; ++b)
        printf("%d ", d[b]);
    printf("\n");
}

/**
 * stup: start unsorted position
 */
void insertion_sort(int *d, int b, int end, int stup)
{
    for (int i = stup; i < end; ++i) {
        int p = i;              /* prev j index */

        for (int j = i - 1; j >= b; --j) {
            IFL(d[p], d[j]) {
                swap(d, j, p);
                p--;
            } else {
                break;
            }
        }
    }

}

struct run_t {
    int *d;
    int s;
};
typedef struct run_t Run;

struct stack_t {
    Run d[MAX_STACK_SIZE];
    int s;
};
typedef struct stack_t Stack;

void dstack(Stack * st)
{
    printf("Stack:\n");
    for (int i = st->s - 1; i >= 0; --i) {
        Run *r = st->d + i;
        printf("%d: ", r->s);
        da(r->d, 0, r->s);
    }
}

int gallop(int* d, int s, int k) {
    int i = 0;
    int last = s - 1;

    if (s < 1) {
        return 0;
    }

    IFL(d[i], k) {
        i = 2;
        if (i > last) {
            i = last;
        }
    } else {
        /* unlucky gallop */
        return 0;
    }

    for (;;) {
        IFL(d[i], k) {
            if (i == last) {
                break;
            }
            i <<= 1; 
            assert(i >= 0);
        } else {
            break;
        }
        if (i > last) {
            i = last;
        }
    }

    /* go_back */
    for (;;) {
        IFL(d[i], k) {
            break;
        }
        i--;
    }

    return i + 1;
}

/* merging top 2 stack elements */
void merge(Stack * st)
{
    Run *l, *r; /* left stack item (top - 1), right stack item (top) */
    int *tmp;
    /* left position, right position, out position */
    int lp = 0, rp = 0, out = 0; 
    int ls, rs; /* left size, right size */
    int r_copy_count = 0; /* count copy from the right */
    int l_copy_count = 0; /* count copy from the left */
    int g = 0; /* gallop count */
    assert(st);

    l = st->d + st->s - 2;
    r = st->d + st->s - 1;
    ls = l->s;
    rs = r->s;
    tmp = alloc_arri(ls);
    memcpy(tmp, l->d, sizeof(int) * ls);

    while (lp < ls || rp < rs) {
        if (lp == ls) {
            break;
        }

        if (rp == rs) {
            memcpy(l->d + out, tmp + lp, sizeof(int) * (ls - lp));
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

        g = 0;
        if (r_copy_count > GALLOP_MODE) {
            r_copy_count = 0;
            g = gallop(r->d + rp, rs - rp, tmp[lp]);
            if (g > 0) {
                memmove(l->d + out, r->d + rp, sizeof(int) * g);
                rp += g;
                out += g;
            }
            continue;
        }

        if (l_copy_count > GALLOP_MODE) {
            l_copy_count = 0;
            g = gallop(tmp + lp, ls - lp, r->d[rp]);
            if (g > 0) {
                memcpy(l->d + out, tmp + lp, sizeof(int) * g);
                lp += g;
                out += g;
            }
            continue;
        }
    }

    st->s--;
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
void merge_stack(Stack * st)
{
    Run *runs;
    assert(st);
    runs = st->d;

    if (st->s < 2) {
        return;
    }

    while (st->s > 1) {
        int s = st->s;
        int is_Z_less;
        Run *X = runs + s - 1;
        Run *Y = runs + s - 2;
        Run *Z = runs + s - 3;
        if (s == 2) {
            if (X->s < Y->s) {
                break;
            }
            merge(st);
            continue;
        }

        if (Z->s > X->s + Y->s && Y->s > X->s) {
            break;
        }

        is_Z_less = Z->s < X->s;
        if (is_Z_less) {
            st->s--;
        }
        merge(st);
        if (is_Z_less) {
            st->s++;
            /* after merge Y with Z. [..., YZ, invalid, X] */
            st->d[st->s - 1] = st->d[st->s];
        }

    }
}

void collapse_stack(Stack * st)
{
    while (st->s > 1) {
        int n = st->s - 2;
        int is_Z_less = n > 0 && st->d[n - 1].s < st->d[n + 1].s;
        if (is_Z_less) {
            st->s--;
        }
        merge(st);
        if (is_Z_less) {
            st->s++;
            /* after merge Y with Z. [..., YZ, invalid, X] */
            st->d[st->s - 1] = st->d[st->s];
        }
    }
}

void ts(int *d, const int s)
{
    int remains = s;
    int st = 0;
    int minrun = get_minrun(s);
    Stack stack = { 0 };

    do {
        int rn = 0;             /* natural run length */
        int rrl;                /* run remains length */
        find_run(d, st, s, &rn);

        if (rn < minrun) {
            rrl = remains > minrun ? minrun : remains;
            insertion_sort(d, st, st + rrl, st + rn);
            rn = rrl;
        }

        assert(stack.s < MAX_STACK_SIZE);
        stack.d[stack.s] = (Run) {
        .d = d + st,.s = rn};
        stack.s++;

        merge_stack(&stack);

        st += rn;
        remains -= rn;

    } while (remains > 0);

    collapse_stack(&stack);
}

/* fy */
#define SWP(arr, i, j) tmp = (arr)[(i)];\
    (arr)[(i)] = (arr)[(j)];\
    (arr)[(j)] = tmp;

void fy_shuffle(int *arr, const int s)
{
    int i = s;
    int j, tmp;
    assert(arr);
    while (i-- > 1) {
        j = rand() % (i + 1);
        SWP(arr, j, i);
    }
}

/* fy end */
void iota(int *a, const int s, int val)
{
    assert(a);
    for (int i = 0; i < s; ++i) {
        a[i] = val++;
    }
}

int is_equal(int const *a, int const *b, int s)
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
    const int *x = a;
    const int *y = b;
    return *x - *y;
}

float measure_ts(int *arr, const int s)
{
    clock_t start, end;
    float seconds;
    start = clock();
    ts(arr, s);
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

float measure_merge(Stack * st)
{
    clock_t start, end;
    float seconds;
    start = clock();
    merge(st);
    end = clock();
    seconds = (float) (end - start) / CLOCKS_PER_SEC;
    return seconds;
}

void stack_test(int *arr, int *r_arr, const int s, int *t_num)
{
    int hs = s / 2;
    Stack st = { 0 };
    Run *res;
    int ok = 1;
    float secs;

    for (int i = 0; i < s; ++i) {
        r_arr[i] = arr[i];
    }

    st.s = 2;
    st.d[0] = (Run) {
    .d = arr,.s = hs};
    st.d[1] = (Run) {
    .d = arr + hs,.s = hs};
    secs = measure_merge(&st);
    res = st.d;
    for (int i = 0; i < res->s; ++i) {
        if (res->d[i] != i + 1)
            ok = 0;
    }

    printf("Stack %d. ", (*t_num)++);
    if (ok) {
        printf("Ok. %fs ", secs);
    } else {
        printf(RED "FF. %fs " RESET, secs);
        printf("\n");
        for (int i = 0; i < res->s; ++i) {
            printf("%d ", res->d[i]);
        }
        printf("\n");
    }
    printf("\n");

    st.s = 2;
    st.d[0] = (Run) {
    .d = r_arr,.s = hs};
    st.d[1] = (Run) {
    .d = r_arr + hs,.s = hs};
    secs = measure_merge(&st);
    res = st.d;
    for (int i = 0; i < res->s; ++i) {
        if (res->d[i] != i + 1)
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

/* TODO: add cmp and void** interface */
int main()
{
    int rn = 300, n = 2;

    int d[20] = {
        50, 49, 48, 47, 46, /*  */ 60, 20, 25, 30, 70,
        35, 36, 37, 38, 39, /*  */ 5, 40, 45, 47, 2,
    };
    records_t r1 = init_records(rn, n);

    ts(d, 20);
    da(d, 0, 20);
    free_records(r1);

    /* stack unit */
    {
        int fs = 100;
        int hs = fs / 2;
        int *arr = alloc_arri(fs);
        /* for test with swap l to r */
        int *arr_r = alloc_arri(fs);
        int t_num = 1;
        // iota(arr, fs, 1);
        // stack_test(arr, arr_r, fs, &t_num);

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
            stack_test(arr, arr_r, fs, &t_num);
        }
        free(arr);
        free(arr_r);
    }
    // return 0;
    {
        /* full size */
        int fs = 1000000;
        int hs = fs / 2;
        int hhs = hs / 2;
        int *arr = alloc_arri(fs);
        /* for test with swap l to r */
        int *arr_r = alloc_arri(fs);
        int t_num = 1;

        iota(arr, fs, 1);
        assert(arr[0] == 1);
        assert(arr[hhs] == hhs + 1);
        assert(arr[hs] == hs + 1);
        assert(arr[hs + hhs] == hs + hhs + 1);
        stack_test(arr, arr_r, fs, &t_num);

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
            stack_test(arr, arr_r, fs, &t_num);
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
            stack_test(arr, arr_r, fs, &t_num);
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
        stack_test(arr, arr_r, fs, &t_num);

        free(arr);
        free(arr_r);
    }
    // return 0;
    /* sorting unit */
    {
        int full_size = 1000000;
        int *ts_arr = alloc_arri(full_size);
        int *qs_arr = alloc_arri(full_size);
        int s = 10;
        int fourth;
        int half;
        float ts_s, qs_s;

        iota(ts_arr, full_size, 1);
        iota(qs_arr, full_size, 1);

        while (s <= full_size) {
            fourth = s / 4;
            half = s /2;
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
                int *b = ts_arr;
                int half_f = fourth/2;

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
