#include <stdlib.h>

#include "combinatorics.h"

static void com_swap(int *p, int l, int r)
{
    int t = p[l];
    p[l] = p[r];
    p[r] = t;
}

static void com_reverse(int *p, int l, int r)
{
    while (l < r) {
        com_swap(p, l, r);
        l++;
        r--;
    }
}

/**
 * p: source vector of int (mutate)
 * s: size of vector p
 * return success of permutation
 */
int next_perm(int *p, const int s)
{
    int i = s - 2;
    int j = s - 1;
    int found = 0;

    /* find largset i p[i] < p[i + 1] */
    while (i >= 0) {
        if (p[i] < p[i + 1]) {
            found = 1;
            break;
        }
        i--;
    }

    if (!found) {
        /* last permutation */
        return 0;
    }

    /* Find largset j, that is p[i] < p[j] */
    while (j > i) {
        if (p[i] < p[j]) {
            break;
        }
        j--;
    }

    com_swap(p, i, j);
    com_reverse(p, i + 1, s - 1);

    return 1;
}

static int mtd_next_indexes(Combination c)
{
    int *indexes = c.indexes;
    int indexes_size = c.s_i;
    int last_i = c.s_i - 1;

    if (indexes[last_i] != c.s - 1) {
        indexes[last_i]++;
        return 1;
    }

    for (int i = indexes_size - 2; i >= 0; i--) {
        if (indexes[i] == indexes[i + 1] - 1) {
            continue;
        }

        indexes[i]++;
        for (int j = i + 1; j < indexes_size; ++j) {
            indexes[j] = indexes[j - 1] + 1;
        }

        return 1;
    }

    return 0;
}

static int *safe_malloc(int s)
{
    int *buf = malloc(s * sizeof(int));
    if (buf == NULL) {
        abort();
    }
    return buf;
}

static void apply_indexes(Combination comb)
{
    int i = 0, j = comb.s / 2, n = 1;
    comb.comb[0] = 1;
    while (n != comb.s) {
        if (i < comb.s_i && comb.indexes[i] == n) {
            comb.comb[i + 1] = ++n;
            i++;
        } else {
            comb.comb[j++] = ++n;
        }
    }
}

/**
 * C(n - 1, (n * 2) - 1)
 */
Combination init_combination(int angles_count, int num_count)
{
    Combination c = { 0 };
    c.s = num_count;
    c.comb = safe_malloc(c.s);

    c.s_i = angles_count - 1;
    c.indexes = safe_malloc(c.s_i);
    for (int i = 0; i < c.s_i; ++i) {
        c.indexes[i] = i + 1;
    }

    apply_indexes(c);
    return c;
}

void free_combination(Combination c)
{
    free(c.comb);
    free(c.indexes);
}

int next_combination(Combination c)
{
    if (!mtd_next_indexes(c)) {
        return 0;
    }
    apply_indexes(c);
    return 1;
}
