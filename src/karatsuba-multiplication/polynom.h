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
Poly alloc_poly_arr(const ull * const data, const int s);
Poly mult(Poly const *const l, Poly const *const r);
Poly naive_mult(Poly const *const l, Poly const *const r);
void free_poly(Poly const *const p);
void dump_poly(Poly const *p);

int poly_degree(Poly const *p);

#endif
