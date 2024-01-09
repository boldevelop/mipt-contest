#ifndef POLYNOM_H
#define POLYNOM_H

#ifndef DEGRADATION_TO_NAIVE
#define DEGRADATION_TO_NAIVE 32
#endif

typedef unsigned long long ull;
typedef struct {
    /* buf[0] - младший a0, an*x^n + ... + a0 */
    ull *buf;
    int s;
} Poly;

Poly alloc_poly_io(const int s);
Poly alloc_mult_poly(Poly * l, Poly * r);
Poly alloc_poly_arr(const ull * data, const int s);
Poly mult(Poly * l, Poly * r);
void free_poly(Poly * p);
void dump_poly(Poly * p);

int poly_degree(Poly * p);
void naive_mult(Poly * l, Poly * r, Poly * out);

#endif
