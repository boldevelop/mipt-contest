/*  получить из него другое число с плавающей точкой,
    инвертировав все нечётные биты фракции (нулевой бит считается чётным)
    и напечатать с точностью до пятого знака после запятой
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float readf()
{
    float buf;
    if (scanf("%f", &buf) != 1) {
        abort();
    }
    return buf;
}

void print_bits(size_t const size, void const *const ptr)
{
    unsigned char *b = (unsigned char *) ptr;
    unsigned char byte;
    int i, j;

    printf("fb: ");
    for (i = size - 1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

void print_fracbits(unsigned f)
{
    unsigned frac = (f << 9) >> 9;
    print_bits(sizeof(frac), &frac);
}

void print_fparts(unsigned f)
{
    printf("fx: %x\n", f);
    print_bits(sizeof(f), &f);
    printf("si: %x\n", f >> 31);
    printf("ex: %x\n", (f >> 23) & 0xff);
    printf("fr: %x\n", (f << 9) >> 9);
}

unsigned as_uint(float f)
{
    unsigned u;
    memcpy(&u, &f, sizeof(unsigned));
    return u;
}

float as_float(unsigned u)
{
    float f;
    memcpy(&f, &u, sizeof(float));
    return f;
}

float invert_odd_fraction_bits(float fv)
{
    unsigned u = as_uint(fv);
    unsigned m = 0x2AAAAA;
    u = u ^ m;
    return as_float(u);
}

void tests()
{
    enum { TSIZE = 5 };
    float ftest[] = { 1.0, 1.23456, 0.00567, 124.5, 102345 };
    float fexp[] = { 1.33333, 1.41152, 0.004498, 105.83333, 121500.32812 };
    for (int i = 0; i < TSIZE; ++i) {
        float res = invert_odd_fraction_bits(ftest[i]);
        printf("%d)\n%.5f\n%.5f\n", i + 1, res, fexp[i]);
    }
}

int main()
{
    float fv;
#ifdef DEBUG
    tests();
#endif
    fv = readf();
    printf("%.5f\n", invert_odd_fraction_bits(fv));
}
