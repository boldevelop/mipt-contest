#include "polynom.h"
#include "mem.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>

static void alloc_sum_poly(Poly * l, Poly * r, Poly * A1pA2, Poly * B1pB2,
                           int half_degree)
{
    A1pA2->s = half_degree;
    A1pA2->buf = alloc_arr_ull(half_degree);
    B1pB2->s = half_degree;
    B1pB2->buf = alloc_arr_ull(half_degree);

    for (int i = 0; i < half_degree; ++i) {
        A1pA2->buf[i] = l->buf[i] + l->buf[i + half_degree];
        B1pB2->buf[i] = r->buf[i] + r->buf[i + half_degree];
    }
}

static void divide_poly(Poly * p, Poly * out1, Poly * out2,
                        int half_degree)
{
    out1->s = half_degree;
    out1->buf = p->buf + half_degree;

    out2->s = half_degree;
    out2->buf = p->buf;
}

static void naive_mult(Poly * l, Poly * r, Poly * out)
{
    for (int i = 0; i < l->s; ++i)
        for (int j = 0; j < r->s; ++j)
            out->buf[i + j] += l->buf[i] * r->buf[j];
}

Poly alloc_poly_io(const int s)
{
    Poly p;
    p.s = s;
    p.buf = alloc_arr_ull(p.s);
    for (int i = 0; i < p.s; ++i) {
        p.buf[i] = read_int();
    }
    return p;
}

void free_poly(Poly * p)
{
    free(p->buf);
}

void dump_poly(Poly * p)
{
    int degree = 0;
    for (int i = p->s - 1; i >= 0; --i)
        if (p->buf[i] != 0) {
            degree = i + 1;
            break;
        }
    dump_arr(p->buf, degree);
}

Poly alloc_mult_poly(Poly * l, Poly * r)
{
    Poly out;
    out.s = l->s + r->s;
    out.buf = alloc_arr_ull(out.s);
    return out;
}

void mult(Poly * l, Poly * r, Poly * out)
{
    int degree = l->s, half_degree = l->s / 2;
    ull *out1, *out2, *out3, *out4;
    ull *sum1, *sum2;
    Poly A1, A2, B1, B2, A1B1, A2B2;

    /* allocated poly */
    Poly A1pA2, B1pB2, A1pA2mB1pB2;

    if (degree < DEGRADATION_TO_NAIVE) {
        naive_mult(l, r, out);
        return;
    }

    /* A1, A2, B1, B2 */
    divide_poly(l, &A1, &A2, half_degree);
    divide_poly(r, &B1, &B2, half_degree);

    /* A1B1 */
    A1B1.s = degree;
    A1B1.buf = out->buf + degree;
    mult(&A1, &B1, &A1B1);

    /* A2B2 */
    A2B2.s = degree;
    A2B2.buf = out->buf;
    mult(&A2, &B2, &A2B2);

    /* A1pA2mB1pB2 */
    alloc_sum_poly(l, r, &A1pA2, &B1pB2, half_degree);
    A1pA2mB1pB2 = alloc_mult_poly(&A1pA2, &B1pB2);
    mult(&A1pA2, &B1pB2, &A1pA2mB1pB2);

    sum1 = A1pA2mB1pB2.buf;
    sum2 = A1pA2mB1pB2.buf + half_degree;

    out1 = out->buf;
    out2 = out->buf + half_degree;
    out3 = out->buf + 2 * half_degree;
    out4 = out->buf + 3 * half_degree;

    for (int i = 0; i < half_degree; i++) {
        ull c1 = out2[i] + sum1[i] - out1[i] - out3[i];
        ull c2 = out3[i] + sum2[i] - out2[i] - out4[i];
        out->buf[half_degree + i] = c1;
        out->buf[degree + i] = c2;
    }

    free_poly(&A1pA2);
    free_poly(&B1pB2);
    free_poly(&A1pA2mB1pB2);
}
