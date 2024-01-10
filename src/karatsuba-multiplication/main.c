/*  умножение гигантских полиномов, алгоритм Карацубы
    длина входных полиномов одинаковая и что это всегда степень двойки.
    хотя бы один коэффициент при первой или старших степенях ненулевой
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "polynom.h"
#include "io.h"

int main()
{
    int ls, rs;
    Poly lp;
    Poly rp;
    Poly res;

    ls = read_int();
    rs = read_int();

    lp = alloc_poly_io(ls);
    rp = alloc_poly_io(rs);

    res = mult(&lp, &rp);

    dump_poly(&res);

    free_poly(&res);
    free_poly(&lp);
    free_poly(&rp);
}
