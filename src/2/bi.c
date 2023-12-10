/* инвертировать N-й по счёту бит в массиве */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef unsigned char uchar;

void bi_free(uchar * buf)
{
    free(buf);
}

uchar *bi_alloc(int count)
{
    uchar *buf;
    buf = calloc(count, sizeof(uchar));
    if (buf == NULL) {
        abort();
    }
    return buf;
}

void bump_bits(uchar * buf, int n)
{
    uchar byte;
    for (int i = 0; i < n; ++i) {
        for (int j = CHAR_BIT - 1; j >= 0; j--) {
            byte = (buf[i] >> j) & 1;
            printf("%u", byte);
        }
        printf(" ");
    }
    printf("\n");
}

void bump(uchar * buf, int n)
{
    for (int i = 0; i < n; ++i) {
        printf("%d ", buf[i]);
    }
    printf("\n");
}

void invert_bit(uchar * buf, int n, int bi)
{
    uchar bite_n, bi_r, mask;
    int bits = n * CHAR_BIT;
    if (bi > bits) {
        return;
    }
    bite_n = bi / CHAR_BIT;
    bi_r = bi % CHAR_BIT;
    mask = 1 << bi_r;
    buf[bite_n] = buf[bite_n] ^ mask;
}

int main()
{
    int n, num, bi;
    uchar *buf;
    if (scanf("%d", &n) != 1) {
        abort();
    }

    buf = bi_alloc(n);
    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &num) != 1) {
            abort();
        }
        buf[i] = (char) num;
#ifdef DEBUG
        printf("%d ", num);
#endif
    }
#ifdef DEBUG
    printf("\n");
    bump_bits(buf, n);
#endif

    if (scanf("%d", &bi) != 1) {
        abort();
    }

    invert_bit(buf, n, bi);
#ifdef DEBUG
    bump_bits(buf, n);
#endif
    bump(buf, n);

    bi_free(buf);
    return 0;
}
