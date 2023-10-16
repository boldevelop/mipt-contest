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

int get_nearest_fib(int required) {
    int t, f = 0, s = 1;
    while (s <= required) {
        t = s;
        s = f + s;
        f = t;
    }

    return f;
}

int get_optimal(int total, int possible) {
    int rest = total, fib;

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

int next_turn(int total, int possible) {
  if (possible >= total) {
    return total;
  }

  return get_optimal(total, possible);
}

#ifdef LOCAL_TEST
int count = 1;
void testCase(int t, int p, int should) {
    int res = next_turn(t, p);
    printf("%d: ", count);
    printf("%d %d = %d (exp %d)\n", t, p, next_turn(t, p), should);
    assert(res == should);
    count++;
}

int main() {
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

    testCase(33, 12, 1);

    testCase(100, 99, 3);
    testCase(10000, 9, 2);

    testCase(1000000, 1000, 55);
    testCase(500500501, 111, 89);
}
#endif
