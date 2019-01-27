//*****************************************************************************
// Compiler: arm-none-eabi-gcc
//******************************************************************************
//Description: Table Branch Byte and Table Branch Halfword.

//These instructions cause a PC-relative forward branch using a table of single byte offsets for TBB,
//or halfword offsets for TBH. Rn provides a pointer to the table, and Rm supplies an index into the
//table. For TBB the branch offset is twice the unsigned value of the byte returned from the table.
//and for TBH the branch offset is twice the unsigned value of the halfword returned from the table.
//The branch occurs to the address at that offset from the address of the byte immediately after
//the TBB or TBH instruction.
        .cpu cortex-m4
        .syntax unified
        .thumb

        .file   "cortex_m4_tb.S"

        .section ".text"

    .global asm_test_tbb
    .global asm_test_tbh

#define inputA          r0
#define inputB          r1
#define result          r0

    .align 4
asm_test_tbb:
    adr R1, BranchTable_Byte
    TBB [R1, inputA] // inputA is the index, R1 is the base address of the
Case0:
    mov result, #10
    bx lr
Case1:
    mov result, #11// an instruction sequence follows
    bx lr
Case2:
    mov result, #12// an instruction sequence follows
    bx lr
Case3:
    mov result, #13// an instruction sequence follows
    bx lr
    // branch table
BranchTable_Byte:
    .byte 0
    .byte ((Case1-Case0)/2)
    .byte ((Case2-Case0)/2)
    .byte ((Case3-Case0)/2)
    bx lr

asm_test_tbh:
    TBH    [PC, inputA, LSL #1]   // inputA is the index, PC is used as base of the
                              // branch table
BranchTable_H:
    .short    ((CaseA - BranchTable_H)/2)  // CaseA offset calculation
    .short    ((CaseB - BranchTable_H)/2)  // CaseB offset calculation
    .short    ((CaseC - BranchTable_H)/2)  // CaseC offset calculation

CaseA:
    mov result, #20
    bx lr
CaseB:
    mov result, #21
    bx lr
CaseC:
    mov result, #22
    bx lr

    .end
