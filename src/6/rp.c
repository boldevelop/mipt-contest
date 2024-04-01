/* 
    На стандартном вводе числитель и знаменатель дроби

    На выходе нижняя (по направлению к нулю) и верхняя (к бесконечности или минус бесконечности)
    аппроксимации при представлении в формате float как два шестнадцатиричных числа:
    экспонента и дробная часть мантиссы
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fenv.h>

float readf()
{
    float b;
    if (scanf("%f", &b) != 1) {
        abort();
    }
    return b;
}

/* 
    FE_DOWNWARD   rounding towards negative infinity
    FE_TONEAREST  rounding towards nearest representable value
    FE_TOWARDZERO rounding towards zero
    FE_UPWARD     rounding towards positive infinity
    fesetround
 */

unsigned as_uint(float f)
{
    unsigned u;
    memcpy(&u, &f, sizeof(unsigned));
    return u;
}

void print_exp_frac(float f)
{
    unsigned u = as_uint(f);
    unsigned frac = (u << 9) >> 9;
    unsigned exp = (u >> 23) & 0xFF;
    printf("0x%x 0x%x", exp, frac);
}

float divide(float num, float den, int round)
{
    fesetround(round);
    return num / den;
}

int main()
{
    float num, den, to_zero, to_inf;
    num = readf();
    den = readf();

    to_zero = divide(num, den, FE_TOWARDZERO);
    to_inf = divide(num, den, num > 0
                    && den > 0 ? FE_UPWARD : FE_DOWNWARD);

    print_exp_frac(to_zero);
    if (to_inf != to_zero) {
        printf(" ");
        print_exp_frac(to_inf);
    }
    printf("\n");
}
