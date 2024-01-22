#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

#include "timsort.h"

typedef unsigned char uchar;

#define GALLOP_MODE 7

#define IFL(x, y) if (state->less((const void*)(x), (const void*)(y)))

// #ifndef TEST
#define MAX_STACK_SIZE 85

struct run_t {
    uchar* d;
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
    uchar* base;
    uchar* tmp;
};
typedef struct ts_state_t TsState;

#define Get(state, i) state->base + (i) * state->es
// #endif

static int get_minrun(int n)
{
    int r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

// static uintptr_t *alloc_arruip(const int n)
// {
//     uintptr_t *buf;
//     buf = calloc(n, sizeof(uintptr_t));
//     if (!buf)
//         abort();
//     return buf;
// }

static uchar *alloc_arruc(int n, int es)
{
    uchar *buf;
    buf = calloc(n, es);
    if (!buf)
        abort();
    return buf;
}

// static void swap(uintptr_t * d, int l, int r)
// {
//     uintptr_t t = d[l];
//     d[l] = d[r];
//     d[r] = t;
// }

static void cswap(void* l, void* r, int es) {
    uchar* a = l;
    uchar* b = r;
    if (a == b) return;

    do {
        uchar t = *a;
        *a++ = *b;
        *b++ = t;
    } while (--es > 0);
}

static int is_less(TsState* state, int l, int r) {
    return state->cmp(Get(state, l), Get(state, r)) < 0;
}

static void reverse(TsState* state, int l, int r)
{
    r--;
    while (l < r) {
        cswap(Get(state, l), Get(state, r), state->es);
        // swap(d, l, r);
        ++l;
        --r;
    }
}

/**
 * find  a1 <= a2 <= ... OR a1 > a2 > ...
 * b: begin positon
 * end: end of array
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


    // IFL(data[l], data[l - 1]) {
    if (is_less(state, l, l - 1)) {
        desc = 1;
    }

    l++;
    (*rn)++;

    if (desc) {
        /* count_run_until_end */
        for (; l < r; ++l) {
            // IFL(data[l], data[l - 1]) {
            if (is_less(state, l, l - 1)) {
                (*rn)++;
            }
            else {
                break;
            }
        }
    } else {
        for (; l < r; ++l) {
            if (is_less(state, l, l - 1)) {
            // IFL(data[l], data[l - 1]) {
                break;
            }
            else {
                (*rn)++;
            }
        }
    }
    if (desc) {
        reverse(state, l_saved, l);
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
            uchar* left = Get(state, p);
            uchar* right = Get(state, j);
            if (is_less(state, p, j)) {
            // IFL(data[p], data[j]) {
                cswap(left, right, state->es);
                // swap(data, j, p);
                p--;
            } else {
                break;
            }
        }
    }

}
static int ggallop(uchar* data, int s, int es, Comparator cmp, uchar* k) {
    int i = 0;
    int last = s - 1;

    if (s < 1) {
        return 0;
    }


    if (cmp(data + i * es, k) < 0) {
    // IFL(data[i], k) {
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
        if (cmp(data + i * es, k) < 0) {
        // IFL(data[i], k) {
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
        if (cmp(data + i * es, k) < 0) {
        // IFL(data[i], k) {
            break;
        }
        i--;
    }

    return i + 1;
}
static int gallop(TsState * state, uchar* data, int s, void* k)
{
    int i = 0;
    int last = s - 1;
    int es = state->es;

    if (s < 1) {
        return 0;
    }


    if (state->cmp(data + i * es, k) < 0) {
    // IFL(data[i], k) {
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
        if (state->cmp(data + i * es, k) < 0) {
        // IFL(data[i], k) {
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
        if (state->cmp(data + i * es, k) < 0) {
        // IFL(data[i], k) {
            break;
        }
        i--;
    }

    return i + 1;
}

/* merge of
    data: [. . . . . . . . . .]
           |<- ls -> |<- rs ->|
 */
void galop_merge(uchar* data, int ls, int rs, int es, Comparator cmp, uchar* tmp) {
    uchar* l = data;
    uchar* r = data + ls * es;
    uchar* outptr, *lptr, *rptr;
    int lp = 0, rp = 0, out = 0;
    int r_copy_count = 0;       /* count copy from the right */
    int l_copy_count = 0;       /* count copy from the left */
    int g = 0;
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
            g = ggallop(rptr, rs - rp, es, cmp, lptr);
            if (g > 0) {
                memmove(outptr, rptr, g * es);
                rp += g;
                out += g;
            }
            continue;
        }

        if (l_copy_count > GALLOP_MODE) {
            l_copy_count = 0;
            g = ggallop(lptr, ls - lp, es, cmp, rptr);
            if (g > 0) {
                memcpy(outptr, lptr, g * es);
                lp += g;
                out += g;
            }
            continue;
        }

        if (cmp(rptr, lptr) < 0) {
        // IFL(r->d[rp], tmp[lp]) {
            // l->d[out++] = r->d[rp++];
            cswap(outptr, rptr, es);
            out++;
            rp++;

            r_copy_count++;
            l_copy_count = 0;
        }
        else {
            // l->d[out++] = tmp[lp++];
            cswap(outptr, lptr, es);
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
    uchar *tmp;
    
    /* left position, right position, out position */
    int lp = 0, rp = 0, out = 0;
    int ls, rs;                 /* left size, right size */
    int r_copy_count = 0;       /* count copy from the right */
    int l_copy_count = 0;       /* count copy from the left */
    int g = 0;                  /* gallop count */
    int es = state->es;
    assert(state);

    l = state->st.d + state->st.s - 2;
    r = state->st.d + state->st.s - 1;
    ls = l->s;
    rs = r->s;
    // tmp = alloc_arruc(ls, state->es);
    galop_merge(l->d, ls, rs, es, state->cmp, state->tmp);

    state->st.s--;
    l->s = ls + rs;
    // free(tmp);
    return;
    memcpy(tmp, l->d, ls * state->es);

    while (lp < ls || rp < rs) {
        if (lp == ls) {
            break;
        }

        if (rp == rs) {
            memcpy(
                l->d + out * state->es,
                tmp + lp * state->es,
                (ls - lp) *state->es);
            break;
        }

        if (state->cmp(r->d + rp * es, tmp + lp * es) < 0) {
        // IFL(r->d[rp], tmp[lp]) {
            // l->d[out++] = r->d[rp++];
            cswap(l->d + out * es, r->d + rp * es, es);
            out++;
            rp++;

            r_copy_count++;
            l_copy_count = 0;
        }
        else {
            // l->d[out++] = tmp[lp++];
            cswap(l->d + out * es, tmp + lp * es, es);
            out++;
            lp++;

            l_copy_count++;
            r_copy_count = 0;
        }

        if (r_copy_count > GALLOP_MODE) {
            r_copy_count = 0;
            g = gallop(state, r->d + rp * es, rs - rp, tmp + lp * es);
            if (g > 0) {
                memmove(l->d + out * es, r->d + rp * es, g * es);
                rp += g;
                out += g;
            }
            continue;
        }

        if (l_copy_count > GALLOP_MODE) {
            l_copy_count = 0;
            g = gallop(state, tmp + lp * es, ls - lp, r->d + rp * es);
            if (g > 0) {
                memcpy(l->d + out * es, tmp + lp * es, g * es);
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
        state.st.d[state.st.s] = (Run) {.d = (uchar*)d + st * es, .s = rn};
        state.st.s++;

        merge_stack(&state);

        st += rn;
        remains -= rn;

    } while (remains > 0);

    collapse_stack(&state);
    free(state.tmp);
}

#undef IFL
