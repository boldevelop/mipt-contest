# (rdi, rsi, rbx, rcx, r8, r9)

  .globl  foo
  .type foo, @function
foo:
# In x86-64 it zeroes the top 32 bits of rdi.
  mov       edi, edi
# to zero xmm1
  pxor      xmm1, xmm1
# 24 bytes for local variables
  sub       rsp, 24
# double xmm1 = rdi (64 bits wide, 8 bytes)
  cvtsi2sd  xmm1, rdi
# movapd - Move Aligned Packed Double Precision Floating-Point Values
# double xmm0 = xmm1
  movapd    xmm0, xmm1
# save xmm1 to a local variable [rsp]
  movsd     QWORD PTR [rsp], xmm1
  call      log
# save xmm0 to a local variable [rsp+8]
  movsd     QWORD PTR [rsp+8], xmm0
# restore local saved xmm1 to xmm0
  movsd     xmm0, QWORD PTR [rsp]
  call      log
  call      log
# xmm0 += local saved [rsp+8]
  addsd     xmm0, QWORD PTR [rsp+8]
# xmm0 *= local saved [rsp]
  mulsd     xmm0, QWORD PTR [rsp]
  call      round
  add       rsp, 24
# return xmm0;
  cvttsd2si rax, xmm0
  ret