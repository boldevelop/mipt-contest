/* Регулярка для емейлов */

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

const char* MAIL_RE = "^[a-zA-Z0-9.]+@[a-zA-Z]+(\\.[a-zA-Z]+)+$";
const char* TEXT_MAIL = "hello.my.spambot@spambot.org.ru";

#define ARRAY_SIZE(array) (sizeof((array)) / sizeof((array)[0]))
char buf[1024];

void is_valid_email(const regex_t* rx, const char* email) {
    regmatch_t pmatch[3];
#ifdef DEBUG
    regoff_t offset, length;
#endif

    int ok;

    int ret = regexec(rx, email, ARRAY_SIZE(pmatch), pmatch, 0);
    if (ret && ret != REG_NOMATCH) {
        regerror(ret, rx, buf, sizeof(buf));
        fprintf(stderr, "Error: regexec: %s\n", buf);
        exit(EXIT_FAILURE);
    }

    ok = ret == 0;
#ifdef DEBUG
    if (ok) {
        offset = pmatch[0].rm_so;
        length = pmatch[0].rm_eo - pmatch[0].rm_so;

        printf("%s\n", email);
        printf("%.*s\n", (int)length, email + offset);

        for (unsigned long j = 0; j < ARRAY_SIZE(pmatch); j++) {
            if (pmatch [j].rm_so == -1)
                break;
        }
    }
#endif

    printf("%d\n", ok);
}


void compile_regex(regex_t* rx, const char* regexstr) {
    int ret = regcomp(rx, regexstr, REG_EXTENDED);
    if (ret != 0) {
        regerror(ret, rx, buf, sizeof(buf));
	    fprintf(stderr, "Error: regcomp: %s\n", buf);
        exit(EXIT_FAILURE);
    }
}

int main() {
    regex_t rx;
    compile_regex(&rx, MAIL_RE);
#if 1
    scanf("%s", buf);
    is_valid_email(&rx, buf);
#else
    is_valid_email(&rx, TEXT_MAIL);
#endif
    regfree(&rx);
}
