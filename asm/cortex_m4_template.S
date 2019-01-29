//*****************************************************************************
// Compiler: arm-none-eabi-gcc
//******************************************************************************
//Description: Floating-point instructions
//These instructions are only available if the FPU is included, and enabled, in the system.
//

        .cpu cortex-m4
        .syntax unified
        .thumb

        .file   "cortex_m4_template.S"

        .section ".text"

//    .global asm_test_tbb
//    .global asm_test_tbh

#define inputA          r0
#define inputB          r1
#define result          r0

    .align 4

    .end
