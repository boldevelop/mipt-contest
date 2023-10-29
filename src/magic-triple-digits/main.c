#include <stdio.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "combinatorics.h"
#include "mtd_mem.h"

void mtd_print(mtd_arr pp)
{
    for (int i = 0; i < pp.s; ++i) {
        printf("%d", At(pp, i) + 1);
    }
    printf("\n");
}

void mtd_init(mtd_arr * pp, const int s)
{
    for (int i = 0; i < s; ++i) {
        mtd_push(pp, i + 1);
    }
}

int mtd_rows_sum_equal(mtd_arr pp)
{
    int first_numbers_size = pp.s / 2;
    int sum = -1;

    for (int i = 0; i < first_numbers_size; ++i) {
        int j = (i + 1) % first_numbers_size, temp_sum = 0;

        temp_sum = At(pp, i);
        temp_sum += At(pp, i + first_numbers_size);
        temp_sum += At(pp, j + first_numbers_size);

        if (sum == -1) {
            sum = temp_sum;
            continue;
        }

        if (sum != temp_sum) {
            return 0;
        }

        continue;
    }

    return 1;
}

int mtd_triangle_sum(mtd_arr pp)
{
    int sum = 0;

    for (int i = pp.s / 2; i < pp.s; ++i) {
        sum += At(pp, i);
    }

    return sum;
}

void mtd_rows_print(mtd_arr pp)
{
    int first_numbers_size = pp.s / 2;

    for (int i = 0; i < first_numbers_size; ++i) {
        int j = (i + 1) % first_numbers_size;

        printf("%d,%d,%d; ", At(pp, i), At(pp, i + first_numbers_size),
               At(pp, j + first_numbers_size));
    }
#ifndef RESEARCH
    // printf("\n");
#endif
}

int mtd_is_cyclic_permutations(mtd_arr pp, mtd_arr saved)
{
    for (int i = 0; i < saved.s; ++i) {
        for (int j = 0; j < pp.s / 2; ++j) {
            if (At(pp, j) == At(saved, i)) {
                return 1;
            }
        }
    }

    return 0;
}


void mtd_init_indexes(mtd_arr * indexes, int angles_count)
{
    for (int i = 0; i < angles_count; ++i) {
        mtd_push(indexes, i);
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

#ifndef RESEARCH
int main()
{
    int prev, angles_count, num_count;
    mtd_arr pp, saved, indexes;
    int res = scanf("%d", &angles_count);
    int sum;
    int sum_eq;

    if (res != 1) {
        abort();
    }
    if (angles_count < 3) {
        printf("Start from 3\n");
        abort();
    }
    num_count = angles_count * 2;

    pp = mtd_alloc(num_count);
    saved = mtd_alloc(num_count);
    indexes = mtd_alloc(angles_count);
    mtd_init_indexes(&indexes, angles_count);
    mtd_init(&pp, num_count);

    do {
        mtd_print(indexes);
    } while (mtd_next_indexes(indexes, num_count));
    return 0;

    do {
        sum = mtd_triangle_sum(pp);
        sum_eq = mtd_rows_sum_equal(pp);
        if (sum_eq/*  || sum % angles_count == 0 */) {
            mtd_rows_print(pp);
            if (sum_eq) {
                printf("+ ");
            } else {
                printf("- ");
            }
            printf("sum = %d; ", sum);
            mtd_print(pp);
            printf("\n");
        }
    } while(next_perm(pp.buf, pp.s));
    return 0;

    prev = At(pp, 0);
    do {
        if (At(pp, 0) >= num_count - 1) {
            break;
        }

        if (prev != At(pp, 0)) {
            mtd_push(&saved, prev);
        }
        prev = At(pp, 0);

        if (mtd_rows_sum_equal(pp)) {
            if (mtd_is_cyclic_permutations(pp, saved)) {
                continue;
            }

            mtd_rows_print(pp);
        }
    } while (next_perm(pp.buf, pp.s));

    mtd_free(pp);
    mtd_free(saved);
}
#else
void mtd_print_arr_ptr(mtd_arr_ptr saved_arrays, int num_count)
{
    int first_numbers_size = num_count / 2;

    for (int arr_i = 0; arr_i < saved_arrays.s; ++arr_i) {
        int *pp = At(saved_arrays, arr_i);

        for (int i = 0; i < first_numbers_size; ++i) {
            int j = (i + 1) % first_numbers_size;

            printf("%d,%d,%d; ", pp[i], pp[i + first_numbers_size],
                   pp[j + first_numbers_size]);
        }

        printf("\n");
    }

}


int mtd_is_cyclic_permutations_slow(mtd_arr pp, mtd_arr_ptr saved_arrays)
{
    int first_numbers_size = pp.s / 2;
    int size = pp.s;

    for (int arr_i = 0; arr_i < saved_arrays.s; ++arr_i) {
        int *saved = At(saved_arrays, arr_i);
        int found = 0;

        /* first row numbers (free points) */
        int i = 0, j = -1;
        while (i < first_numbers_size) {
            if (saved[0] == At(pp, i)) {
                j = i;
                break;
            }
            i++;
        }
        if (j == -1) {
            continue;
        }
        i = 0;

        do {
            j = (j + 1) % first_numbers_size;
            i = (i + 1) % first_numbers_size;
            if (saved[i] != At(pp, j)) {
                found = 1;
                break;
            }
        } while (saved[i] != saved[0]);
        if (found) {
            continue;
        }

        /* rest numbers which is included in rectangle */
        i = first_numbers_size, j = -1;
        while (i < size) {
            if (saved[first_numbers_size] == At(pp, i)) {
                j = i;
                break;
            }
            i++;
        }
        if (j == -1) {
            continue;
        }
        i = first_numbers_size;

        found = 0;
        do {
            j = (j + 1) % first_numbers_size + first_numbers_size;
            i = (i + 1) % first_numbers_size + first_numbers_size;
            if (saved[i] != At(pp, j)) {
                found = 1;
                break;
            }
        } while (saved[i] != saved[first_numbers_size]);
        if (found) {
            continue;
        }

        return 1;
    }

    return 0;
}

int main()
{
    int prev, num_count;
    mtd_arr pp, saved;
    mtd_arr_ptr saved_arrays;

    for (int n = 3; n <= 7; ++n) {
        num_count = n * 2;

        pp = mtd_alloc(num_count);
        saved = mtd_alloc(num_count);
        saved_arrays = mtd_alloc_ptr(10000);
        mtd_init(&pp, num_count);

        prev = At(pp, 0);
        do {
            if (At(pp, 0) >= num_count - 1) {
                break;
            }

            if (prev != At(pp, 0)) {
                mtd_push(&saved, prev);
            }
            prev = At(pp, 0);


            if (mtd_rows_sum_equal(pp)) {
                if (mtd_is_cyclic_permutations_slow(pp, saved_arrays)) {
                    continue;
                }

                mtd_push_ptr(&saved_arrays, pp);        // alloc and copy
            }
        } while (next_perm(pp.buf, pp.s));
        printf("N = %d ---\n", n);
        mtd_print_arr_ptr(saved_arrays, num_count);
        printf("\n");
        mtd_free(pp);
        mtd_free(saved);
        mtd_clear_ptr(&saved_arrays);
    }


    mtd_free_ptr(saved_arrays);
}
#endif
