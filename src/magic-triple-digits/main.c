/*
    n: кол-во углов у фигуры
    memory: O(2n)
    complexity: O(2 * a(n) * P(n-1) * P(n) * n )

    Берем все сочетания из n по 2n: C(n, 2n)
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
#include "mtd_mem.h"

void mtd_init(mtd_arr * pp, const int s)
{
    for (int i = 0; i < s; ++i) {
        mtd_push(pp, i + 1);
    }
}

void mtd_init_indexes(mtd_arr * indexes, const int angles_count)
{
    for (int i = 0; i < angles_count; ++i) {
        mtd_push(indexes, i + 1);
    }
}

int mtd_next_indexes(mtd_arr indexes, const int num_count)
{
    if (At(indexes, indexes.s - 1) != num_count - 1) {
        At(indexes, indexes.s - 1)++;
        return 1;
    }

    for (int i = indexes.s - 2; i >= 0; i--) {
        if (At(indexes, i) == At(indexes, i + 1) - 1) {
            continue;
        }

        At(indexes, i)++;
        for (int j = i + 1; j < indexes.s; ++j) {
            At(indexes, j) = At(indexes, j - 1) + 1;
        }

        return 1;
    }

    return 0;
}

int is_satisfy(const mtd_arr comb)
{
    int sum = 0;
    int half = comb.s / 2;
    for (int i = 0; i < half; ++i) {
        sum += At(comb, i);
    }
    return sum % half == 0;
}

void get_comb(const mtd_arr indexes, mtd_arr comb)
{
    int i = 0, j = comb.s / 2, n = 1;
    At(comb, 0) = 1;
    while (n != comb.s) {
        if (i < indexes.s && At(indexes, i) == n) {
            At(comb, i + 1) = ++n;
            i++;
        } else {
            At(comb, j++) = ++n;
        }
    }
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

void try_comb(const mtd_arr comb, int *f, int *s)
{
    int n = comb.s / 2;
    memcpy(f, comb.buf, n * sizeof(int));
    memcpy(s, comb.buf + n, n * sizeof(int));
    do {
        do {
            if (is_sum_equal(f, s, n)) {
                print_solution(f, s, n);
            }
        } while (next_perm(s, n));
        memcpy(s, comb.buf + n, n * sizeof(int));
    } while (next_perm(f + 1, n - 1));

    memcpy(f, comb.buf, n * sizeof(int));

    do {
        do {
            if (is_sum_equal(s, f, n)) {
                print_solution(s, f, n);
            }
        } while (next_perm(f, n));
        memcpy(f, comb.buf, n * sizeof(int));
    } while (next_perm(s + 1, n - 1));
}

void solution(mtd_arr comb, mtd_arr comb_ind, const int num_count)
{
    int n = comb.s / 2;
    int *f = malloc(n * sizeof(int));
    int *s = malloc(n * sizeof(int));

    do {
        get_comb(comb_ind, comb);
        if (!is_satisfy(comb)) {
            continue;
        }
        try_comb(comb, f, s);
    } while (mtd_next_indexes(comb_ind, num_count));

    free(f);
    free(s);
}

int main()
{
    int angles_count, num_count;
    mtd_arr comb, comb_ind;
    int res = scanf("%d", &angles_count);
    num_count = angles_count * 2;

    if (res != 1) {
        abort();
    }
    if (angles_count < 3) {
        printf("Start from 3\n");
        abort();
    }

    comb = mtd_alloc(num_count);
    comb_ind = mtd_alloc(angles_count - 1);

    mtd_init_indexes(&comb_ind, angles_count - 1);
    mtd_init(&comb, num_count);

    solution(comb, comb_ind, num_count);

    mtd_free(comb);
    mtd_free(comb_ind);

    return 0;
}
