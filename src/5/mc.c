/* Размен монет */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

void dump_arr(int *arr, int s)
{
    for (int i = 0; i < s; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int readint()
{
    int buf;
    if (scanf("%d", &buf) != 1) {
        abort();
    }
    return buf;
}

void *my_calloc(size_t n, size_t size)
{
    void *b = calloc(n, size);
    if (!b) {
        abort();
    }
    return b;
}

inline static int min(int l, int r)
{
    return l < r ? l : r;
}

int calc(int es, int cs, int *c, int *V)
{
    if (es == 0)
        return 0;
    if (es < 0)
        return INT_MAX;
    if (V[es] != INT_MAX)
        return V[es];

    for (int i = 0; i < cs; ++i) {
        int tmp = calc(es - c[i], cs, c, V);
        if (tmp == INT_MAX)
            continue;
        V[es] = min(V[es], tmp + 1);
    }
    return V[es];
}

int *init_V(size_t s)
{
    int *V = my_calloc(s, sizeof(int));
    for (size_t i = 0; i < s; ++i) {
        V[i] = INT_MAX;
    }
    return V;
}

int count_coins(int exchange_sum, int coins_s, int *coins)
{
    int res;
    int v_size = exchange_sum + 1;
    int *V = init_V(v_size);

    res = calc(exchange_sum, coins_s, coins, V);

    free(V);

    return res;
}

int main()
{
    int exchange_sum, coins_s;
    int *coins;
    exchange_sum = readint();
    coins_s = readint();
    coins = my_calloc(coins_s, sizeof(int));
    for (int i = 0; i < coins_s; ++i) {
        coins[i] = readint();
    }

#ifdef DEBUG
    printf("%d\n", exchange_sum);
    dump_arr(coins, coins_s);
#endif

    printf("%d\n", count_coins(exchange_sum, coins_s, coins));

    free(coins);
}
