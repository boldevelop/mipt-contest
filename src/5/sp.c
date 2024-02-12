/*  замена в строке
    функцию с наперёд заданным прототипом,
    осуществляющую замену всех подстрок в строке */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *allocstr(size_t l)
{
    char *buf = calloc(l + 1, sizeof(char));
    if (!buf) {
        abort();
    }
    return buf;
}

char *cat(char *src, size_t ofs, size_t *bufs, char const *dst,
          size_t dst_s)
{
    size_t src_s = (*bufs) - ofs;
    char *src_tmp;
    if (src_s <= dst_s) {
        *bufs = ofs + dst_s + 1;
        src_tmp = realloc(src, *bufs);
        if (!src_tmp) {
            abort();
        }
        src = src_tmp;
    }
    memcpy(src + ofs, dst, dst_s);
    return src;
}

char *replace(char *str, char const *from, char const *to)
{
    size_t str_l = strlen(str);
    size_t to_l = strlen(to);
    size_t from_l = strlen(from);

    size_t res_l = 0;
    char *resp = allocstr(str_l);

    size_t bufs;

    char *begin = str;
    char *strp = str;
    int s = 0;

    str_l++;
    bufs = str_l;

    while ((begin = strstr(begin, from))) {

        /* copy non template */

        s = begin - strp;
        resp = cat(resp, res_l, &bufs, strp, s);
        res_l += s;
        strp += s;

        /* copy template */
        resp = cat(resp, res_l, &bufs, to, to_l);
        res_l += to_l;
        strp += from_l;

        begin += from_l;
    }

    s = str + str_l - strp;
    resp = cat(resp, res_l, &bufs, strp, s);
    res_l += s;

    resp[res_l] = '\0';

    return resp;
}

#ifdef DEBUG

void Test(char *str, char const *replacer)
{
    char *tmp = replace(str, "%d", replacer);
    printf("i: %s\n", str);
    printf("o: %s\n", tmp);
    free(tmp);
}

char const *SIMPLE = "12";
char const *HARD = "9999";

int main()
{
    Test(".%d.", SIMPLE);
    Test("%d.", SIMPLE);
    Test(".%d", SIMPLE);
    Test("%d.%d", SIMPLE);
    Test(".", SIMPLE);
    Test("%d", SIMPLE);
    Test("%d%d%d", SIMPLE);
    Test(".%d.%d", SIMPLE);
    Test("%d.%d.", SIMPLE);
    Test(".%d.%d.", SIMPLE);

    Test(".%d.", HARD);
    Test("%d.", HARD);
    Test(".%d", HARD);
    Test("%d.%d", HARD);
    Test(".", HARD);
    Test("%d", HARD);
    Test("%d%d%d", HARD);
    Test(".%d.%d", HARD);
    Test("%d.%d.", HARD);
    Test(".%d.%d.", HARD);
}

#endif
