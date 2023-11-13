#ifndef COMBINATORICS_H
#define COMBINATORICS_H

typedef struct {
    int* comb;
    int* indexes;
    int s;
    int s_i;
} Combination;

int next_perm(int* p, const int s);

Combination init_combination(int angles_count, int num_count);
void free_combination(Combination c);
int next_combination(Combination c);

#endif
