/* тест Ферма
Малая теорема Ферма
Проверка числа на простоту */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long long ull;

/* n^k mod m */
ull pow_mod(ull n, ull k, ull m)
{
    ull mult = n % m;
    ull prod = 1ull;
    while (k > 0) {
        if ((k % 2) == 1) {
            prod = (prod * mult) % m;
            k -= 1ull;
        }
        mult = (mult * mult) % m;
        k >>= 1;
    }
    return prod;
}

int main()
{
    /* struct timespec ts;
       timespec_get(&ts, TIME_UTC); */

    ull p, a;
    int i = 0;
    int prime_c = 0, nonprime_c = 0;
    if (scanf("%llu", &p) != 1) {
        abort();
    }
    srand(time(NULL));
    while (i < 100) {
        a = rand() * 2;
        if (a % p == 0 || a == 0 || a > p - 2) {
            continue;
        }
        if (pow_mod(a, p - 1ull, p) == 1ull) {
            prime_c++;
        } else {
            nonprime_c++;
        }

        i++;
    }
    printf("%d\n", prime_c >= nonprime_c);
    return 0;

    /* time_t epoch = time(NULL);
       struct tm* ptm;
       ptm = localtime(&epoch);
       printf("%s", asctime(ptm)); */
}
