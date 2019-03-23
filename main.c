/*
 * main.c
 *
 *  Created on: 2018 Dec 28 00:03:20
 *  Author: zzr
 */




#include <DAVE.h>                 //Declarations from DAVE Code Generation (includes SFR declaration)

#include  <errno.h>
#include  <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO

#include <xmc_scu.h>
#include <xmc_fce.h>

#include <math.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "asm_prototype.h"
#include "test_header.h"

/* Set mainCREATE_SIMPLE_BLINKY_DEMO_ONLY to one to run the simple blinky demo,
or 0 to run the more comprehensive test and demo application. */
#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY	1
/*-----------------------------------------------------------*/
#define	RETARGET_SWD	0
#define	RETARGET_UART	1

//#define	STDIO_RETARGET	RETARGET_SWD
#define	STDIO_RETARGET	RETARGET_UART

//Retarget IO
#if(STDIO_RETARGET == RETARGET_SWD)
int _write(int file, char *data, int len) {
	int i;

	if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
		errno = EBADF;
		return -1;
	}

	for (i = 0; i < len; ++i) {
		ITM_SendChar(*(data + i));
	}

	// return # of bytes written - as best we can tell
	return len;
}
#elif(STDIO_RETARGET == RETARGET_UART)
int _write(int file, char *data, int len) {
	int i;

	if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
		errno = EBADF;
		return -1;
	}

	UART_Transmit(&UART_0, data, len);

	// return # of bytes written - as best we can tell
	return len;
}
#endif

void TestFunct(void) {
	printf("CPUID:%08X\n", SCB->CPUID);
}

void __SVC_1( uint32_t in ) __attribute__ (( naked ));

void __SVC_1( uint32_t in ) {
	printf("%s:%08X\n",
			__func__,
			SCB->CPUID);
}

uint32_t __attribute__((section(".data"))) g_var1 = 1;
uint32_t __attribute__((section(".bss"))) g_var2 = 2;
uint32_t __attribute__((section(".no_init"))) g_var3 = 3;

uint32_t __attribute__((section("PSRAM_DATA"))) g_var4 = 4;
uint32_t __attribute__((section("DSRAM2_DATA"))) g_var5 = 5;

void print_global_var_test(void) {
	printf(" %p-%u \n %p-%u \n %p-%u \n %p-%u \n %p-%u\n",
			&g_var1, g_var1,
			&g_var2, g_var2,
			&g_var3, g_var3,
			&g_var4, g_var4,
			&g_var5, g_var5);
}

#define __ASM __asm /*!< asm keyword for GNU Compiler */
#define __INLINE inline /*!< inline keyword for GNU Compiler */
#define __STATIC_INLINE static inline

/**
\brief Get Link Register
\details Returns the current value of the Link Register (LR).
\return LR Register value
*/
__attribute__( ( always_inline ) ) __STATIC_INLINE uint32_t __get_LR(void)
{
  register uint32_t result;

  __ASM volatile ("MOV %0, LR\n" : "=r" (result) );
  return(result);
}

uint32_t g_lr_val[3];

void test_rom_func(void) {
	//	printf("%08X %08X %08X\n",
	//			SCB->VTOR,
	//			SCU_GENERAL->IDCHIP,
	//			SCU_GENERAL->IDMANUF);

	//	for(uint32_t i=1; i<32; ++i){
	//		printf("%08X ", *(uint32_t*)(i*sizeof(uint32_t)));
	//	}

	printf("%u\n",  xTaskGetTickCount());
	printf("\nWDT Cnt:%u\n", XMC_WDT_GetCounter());
	XMC_WDT_Service();

}

void test_lr_func(void) {
	g_lr_val[1] = __get_LR();
}

uint32_t SM3_test_suite(void);

void configTOGGLE_LED(void) {
	g_lr_val[0] = __get_LR();
	test_lr_func();
	g_lr_val[2] = __get_LR();

	DIGITAL_IO_ToggleOutput(&DIGITAL_IO_0);
	DIGITAL_IO_ToggleOutput(&DIGITAL_IO_1);
	//	printf("%u Hz, %08X\n", SystemCoreClock, SCB->CPUID);

	//T_DTS = (RESULT - 605) / 2.05 [°C]
	uint32_t tmpDts = XMC_SCU_GetTemperatureMeasurement();
	float tmpCel = (tmpDts-605)/2.05;
	printf("%f\n", tmpCel);

	float tmpV13 = XMC_SCU_POWER_GetEVR13Voltage();
	float tmpV33 = XMC_SCU_POWER_GetEVR33Voltage();
	printf("%f %f\n", tmpV13, tmpV33);

	printf("\n\n\n\n\n\n\n\n\n\n\n");
	printf("Return Addr %08X %08X %08X\n", g_lr_val[0], g_lr_val[1], g_lr_val[2]);

//	uint32_t sm3_ret = SM3_test_suite();
//	printf("\n");
//	printf("SM3 Test Suite Return: %08X\n", sm3_ret);

	//	printf("ASM Test 1 Result:%u\n", asm_get_8bit_number());
	//	printf("ASM Test 2 Result:%08X\t[%08X]\n", asm_get_xor(0x12345678, 0x34567890), 0x12345678^0x34567890);
	//	printf("ASM Test 3 Direct Jump:%08X\n", TestFunct);
	//	printf("Jump 1, Before.%08X\n", __get_MSP());
	//	asm_direct_jump_1(TestFunct);
	//	printf("Jump 1, After.%08X\n\n", __get_MSP());
	//
	//	printf("Jump 2, Before.%08X\n", __get_MSP());
	//	asm_direct_jump_2(TestFunct);
	//	printf("Jump 2, After.%08X\n\n", __get_MSP());
	//
	//
	//	printf("ASM Test 4 :%u\t[%u]\n", asm_add2(34), 34+2);
	//	printf("ASM Test 5 :%u\t[%u]\n", asm_simple_add(123, 456), 123+456);
	//	printf("ASM Test 6 :%u\t[%u]\n", asm_pc_add(), 7);
	//
	//	printf("ASM Test 7 :%d\t[%d]\n", asm_sub20(34), 34-20);
	//	printf("ASM Test 8 :%d\t[%d]\n", asm_simple_sub(123, 456), 123-456);
	//	printf("ASM Test 9 :%d\t[%d]\n", asm_get_neg(1024), 0-1024);
	//
	//	printf("ASM Test 10 Result:%u\t[%u]\n", asm_simple_mul(123, 456), 123*456);
	//
	//	//Test Addition/Mulitiplication Cycles
	//#define	TEST_ADD_MUL_NUM	500000
	//	//If the multiplication takes similar cycles, it is a single cycle multiplication implementation
	//	uint32_t tmpTick = xTaskGetTickCount();
	//	for(uint32_t i=0; i<TEST_ADD_MUL_NUM; ++i)
	//	{
	//		uint32_t tn = 101;
	//		asm_simple_add(tn, 456);
	//	}
	//	tmpTick = xTaskGetTickCount()-tmpTick;
	//	printf("A:%u\n", tmpTick);
	//
	//	tmpTick = xTaskGetTickCount();
	//	for(uint32_t i=0; i<TEST_ADD_MUL_NUM; ++i)
	//	{
	//		uint32_t tn = 101;
	//		asm_simple_mul(tn, 456);
	//	}
	//	tmpTick = xTaskGetTickCount()-tmpTick;
	//	printf("M:%u\n", tmpTick);
	//
	//	//Test Division
	//	{
	//		uint32_t ta = 10;
	//		uint32_t tb = 2;
	//		uint32_t tc = ta/tb;
	//		printf("%u %u %u\n", ta, tb, tc);
	//	}
	//
	//	printf("ASM Test 11 Result:%u\t[%u]\n", asm_test_cmp(123, 456), (123==456));
	//	printf("ASM Test 12 Result:%u\t[%u]\n", asm_test_cmn(123, 456), (123!=456));
	//	printf("ASM Test 13 Result:%08X\t[%08X]\n", asm_get_and(0x12345678, 0x34567890), 0x12345678 & 0x34567890);
	//	printf("ASM Test 14 Result:%08X\t[%08X]\n", asm_get_or(0x12345678, 0x34567890), (0x12345678 | 0x34567890));
	//	printf("ASM Test 15 Result:%08X\t[%08X]\n", asm_get_not(0x12345678), 0-0x12345678);
	//
	//	uint32_t g_TestVar32 = 0x12345678;
	//	printf("ASM Test 20 Result:%08X\t[%08X]\n", asm_ldr32(&g_TestVar32), g_TestVar32);
	//	asm_str32(&g_TestVar32, 0x78904563);
	//	printf("ASM Test 21 Result:%08X\t[%08X]\n", asm_ldr32(&g_TestVar32), g_TestVar32);
	//	printf("ASM Test 22 Result:%u\t[%d]\n", asm_test_push_pop(123, 456), 123+456+456+4);
	//
	//	//Part 7: Test Extend, Reverse
	//	printf("Part 7\n");
	//	printf("ASM Test 23 Result:%08X\t[%08X]\n", asm_s16ext((int16_t)0x8001), (int32_t)0x8001);
	//	printf("ASM Test 24 Result:%08X\t[%08X]\n", asm_s8ext((int8_t)0xC4), (int32_t)0xC4);
	//	printf("ASM Test 25 Result:%08X\t[%08X]\n", asm_u16ext((uint16_t)0x8001), (uint32_t)0x8001);
	//	printf("ASM Test 26 Result:%08X\t[%08X]\n", asm_rev(0x123456C8), __REV(0x123456C8));
	//	printf("ASM Test 27 Result:%08X\t[%08X]\n", asm_rev16(0x123456C8), __REV16(0x123456C8));
	//	printf("ASM Test 28 Result:%08X\t[%08X]\n", asm_revsh(0x123456C8), __REVSH(0x123456C8));
	//
	//	//Part 8: Test SVC, MSR, MRS
	//	printf("Part 8\n");
	//	printf("ASM Test 29, Before SVC\n");
	//	//	asm_svc_1(1000);
	//	printf("After SVC\n");
	//
	//	printf("ASM Test 30 Result:%08X\n", asm_test_mrs());
	//	printf("ASM Test 31 Tick:%u\n", SysTick->VAL);
	//	asm_test_msr(0x00000001);
	//	uint32_t p1 = asm_test_mrs();
	//	asm_test_msr(0x00000000);
	//	uint32_t p2 = asm_test_mrs();
	//	printf("%08X\t%08X\n", p1, p2);

	//	{
	//		uint32_t au = ((1<<31) -1);
	//		printf("USAT %u = %u\n", au, asm_usat(au));
	//
	//		pack32 ap32;
	//		ap32.u16[0] = UINT16_MAX;
	//		ap32.u16[1] = UINT16_MAX/2;
	//		pack32 res_ap32 = asm_usat16(ap32);
	//		printf("USAT16 %u %u = %u %u\n",
	//				ap32.u16[0], ap32.u16[1], res_ap32.u16[0], res_ap32.u16[1]);
	//
	//		int32_t ai = INT32_MAX;
	//		printf("SSAT %i = %i\n", ai, asm_ssat(ai));
	//
	//		ap32.i16[0] = (int16_t)UINT16_MAX;
	//		ap32.i16[1] = INT16_MAX;
	//		res_ap32 = asm_ssat16(ap32);
	//		printf("SSAT16 %i %i = %i %i\n",
	//				ap32.i16[0], ap32.i16[1], res_ap32.i16[0], res_ap32.i16[1]);
	//
	//		int32_t bi = INT32_MAX;
	//		printf("QADD %i+%i = %i\n", ai, bi, asm_qadd(ai, bi));
	//
	//		bi = INT32_MAX/2;
	//		printf("QADD %i+%i = %i\n", ai, bi, asm_qadd(ai, bi));
	//
	//		ai = INT32_MIN;
	//		bi = 1;
	//		printf("QSUB %i-%i = %i\n", ai, bi, asm_qsub(ai, bi));
	//
	//		ai = INT32_MAX;
	//		bi = INT32_MIN;
	//		printf("QSUB %i-%i = %i\n", ai, bi, asm_qsub(ai, bi));
	//	}
	//
	//	{
	//		uint32_t au = 0;
	//		uint32_t res = asm_test_tbb(au);
	//		printf("%u %u\n", au, res);
	//
	//		au = 1;
	//		res = asm_test_tbb(au);
	//		printf("%u %u\n", au, res);
	//
	//		au = 2;
	//		res = asm_test_tbb(au);
	//		printf("%u %u\n", au, res);
	//
	//		au = 3;
	//		res = asm_test_tbb(au);
	//		printf("%u %u\n", au, res);
	//
	//		//index exceed the table will incur a usage fault
	//		//		au = 4;
	//		//		res = asm_test_tbb(au);
	//		//		printf("%u %u\n", au, res);
	//
	//		au = 0;
	//		res = asm_test_tbh(au);
	//		printf("%u %u\n", au, res);
	//
	//		au = 1;
	//		res = asm_test_tbh(au);
	//		printf("%u %u\n", au, res);
	//
	//		au = 2;
	//		res = asm_test_tbh(au);
	//		printf("%u %u\n", au, res);
	//
	//		//index exceed the table will incur a usage fault
	////		au = 3;
	////		res = asm_test_tbh(au);
	////		printf("%u %u\n", au, res);
	//	}
	//	{
	//		float fA = -1.1;
	//		printf("VABS %f = %f\n", fA, asm_vabs(fA));
	//
	//		float fB = 3.3;
	//		printf("VADD %f + %f = %f\n", fA, fB, asm_vadd(fA, fB));
	//		fA = M_PI;
	//		fB= M_E;
	//		printf("VADD %f + %f = %f\n", fA, fB, asm_vadd(fA, fB));
	//
	//		fB = 3.3;
	//		printf("VSUB %f - %f = %f\n", fA, fB, asm_vsub(fA, fB));
	//		fA = M_PI;
	//		fB= M_E;
	//		printf("VSUB %f - %f = %f\n", fA, fB, asm_vsub(fA, fB));
	//
	//		fB = fA;
	//		printf("VCMP %f , %f = %08X\n", fA, fB, asm_vcmp(fA, fB));
	//		fA = M_PI;
	//		fB= M_E;
	//		printf("VCMP %f , %f = %08X\n", fA, fB, asm_vcmp(fA, fB));
	//
	//		fA = -1.1;
	//		printf("VCVT S32 %f = %i\n", fA, asm_vcvt_s32(fA));
	//		fA = M_PI;
	//		printf("VCVT S32 %f = %i\n", fA, asm_vcvt_s32(fA));
	//
	//		fA = -1.1;
	//		printf("VCVT U32 %f = %i\n", fA, asm_vcvt_u32(fA));
	//		fA = M_PI;
	//		printf("VCVT U32 %f = %i\n", fA, asm_vcvt_u32(fA));
	//
	//		fA = -1.1;
	//		printf("VSQRT %f = %f\n", fA, asm_vsqrt(fA));
	//		fA = 2;
	//		printf("VSQRT %f = %f\n", fA, asm_vsqrt(fA));
	//
	//		fB = 3.3;
	//		printf("VMUL %f * %f = %f\n", fA, fB, asm_vmul(fA, fB));
	//		fA = M_PI;
	//		fB= M_E;
	//		printf("VMUL %f * %f = %f\n", fA, fB, asm_vmul(fA, fB));
	//
	//		fB = 3.3;
	//		printf("VDIV %f / %f = %f\n", fA, fB, asm_vdiv(fA, fB));
	//		fA = M_PI;
	//		fB= M_E;
	//		printf("VDIV %f / %f = %f\n", fA, fB, asm_vdiv(fA, fB));
	//
	//		float fC = M_SQRT2;
	//		printf("VFMA %f %f %f = %f\n", fA, fB, fC, asm_vfma(fA, fB, fC));
	//		fA = M_PI;
	//		fB= M_E;
	//		printf("VFMA %f %f %f = %f\n", fA, fB, fC, asm_vfma(fA, fB, fC));
	//
	//		fC = M_SQRT2;
	//		printf("VFMS %f %f %f = %f\n", fA, fB, fC, asm_vfms(fA, fB, fC));
	//		fA = M_PI;
	//		fB= M_E;
	//		printf("VFMS %f %f %f = %f\n", fA, fB, fC, asm_vfms(fA, fB, fC));
	//
	//		fC = M_SQRT2;
	//		printf("VLMA %f %f %f = %f\n", fA, fB, fC, asm_vmla(fA, fB, fC));
	//		fA = M_PI;
	//		fB= M_E;
	//		printf("VLMA %f %f %f = %f\n", fA, fB, fC, asm_vmla(fA, fB, fC));
	//
	//		fC = M_SQRT2;
	//		printf("VLMS %f %f %f = %f\n", fA, fB, fC, asm_vmls(fA, fB, fC));
	//		fA = M_PI;
	//		fB= M_E;
	//		printf("VLMS %f %f %f = %f\n", fA, fB, fC, asm_vmls(fA, fB, fC));
	//	}
	//	{
	//		printf("SCB->CCR=%08X\n", SCB->CCR);
	//		/* Disable divide by zero trap */
	//		SCB->CCR &= ~SCB_CCR_DIV_0_TRP_Msk;
	//		printf("SCB->CCR=%08X\n", SCB->CCR);
	//
	//		float fA = M_PI;
	//		float fB = 0;
	//		printf("VDIV %f / %f = %f\n", fA, fB, asm_vdiv(fA, fB));
	//
	//		uint32_t uA = 1;
	//		uint32_t uB = 0;
	//		printf("UDIV %u / %u = %u\n", uA, uB, asm_simple_udiv(uA, uB));
	//
	//		int32_t iA = 1;
	//		int32_t iB = 0;
	//		printf("SDIV %i / %i = %i\n", iA, iB, asm_simple_sdiv(iA, iB));
	//
	//		printf("SCB->CCR=%08X\n", SCB->CCR);
	//		/* Enable divide by zero trap */
	//		SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
	//		printf("SCB->CCR=%08X\n", SCB->CCR);
	//
	//		//Trap Div by Zero as Usage Fault
	////		printf("UDIV %u / %u = %u\n", uA, uB, asm_simple_udiv(uA, uB));
	////		printf("SDIV %i / %i = %i\n", iA, iB, asm_simple_sdiv(iA, iB));
	//		printf("VDIV %f / %f = %f\n", fA, fB, asm_vdiv(fA, fB));
	//	}

	//	print_global_var_test();
	//	test_rom_func();


	XMC_SCU_StartTemperatureMeasurement();
}

static void error(void){
	while(1){
		;}
}

void test_whets(void);
void test_whetd(void);

#define	TEST_LOOP_N	2000000

void __attribute__((section(".ram_code"))) test_div_psram(void){
	printf("%s %p\n", __func__, test_div_psram);

	for(uint32_t i=0; i<TEST_LOOP_N; ++i){
		{
			//Test unsigned integer division
			volatile uint32_t au = 101;
			volatile uint32_t bu = 10;
			volatile uint32_t cu = au/bu;
			volatile uint32_t du = au%bu;

			//		printf("%u %u -> %u %u\n",
			//				au, bu, cu, du);
		}

		//Test signed integer division
		{
			volatile int32_t ai = 101;
			volatile int32_t bi = 10;
			volatile int32_t ci = ai/bi;
			volatile int32_t di = ai%bi;

			//		printf("%i %i -> %i %i\n",
			//				ai, bi, ci, di);
		}

		//Test float division
		{
			volatile float af = 101.0;
			volatile float bf = 10.0;
			volatile float cf = af/bf;
			//		float df = af%bf;

			//		printf("%f %f -> %f\n",
			//				af, bf, cf);
		}
	}
}

void test_div_flash(void){
	printf("%s %p\n", __func__, test_div_flash);

	for(uint32_t i=0; i<TEST_LOOP_N; ++i){
		{
			//Test unsigned integer division
			volatile uint32_t au = 101;
			volatile uint32_t bu = 10;
			volatile uint32_t cu = au/bu;
			volatile uint32_t du = au%bu;

			//		printf("%u %u -> %u %u\n",
			//				au, bu, cu, du);
		}

		//Test signed integer division
		{
			volatile int32_t ai = 101;
			volatile int32_t bi = 10;
			volatile int32_t ci = ai/bi;
			volatile int32_t di = ai%bi;

			//		printf("%i %i -> %i %i\n",
			//				ai, bi, ci, di);
		}

		//Test float division
		{
			volatile float af = 101.0;
			volatile float bf = 10.0;
			volatile float cf = af/bf;
			//		float df = af%bf;

			//		printf("%f %f -> %f\n",
			//				af, bf, cf);
		}
	}
}


/*
 * Set up the hardware ready to run this demo.
 */
static void prvSetupHardware( void );

/*
 * main_blinky() is used when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 1.
 * main_full() is used when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 0.
 */
extern void main_blinky( void );
extern void main_full( void );

static char *testhex[] = {
		/* 0 "abc" */
		"616263",
		/* 1 "abcd" 16 times */
		"6162636461626364616263646162636461626364616263646162636461626364"
		"6162636461626364616263646162636461626364616263646162636461626364",
		/* 2 p.57 ZA */
		"0090"
		"414C494345313233405941484F4F2E434F4D"
		"787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498"
		"63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A"
		"421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
		"0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
		"0AE4C7798AA0F119471BEE11825BE46202BB79E2A5844495E97C04FF4DF2548A"
		"7C0240F88F1CD4E16352A73C17B7F16F07353E53A176D684A9FE0C6BB798E857",
		/* 3 p.59 ZA */
		"0090"
		"414C494345313233405941484F4F2E434F4D"
		"000000000000000000000000000000000000000000000000000000000000000000"
		"00E78BCD09746C202378A7E72B12BCE00266B9627ECB0B5A25367AD1AD4CC6242B"
		"00CDB9CA7F1E6B0441F658343F4B10297C0EF9B6491082400A62E7A7485735FADD"
		"013DE74DA65951C4D76DC89220D5F7777A611B1C38BAE260B175951DC8060C2B3E"
		"0165961645281A8626607B917F657D7E9382F1EA5CD931F40F6627F357542653B2"
		"01686522130D590FB8DE635D8FCA715CC6BF3D05BEF3F75DA5D543454448166612",
		/* 4 p.72 ZA */
		"0090"
		"414C494345313233405941484F4F2E434F4D"
		"787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498"
		"63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A"
		"421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
		"0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
		"3099093BF3C137D8FCBBCDF4A2AE50F3B0F216C3122D79425FE03A45DBFE1655"
		"3DF79E8DAC1CF0ECBAA2F2B49D51A4B387F2EFAF482339086A27A8E05BAED98B",
		/* 5 p.72 ZB */
		"0088"
		"42494C4C343536405941484F4F2E434F4D"
		"787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498"
		"63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A"
		"421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
		"0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
		"245493D446C38D8CC0F118374690E7DF633A8A4BFB3329B5ECE604B2B4F37F43"
		"53C0869F4B9E17773DE68FEC45E14904E0DEA45BF6CECF9918C85EA047C60A4C",
		/* 6 p.75 ZA */
		"0090"
		"414C494345313233405941484F4F2E434F4D"
		"000000000000000000000000000000000000000000000000000000000000000000"
		"00E78BCD09746C202378A7E72B12BCE00266B9627ECB0B5A25367AD1AD4CC6242B"
		"00CDB9CA7F1E6B0441F658343F4B10297C0EF9B6491082400A62E7A7485735FADD"
		"013DE74DA65951C4D76DC89220D5F7777A611B1C38BAE260B175951DC8060C2B3E"
		"008E3BDB2E11F9193388F1F901CCC857BF49CFC065FB38B9069CAAE6D5AFC3592F"
		"004555122AAC0075F42E0A8BBD2C0665C789120DF19D77B4E3EE4712F598040415",
		/* 7 p.76 ZB */
		"0088"
		"42494C4C343536405941484F4F2E434F4D"
		"000000000000000000000000000000000000000000000000000000000000000000"
		"00E78BCD09746C202378A7E72B12BCE00266B9627ECB0B5A25367AD1AD4CC6242B"
		"00CDB9CA7F1E6B0441F658343F4B10297C0EF9B6491082400A62E7A7485735FADD"
		"013DE74DA65951C4D76DC89220D5F7777A611B1C38BAE260B175951DC8060C2B3E"
		"0034297DD83AB14D5B393B6712F32B2F2E938D4690B095424B89DA880C52D4A7D9"
		"0199BBF11AC95A0EA34BBD00CA50B93EC24ACB68335D20BA5DCFE3B33BDBD2B62D",
		/* 8 TopsecCA cert ZA */
		"0080"
		"31323334353637383132333435363738"
		"FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC"
		"28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93"
		"32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7"
		"BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0"
		"D69C2F1EEC3BFB6B95B30C28085C77B125D77A9C39525D8190768F37D6B205B5"
		"89DCD316BBE7D89A9DC21917F17799E698531F5E6E3E10BD31370B259C3F81C3",
		/* 9 */
		"4D38D2958CA7FD2CFAE3AF04486959CF92C8EF48E8B83A05C112E739D5F181D0"
		"3082020CA003020102020900"
		"AF28725D98D33143300C06082A811CCF"
		"550183750500307D310B300906035504"
		"060C02636E310B300906035504080C02"
		"626A310B300906035504070C02626A31"
		"0F300D060355040A0C06746F70736563"
		"310F300D060355040B0C06746F707365"
		"633111300F06035504030C08546F7073"
		"65634341311F301D06092A864886F70D"
		"0109010C10626A40746F707365632E63"
		"6F6D2E636E301E170D31323036323430"
		"37353433395A170D3332303632303037"
		"353433395A307D310B30090603550406"
		"0C02636E310B300906035504080C0262"
		"6A310B300906035504070C02626A310F"
		"300D060355040A0C06746F7073656331"
		"0F300D060355040B0C06746F70736563"
		"3111300F06035504030C08546F707365"
		"634341311F301D06092A864886F70D01"
		"09010C10626A40746F707365632E636F"
		"6D2E636E3059301306072A8648CE3D02"
		"0106082A811CCF5501822D03420004D6"
		"9C2F1EEC3BFB6B95B30C28085C77B125"
		"D77A9C39525D8190768F37D6B205B589"
		"DCD316BBE7D89A9DC21917F17799E698"
		"531F5E6E3E10BD31370B259C3F81C3A3"
		"733071300F0603551D130101FF040530"
		"030101FF301D0603551D0E041604148E"
		"5D90347858BAAAD870D8BDFBA6A85E7B"
		"563B64301F0603551D23041830168014"
		"8E5D90347858BAAAD870D8BDFBA6A85E"
		"7B563B64300B0603551D0F0404030201"
		"06301106096086480186F84201010404"
		"03020057",
};

static char *dgsthex[] = {
		"66c7f0f462eeedd9d1f2d46bdc10e4e24167c4875cf2f7a2297da02b8f4ba8e0",
		"debe9ff92275b8a138604889c18e5a4d6fdb70e5387e5765293dcba39c0c5732",
		"F4A38489E32B45B6F876E3AC2168CA392362DC8F23459C1D1146FC3DBFB7BC9A",
		"26352AF82EC19F207BBC6F9474E11E90CE0F7DDACE03B27F801817E897A81FD5",
		"E4D1D0C3CA4C7F11BC8FF8CB3F4C02A78F108FA098E51A668487240F75E20F31",
		"6B4B6D0E276691BD4A11BF72F4FB501AE309FDACB72FA6CC336E6656119ABD67",
		"329c2f6030cc7e0ca3af6c97b76243ca250338ad3d3dc3a8b322d1cfdf98c2b7",
		"557BAD30E183559AEEC3B2256E1C7C11F870D22B165D015ACF9465B09B87B527",
		"4D38D2958CA7FD2CFAE3AF04486959CF92C8EF48E8B83A05C112E739D5F181D0",
		"C3B02E500A8B60B77DEDCF6F4C11BEF8D56E5CDE708C72065654FD7B2167915A",
};

/******************************************************************************
 Function: SM3_test_suite
 Description: test whether the SM3 calculation is correct by comparing
  the hash result with the standard result
 Calls: SM3_256
 Called By:
 Input: null
 Output: null
 Return: 0 //the SM3 operation is correct
 1 //the sm3 operation is wrong
 Others:
 *******************************************************************************/
uint32_t SM3_test_suite(void) {
	uint32_t ret_v=0;
	uint8_t MsgHash1[SM3_RES_L];

	uint8_t test_hw_msg[] = "hello world";
	print_U8(test_hw_msg, strlen(test_hw_msg), "hw");
	SM3_256(test_hw_msg, strlen(test_hw_msg), MsgHash1);
	print_U8(MsgHash1, SM3_RES_L, "sm3 hw hash");
	printf("Ref: 44F0061E69FA6FDFC290C494654A05DC0C053DA7E5C52B84EF93A9D67D3FFF88\n");
	printf("\n\n");

	uint8_t big_buf[2048];
	for(uint8_t i=0; i<sizeof(testhex)/sizeof(char*); ++i) {
		uint32_t msg_len = strlen(testhex[i]);
		for(uint32_t j=0; j<msg_len/2; ++j) {
			big_buf[j] = BCH_To_Byte(testhex[i][j*2], testhex[i][1+j*2]);
		}
		uint8_t tmp_name[32];
		sprintf(tmp_name, "msg %u", i);
		print_U8(big_buf, msg_len/2, tmp_name);
		SM3_256(big_buf, msg_len/2, MsgHash1);
		sprintf(tmp_name, "hash %u", i);
		print_U8(MsgHash1, SM3_RES_L, tmp_name);

		printf("Ref:");
		printf(dgsthex[i]);
		printf("\n\n");
	}

	return ret_v;
}

/*-----------------------------------------------------------*/

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

int main(void) {
	DAVE_STATUS_t status;

	status = DAVE_Init();           /* Initialization of DAVE APPs  */

	//	XMC_WDT_Start();
	//	printf("\nWDT Cnt:%u\n", XMC_WDT_GetCounter());

	XMC_SCU_EnableTemperatureSensor();
	XMC_SCU_StartTemperatureMeasurement();

	if(status != DAVE_STATUS_SUCCESS)
	{
		/* Placeholder for error handler code. The while loop below can be replaced with an user error handler. */
		XMC_DEBUG("DAVE APPs initialization failed\n");

		while(1U){
		}
	}

	//	SEGGER_RTT_Write(0, "test", 4);

	printf("%u Hz, %08X, CM:%d, FPU_USED:%d\n",
			SystemCoreClock, SCB->CPUID,
			__CORTEX_M, __FPU_USED);
	printf("Boot Mode:%u\n", XMC_SCU_GetBootMode());

	//	uint32_t tick0;
	//	uint32_t tick1;
	//	uint32_t tick2;
	//
	//	tick0 = SYSTIMER_GetTickCount();
	//	test_div_flash();
	//	tick1 = SYSTIMER_GetTickCount();
	//	test_div_psram();
	//	tick2 = SYSTIMER_GetTickCount();
	//
	//	printf("%u %u\n", tick1-tick0, tick2-tick1);

	//	// Create Software timer
	//#define ONESEC	1000
	//	uint32_t TimerId = (uint32_t)SYSTIMER_CreateTimer(1000*SYSTIMER_TICK_PERIOD_US,
	//			SYSTIMER_MODE_PERIODIC,
	//			(void*)LED_Toggle_EverySec,NULL);
	//
	//	SYSTIMER_Start();
	//	SYSTIMER_StartTimer(TimerId);

	/* Prepare the hardware to run this demo. */
	prvSetupHardware();

	/* The mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting is described at the top
	of this file. */
#if mainCREATE_SIMPLE_BLINKY_DEMO_ONLY == 1
	main_blinky();
#else
	main_full();
#endif

	/* Placeholder for user application code. The while loop below can be replaced with user application code. */
	while(1U)
	{
		//		test_whets();
		//		test_whetd();
	}
}

/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	//	configCONFIGURE_LED();

	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_SetPriorityGrouping( 0 );
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
}
/*-----------------------------------------------------------*/

#ifdef JUST_AN_EXAMPLE_ISR

void Dummy_IRQHandler(void)
{
	long lHigherPriorityTaskWoken = pdFALSE;

	/* Clear the interrupt if necessary. */
	Dummy_ClearITPendingBit();

	/* This interrupt does nothing more than demonstrate how to synchronise a
	task with an interrupt.  A semaphore is used for this purpose.  Note
	lHigherPriorityTaskWoken is initialised to zero. */
	xSemaphoreGiveFromISR( xTestSemaphore, &lHigherPriorityTaskWoken );

	/* If there was a task that was blocked on the semaphore, and giving the
	semaphore caused the task to unblock, and the unblocked task has a priority
	higher than the current Running state task (the task that this interrupt
	interrupted), then lHigherPriorityTaskWoken will have been set to pdTRUE
	internally within xSemaphoreGiveFromISR().  Passing pdTRUE into the
	portEND_SWITCHING_ISR() macro will result in a context switch being pended to
	ensure this interrupt returns directly to the unblocked, higher priority,
	task.  Passing pdFALSE into portEND_SWITCHING_ISR() has no effect. */
	portEND_SWITCHING_ISR( lHigherPriorityTaskWoken );
}

#endif /* JUST_AN_EXAMPLE_ISR */

