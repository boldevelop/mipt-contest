#include <stdio.h>
#include <limits.h>
#include <stdio.h>

#include "combinatorics.h"
#include "mtd_mem.h"

#define N 3

void mtd_print(int *p, const int s) {
    for (int i = 0; i < s; ++i) {
        printf("%d", p[i]);
    }
    printf("\n");
}

void mtd_init(int *p, const int s) {
    for (int i = 0; i < s; ++i) {
        p[i] = i + 1;
    }
}

int mtd_rows_sum_equal(const int *p, const int s) {
    int first_numbers_size = s / 2;
    int sum = -1;

    for (int i = 0; i < first_numbers_size; ++i) {
        int j = (i + 1) % first_numbers_size, temp_sum = 0;

        temp_sum = p[i];
        temp_sum += p[i + first_numbers_size];
        temp_sum += p[j + first_numbers_size];

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

void mtd_rows_print(const int *p, const int s) {
    int first_numbers_size = s / 2;

    for (int i = 0; i < first_numbers_size; ++i) {
        int j = (i + 1) % first_numbers_size;

        printf("%d,", p[i]);
        printf("%d,", p[i + first_numbers_size]);
        printf("%d; ", p[j + first_numbers_size]);
    }
#ifndef RESEARCH
    printf("\n");
#endif
}

int mtd_is_cyclic_permutations(const int *p, const int s, const int *saved, const int saved_s) {
    for (int i = 0; i < saved_s; ++i) {
        for (int j = 0; j < s / 2; ++j) {
            if (p[j] == saved[i]) {
                return 1;
            }
        }
    }

    return 0;
}

void mtd_push_saved(int v, int *saved, int* saved_s) {
    saved[*saved_s] = v;
    *saved_s += 1;
}

#ifndef RESEARCH
int main()
{
    int p[N * 2] = {0};
    int saved[N * 2] = {0};
    int saved_s = 0;
    int s = N * 2;
    int prev;
    int res;
    int NN;

    res = scanf("%d", &NN);
    if (res != 1) {
        abort();
    }
    if (NN < 3) {
        printf("Start from 3\n");
    }
    mtd_alloc(NN);
    mtd_init(p, s);
    prev = p[0];

    do {
        if (p[0] >= s - 1) {
            break;
        }

        if (prev != p[0]) {
            mtd_push_saved(prev, saved, &saved_s);
        }
        prev = p[0];

        if (mtd_is_cyclic_permutations(p, s, saved, saved_s)) {
            continue;
        }

        if (mtd_rows_sum_equal(p, s)) {
            mtd_rows_print(p, s);
        }
    } while (next_perm(p, s));
}
#else
void is_each_rows_div_N(const int *p, const int s) {
    int first_numbers_size = s / 2;
    int sum = 0;

    for (int i = 0; i < first_numbers_size; ++i) {
        int j = (i + 1) % first_numbers_size;
        sum = p[i] + p[i + first_numbers_size] + p[j + first_numbers_size];
        if (sum % N != 0) {
            printf("erd: -");
            return;
        }
    }

    printf("erd: +");
}


int mtd_sum(const int *p, int l, const int r) {
    int sum = 0;
    while (l < r) {
        sum += p[l++];
    }
    return sum;
}

int main()
{
    int p[N * 2] = {0};
    int s = N * 2;
    int half = s/2;

    mtd_init(p, s);

    do {
        mtd_rows_print(p, s);
        if (mtd_rows_sum_equal(p, s)) {
            printf("+ ");
        } else {
            printf("- ");
        }
        printf("suml %d, ", mtd_sum(p, 0, half));
        printf("sumr %d, ", mtd_sum(p, half, s));
        is_each_rows_div_N(p, s);
        printf("\n");
    } while (next_perm(p, s));
}
#endif

/*

1,5,6; 2,6,4; 3,4,5;
1,6,5; 3,5,4; 2,4,6;
1,4,6; 3,6,2; 5,2,4;
1,6,4; 5,4,2; 3,2,6;

2,4,6; 1,6,5; 3,5,4; 2
2,6,4; 3,4,5; 1,5,6; 1
2,3,5; 4,5,1; 6,1,3;
2,5,3; 6,3,1; 4,1,5;

3,4,5; 1,5,6; 2,6,4; 1
3,2,6; 1,6,4; 5,4,2; 4
3,5,4; 2,4,6; 1,6,5; 2
3,6,2; 5,2,4; 1,4,6; 3

4,1,5; 2,5,3; 6,3,1; 8
4,2,3; 5,3,1; 6,1,2;
4,5,1; 6,1,3; 2,3,5; 7
4,3,2; 6,2,1; 5,1,3;

5,2,4; 1,4,6; 3,6,2; 3
5,4,2; 3,2,6; 1,6,4; 4
5,1,3; 4,3,2; 6,2,1; 16
5,3,1; 6,1,2; 4,2,3; 14

6,1,3; 2,3,5; 4,5,1; 7
6,3,1; 4,1,5; 2,5,3; 8
6,1,2; 4,2,3; 5,3,1; 14
6,2,1; 5,1,3; 4,3,2; 16

- - - -
1,5,6; 2,6,4; 3,4,5;
1,6,5; 3,5,4; 2,4,6;
1,4,6; 3,6,2; 5,2,4;
1,6,4; 5,4,2; 3,2,6;

2,3,5; 4,5,1; 6,1,3;
2,5,3; 6,3,1; 4,1,5;

4,2,3; 5,3,1; 6,1,2;
4,3,2; 6,2,1; 5,1,3;

*/
