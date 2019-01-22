//*****************************************************************************
// File: cortex_m0_test_8.S
// Compiler: arm-none-eabi-gcc
//******************************************************************************
    .thumb

    .global asm_get_8bit_number
    .global asm_get_xor
    .global asm_direct_jump_1
    .global asm_direct_jump_2

#define inputA          r0
#define inputB          r1
#define result          r0

    .align 4

//Get an 8bit immediate number
asm_get_8bit_number:
    MOVS r0, #234
    bx lr

//Get the XOR result
asm_get_xor:
    eor r1, r0, r1
    movs r0, r1
    bx lr

//Direct Jump Test 1
asm_direct_jump_1:
//The following line will get warning:
//MOV pc,<rn> instruction used, but BX <rn> is preferred
    mov PC,r0

//Direct Jump Test 2
asm_direct_jump_2:
    bx r0

    .end
