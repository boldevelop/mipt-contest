#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "timsort.h"

#define GALLOP_MODE 7

#define IFL(x, y) if (state->less((const void*)(x), (const void*)(y)))

#ifndef TEST
#define MAX_STACK_SIZE 85

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

struct ts_state_t {
    Less less;
    Stack st;
};
typedef struct ts_state_t TsState;
#endif

static int get_minrun(int n)
{
    int r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

static uintptr_t *alloc_arruip(const int n)
{
    uintptr_t *buf;
    buf = calloc(n, sizeof(uintptr_t));
    if (!buf)
        abort();
    return buf;
}

static void swap(uintptr_t * d, int l, int r)
{
    uintptr_t t = d[l];
    d[l] = d[r];
    d[r] = t;
}

static void reverse(uintptr_t * d, int st, int end)
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
static void find_run(TsState * state, uintptr_t * data, int b, int end, int *rn)
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

/**
 * stup: start unsorted position
 */
static void insertion_sort(TsState * state, uintptr_t * data, int b, int end,
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

static int gallop(TsState * state, uintptr_t * data, int s, uintptr_t k)
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
static void merge_stack(TsState * state)
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

static void collapse_stack(TsState * state)
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
 * timsort for jagged array
 * d - pointer to begin data
 * s - elems count
 * less - funct return 1 if a < b
 */
void timsort(uintptr_t * d, const int s, Less less)
{
    int remains = s;
    int st = 0;
    int minrun = get_minrun(s);
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

#undef IFL