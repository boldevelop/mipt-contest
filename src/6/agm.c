/*
// --- int f(int *x, int y, int z) ---
// edi = x
// esi = y
// edx = z
// return value = eax
    .globl  foo
    .type foo, @function
  foo:
    xor r8d, r8d
    test  edx, edx
    je  .L1
    mov eax, edx
    sub eax, 1
    js  .L7
    cdqe
    jmp .L4
  .L5:
    mov DWORD PTR 4[rdi+rax*4], edx
    sub rax, 1
    test  eax, eax
    js  .L1
  .L4:
    mov edx, DWORD PTR [rdi+rax*4]
    mov r8d, eax
    cmp edx, esi
    jg  .L5
    add r8d, 1
  .L1:
    mov eax, r8d
    ret
  .L7:
    mov r8d, edx
    jmp .L1
*/

#include <stdio.h>
#include <stdlib.h>

/* 
ZF - zero flags (je)
SF - sign flags (js Jump short if sign (SF=1).) 
OF - Overflow flag  
indicating that the signed two's-complement result
would not fit in the number of bits used for the result.

CF - carry (borrow) flag

CDQE	REX.W + 98		RAX ← sign-extend of EAX.

Косвенная адресация регистров со смещением и масштабированием
([reg64 + offset + reg64 * scale])

jg / jnle: проверяет одновременно два условия SF == OF и ZF == 0
 */

int foo(int *arr, int size, int start_i)
{
    int eax, r8d;
    if (start_i == 0) {
        return 0;
    }
    eax = start_i;
    eax--;
    if (eax < 0) {
        return start_i;
    }

  L4:
    start_i = *(arr + eax);
    r8d = eax;
    if (start_i > size) {
        *(arr + eax + 4) = start_i;
        eax--;
        if (eax == 0) {
            return r8d;
        }
        goto L4;
    } else {
        r8d++;
        return r8d;
    }
}

void dump_arr(int *x, int s) {
    for (int i =0; i < s; ++i) {
        printf("%d ", x[i]);
    }
    printf("\n");
}

int main()
{
    int x[] = {1, 2, 4, 5, 3, 2};
    int res = foo(x, 6, 4);
    printf("res = %d\n", res);
    dump_arr(x, 6);
    printf("%d\n", x[res]);
}
