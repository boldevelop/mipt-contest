/*  переворот подстрок
    переворачивает в некоей строке все вхождения определённой подстроки */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int readint()
{
    int d;
    if (scanf("%d", &d) != 1) {
        abort();
    }
    return d;
}

char *readtext(int l)
{
    char c;
    char *str = calloc(l + 1, sizeof(char));
    if (!str) {
        abort();
    }
    scanf("%c", &c);            // space
    for (int i = 0; i < l; ++i) {
        if (scanf("%c", &c) != 1) {
            abort();
        }
        str[i] = c;
    }
    str[l] = '\0';

    return str;
}

char *readstr()
{
    int l = readint();
    char *str = readtext(l);
    return str;
}

void dumpstr(char *str)
{
    printf("%ld %s\n", strlen(str), str);
}

void reversestr(char *str, size_t s)
{
    size_t l = 0, r = s - 1;
    char c;
    while (l < r) {
        c = str[l];
        str[l] = str[r];
        str[r] = c;
        l++;
        r--;
    }
}

void reverse(char *text, char *word)
{
    char *begin = text;
    size_t wl = strlen(word);

    while ((begin = strstr(begin, word))) {
        reversestr(begin, wl);
        begin += wl;
    }
}

int main()
{
    char *word;
    char *text;
    word = readstr();
    text = readstr();

    reverse(text, word);
    printf("%s\n", text);

    free(word);
    free(text);
}
