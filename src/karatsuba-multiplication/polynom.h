#ifndef POLYNOM_H
#define POLYNOM_H

#define DEGRADATION_TO_NAIVE 32

typedef unsigned long long ull;
typedef struct {
    /* buf[0] - младший a0, an*x^n + ... + a0 */
    ull *buf;
    int s;
} Poly;

Poly alloc_poly_io(const int s);
Poly alloc_mult_poly(Poly * l, Poly * r);
void mult(Poly * l, Poly * r, Poly * out);
void free_poly(Poly * p);
void dump_poly(Poly * p);

#endif