# ARM  foo(x, y)
# x - 64 bits
# w - 32 low bits of x reg
foo:
# store address to x6
    mov     x6, x0
# load value to w0 (x)
    ldr     w0, [x0]
# if (y <= 1) return w0
    cmp     w1, 1
    ble     .L1
# x3 = 1 
    mov     x3, 1
# (w2) = x3 = 1
    mov     w2, w3
# w7 = x3 = 1
    mov     w7, w3

.L6:
# w4 = x6+x3*4; w4 = *(x + x3) next int
    ldr     w4, [x6, x3, lsl 2]

# w5 = w2 - 1
    sub     w5, w2, #1
# i: w3++;
    add     x3, x3, 1
    cmp     w4, w0
# w2 = (w4 != w0) ? w5 : w2 + 1 (0 или 2)
    csinc   w2, w5, w2, ne
    cmp     w2, 0
# w0 = w2 == 0 ? w4 : w0;
    csel    w0, w4, w0, eq
# w2 = w2 != 0 ? w2 : w7;
    csel    w2, w2, w7, ne

# if (y <= w3) return ...
    cmp     w1, w3
    bgt     .L6
# for (int i = 1; i < y; ++i) {}
.L1:
    ret