/**
 * Два игрока играют в интересную игру: изначально дано N спичек.
 * Первый игрок берёт любое количество, но не все сразу спички.
 * Теперь второй может взять не больше, чем вдвое больше чем первый.
 * Далее первый берёт не больше чем вдвое больше второго.
 * И так далее. Выигрывает тот, кто взял последнюю спичку
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* while calculate fib for int required
   its not supposed to be bigger then 48
   46 cuz we skip first 2 values (48 - 2) */
#define NFIB 46

long long m_fib[NFIB] = { 0 };

int m_fib_size = 0;

void init_mfib()
{
    m_fib[0] = 1;
    m_fib[1] = 2;
    m_fib_size = 2;
}

int get_nearest_fib(int required)
{
#if NAIVE
    int t, f = 0, s = 1;
    while (s <= required) {
        t = s;
        s = f + s;
        f = t;
    }

    return f;
#else
    int i_fib;
    while (m_fib[m_fib_size - 1] < required) {
        assert(m_fib_size < NFIB);
        m_fib[m_fib_size] = m_fib[m_fib_size - 2] + m_fib[m_fib_size - 1];
        m_fib_size++;
    }

    /* maybe binsearch */
    i_fib = m_fib_size - 1;
    while (m_fib[i_fib] > required) {
        i_fib--;
    }

    return m_fib[i_fib];
#endif
}

int get_optimal(int total, int possible)
{
    long long fib;
    int rest = total;

    for (;;) {
        fib = get_nearest_fib(rest);
        if (fib == rest) {
            break;
        }
        rest = rest - fib;
    }

    if (rest > possible) {
        return 1;
    }
    return rest;
}

int next_turn(int total, int possible)
{
    assert(m_fib_size >= 2);
    if (possible >= total) {
        return total;
    }

    return get_optimal(total, possible);
}

#ifdef LOCAL_TEST
int count = 1;
void testCase(int t, int p, int should)
{
    int res = next_turn(t, p);
    printf("%d: ", count);
    printf("%d %d = %d (exp %d)\n", t, p, next_turn(t, p), should);
    assert(res == should);
    count++;
}

int main()
{
    init_mfib();


    testCase(10, 9, 2);
    testCase(100, 99, 3);
    testCase(1000, 999, 13);
    testCase(10000, 9, 2);
    testCase(100000, 8, 1);
    testCase(1000000, 1000, 55);
    testCase(500500501, 111, 89);
    testCase(500500501, 11, 1);

    testCase(14, 5, 1);
    testCase(7, 7, 7);
    testCase(6, 7, 6);
    testCase(1, 2, 1);
    testCase(3, 2, 1);

    testCase(2147483647, 2147483646, 5);
}
#endif
#ifdef LOCAL_STRESS_TEST
void testCase(int t, int p, int should)
{
    int res = next_turn(t, p);
    assert(res == should);
}

/*
naive
real    0m6.536s
user    0m6.536s
mem
real    0m3.855s
user    0m3.856s
 */
int main()
{
    init_mfib();
    for (int i = 0; i < 10000000; ++i) {
        testCase(500500501, 111, 89);
        testCase(1000000, 1000, 55);
    }
}
#endif
