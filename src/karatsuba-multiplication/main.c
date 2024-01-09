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
    int left_s, right_s;
    Poly left_p;
    Poly right_p;
    Poly res;

    left_s = read_int();
    right_s = read_int();

    left_p = alloc_poly_io(left_s);
    right_p = alloc_poly_io(right_s);

    res = mult(&left_p, &right_p);

    dump_poly(&res);

    free_poly(&res);
    free_poly(&left_p);
    free_poly(&right_p);
}
