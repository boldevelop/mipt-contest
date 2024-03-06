/* кодировщик
необходимо закодировать в hex пришедшие вам на stdin мнемоники
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
#include <string.h>

enum MNEMONIC {
    MOVI = 0x00,
    ADD = 0x80,
    SUB = 0x90,
    MUL = 0xA0,
    DIV = 0xB0,
    IN = 0xC0,
    OUT = 0xC4,
};

enum REGISTERS {
    A,
    B,
    C,
    D,
};

/* 
^\s*(MOVI)\s+([0-9]+)\s*$

^\s*(ADD|SUB|MUL|DIV)\s+(A|B|C|D)\s+(A|B|C|D)\s*$

^\s*(IN|OUT)\s+(A|B|C|D)\s*$

^\s*((MOVI)\s+([0-9]+)|(ADD|SUB|MUL|DIV)\s+(A|B|C|D)\s+(A|B|C|D)|(IN|OUT)\s+(A|B|C|D))\s*$
*/
#define SPACE_RE "\\s"
#define REGISTERS_RE "(A|B|C|D)"
#define MNEMONIC_RE "^"SPACE_RE"*"\
    "("\
    "(MOVI)"SPACE_RE"+([0-9]+)|"\
    "(ADD|SUB|MUL|DIV)"SPACE_RE"+"REGISTERS_RE SPACE_RE"*,"SPACE_RE"+"REGISTERS_RE"|"\
    "(IN|OUT)"SPACE_RE"+"REGISTERS_RE""\
    ")"\
    ""SPACE_RE"*$"

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

#define P_MATCH_S 10

/* TODO: change regex logic */
int operation_mnem_val(char *mnem)
{
    if (strcmp(mnem, "ADD") == 0) {
        return ADD;
    } else if (strcmp(mnem, "SUB") == 0) {
        return SUB;
    } else if (strcmp(mnem, "MUL") == 0) {
        return MUL;
    }
    return DIV;
}

int io_mnem_val(char *mnem)
{
    if (strcmp(mnem, "IN") == 0) {
        return IN;
    }
    return OUT;
}

int register_val(char *reg)
{
    if (strcmp(reg, "A") == 0) {
        return A;
    } else if (strcmp(reg, "B") == 0) {
        return B;
    } else if (strcmp(reg, "C") == 0) {
        return C;
    }
    return D;
}

void mnemonic_to_hex(char *instr, regmatch_t * pmatch)
{
    int res = 0;
    regoff_t offset, length;

    for (int i = 2; i < P_MATCH_S; i++) {
        char temp[1024] = { 0 };
        int num;

        offset = pmatch[i].rm_so;
        length = pmatch[i].rm_eo - pmatch[i].rm_so;
        if (pmatch[i].rm_so == -1) {
            continue;
        }
        strncpy(temp, instr + offset, length);
        switch (i) {
        case 2:
            res += MOVI;
            continue;
        case 3:
            num = atoi(temp);
            num &= 0x7F;
            res += num;
            continue;
        case 4:
            res += operation_mnem_val(temp);
            continue;
        case 5:
            num = register_val(temp);
            res += num << 2;
            continue;
        case 6:
            res += register_val(temp);
            continue;
        case 7:
            res += io_mnem_val(temp);
            continue;
        case 8:
            res += register_val(temp);
            continue;
        }
    }

    printf("%#02x ", res);
}

int read_mnemonic(char *instruction)
{
    int ch;
    int i = 0;
    while ((ch = getchar()) != '\n') {
        if (ch == EOF) {
            instruction[i++] = '\0';
            return 0;
        }
        instruction[i++] = ch;
    }
    instruction[i++] = '\0';
    return 1;
}

int process_instruction(char *instruction, regex_t * rx,
                        regmatch_t * pmatch)
{
    int ret;
    if (instruction[0] == '\0') {
        return 1;
    }
    ret = exec_regex(rx, instruction, P_MATCH_S, pmatch);

    if (ret != REG_NOMATCH) {
        mnemonic_to_hex(instruction, pmatch);
        return 1;
    }

    printf("ERROR");
    return 0;
}

void handle_instructions()
{
    regex_t rx;
    regmatch_t pmatch[P_MATCH_S];
    char instruction[1024] = { 0 };
    int fail = 0;

    compile_regex(&rx, MNEMONIC_RE);

    while (read_mnemonic(instruction)) {
        if (!process_instruction(instruction, &rx, pmatch)) {
            fail = 1;
            break;
        }
    }
    if (!fail) {
        process_instruction(instruction, &rx, pmatch);
    }
    printf("\n");
    regfree(&rx);
}

int main()
{
    handle_instructions();
}
