/* 
JBE выполняет короткий переход, если первый операнд МЕНЬШЕ или РАВЕН второму операнду
jne: проверяет условие ZF == 0 и выполняет переход, если оба операнда НЕ равны
gcc -S -masm=intel -Og -o ap1.asm ap.c
*/

#include <stdio.h>

// int is_prime(unsigned x) {
//     if (x <= 1) {
//         return 0;
//     }
//     if (x <= 3) {
//         return 1;
//     }
//     if (x % 2 == 0) {
//         return 0;
//     }
//     for (int i = 2; i * i <= x; ++i) {
//         if (x % i == 0) {
//             return 0;
//         }
//     }
//     return 1;
// }

extern int is_prime(unsigned);

int main() {
    int primes[] = {2, 3, 5, 7, 11, 13, 17, 19};
    printf("Check Primes...\n");
    for (int i = 0; i < 8; ++i) {
        if (!is_prime(primes[i])) {
            printf("%d is not prime\n", i);
        }
    }
    printf("Check Composite...\n");
    for (int i = 0; i < 21; ++i) {
        if (is_prime(i)) {
            printf("%d is prime\n", i);
        }
    }
}