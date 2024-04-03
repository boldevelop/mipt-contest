# guess risc-v | the 32 x registers are each 32 bits wide
# a0 a1 a2 a3 a4 a5 a6 a7 
# t1 -- t3 t4

# addi can add an immediate value in the range -2048 to 2047

# A load reads a value from memory into a register.
# b - byte - 8 bits
# h - half word - 16 bits (2 bytes)
# w - word - 32 bits (4 bytes)
# d - double word - 64 bits (8 bytes)

# lw rd, offset(rs1)
# The offset is a 12-bit signed immediate,
# so can reach addresses -2048 to +2047 bytes from the the address in rs1

foo:
# a5 = a1 << 32 or a5 = 0 !
    slli    a5, a1, 32
# t4 = a5 >> 30
    srli    t4, a5, 30
# t4 = a0 + t4;
    add     t4, a0, t4
# t3 = *t4;
    lw      t3, 0(t4)
# a6 = a0
    mv      a6, a0
# Sign extend word
# a7 = a2;
    sext.w  a7, a2
# if (a1 >= a2) goto L7 (end)
    bgeu    a1, a2, .L7 # ------------*
# t1 = a7 << 2
    slli    t1, a7, 2   #             |
# a5 = a0 + t1;
    add     a5, a0, t1  #             |
# a4 = a7
    mv      a4, a7      #             |
.L6: # <---------------------------*  |
# a3 = *a5;
    lw      a3, 0(a5)   #           | |
# a2 = a6 + t1;
    add     a2, a6, t1  #           | |
# a4 = a4 - 1;
    addiw   a4, a4, -1  #           | |
# if (a3 <= t3) goto: L5    Branch If Less Than or Equal
    ble     a3, t3, .L5 #  -*       | |
# a0 = *a2;
    lw      a0, 0(a2)   #   |       | |
# a7 = a7 - 1;
    addiw   a7, a7, -1  #   |       | |
# *a2 = a3;
    sw      a3, 0(a2)   #   |       | |
# *a5 = a0;
    sw      a0, 0(a5)   #   |       | |
# t1 = a7 << 2
    slli    t1, a7, 2   #   |       | |
.L5: #  <-------------------*       | |
# a5 = a5 - 4;
    addi    a5, a5, -4  #           | |
# if (a1 < a4) goto L6              | |
    bltu    a1, a4, .L6 #  ---------* |
# t3 = *t4;
    lw      t3, 0(t4)   #             |
# a6 = a6 + t1;
    add     a6, a6, t1  #             |
# a5 = *a6;
    lw      a5, 0(a6)   #             |
# *a6 = t3;
    sw      t3, 0(a6)   #             |
# a0 = a7;
    sext.w  a0, a7      #             |
# *t4 = a5;
    sw      a5, 0(t4)   #             |
    ret                 #             |
                        #             |
.L7: # <------------------------------*
# t1 = a7 << 2;
    slli    t1, a7, 2
# a6 = a6 + t1;
    add     a6, a6, t1
# a5 = *a6;
    lw      a5, 0(a6)
# *a6 = t3;
    sw      t3, 0(a6)
# a0 = a2
    mv      a0, a2
# *t4 = a5;
    sw      a5, 0(t4)
    ret


foo:
    slli    a5, a1, 32
    srli    t4, a5, 30
    add     t4, a0, t4
    lw      t3, 0(t4)
    mv      a6, a0
    sext.w  a7, a2
    bgeu    a1, a2, .L7

    slli    t1, a7, 2   

    add     a5, a0, t1  

    mv      a4, a7      
.L6: 

    lw      a3, 0(a5)   

    add     a2, a6, t1  

    addiw   a4, a4, -1  

    ble     a3, t3, .L5 

    lw      a0, 0(a2)   

    addiw   a7, a7, -1  

    sw      a3, 0(a2)   

    sw      a0, 0(a5)   

    slli    t1, a7, 2   
.L5: 

    addi    a5, a5, -4  

    bltu    a1, a4, .L6 

    lw      t3, 0(t4)   

    add     a6, a6, t1  

    lw      a5, 0(a6)   

    sw      t3, 0(a6)   

    sext.w  a0, a7      

    sw      a5, 0(t4)   
    ret                 
                        
.L7: 

    slli    t1, a7, 2

    add     a6, a6, t1

    lw      a5, 0(a6)

    sw      t3, 0(a6)

    mv      a0, a2

    sw      a5, 0(t4)
    ret
