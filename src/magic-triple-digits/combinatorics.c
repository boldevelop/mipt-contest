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
