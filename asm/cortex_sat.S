//*****************************************************************************
// Compiler: arm-none-eabi-gcc
//******************************************************************************
//Description: Saturating instructions

//SSAT Signed Saturate SSAT and USAT      on page 3-96
//SSAT16 Signed Saturate Halfword SSAT16 and USAT16       on page 3-97
//USAT Unsigned Saturate SSAT and USAT        on page 3-96
//USAT16 Unsigned Saturate Halfword SSAT16 and USAT16         on page 3-97
//QADD Saturating Add QADD and QSUB       on page 3-98
//QSUB Saturating Subtract QADD and QSUB      on page 3-98
//QSUB16 Saturating Subtract 16 QADD and QSUB         on page 3-98
//QASX Saturating Add and Subtract with Exchange QASX and QSAX        on page 3-100
//QSAX Saturating Subtract and Add with Exchange QASX and QSAX        on page 3-100
//QDADD Saturating Double and Add QDADD and QDSUB         on page 3-102
//QDSUB Saturating Double and Subtract QDADD and QDSUB        on page 3-102
//UQADD16 Unsigned Saturating Add 16 UQADD and UQSUB      on page 3-105
//UQADD8 Unsigned Saturating Add 8 UQADD and UQSUB        on page 3-105
//UQASX Unsigned Saturating Add and Subtract with Exchange UQASX and UQSAX        on page 3-103
//UQSAX Unsigned Saturating Subtract and Add with Exchange UQASX and UQSAX        on page 3-103
//UQSUB16 Unsigned Saturating Subtract 16 UQADD and UQSUB         on page 3-105
//UQSUB8 Unsigned Saturating Subtract 8 UQADD and UQSUB       on page 3-105

        .syntax unified
        .thumb

        .file   "SVC_Table.S"

//        .section ".svc_table"
        .section ".text"

    .global asm_ssat
    .global asm_usat
    .global asm_ssat16
    .global asm_usat16
    .global asm_qadd
    .global asm_qsub

#define inputA          r0
#define inputB          r1
#define result          r0

    .align 4

asm_ssat:
    ssat result, #30, inputA
    bx lr

asm_usat:
    usat result, #30, inputA
    bx lr

asm_ssat16:
    ssat16 result, #14, inputA
    bx lr

asm_usat16:
    usat16 result, #14, inputA
    bx lr

asm_qadd:
    qadd result, inputA, inputB
    bx lr

asm_qsub:
    qsub result, inputA, inputB
    bx lr

asm_uqadd16:
    uqadd16 result, inputA, inputB
    bx lr

asm_uqsub16:
    uqsub16 result, inputA, inputB
    bx lr

    .end
