/* минимум и максимум */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int read_int()
{
    int n;
    if (scanf("%d", &n) != 1) {
        abort();
    }
    return n;
}

int main()
{
    int s, max = INT_MIN, min = INT_MAX, temp;
    s = read_int();
    if (s == 0) {
        min = max = 0;
    }

    while (s-- > 0) {
        temp = read_int();
        if (temp < min) {
            min = temp;
        }
        if (temp > max) {
            max = temp;
        }
    }

    printf("%d %d\n", min, max);
}
