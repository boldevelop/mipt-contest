/* Линейные диофантовы уравнения */
#include <stdlib.h>
#include <stdio.h>

/* ax + by = d */
void extended_euclid(int x, int y, int *s, int *t, int *d)
{
    int r, q, temp_s, temp_t;
    int s0 = 1, t0 = 0;
    int s1 = 0, t1 = 1;

    for (;;) {
        q = x / y;

        r = x - q * y;

        temp_s = s1;
        s1 = s0 - q * s1;
        s0 = temp_s;

        temp_t = t1;
        t1 = t0 - q * t1;
        t0 = temp_t;

        x = y;
        y = r;

        if (r == 0) {
            // x = gcd, coef Bezu = s0, t0
            *s = x < 0 ? -s0 : s0;
            *t = x < 0 ? -t0 : t0;
            *d = x < 0 ? -x : x;
            return;
        }
    }
}

/* ax+by=c, print x, y */
void diophantine_e(int a, int b, int c)
{
    int d, q, x, y;

    /* ak + bl = d */
    extended_euclid(a, b, &x, &y, &d);
    if (c % d != 0) {
        printf("NONE\n");
        return;
    }
    q = c / d;
    printf("%d %d\n", x * q, y * q);
}

int main(void)
{
    int a, b, c;
    int res = scanf("%d%d%d", &a, &b, &c);
    if (res != 3) {
        abort();
    }
    diophantine_e(a, b, c);
}
