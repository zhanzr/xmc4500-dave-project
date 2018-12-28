XMC4500 Relax Lite Kit. Dave project Demo.

Whetstone single float, double float test.
With and without FPU.

Soft FPU with softFPU abi. XMC4500, GCC4.9.3 -O3
[Dave Could not debug, no output]
Soft FPU with softFPU abi. [fastmath] XMC4500, GCC4.9.3 -O3
[Dave Could not debug, no output]

Hardware FPU with softFPU abi. XMC4500, GCC4.9.3 -O3
120000000 Hz, 410FC241, CM:4, FPU_USED:1
 Benchmark #2 -- Single Precision Whetstone (A001)
 2000 LOOPS
time=10.112000 secs, Single Whetstone MIPS 19.778
 Benchmark #2 -- Double Precision Whetstone (A001)
 2000 LOOPS
time=8.739000 secs, Double Whetstone MIPS 22.886

Hardware FPU with softFPU abi. [fastmath] XMC4500, GCC4.9.3 -O3
120000000 Hz, 410FC241, CM:4, FPU_USED:1
 Benchmark #2 -- Single Precision Whetstone (A001)
 2000 LOOPS
time=0.235000 secs, Single Whetstone MIPS 851.064
 Benchmark #2 -- Double Precision Whetstone (A001)
 2000 LOOPS
time=2.804000 secs, Double Whetstone MIPS 71.327

 Full Hardware ABI.
120000000 Hz, 410FC241, CM:4, FPU_USED:1
 Benchmark #2 -- Single Precision Whetstone (A001)
 2000 LOOPS
time=10.383000 secs, Single Whetstone MIPS 19.262
 Benchmark #2 -- Double Precision Whetstone (A001)
 2000 LOOPS
time=9.050000 secs, Double Whetstone MIPS 22.099

 Full Hardware ABI. [fastmath]
120000000 Hz, 410FC241, CM:4, FPU_USED:1
 Benchmark #2 -- Single Precision Whetstone (A001)
 2000 LOOPS
time=0.235000 secs, Single Whetstone MIPS 851.064
 Benchmark #2 -- Double Precision Whetstone (A001)
 2000 LOOPS
time=2.804000 secs, Double Whetstone MIPS 71.327
