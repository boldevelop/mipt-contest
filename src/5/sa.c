/*  SA — конкатенировать с реаллокацией
    написать функцию с наперёд заданным прототипом,
    осуществляющую конкатенацию строк с реаллокацией */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strcat_r(char *dest, const char *src, int *bufsz)
{
    int nl = strlen(dest) + strlen(src) + 1;
    char *ndest;
    if (nl <= *bufsz) {
        strcat(dest, src);
        return dest;
    }

    ndest = realloc(dest, nl);
    if (ndest == NULL) {
        free(dest);
        free((void *) src);
        abort();
    }

    *bufsz = nl;
    strcat(ndest, src);

    return ndest;
}

#ifdef DEBUG

char *allocstr(int l)
{
    char *b = calloc(l + 1, sizeof(char));
    if (!b) {
        abort();
    }
    return b;
}


const char *HELLO = "Hello";
const char *WORLD = ", world!";

int main()
{
    /* "Hello", 6, ", world!" */
    int bufs = 6;
    char *dst, *src;
    dst = allocstr(bufs);
    bufs++;
    strcpy(dst, HELLO);
    src = allocstr(8);
    strcpy(src, WORLD);
    dst = strcat_r(dst, src, &bufs);
    printf("%s\n", dst);
    free(dst);
    free(src);
}

#endif
