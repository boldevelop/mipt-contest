/*  умножение гигантских полиномов, алгоритм Карацубы
    длина входных полиномов одинаковая и что это всегда степень двойки.
    хотя бы один коэффициент при первой или старших степенях ненулевой
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEGRADATION_TO_NAIVE 32

typedef unsigned long long ull;
typedef struct {
    /* buf[0] - младший a0, an*x^n + ... + a0 */
    ull * buf;
    int s;
} Poly;

int read_int();
void read_poly(Poly * p);
void dump_arr(ull * arr, const int s);
ull *alloc_arr(const int s);
Poly alloc_mult_poly(Poly * l, Poly * r);
Poly alloc_sum_poly(Poly * l, Poly * r);
void free_poly(Poly * p);
void divide_poly(Poly * p, Poly * out1, Poly * out2, int degree);

void naive_mult(Poly * l, Poly * r, Poly * out)
{
    for (int i = 0; i < l->s; ++i)
        for (int j = 0; j < r->s; ++j)
            out->buf[i + j] += l->buf[i] * r->buf[j];
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

void mult(Poly * l, Poly * r, Poly * out)
{
    int deg = l->s;
    Poly A1, A2, B1, B2;
    /* allocated poly */
    Poly A1B1, A2B2, A1pA2, B1pB2, A1pA2mB1pB2;

    if (deg < DEGRADATION_TO_NAIVE) {
        naive_mult(l, r, out);
        return;
    }

    /* A1, A2, B1, B2 */
    divide_poly(l, &A1, &A2, deg);
    divide_poly(r, &B1, &B2, deg);

    /* A1B1 */
    A1B1 = alloc_mult_poly(&A1, &B1);
    mult(&A1, &B1, &A1B1);

    /* A2B2 */
    A2B2 = alloc_mult_poly(&A2, &B2);
    mult(&A2, &B2, &A2B2);

    /* A1pA2mB1pB2 */
    A1pA2 = alloc_sum_poly(&A1, &A2);
    B1pB2 = alloc_sum_poly(&B1, &B2);
    A1pA2mB1pB2 = alloc_mult_poly(&A1pA2, &B1pB2);
    mult(&A1pA2, &B1pB2, &A1pA2mB1pB2);

    // for (int i = 0; i < A1B1.s; ++i) {
    //     assert(i + deg < out->s);
    //     out->buf[i + deg] += A1B1.buf[i];
    // }
    // for (int i = 0; i < A1B1.s; ++i) {
    //     assert(i + deg / 2 < out->s);
    //     out->buf[i + deg / 2] +=
    //         A1pA2mB1pB2.buf[i] - A1B1.buf[i] - A2B2.buf[i];
    // }
    // for (int i = 0; i < A2B2.s; ++i)
    //     out->buf[i] += A2B2.buf[i];

    for (int i = 0; i < A1B1.s; ++i) {
        assert(i + deg < out->s);
        out->buf[i + deg] += A1B1.buf[i];

        assert(i + deg / 2 < out->s);
        out->buf[i + deg / 2] +=
            A1pA2mB1pB2.buf[i] - A1B1.buf[i] - A2B2.buf[i];
        out->buf[i] += A2B2.buf[i];
    }

    free_poly(&A1B1);
    free_poly(&A2B2);
    free_poly(&A1pA2);
    free_poly(&B1pB2);
    free_poly(&A1pA2mB1pB2);
}

int main()
{
    Poly left_p;
    Poly right_p;
    Poly res;

    left_p.s = read_int();
    right_p.s = read_int();
    assert(left_p.s % 2 == 0);  /* pow of 2 */
    assert(left_p.s == right_p.s);
    read_poly(&left_p);
    read_poly(&right_p);

    res = alloc_mult_poly(&left_p, &right_p);
    mult(&left_p, &right_p, &res);

    dump_poly(&res);

    free_poly(&res);
    free_poly(&left_p);
    free_poly(&right_p);
}

Poly alloc_sum_poly(Poly * l, Poly * r)
{
    Poly out;
    int i = 0;
    out.s = l->s > r->s ? l->s : r->s;
    out.buf = alloc_arr(out.s);

    while (i < out.s) {
        if (i < l->s) {
            out.buf[i] += l->buf[i];
        }
        if (i < r->s) {
            out.buf[i] += r->buf[i];
        }
        i++;
    }
    return out;
}

void divide_poly(Poly * p, Poly * out1, Poly * out2, int degree)
{
    degree /= 2;
    assert(p->s - degree > 0);
    out1->s = p->s - degree;
    out1->buf = p->buf + degree;

    out2->s = degree;
    out2->buf = p->buf;
}

int read_int()
{
    int n;
    if (scanf("%d", &n) != 1) {
        abort();
    }
    return n;
}

void free_poly(Poly * p)
{
    free(p->buf);
}

Poly alloc_mult_poly(Poly * l, Poly * r)
{
    Poly out;
    out.s = l->s + r->s - 1;
    out.buf = alloc_arr(out.s);
    return out;
}

ull *alloc_arr(const int s)
{
    ull *buf;
    buf = calloc(sizeof(ull), s);
    if (buf == NULL) {
        abort();
    }
    return buf;
}

void read_poly(Poly * p)
{
    p->buf = alloc_arr(p->s);
    for (int i = 0; i < p->s; ++i) {
        p->buf[i] = read_int();
    }
}

void dump_arr(ull * arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%lld ", arr[i]);
    }
    printf("\n");
}
