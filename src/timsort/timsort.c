#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "timsort.h"

typedef unsigned char uchar;

#define GALLOP_MODE 7
#define MAX_STACK_SIZE 85

struct run_t {
    uchar *d;
    int s;
};
typedef struct run_t Run;

struct stack_t {
    Run d[MAX_STACK_SIZE];
    int s;
};
typedef struct stack_t Stack;

struct ts_state_t {
    Comparator cmp;
    Stack st;
    int es;
    uchar *base;
    uchar *tmp;
    uchar *tmp_swap;
};
typedef struct ts_state_t TsState;

#define Get(state, i) state->base + (i) * state->es

static int get_minrun(int n)
{
    int r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

static uchar *alloc_arruc(int n, int es)
{
    uchar *buf;
    buf = calloc(n, es);
    if (!buf)
        abort();
    return buf;
}

static void memswap(void *lhs, void *rhs, size_t size, void *tmp)
{
    memcpy(tmp, rhs, size);
    memcpy(rhs, lhs, size);
    memcpy(lhs, tmp, size);
}

static void reverse(TsState * state, int l, int r)
{
    r--;
    while (l < r) {
        memswap(Get(state, l), Get(state, r), state->es, state->tmp_swap);
        ++l;
        --r;
    }
}

/* a1 > a2 > */
static int move_on_desc_run(TsState * state, int l, int r, int *rn)
{
    while (l < r) {
        if (state->cmp(Get(state, l), Get(state, l - 1)) < 0) {
            (*rn)++;
            l++;
            continue;
        }
        break;
    }
    return l;
}

/* a1 <= a2 <= */
static void move_on_asc_run(TsState * state, int l, int r, int *rn)
{
    while (l < r) {
        if (state->cmp(Get(state, l), Get(state, l - 1)) >= 0) {
            (*rn)++;
            l++;
            continue;
        }
        break;
    }
}

/**
 * find  a1 <= a2 <= ... OR a1 > a2 > ...
 * l: begin positon
 * r: end of array
 * rn: count run
*/
static void find_run(TsState * state, int l, int r, int *rn)
{
    /* desc ordering */
    int desc = 0;
    /* preserve begin position */
    int l_saved = l;

    if (l == r) {
        *rn = 0;
        return;
    }
    *rn = 1;
    l++;

    if (l == r) {
        return;
    }

    desc = state->cmp(Get(state, l), Get(state, l - 1)) < 0;
    l++;
    (*rn)++;

    if (desc) {
        l = move_on_desc_run(state, l, r, rn);
        reverse(state, l_saved, l);
    } else {
        move_on_asc_run(state, l, r, rn);
    }
}

/**
 * stup: start unsorted position
 */
static void insertion_sort(TsState * state, int l, int r, int stup)
{
    for (int i = stup; i < r; ++i) {
        int p = i;              /* prev j index */

        for (int j = i - 1; j >= l; --j) {
            uchar *left = Get(state, p);
            uchar *right = Get(state, j);
            if (state->cmp(left, right) < 0) {
                memswap(left, right, state->es, state->tmp_swap);
                p--;
            } else {
                break;
            }
        }
    }
}

static int gallop(uchar * data, int s, int es, Comparator cmp, uchar * k) {
    int i = 1;
    if (s == 0) {
        return 0;
    }

    while (i < s) {
        if (cmp(data + (i - 1) * es, k) < 0) {
            i <<= 1;
            assert(i >= 0);
        } else {
            break;
        }
    }

    if (i >= s) {
        i = s - 1;
    }

    while (i > 0) {
        if (cmp(data + i * es, k) < 0) {
            i++;
            break;
        }
        i--;
    }

    return i;
}

/* merge of
    data: [. . . . . . . . . .]
           |<- ls -> |<- rs ->|
 */
void galop_merge(uchar * data, int ls, int rs, int es, Comparator cmp,
                 uchar * tmp, uchar * tmp_swap)
{
    uchar *l = data;
    uchar *r = data + ls * es;
    uchar *outptr, *lptr, *rptr;
    int lp = 0, rp = 0, out = 0;
    int r_copy_count = 0;       /* count copy from the right */
    int l_copy_count = 0;       /* count copy from the left */
    int g = 0;
    assert(tmp);
    assert(tmp_swap);
    assert(data);
    memcpy(tmp, l, ls * es);

    while (lp < ls || rp < rs) {
        outptr = l + out * es;
        lptr = tmp + lp * es;
        rptr = r + rp * es;

        if (lp == ls) {
            break;
        }

        if (rp == rs) {
            memcpy(outptr, lptr, (ls - lp) * es);
            break;
        }

        if (r_copy_count > GALLOP_MODE) {
            r_copy_count = 0;
            g = gallop(rptr, rs - rp, es, cmp, lptr);
            if (g > 0) {
                memmove(outptr, rptr, g * es);
                rp += g;
                out += g;
            }
            continue;
        }

        if (l_copy_count > GALLOP_MODE) {
            l_copy_count = 0;
            g = gallop(lptr, ls - lp, es, cmp, rptr);
            if (g > 0) {
                memcpy(outptr, lptr, g * es);
                lp += g;
                out += g;
            }
            continue;
        }

        if (cmp(rptr, lptr) < 0) {
            memswap(outptr, rptr, es, tmp_swap);
            out++;
            rp++;

            r_copy_count++;
            l_copy_count = 0;
        } else {
            memswap(outptr, lptr, es, tmp_swap);
            out++;
            lp++;

            l_copy_count++;
            r_copy_count = 0;
        }
    }
}

/* merging top 2 stack elements */
void merge(TsState * state)
{
    Run *l, *r;                 /* left stack item (top - 1), right stack item (top) */
    int ls, rs;                 /* left size, right size */
    int es = state->es;
    assert(state);

    l = state->st.d + state->st.s - 2;
    r = state->st.d + state->st.s - 1;
    ls = l->s;
    rs = r->s;

    galop_merge(l->d, ls, rs, es, state->cmp, state->tmp, state->tmp_swap);
    state->st.s--;
    l->s = ls + rs;
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
void timsort(void *const d, int s, int es, Comparator cmp)
{
    int remains = s;
    int st = 0;
    int minrun = get_minrun(s);
    TsState state = { 0 };
    state.cmp = cmp;
    state.es = es;
    state.base = d;
    state.tmp = alloc_arruc(s, es);
    state.tmp_swap = calloc(1, es);
    if (!state.tmp_swap) {
        abort();
    }

    do {
        int rn = 0;             /* natural run length */
        int rrl;                /* run remains length */
        find_run(&state, st, s, &rn);

        if (rn < minrun) {
            rrl = remains > minrun ? minrun : remains;
            insertion_sort(&state, st, st + rrl, st + rn);
            rn = rrl;
        }

        assert(state.st.s < MAX_STACK_SIZE);
        state.st.d[state.st.s] = (Run) {.d = (uchar *) d + st * es,.s = rn};
        state.st.s++;

        merge_stack(&state);

        st += rn;
        remains -= rn;

    } while (remains > 0);

    collapse_stack(&state);

    free(state.tmp);
    free(state.tmp_swap);
}

#undef Get
