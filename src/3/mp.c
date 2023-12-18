/*  умножение полиномов (наивное) */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    /* buf[0] - младший a0, an*x^n + ... + a0 */
    int *buf;
    int s;
} Poly;

int read_int();
void read_poly(Poly * p);
void dump_arr(int *arr, const int s);
int *alloc_arr(const int s);

Poly mult_polynomials(Poly l, Poly r)
{
    Poly res;
    res.s = l.s + r.s - 1;
    res.buf = alloc_arr(res.s);

    for (int i = 0; i < l.s; ++i)
        for (int j = 0; j < r.s; ++j)
            res.buf[i + j] += l.buf[i] * r.buf[j];

    return res;
}

void dump_poly(Poly p)
{
    int degree = 0;
    for (int i = p.s - 1; i >= 0; --i)
        if (p.buf[i] != 0) {
            degree = i + 1;
            break;
        }
    dump_arr(p.buf, degree);
}

int main()
{
    Poly left_p;
    Poly right_p;
    Poly res;

    left_p.s = read_int();
    right_p.s = read_int();
    read_poly(&left_p);
    read_poly(&right_p);

    res = mult_polynomials(left_p, right_p);
    dump_poly(res);

    free(res.buf);
    free(left_p.buf);
    free(right_p.buf);
}

int read_int()
{
    int n;
    if (scanf("%d", &n) != 1) {
        abort();
    }
    return n;
}

int *alloc_arr(const int s)
{
    int *buf;
    buf = calloc(sizeof(int), s);
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

void dump_arr(int *arr, const int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
