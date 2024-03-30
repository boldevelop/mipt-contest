/* 
Parameters to functions are passed in the registers rdi, rsi, rdx, rcx, r8, r9
and further values are passed on the stack in reverse order

Functions preserve the registers rbx, rsp, rbp, r12, r13, r14, and r1
while rax, rdi, rsi, rdx, rcx, r8, r9, r10, r11 are scratch registers.

64-bit	32-bit	16-bit	8 high bits 8-bit
RAX	    EAX	    AX	    AH	        AL	        Accumulator
RBX	    EBX	    BX	    BH	        BL	        Base
-> RCX	    ECX	    CX	    CH	        CL	        Counter
-> RDX	    EDX	    DX	    DH	        DL	        Data (commonly extends the A register)
-> RSI	    ESI	    SI	    N/A	        SIL	        Source index for string operations
-> RDI	    EDI	    DI	    N/A	        DIL	        Destination index for string operations
RSP	    ESP	    SP	    N/A	        SPL	        Stack Pointer
RBP	    EBP	    BP	    N/A	        BPL	        Base Pointer (meant for stack frames)
-> R8	    R8D	    R8W	    N/A	        R8B	        General purpose
-> R9	    R9D	    R9W	    N/A	        R9B	        General purpose
R10	    R10D	R10W	N/A	        R10B	    General purpose
R11	    R11D	R11W	N/A	        R11B	    General purpose
R12	    R12D	R12W	N/A	        R12B	    General purpose
R13	    R13D	R13W	N/A	        R13B	    General purpose
R14	    R14D	R14W	N/A	        R14B	    General purpose
R15	    R15D	R15W	N/A	        R15B	    General purpose
 */

extern unsigned callee(unsigned rdi, unsigned rsi, unsigned rdx);

unsigned caller(unsigned rdi, unsigned rsi, unsigned rdx) {
    unsigned r12 = 1;
    unsigned rbp;
    if (rsi == 0) {
        return r12;
    }
    rbp = (rdi % rdx);
    r12 = 1;

    while (rsi != 0) {
        if (rsi % 2 != 1) {
            rbp = callee(rbp, rbp, rdx);
            rsi >>= 1;
            continue;
        }
        r12 = callee(r12, rbp, rdx);
        rsi--;
    }

    return r12;
}