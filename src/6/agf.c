/* 
// --- int f(int x, int y) ---
// edi = x
// esi = y
// return value = eax
  .globl  foo
  .type foo, @function
foo:
  mov eax, edi
  imul  eax, edi
  imul  eax, edi
  test  eax, eax
  js  .L4
  ret
.L4:
  imul  esi, esi
  add edi, edi
  mov edx, 17
  sub edx, edi
  sub edx, esi
  add eax, edx
  ret
*/

#include <stdio.h>
#include <stdlib.h>

int foo(int x, int y)
{
    int tmp = x * x * x;
    if (tmp >= 0) {
        return tmp;
    }

    return tmp + (17 - 2 * x - y * y);
}

#ifdef DEBUG
int main()
{
    printf("%d\n", foo(3, 4));
    printf("%d\n", foo(-3, 4));
}
#endif
