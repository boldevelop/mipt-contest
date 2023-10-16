/* системы счисления */
#include <stdio.h>
#include <stdlib.h>

#define N 32

void print_num(unsigned int x, unsigned int y) {
    unsigned int n[N];
    int i = 0;

    for (;;) {
        if (x < y) {
            n[i] = x;
            break;
        }
        n[i++] = x % y;
        x /= y;
    }

    while (i >= 0) {
        printf("%d", n[i]);
        i--;
    }
    printf("\n");
}

int main(void) {
    unsigned int x, y;
    int res = scanf("%d%d", &x, &y);
    if (res < 2) {
        abort();
    }
    print_num(x, y);
    return 0;
}
