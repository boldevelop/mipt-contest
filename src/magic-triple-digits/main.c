#include <stdio.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "combinatorics.h"
#include "mtd_mem.h"

void get_comb(mtd_arr indexes, mtd_arr comb)
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
void mtd_init(mtd_arr * pp, const int s)
{
    for (int i = 0; i < s; ++i) {
        mtd_push(pp, i + 1);
    }
}
void mtd_init_indexes(mtd_arr * indexes, int angles_count)
{
    for (int i = 0; i < angles_count; ++i) {
        mtd_push(indexes, i + 1);
    }
}

int mtd_next_indexes(mtd_arr indexes, int num_count) {
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

void mtd_print_sep(mtd_arr pp)
{
    for (int i = 0; i < pp.s / 2; ++i) {
        printf("%d,", At(pp, i));
    }
    printf("  ");
    for (int i = pp.s / 2; i < pp.s; ++i) {
        printf("%d,", At(pp, i));
    }
    // printf("\n");
}

int is_satisfy(mtd_arr comb) {
    int sum = 0;
    int half = comb.s / 2;
    for (int i = 0; i < half; ++i) {
        sum += At(comb, i);
    }
    return sum % half == 0;
}

int is_sum_equal(int *f, int *s, int n)
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

int* copy_arr(int *src, int l, int r) {
    // assert(r > l);
    int s = r - l;
    int bytes = s * sizeof(int);
    int *buf = malloc(bytes);
    if (buf == NULL) {
        abort();
    }
    memcpy(buf, src + l, bytes);
    return buf;
}

void reinit_s(int *s, int n, mtd_arr comb) {
    int bytes = (comb.s - n) * sizeof(int);
    s[0] = At(comb, comb.s - 1);
    memcpy(s + 1, comb.buf + comb.s / 2, bytes - sizeof(int));
}

int *init_s(mtd_arr comb) {
    int n = comb.s / 2;
    int bytes = (comb.s - n) * sizeof(int);
    int *s = malloc(bytes);
    reinit_s(s, n, comb);
    return s;
}

void get_next_answer(int* f, int* s, int n) {
    com_reverse(f, 1, n - 1);
    com_reverse(s, 0, 1);
    com_reverse(s, 2, n - 1);
}

void print_ptr(int* f, int* s, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d,%d,%d; ", f[i], s[i], s[(i + 1) % n]);
    }
    printf("\n");
}
void print_comb(int* f, int *s, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d,", f[i]);
    }
    printf(" ");
    for (int i = 0; i < n; ++i) {
        printf("%d,", s[i]);
    }
    printf("\n");
}
void print_answer(int* f, int *s, int n) {
    print_ptr(f, s, n);
    get_next_answer(f, s, n);
    print_ptr(f, s, n);
}
void print_answer_d(int* f, int *s, int n) {
    print_ptr(f, s, n);
    print_comb(f, s, n);
    get_next_answer(f, s, n);
    print_ptr(f, s, n);
    print_comb(f, s, n);
}

int try_get_answer(mtd_arr comb) {
    int n = comb.s / 2;
    int next = 0;
    int *f = copy_arr(comb.buf, 0, n);
    int *s = init_s(comb);
    do {
        do {
            if (is_sum_equal(f, s, n)) {
                print_answer_d(f, s, n);
                next = 1;
                break;
            }
        // } while (next_perm(s + 1, n - 1));
        } while (next_perm(s, n));
        if (next) {
            break;
        }
        reinit_s(s, n, comb);
    } while (next_perm(f + 1, n - 1));

    f = memcpy(f, comb.buf + n, n * sizeof(int));
    s[0] = At(comb, n - 1);
    memcpy(s + 1, comb.buf, (n - 1) * sizeof(int));

    do {
        do {
            if (is_sum_equal(f, s, n)) {
                print_answer_d(f, s, n);

                return 1;
            }
        // } while (next_perm(s + 1, n - 1));
        } while (next_perm(s, n));
        s[0] = At(comb, n - 1);
        memcpy(s + 1, comb.buf, (n - 1) * sizeof(int));
    } while (next_perm(f + 1, n - 1));

    return 0;
}

void slow_solution(mtd_arr numbers, mtd_arr comb_ind, int num_count);
void try_one_comb();

int main() {
    int angles_count, num_count;
    mtd_arr comb, comb_ind;
    int res = scanf("%d", &angles_count);

    if (res != 1) {
        abort();
    }
    if (angles_count < 3) {
        printf("Start from 3\n");
        abort();
    }

    num_count = angles_count * 2;
    comb = mtd_alloc(num_count);
    comb_ind = mtd_alloc(angles_count - 1);

    mtd_init_indexes(&comb_ind, angles_count - 1);
    mtd_init(&comb, num_count); // todo replace

    solution(comb, comb_ind, num_count);

    return 0;
}

void try_one_comb() {
    int n = 6;
    int f[6] = {1, 2, 3, 4, 5, 9};
    int s[6] = {6, 7, 8, 10, 11, 12};
    int ss[6] = {6, 7, 8, 10, 11, 12};
    do {
        int found = 0;
        do {
            if (is_sum_equal(f, s, n)) {
                print_ptr(f, s, n);
                print_comb(f, s, n);
                // print_answer_d(f, s, n);
                // get_next_answer(f, s, n);
                // found = 1;
                // break;
            }
        } while (next_perm(s, n));
        memcpy(s, ss, n * sizeof(int));
        if (found) {
            break;
        }
    } while (next_perm(f + 1, n - 1));
}

void try_comb(mtd_arr comb, int* f, int* s) {
    int n = comb.s / 2;
    memcpy(f, comb.buf, n * sizeof(int));
    memcpy(s, comb.buf + n, n * sizeof(int));
    do {
        do {
            if (is_sum_equal(f, s, n)) {
                print_comb(f, s, n);
            }
        } while (next_perm(s, n));
        memcpy(s, comb.buf + n, n * sizeof(int));
    } while (next_perm(f + 1, n - 1));

    memcpy(f, comb.buf, n * sizeof(int));

    do {
        do {
            if (is_sum_equal(s, f, n)) {
                print_comb(s, f, n);
            }
        } while (next_perm(f, n));
        memcpy(f, comb.buf, n * sizeof(int));
    } while (next_perm(s + 1, n - 1));
}

void slow_solution(mtd_arr numbers, mtd_arr comb_ind, int num_count)
{
    int n = numbers.s / 2;
    int*f = malloc(n * sizeof(int));
    int*s = malloc(n * sizeof(int));
    do {
        get_comb(comb_ind, numbers);
        if (!is_satisfy(numbers)) {
            continue;
        }
        try_comb(numbers, f, s);
    } while (mtd_next_indexes(comb_ind, num_count));
}
