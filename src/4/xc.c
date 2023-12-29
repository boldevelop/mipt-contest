/*  подсчёт коллизий в хеш-функции
    значение хеша для каждой из строк
    не больше чем заданная наперед константа HASH_MAX*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
typedef int (*get_hash_t)(const char *s);
#define HASH_MAX 10
#endif

// подсчитать количество коллизий функции f над
// массивом strs
int ncollisions(char **strs, int n, get_hash_t f)
{
    int *bckts = calloc(HASH_MAX, sizeof(int));
    int col = 0;
    if (!bckts) {
        abort();
    }
    for (int i = 0; i < n; ++i) {
        int b = f(strs[i]);
        if (bckts[b] > 0) {
            col++;
        }
        bckts[b]++;
    }
    free(bckts);

    return col;
}

#ifdef DEBUG
int h(const char *s)
{
    return strlen(s);
}

int main()
{
    int s = 7, col;
    char *s1 = "a";
    char *s2 = "b";
    char *s3 = "bc";
    char *s4 = "bc";
    char *s5 = "bce";
    char *s6 = "bc";
    char *s7 = "bcee";
    char *strs[7] = { 0 };
    strs[0] = s1;
    strs[1] = s2;
    strs[2] = s3;
    strs[3] = s4;
    strs[4] = s5;
    strs[5] = s6;
    strs[6] = s7;
    col = ncollisions(strs, s, h);
    printf("%d\n", col);
}
#endif
