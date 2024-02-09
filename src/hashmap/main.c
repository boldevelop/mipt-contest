#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "hashmap.h"

int readint()
{
    int n;
    if (scanf("%d", &n) != 1) {
        abort();
    }
    return n;
}

void flush()
{
    while (getchar() != '\n');
}

int read_word(int strl, char *buf)
{
    int c;
    int bi = 0;
    do {
        c = getchar();
        strl--;

        if (isspace(c) || c == EOF) {
            buf[bi++] = '\0';
            break;
        }

        buf[bi++] = c;
    } while (strl > 0);

    return strl;
}

#define BUF_S 1024

int *alloc_arri(size_t s)
{
    int *buf = calloc(s, sizeof(int));
    if (!buf) {
        abort();
    }
    return buf;
}

int main()
{
    int exp_a, text_l;
    char buf[BUF_S];
    int *answers;
    int ind = 0;
    HashMap *hm;

    srand(738547485u);

    exp_a = readint();
    answers = alloc_arri(exp_a);
    text_l = readint();
    hm = init_hm(text_l);

    text_l++;
    flush();
    do {
        text_l = read_word(text_l, buf);
        insert_hm(hm, buf);
    } while (text_l);

    text_l = readint();
    text_l++;
    flush();
    do {
        text_l = read_word(text_l, buf);
        answers[ind++] = find_hm(hm, buf);
    } while (text_l);

    for (int i = 0; i < exp_a; ++i) {
        printf("%d ", answers[i]);
    }
    printf("\n");

    destroy_hm(hm);
    free(answers);

    return 0;
}
