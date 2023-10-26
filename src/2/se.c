/* количество простых чисел */
#include <stdio.h>
#include <stdlib.h>

int count_primes(int n)
{
    int count = 0;
    int *sieve;
    if (n < 2) {
        return 0;
    }
    if (n == 2) {
        return 1;
    }

    sieve = calloc(n + 1, sizeof(int));
    if (sieve == NULL) {
        abort();
    }

    for (int i = 2; i * i <= n; ++i) {
        for (int j = i * i; j <= n; j += i) {
            sieve[j] = 1;
        }
    }

    for (int i = 2; i <= n; ++i) {
        if (sieve[i] == 0) {
            count++;
        }
    }

    free(sieve);

    return count;
}

int main()
{
    int n;
    if (scanf("%d", &n) != 1) {
        abort();
    }

    n = count_primes(n);

    printf("%d\n", n);
}
