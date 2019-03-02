//*****************************************************************************
// Compiler: arm-none-eabi-gcc
//******************************************************************************
//Description: Floating-point instructions
//These instructions are only available if the FPU is included, and enabled, in the system.

//VABS Floating-point Absolute VABS on page 3-128
//VADD Floating-point Add VADD on page 3-129
//VCMP Compare two floating-point registers, or one floating-point
//register and zero
//VCMP, VCMPE on page 3-130
//VCMPE Compare two floating-point registers, or one floating-point
//register and zero with Invalid Operation check
//VCMP, VCMPE on page 3-130
//VCVT Convert between floating-point and integer VCVT, VCVTR between floating-point and integer on
//page 3-131
//VCVT Convert between floating-point and fixed point VCVT between floating-point and fixed-point on
//page 3-132
//VCVTR Convert between floating-point and integer with rounding VCVT, VCVTR between floating-point and integer on
//page 3-131
//VCVTB Converts half-precision value to single-precision VCVTB, VCVTT on page 3-133
//VCVTT Converts single-precision register to half-precision VCVTB, VCVTT on page 3-133
//VDIV Floating-point Divide VDIV on page 3-134
//VFMA Floating-point Fused Multiply Accumulate VFMA, VFMS on page 3-135
//VFNMA Floating-point Fused Negate Multiply Accumulate VFNMA, VFNMS on page 3-136
//VFMS Floating-point Fused Multiply Subtract VFMA, VFMS on page 3-135
//VFNMS Floating-point Fused Negate Multiply Subtract VFNMA, VFNMS on page 3-136
//VLDM Load Multiple extension registers VLDM on page 3-137
//VLDR Loads an extension register from memory VLDR on page 3-138
//VLMA Floating-point Multiply Accumulate VLMA, VLMS on page 3-139
//VLMS Floating-point Multiply Subtract VLMA, VLMS on page 3-139
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

        .file   "cortex_m4_float.S"

        .section ".text"

    .global asm_vabs
    .global asm_vadd
    .global asm_vsub
    .global asm_vcmp
    .global asm_vcvt_s32
    .global asm_vcvt_u32
    .global asm_vmul
    .global asm_vdiv
    .global asm_vsqrt
    .global asm_vfma
    .global asm_vfms
    .global asm_vmla
    .global asm_vmls

#define inputA          r0
#define inputB          r1
#define inputC          r2
#define result          r0

    .align 4
//Floating-point Absolute VABS on page 3-128
asm_vabs:
    VMOV.F32 S0, inputA
    VABS.F32 S0, S0
    VMOV.F32 result, S0
    bx lr

asm_vsqrt:
    VMOV.F32 S0, inputA
    VSQRT.F32 S0, S0
    VMOV.F32 result, S0
    bx lr

asm_vadd:
    VMOV.F32 S0, inputA
    VMOV.F32 S1, inputB
    VADD.F32 S0, S0, S1
    VMOV.F32 result, S0
    bx lr

asm_vsub:
    VMOV.F32 S0, inputA
    VMOV.F32 S1, inputB
    VSUB.F32 S0, S0, S1
    VMOV.F32 result, S0
    bx lr

//VCMP.F32 S4, #0.0
asm_vcmp:
    VMOV.F32 S0, inputA
    VMOV.F32 S1, inputB
    VCMP.F32 S0, S1
    VMRS result, FPSCR
    bx lr

asm_vcvt_s32:
    VMOV.F32 S0, inputA
    VCVT.S32.F32 S0, S0
    VMOV.F32 result, S0
    bx lr

asm_vcvt_u32:
    VMOV.F32 S0, inputA
    VCVT.U32.F32 S0, S0
    VMOV.F32 result, S0
    bx lr

asm_vmul:
    VMOV.F32 S0, inputA
    VMOV.F32 S1, inputB
    VMUL.F32 S0, S0, S1
    VMOV.F32 result, S0
    bx lr

asm_vdiv:
    VMOV.F32 S0, inputA
    VMOV.F32 S1, inputB
    VDIV.F32 S0, S0, S1
    VMOV.F32 result, S0
    bx lr

//Fused MAC instructions: intermediate result not rounded Better accuracy
asm_vfma:
    VMOV.F32 S0, inputA
    VMOV.F32 S1, inputB
    VMOV.F32 S2, inputC
    VFMA.F32 S0, S1, S2
    VMOV.F32 result, S0
    bx lr

asm_vfms:
    VMOV.F32 S0, inputA
    VMOV.F32 S1, inputB
    VMOV.F32 S2, inputC
    VFMS.F32 S0, S1, S2
    VMOV.F32 result, S0
    bx lr

asm_vmla:
    VMOV.F32 S0, inputA
    VMOV.F32 S1, inputB
    VMOV.F32 S2, inputC
    VMLA.F32 S0, S1, S2
    VMOV.F32 result, S0
    bx lr

asm_vmls:
    VMOV.F32 S0, inputA
    VMOV.F32 S1, inputB
    VMOV.F32 S2, inputC
    VMLS.F32 S0, S1, S2
    VMOV.F32 result, S0
    bx lr

    .end
