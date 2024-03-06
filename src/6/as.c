/* необходимо закодировать в hex пришедшие вам на stdin мнемоники
    Register file
    Register	        A	B	C	D
    Encoding (binary)	00	01	10	11

    Instruction set
    Mnemonic	Encoding (binary)
    MOVI Imm — move constant to D register	0IIIIIII
    ADD R, R — add two registers modulo 256	1000RRRR
    SUB R, R — sub two registers modulo 256	1001RRRR
    MUL R, R — mul two registers modulo 256	1010RRRR
    DIV R, R — div two registers modulo 256	1011RRRR
    IN R — read register R from keyboard	110000RR
    OUT R — write register R to screen	    110001RR
*/

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

/* 
^[\s]*(MOVI)[\s]+([0-9]+)[\s]*$

^[\s]*(ADD|SUB|MUL|DIV)[\s]+(A|B|C|D)[\s]+(A|B|C|D)[\s]*$

^[\s]*(IN|OUT)[\s]+(A|B|C|D)[\s]*$

^[\s]*((MOVI)[\s]+([0-9]+)|(ADD|SUB|MUL|DIV)[\s]+(A|B|C|D)[\s]+(A|B|C|D)|(IN|OUT)[\s]+(A|B|C|D))[\s]*$
*/

#define MNEMONIC_RE "^[\\s]*"\
    "("\
    "(MOVI)[\\s]+([0-9]+)|"\
    "(ADD|SUB|MUL|DIV)[\\s]+(A|B|C|D)[\\s]+(A|B|C|D)|"\
    "(IN|OUT)[\\s]+(A|B|C|D)"\
    ")"\
    "[\\s]*$"

#define ARRAY_SIZE(array) (sizeof((array)) / sizeof((array)[0]))
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

#define P_MATCH_S 4

int main() {
    regex_t rx;

    regmatch_t pmatch[P_MATCH_S];
    regoff_t offset, length;
    char *b_text = "MOVI 123";
    int ret;
    compile_regex(&rx, MNEMONIC_RE);
    /* iterate through group */
    ret = exec_regex(&rx, b_text, P_MATCH_S, pmatch);

    printf("%d\n", ret);
    printf("%s\n", __FILE__);
    regfree(&rx);
}