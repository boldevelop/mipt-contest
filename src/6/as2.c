/* декодер
необходимо распечатать hex ввод как текст программы
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
    MOV_T,
    OP_T,
    IO_T
} InstrType;

typedef struct {
    InstrType type;
    MNEMONIC mnem;
    int arg1;
    int arg2;
} Instr;

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

void print_args(Instr * instr)
{
    switch (instr->type) {
    case MOV_T:
        printf("%d", instr->arg1);
        break;
    case OP_T:
        print_register(instr->arg1);
        printf(", ");
        print_register(instr->arg2);
        break;
    case IO_T:
        print_register(instr->arg1);
        break;
    }
}

void print_mnem(Instr * instr)
{
    switch (instr->mnem) {
    case MOVI:
        printf("MOVI ");
        break;
    case ADD:
        printf("ADD ");
        break;
    case SUB:
        printf("SUB ");
        break;
    case MUL:
        printf("MUL ");
        break;
    case DIV:
        printf("DIV ");
        break;
    case IN:
        printf("IN ");
        break;
    case OUT:
        printf("OUT ");
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
            return 0;
        }
        instr->mnem = mnem;
        instr->arg1 = hval & 0x3;
        return 1;
    }
    instr->type = OP_T;
    mnem = hval & ~0xf;
    if (mnem != SUB && mnem != DIV && mnem != MUL && mnem != ADD) {
        return 0;
    }
    instr->mnem = mnem;
    instr->arg2 = hval & 0x3;
    hval >>= 2;
    instr->arg1 = hval & 0x3;
    return 1;
}

void print_instr(Instr * instr)
{
    print_mnem(instr);
    print_args(instr);
    printf("\n");
}

int readhex(unsigned int *buf)
{
    if (scanf("%x", buf) != 1) {
        return 0;
    }
    return 1;
}

int main()
{
    unsigned int hex;
    while (readhex(&hex)) {
        Instr instr = { 0 };
        if (!contruct_instr(&hex, &instr)) {
            printf("ERROR\n");
            break;
        }
        print_instr(&instr);
    }
}
