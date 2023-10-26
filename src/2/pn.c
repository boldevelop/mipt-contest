/*  N-е простое число */
#include <stdio.h>
#include <stdlib.h>

int is_prime(int n)
{
    if (n < 2) {
        return 1;
    }
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            return 0;
        }
    }

    return 1;
}

int nth_prime(int n)
{
    int prime_count = 0, prime = 2;

    while (1) {
        if (is_prime(prime)) {
            prime_count++;
        }
        if (prime_count == n) {
            break;
        }
        prime++;
    }

    return prime;
}

int main()
{
    int prime, n;
    if (scanf("%d", &n) < 1) {
        abort();
    }
    prime = nth_prime(n);
    printf("%d\n", prime);
}
