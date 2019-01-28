//*****************************************************************************
// Compiler: arm-none-eabi-gcc
//******************************************************************************
//Description: Floating-point instructions
//These instructions are only available if the FPU is included, and enabled, in the system.
//
//VMOV Floating-point Move Immediate VMOV Immediate on page 3-140
//VMOV Floating-point Move Register VMOV Register on page 3-141
//VMOV Copy ARM core register to single precision VMOV ARM Core register to single precision on
//page 3-143
//VMOV Copy 2 ARM core registers to 2 single precision VMOV Two ARM Core registers to two single precision
//on page 3-14
//VMOV Copies between ARM core register to scalar VMOV ARM Core register to scalar on page 3-145
//VMOV Copies between Scalar to ARM core register VMOV Scalar to ARM Core register on page 3-142
//VMRS Move to ARM core register from floating-point System
//Register
//VMRS on page 3-146
//VMSR Move to floating-point System Register from ARM Core
//register
//VMSR on page 3-147
//VMUL Multiply floating-point VMUL on page 3-148
//VNEG Floating-point negate VNEG on page 3-149
//VNMLA Floating-point multiply and add VNMLA, VNMLS, VNMUL on page 3-150
//VNMLS Floating-point multiply and subtract VNMLA, VNMLS, VNMUL on page 3-150
//VNMUL Floating-point multiply VNMLA, VNMLS, VNMUL on page 3-150
//VPOP Pop extension registers VPOP on page 3-151
//VPUSH Push extension registers VPUSH on page 3-152
//VSQRT Floating-point square root VSQRT on page 3-153
//VSTM Store Multiple extension registers VSTM on page 3-154
//VSTR Stores an extension register to memory VSTR on page 3-155
//VSUB Floating-point Subtract VSUB on page 3-156

        .cpu cortex-m4
        .syntax unified
        .thumb

        .file   "cortex_m4_float2.S"

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
