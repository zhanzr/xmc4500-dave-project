//*****************************************************************************
// Compiler: arm-none-eabi-gcc
//******************************************************************************
//Description: Floating-point instructions
//These instructions are only available if the FPU is included, and enabled, in the system.
//
        .cpu cortex-m4
        .syntax unified
        .thumb

        .file   "cortex_m4_div.S"

        .section ".text"

    .global asm_simple_udiv
    .global asm_simple_sdiv

#define inputA          r0
#define inputB          r1
#define result          r0

    .align 4
//Simple UDIV
asm_simple_udiv:
    UDIV result, inputA, inputB
    bx lr

//Simple SDIV
asm_simple_sdiv:
    SDIV result, inputA, inputB
    bx lr

    .end
