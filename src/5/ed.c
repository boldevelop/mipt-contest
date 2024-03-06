/* Расстояние редактирования */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readint() {
    int b;
    if (scanf("%d", &b) != 1) {
        abort();
    }
    return b;
}

void* my_calloc(size_t num, size_t size) {
    void* buf = calloc(num, size);
    if (!buf) {
        abort();
    }
    return buf;
}

char* readstr() {
    int s = readint();
    char *str = my_calloc(s + 1, sizeof(char));    
    int c;
    getchar();                  // eat space
    for (int i = 0; i < s; ++i) {
        c = getchar();
        if (c == EOF) {
            break;
        }
        str[i] = c;
    }
    return str;
}

typedef struct {
    int add;
    int rmv;
    int chg;
} Costs;

int distances() {
    return 0;
}

int main() {
    char* word1, *word2;
    int* dp;
    int w1l, w2l;
    Costs costs = {0};
    costs.add = readint();
    costs.rmv = readint();
    costs.chg = readint();
    word1 = readstr();
    w1l = strlen(word1);
    word2 = readstr();
    w2l = strlen(word2);
    w2l++;
    dp = my_calloc(w2l * 2, sizeof(int));

    for (int i = 1; i < w2l; ++i) {
        dp[i] = i * costs.add;
    }
    for (int j = 1; j <= w1l; ++j) {
        dp[w2l] = j * costs.rmv;

        for (int i = 1; i < w2l; ++i) {
            int rmv_cost = dp[w2l + i - 1] + costs.add;
            int add_cost = dp[i] + costs.rmv;
            int chg_cost = dp[i - 1];
            if (word2[i - 1] != word1[j - 1]) {
                chg_cost += costs.chg;
            }
            rmv_cost = rmv_cost < add_cost ? rmv_cost : add_cost;
            rmv_cost = rmv_cost < chg_cost ? rmv_cost : chg_cost;
            dp[w2l + i] = rmv_cost;
        }

        memcpy(dp, dp + w2l, w2l * sizeof(int));
    }
    
    printf("%d\n", dp[w2l * 2 - 1]);

    free(dp);
    free(word1);
    free(word2);
}