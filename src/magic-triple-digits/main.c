/*
    n: кол-во углов у фигуры
    memory: O(2n)
    complexity: O(2 * a(n) * P(n-1) * P(n) * n )

    Берем все сочетания из n по 2n: C(n, 2n)
    Но в программе считаем C(n - 1, 2n - 1), так как фиксирум единицу [1, [...n - 1], ...2n]
    Для каждого сочетания проверяем условие: "Сумма первых n делится на n" (1)
    Для тех сочетаний, которые удовлетворяют условию (1)
    считаем n - 1 перестановок (первое число не берем) для первых n чисел (2)
    и для каждой перестановки (2) считаем n перестановок вторых n чисел (3)
    и проверяем условие: "чтобы все тройки в ряд имели одинаковую сумму" (4)
    Меняем местами первый n числа со вторыми и повторяем (2)-(4)

    Так как кол-во сочетаний C(n, 2n) удовлетворяющих условию (1) равно последовательности a(n)
    Где a(n): https://oeis.org/search?q=4+9+26+76+246+809&go=Search,
    То число операций: a(n)
*/
#include <stdio.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "combinatorics.h"

int is_satisfy(const int *numbers, const int half)
{
    int sum = 0;
    for (int i = 0; i < half; ++i) {
        sum += numbers[i];
    }
    return sum % half == 0;
}


int is_sum_equal(const int *f, const int *s, const int n)
{
    int sum = -1;

    for (int i = 0; i < n; ++i) {
        int temp_sum = 0;

        temp_sum = f[i];
        temp_sum += s[i];
        temp_sum += s[(i + 1) % n];

        if (sum == -1) {
            sum = temp_sum;
            continue;
        }

        if (sum != temp_sum) {
            return 0;
        }
    }

    return 1;
}

void print_solution(const int *f, const int *s, const int n)
{
    for (int i = 0; i < n; ++i) {
        if (i != 0) {
            printf("; ");
        }
        printf("%d,%d,%d", f[i], s[i], s[(i + 1) % n]);
    }
    printf("\n");
}

void try_comb(const Combination comb, int *f, int *s)
{
    int n = comb.s / 2;
    int *buf = comb.comb;
    int bytes = n * sizeof(int);

    memcpy(f, buf, bytes);
    memcpy(s, buf + n, bytes);
    do {
        do {
            if (is_sum_equal(f, s, n)) {
                print_solution(f, s, n);
            }
        } while (next_perm(s, n));
        memcpy(s, buf + n, bytes);
    } while (next_perm(f + 1, n - 1));

    memcpy(f, buf, bytes);

    do {
        do {
            if (is_sum_equal(s, f, n)) {
                print_solution(s, f, n);
            }
        } while (next_perm(f, n));
        memcpy(f, buf, bytes);
    } while (next_perm(s + 1, n - 1));
}

int main()
{
    int angles_count, num_count;
    int *f;
    int *s;
    Combination comb;
    int res = scanf("%d", &angles_count);
    num_count = angles_count * 2;

    if (res != 1) {
        abort();
    }
    if (angles_count < 3) {
        printf("Start from 3\n");
        abort();
    }

    comb = init_combination(angles_count, num_count);
    f = malloc(angles_count * sizeof(int));
    s = malloc(angles_count * sizeof(int));

    do {
        if (!is_satisfy(comb.comb, angles_count)) {
            continue;
        }
        try_comb(comb, f, s);
    } while (next_combination(comb));

    free(f);
    free(s);
    free_combination(comb);

    return 0;
}
