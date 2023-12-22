/* период генератора */

#include <stdlib.h>
#include <stdio.h>

#ifdef DEBUG
typedef int (*generator_t)(int);
#endif

unsigned brent_impl(generator_t gen)
{
    int x = 0;
    int power = 1, lam = 1;
    int tu = x;
    int hare = gen(tu);
    while (hare != tu) {
        if (power == lam) {
            tu = hare;
            power *= 2;
            lam = 0;
        }
        hare = gen(hare);
        lam += 1;
    }
    return lam;
}

// определяет длину петли в генераторе
unsigned cycle_len(generator_t gen)
{
    return brent_impl(gen);
}

#ifdef DEBUG
int f1(int x)
{
    x += 2;
    return x % 5;
}

int f2(int x)
{
    x += 2;
    return x % 5 + 1;
}

int f3(int x)
{
    x += 2;
    return x % 5 + 3;
}

int f4(int x)
{
    x *= 3;
    x += 2;
    return x % 5 + 4;
}

int main()
{
    printf("%d \n", cycle_len(f1));
    printf("%d \n", cycle_len(f2));
    printf("%d \n", cycle_len(f3));
    printf("%d \n", cycle_len(f4));
}
#endif
