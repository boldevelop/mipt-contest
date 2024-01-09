#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "polynom.h"
#include "io.h"

int is_equal(Poly l, Poly r)
{
    int ld = poly_degree(&l);
    int rd = poly_degree(&r);
    if (ld != rd) {
        printf("Deg not equal: %d != %d\n", ld, rd);
        return 0;
    }
    for (int i = 0; i < ld; i++)
        if (l.buf[i] != r.buf[i])
            return 0;

    return 1;
}

int test_case(const ull * l, const int ls, const ull * r, const int rs)
{
    Poly lp = alloc_poly_arr(l, ls);
    Poly rp = alloc_poly_arr(r, rs);
    Poly naive_mp;
    Poly mp;
    int ok = 1;

    naive_mp = naive_mult(&lp, &rp);
    mp = mult(&lp, &rp);

    if (!is_equal(naive_mp, mp)) {
        printf("Inp: ls=%d, rs=%d\n", ls, rs);
        dump_arr(l, ls);
        dump_arr(r, rs);
        printf("Exp: ");
        dump_poly(&naive_mp);
        printf("Act: ");
        dump_poly(&mp);
        ok = 0;
    }

    free_poly(&lp);
    free_poly(&rp);
    free_poly(&naive_mp);
    free_poly(&mp);
    return ok;
}

void test(const ull * l, const int ls, const ull * r, const int rs,
          int *ok, int *failed)
{
    if (test_case(l, ls, r, rs))
        (*ok)++;
    else
        (*failed)++;
    if (test_case(r, rs, l, ls))
        (*ok)++;
    else
        (*failed)++;
}

#define S 100

int main()
{
    int failed = 0;
    int ok = 0;

    const int s = S;
    ull l[S] = { 0 };
    ull r[S] = { 0 };

    for (int i = 0; i < s; ++i) {
        l[i] = 1;
        for (int j = 0; j < s; ++j) {
            r[j] = 1;
            test(l, i + 1, r, j + 1, &ok, &failed);
        }

        for (int j = 0; j < s - 1; ++j) {
            r[j] = 0;
            test(l, i + 1, r, j + 1, &ok, &failed);
        }
        r[s - 1] = 0;
    }

    for (int i = 0; i < s; ++i) {
        l[i] = 0;
        r[i] = 0;
    }

    l[0] = 1;
    l[s - 1] = 1;

    /* test 1..0..1 */
    for (int i = 1; i < s; ++i) {
        for (int j = 0; j < s; ++j) {
            r[j] = 1;
            test(l, s, r, j + 1, &ok, &failed);
        }

        for (int j = 0; j < s - 1; ++j) {
            r[j] = 0;
            test(l, s, r, j + 1, &ok, &failed);
        }
        r[s - 1] = 0;
        l[i] = 1;
    }

    printf("- - -\nPassed: %d\nFailed: %d\n", ok, failed);
}
