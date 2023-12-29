/*  алгоритм Рабина-Карпа с учётом коллизий */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
// подсчитать хеш строки от start до fin
int get_hash(const char *start, const char *fin);
// обновить хеш current, удалив из него cprev и добавив в него cnext
// здесь n это pow_mod(R, right - 1, Q)
// возвращает новый хеш
int update_hash(int current, int n, char cprev, char cnext);
// raise n to power k modulo m
unsigned long long pow_mod(unsigned n, unsigned k, unsigned m);
#define R 13
#define Q 256

#endif

int is_substr_equal(const char *needle, const char *haystack, int l, int r)
{
    int len = strlen(needle);
    int i = 0;
    if (r - l != len) {
        return 0;
    }
    while (l < r) {
        if (needle[i] != haystack[l]) {
            return 0;
        }
        l++;
        i++;
    }
    return 1;
}

#ifndef DEBUG
// возвращает номер позиции на которой needle расположена внутри haystack
// или -1 если ничего не найдено
int rabin_karp(const char *needle, const char *haystack)
{
    unsigned n, target, cur, count = 0, left = 0, right = strlen(needle);

    target = get_hash(needle, needle + right);
    cur = get_hash(haystack, haystack + right);
    n = pow_mod(R, right - 1, Q);

    while (haystack[right] != 0) {
        if (target == cur
            && is_substr_equal(needle, haystack, left, right)) {
            return left;
        }
        cur = update_hash(cur, n, haystack[left], haystack[right]);
        left += 1;
        right += 1;
    }

    return -1;
}
#endif

#ifdef DEBUG
int main()
{
    char *str = "1234 567 890";
    char *n1 = "123";
    char *n2 = "890";
    printf("%d\n", is_substr_equal(n1, str, 0, 3));
    printf("%d\n", is_substr_equal(n2, str, 9, strlen(str)));
    printf("%d\n", is_substr_equal(n1, str, 1, 4));
    printf("%d\n", is_substr_equal(n1, str, 0, 2));
}
#endif
