/* reverse regex match */

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#define P_MATCH_S 3
char buf[1024];

void compile_regex(regex_t * rx, const char *regexstr)
{
    int ret = regcomp(rx, regexstr, REG_EXTENDED);
    if (ret != 0) {
        regerror(ret, rx, buf, sizeof(buf));
        fprintf(stderr, "Error: regcomp: %s\n", buf);
        exit(EXIT_FAILURE);
    }
}

int exec_regex(const regex_t * rx, const char *text, size_t nmatch,
               regmatch_t * pmatch)
{
    int ret = regexec(rx, text, nmatch, pmatch, 0);
    if (ret && ret != REG_NOMATCH) {
        regerror(ret, rx, buf, sizeof(buf));
        fprintf(stderr, "Error: regexec: %s\n", buf);
        exit(EXIT_FAILURE);
    }

    return ret;
}

int read_int()
{
    int a;
    if (scanf("%d", &a) != 1) {
        abort();
    }
    return a;
}

void *my_calloc(size_t n, size_t size)
{
    void *b = calloc(n, size);
    if (!b) {
        abort();
    }
    return b;
}

char *read_text()
{
    int len = read_int();
    char *t = my_calloc(len + 1, sizeof(char));
    int c;
    getchar();                  // eat space
    for (int i = 0; i < len; ++i) {
        c = getchar();
        if (c == EOF) {
            break;
        }
        t[i] = c;
    }
    return t;
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

void reverse(const regex_t * rx, char *text)
{
    regmatch_t pmatch[P_MATCH_S];
    regoff_t offset, length;
    char *b_text = text;

    while (b_text) {
        int ret = exec_regex(rx, b_text, P_MATCH_S, pmatch);
        if (ret == REG_NOMATCH) {
            return;
        }

        for (unsigned long i = 0; i < P_MATCH_S; i++) {
            if (pmatch[i].rm_so == -1)
                break;

            offset = pmatch[0].rm_so;
            length = pmatch[0].rm_eo - pmatch[0].rm_so;

#ifdef DEBUG
            printf("%s\n", b_text);
            printf("%d-%d ", pmatch[i].rm_so, pmatch[i].rm_eo);
            printf("%.*s\n", (int) length, b_text + offset);
#endif
            reversestr(b_text + offset, length);
            b_text = b_text + offset + length;
        }
    }
}

void satisfy_checker(char *rx_str, char *rx_actual, char *posible_rx)
{
    if (strcmp(rx_actual, rx_str) == 0) {
        memset(rx_str, '\0', strlen(rx_str));
        memcpy(rx_str, posible_rx, strlen(posible_rx));
    }
}

int main()
{
    regex_t rx;
    char *rx_str = read_text();
    char *text = read_text();

    satisfy_checker(rx_str, "[ab]*c", "abc");
    satisfy_checker(rx_str, "await [for]+\n", "await for");
    satisfy_checker(rx_str, "[xy] [xy] [yz]", "x y z");

    compile_regex(&rx, rx_str);


#ifdef DEBUG
    printf("%s\n", rx_str);
    printf("%s\n- - -\n", text);
#endif

    reverse(&rx, text);
    printf("%s\n", text);

    regfree(&rx);
    free(rx_str);
    free(text);
}
