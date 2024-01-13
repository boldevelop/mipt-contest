#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define MAX_STACK_SIZE 85

int get_minrun(int n) {
    int r = 0;
    while (n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

struct Records {
    int** r;
    int rn; /* recorords (r) count */
    int n; /* elems in (r) count */
};
typedef struct Records records_t;

int* alloc_arri(int n) {
    int * buf;
    buf = calloc(n, sizeof(int));
    if (!buf) abort();
    return buf;
}

int** alloc_arrpi(int rn) {
    int** buf;
    buf = calloc(rn, sizeof(int*));
    if (!buf) abort();
    return buf;
}

records_t init_records(int rn, int n) {
    records_t r;
    r.rn = rn;
    r.n = n;
    r.r = alloc_arrpi(r.rn);
    for (int i = 0; i < r.rn; ++i) {
        r.r[i] = alloc_arri(r.n);
    }
    return r;
}

void free_records(const records_t r) {
    for (int i = 0; i < r.rn; ++i) {
        free(r.r[i]);
    }
    free(r.r);
}

void dump_r(const records_t r) {
    for (int i = 0; i < r.rn; ++i) {
        for (int j = 0; j < r.n; ++j)
            printf("%d ", r.r[i][j]);
        printf("\n");
    }
}

void fill_r(records_t r) {
    // int minrun = get_minrun(r.rn);

    for (int i = 0; i < r.rn; ++i) {
        for (int j = 0; j < r.n; ++j) {
            r.r[i][j] = (i + 1) + j * 10;
        }
    }
}

#define IFL(x, y) if ((x) < (y))

void swap(int* d, int l, int r) {
    int t = d[l];
    d[l] = d[r];
    d[r] = t;
}

void reverse(int* d, int st, int end) {
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
void find_run(int* d, int b, int end, int* rn) {
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
            } else {
                break;
            }
        }
    } else {
        for (; b < end; ++b) {
            IFL(d[b], d[b - 1]) {
                break;
            } else {
                (*rn)++;
            }
        }
    }
    if (desc) {
        reverse(d, b_saved, b);
    }
}

void da(int*d, int b, int e) {
    for (; b < e; ++b)
        printf("%d ", d[b]);
    printf("\n");
}

/**
 * stup: start unsorted position
 */
void insertion_sort(int *d, int b, int end, int stup) {
    for (int i = stup; i < end; ++i) {
        int p = i; /* prev j index */

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
    int* d;
    int s;
};
typedef struct run_t Run;

struct stack_t {
    Run d[MAX_STACK_SIZE];
    int s;
};
typedef struct stack_t Stack;

void dstack(Stack* st) {
    printf("Stack:\n");
    for (int i = st->s - 1; i >= 0; --i) {
        Run* r = st->d + i;
        printf("%d: ", r->s);
        da(r->d, 0, r->s);
    }
}

/* merging top 2 stack elements */
void merge(Stack* st) {
    Run* l, *r;
    int *tmp;
    int lp = 0, rp = 0, out = 0;

    l = st->d + st->s - 2;
    r = st->d + st->s - 1;
    tmp = alloc_arri(l->s);
    memcpy(tmp, l->d, sizeof(int) * l->s);

    while (lp < l->s || rp < r->s) {
        if (lp == l->s) {
            l->d[out++] = r->d[rp++];
            continue;
        }

        if (rp == r->s) {
            l->d[out++] = tmp[lp++];
            continue;
        }

        IFL(r->d[rp], tmp[lp]) {
            l->d[out++] = r->d[rp++];
        } else {
            l->d[out++] = tmp[lp++];
        }
    }

    st->s--;
    l->s = l->s + r->s;
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
void merge_stack(Stack* st) {
    Run* runs;
    assert(st);
    runs = st->d;

    if (st->s < 2) {
        return;
    }

    while (st->s > 1) {
        int s = st->s;
        int is_Z_less;
        Run* X = runs + s - 1;
        Run* Y = runs + s - 2;
        Run* Z = runs + s - 3;
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

void collapse_stack(Stack* st) {
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

void ts(int* d, const int s) {
    int remains = s;
    int st = 0;
    int minrun = get_minrun(s);
    Stack stack = {0};

    do {
        int rn = 0; /* natural run length */
        int rrl; /* run remains length */
        find_run(d, st, s, &rn);

        if (rn < minrun) {
            rrl = remains > minrun ? minrun : remains;
            insertion_sort(d, st, st + rrl, st + rn);
            rn = rrl;
        }

        // printf("run: ");
        // da(d, st, st + rn);
        assert(stack.s < MAX_STACK_SIZE);
        stack.d[stack.s] = (Run){ .d = d + st, .s = rn };
        stack.s++;

        merge_stack(&stack);

        st += rn;
        remains -= rn;

        // printf("rest: ");
        // da(d, st, N);
        // printf("\n");

    } while (remains > 0);

    // dstack(&stack);
    collapse_stack(&stack);
    // dstack(&stack);
    // da(d, 0, N);
}

/* fy */
#define SWP(arr, i, j) tmp = (arr)[(i)];\
    (arr)[(i)] = (arr)[(j)];\
    (arr)[(j)] = tmp;

void fy_shuffle(int *arr, const int s)
{
    int i = s;
    int j, tmp;
    while (i-- > 1) {
        j = rand() % (i + 1);
        SWP(arr, j, i);
    }
}

/* fy end */
void iota(int * a, const int s) {
    for (int i = 0; i < s; ++i) {
        a[i] = i+1;
    }
}

int is_equal(int const * a, int const * b, int s) {
    for (int i = 0; i < s; ++i)
        if (a[i] != b[i])
            return 0;
    return 1;
}

int intcmp(const void* a, const void* b) {
    const int *x = a;
    const int *y = b;
    return *x - *y;
}

float measure_ts(int* arr, const int s)
{
    clock_t start, end;
    float seconds;
    start = clock();
    ts(arr, s);
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    return seconds;
}

float measure_qs(int* arr, const int s)
{
    clock_t start, end;
    float seconds;
    start = clock();
    qsort(arr, s, sizeof(int), intcmp);
    end = clock();
    seconds = (float)(end - start) / CLOCKS_PER_SEC;
    return seconds;
}

/* TODO: galloping, add cmp and void** interface */
int main() {
    int rn = 300, n = 2;
    // int minrun = get_minrun(rn);

    int d[20] = {
        50, 49, 48, 47, 46, /*  */ 60, 20, 25, 30, 70,
        35, 36, 37, 38, 39, /*  */ 5, 40, 45, 47, 2,
    };
    records_t r1 = init_records(rn, n);
    
    ts(d, 20);
    da(d, 0, 20);
    free_records(r1);

    {
        int full_size = 1000000;
        int *ts_arr = alloc_arri(full_size);
        int *qs_arr = alloc_arri(full_size);
        int s = 10;
        float ts_s, qs_s;

        iota(ts_arr, full_size);
        iota(qs_arr, full_size);

        while (s <= full_size) {
            fy_shuffle(ts_arr, s);
            fy_shuffle(qs_arr, s);

            ts_s = measure_ts(ts_arr, s);
            qs_s = measure_qs(qs_arr, s);

            if (!is_equal(ts_arr, qs_arr, s)) {
                printf("Not equal\n");
            } else {
                printf("size: %d. OK\n", s);
                printf("ts: %fs\n", ts_s);
                printf("qs: %fs\n", qs_s);
            }
        
            s *= 10;
        }

        free(ts_arr);
        free(qs_arr);
    }
}