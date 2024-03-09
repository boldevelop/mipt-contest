/*  необходимо симулировать работу программы и вывести всё что она выводит на stdout
    приходит закодированная программа как параметр argv[1] и стандартный ввод на stdin

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    MOVI = 0x00,
    ADD = 0x80,
    SUB = 0x90,
    MUL = 0xA0,
    DIV = 0xB0,
    IN = 0xC0,
    OUT = 0xC4,
} MNEMONIC;

typedef enum {
    A,
    B,
    C,
    D,
} REGISTERS;

typedef enum {
    INV_T,
    MOV_T,
    OP_T,
    IO_T
} InstrType;

typedef struct {
    InstrType type;
    MNEMONIC mnem;
    unsigned int arg1;
    unsigned int arg2;
} Instr;

typedef struct {
    unsigned int registers[4];
    Instr instrs[1024];
    size_t i_count;
} Env;

void print_register(REGISTERS reg)
{
    switch (reg) {
    case A:
        printf("A");
        break;
    case B:
        printf("B");
        break;
    case C:
        printf("C");
        break;
    case D:
        printf("D");
        break;
    }
}

int check_bit(unsigned int val, unsigned int b)
{
    return (val >> b) & 1;
}

int contruct_instr(unsigned int *hex, Instr * instr)
{
    unsigned int hval = *hex;
    int mnem;
    hval &= 0xFF;
    if (!check_bit(hval, 7)) {
        instr->type = MOV_T;
        instr->mnem = MOVI;
        instr->arg1 = hval & 0x7f;
        return 1;
    }
    if (check_bit(hval, 6)) {
        mnem = hval & ~0x3;
        instr->type = IO_T;
        if (mnem != IN && mnem != OUT) {
            instr->type = INV_T;
            return 0;
        }
        instr->mnem = mnem;
        instr->arg1 = hval & 0x3;
        return 1;
    }
    instr->type = OP_T;
    mnem = hval & ~0xf;
    if (mnem != SUB && mnem != DIV && mnem != MUL && mnem != ADD) {
        instr->type = INV_T;
        return 0;
    }
    instr->mnem = mnem;
    instr->arg2 = hval & 0x3;
    hval >>= 2;
    instr->arg1 = hval & 0x3;
    return 1;
}

int readhex(FILE * file, unsigned int *buf)
{
    if (fscanf(file, "%x", buf) != 1) {
        return 0;
    }
    return 1;
}

int readint()
{
    int buf;
    if (scanf("%d", &buf) != 1) {
        abort();
    }
    return buf;
}

void eval(unsigned int registers[4], Instr * instr)
{
    int temp;
    switch (instr->mnem) {
    case MOVI:
        registers[D] = instr->arg1;
        break;
    case ADD:
        temp = registers[instr->arg1] + registers[instr->arg2];
        temp %= 256;
        registers[instr->arg1] = temp;
        break;
    case SUB:
        temp = registers[instr->arg1] - registers[instr->arg2];
        if (temp < 0) {
            temp = 256 + temp;
        }
        registers[instr->arg1] = temp;
        break;
    case MUL:
        temp = registers[instr->arg1] * registers[instr->arg2];
        temp %= 256;
        registers[instr->arg1] = temp;
        break;
    case DIV:
        temp = registers[instr->arg1] / registers[instr->arg2];
        temp %= 256;
        registers[instr->arg1] = temp;
        break;
    case IN:
        temp = readint();
        temp %= 256;
        registers[instr->arg1] = temp;
        break;
    case OUT:
        printf("%d\n", registers[instr->arg1]);
        break;
    }
}

void run(Env * env)
{
    for (size_t i = 0; i < env->i_count; ++i) {
        eval(env->registers, env->instrs + i);
    }
}

FILE *open_file(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror(path);
        abort();
    }
    return fp;
}

int main( __attribute__((unused))
         int argc, char **argv)
{
    unsigned int hex;
    FILE *prog_file = open_file(argv[1]);
    size_t i = 0;
    Env env = { 0 };

    while (readhex(prog_file, &hex)) {
        if (!contruct_instr(&hex, env.instrs + i)) {
            printf("ERROR\n");
            break;
        }
        i++;
    }
    env.i_count = i;

    fclose(prog_file);
    run(&env);
}
