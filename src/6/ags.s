# функция caller в System V AMD ABI, вызывающая из себя функцию callee

/*
    Parameters to functions are passed in the registers rdi, rsi, rdx, rcx, r8, r9
    and further values are passed on the stack in reverse order

    Functions preserve the registers rbx, rsp, rbp, r12, r13, r14, and r1
    while rax, rdi, rsi, rdx, rcx, r8, r9, r10, r11 are scratch registers.
    
    (rdi, rsi, rbx, rcx, r8, r9)
*/
  .text
  .globl  caller
  .type caller, @function
caller:
  push  r13
  push  r12
  push  rbp
  push  rbx
  sub rsp, 8
  # r12d = 1;
  mov r12d, 1
  # i (RSI == 0) return 1;
  test  esi, esi
  je  exit
  # eax = rdi
  mov eax, edi
  # ebx = rsi
  mov ebx, esi
  # r13 = rdx
  mov r13d, edx
  # RDX = 0
  mov edx, 0
  # div eax % r13 -> eax, reminder edx
  div r13d
  # reminder set to : rbp = (eax % r13)
  mov rbp, rdx
  # r12 = 1
  mov r12d, 1
  jmp .L5
composite:
  mov edi, ebp
  mov edx, r13d
  mov esi, ebp
/*
rdi edi
rsi esi
rdx edx
rcx ecx
r8 r12d
r9 r15d
*/
  call  callee
  mov ebp, eax
  shr ebx
.L4:
  test  ebx, ebx
  je  exit
.L5:
# RBX	    EBX	    BX	    BH
  test  bl, 1
  je  composite
  mov edx, r13d
  mov esi, ebp
  mov edi, r12d
  call  callee
  mov r12d, eax
  sub ebx, 1
  jmp .L4
exit:
  mov eax, r12d
  add rsp, 8
  pop rbx
  pop rbp
  pop r12
  pop r13
  ret