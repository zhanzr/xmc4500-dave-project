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

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "asm_prototype.h"

/* Set mainCREATE_SIMPLE_BLINKY_DEMO_ONLY to one to run the simple blinky demo,
or 0 to run the more comprehensive test and demo application. */
#define mainCREATE_SIMPLE_BLINKY_DEMO_ONLY	1

/*-----------------------------------------------------------*/

//Retarget IO
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

void TestFunct(void){
	printf("CPUID:%08X\n", SCB->CPUID);
}

void __SVC_1( uint32_t in ) __attribute__ (( naked ));

void __SVC_1( uint32_t in )
{
	printf("%s:%08X\n",
			__func__,
			SCB->CPUID);
}

void LED_Toggle_EverySec(void){
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

	printf("ASM Test 1 Result:%u\n", asm_get_8bit_number());
	printf("ASM Test 2 Result:%08X\t[%08X]\n", asm_get_xor(0x12345678, 0x34567890), 0x12345678^0x34567890);
	printf("ASM Test 3 Direct Jump:%08X\n", TestFunct);
	printf("Jump 1, Before.%08X\n", __get_MSP());
	asm_direct_jump_1(TestFunct);
	printf("Jump 1, After.%08X\n\n", __get_MSP());

	printf("Jump 2, Before.%08X\n", __get_MSP());
	asm_direct_jump_2(TestFunct);
	printf("Jump 2, After.%08X\n\n", __get_MSP());


	printf("ASM Test 4 :%u\t[%u]\n", asm_add2(34), 34+2);
	printf("ASM Test 5 :%u\t[%u]\n", asm_simple_add(123, 456), 123+456);
	printf("ASM Test 6 :%u\t[%u]\n", asm_pc_add(), 7);

	printf("ASM Test 7 :%d\t[%d]\n", asm_sub20(34), 34-20);
	printf("ASM Test 8 :%d\t[%d]\n", asm_simple_sub(123, 456), 123-456);
	printf("ASM Test 9 :%d\t[%d]\n", asm_get_neg(1024), 0-1024);

	printf("ASM Test 10 Result:%u\t[%u]\n", asm_simple_mul(123, 456), 123*456);

	//Test Addition/Mulitiplication Cycles
#define	TEST_ADD_MUL_NUM	500000
	//If the multiplication takes similar cycles, it is a single cycle multiplication implementation
	uint32_t tmpTick = xTaskGetTickCount();
	for(uint32_t i=0; i<TEST_ADD_MUL_NUM; ++i)
	{
		uint32_t tn = 101;
		asm_simple_add(tn, 456);
	}
	tmpTick = xTaskGetTickCount()-tmpTick;
	printf("A:%u\n", tmpTick);

	tmpTick = xTaskGetTickCount();
	for(uint32_t i=0; i<TEST_ADD_MUL_NUM; ++i)
	{
		uint32_t tn = 101;
		asm_simple_mul(tn, 456);
	}
	tmpTick = xTaskGetTickCount()-tmpTick;
	printf("M:%u\n", tmpTick);

	//Test Division
	{
		uint32_t ta = 10;
		uint32_t tb = 2;
		uint32_t tc = ta/tb;
		printf("%u %u %u\n", ta, tb, tc);
	}

	printf("ASM Test 11 Result:%u\t[%u]\n", asm_test_cmp(123, 456), (123==456));
	printf("ASM Test 12 Result:%u\t[%u]\n", asm_test_cmn(123, 456), (123!=456));
	printf("ASM Test 13 Result:%08X\t[%08X]\n", asm_get_and(0x12345678, 0x34567890), 0x12345678 & 0x34567890);
	printf("ASM Test 14 Result:%08X\t[%08X]\n", asm_get_or(0x12345678, 0x34567890), (0x12345678 | 0x34567890));
	printf("ASM Test 15 Result:%08X\t[%08X]\n", asm_get_not(0x12345678), 0-0x12345678);

	uint32_t g_TestVar32 = 0x12345678;
	printf("ASM Test 20 Result:%08X\t[%08X]\n", asm_ldr32(&g_TestVar32), g_TestVar32);
	asm_str32(&g_TestVar32, 0x78904563);
	printf("ASM Test 21 Result:%08X\t[%08X]\n", asm_ldr32(&g_TestVar32), g_TestVar32);
	printf("ASM Test 22 Result:%u\t[%d]\n", asm_test_push_pop(123, 456), 123+456+456+4);

	//Part 7: Test Extend, Reverse
	printf("Part 7\n");
	printf("ASM Test 23 Result:%08X\t[%08X]\n", asm_s16ext((int16_t)0x8001), (int32_t)0x8001);
	printf("ASM Test 24 Result:%08X\t[%08X]\n", asm_s8ext((int8_t)0xC4), (int32_t)0xC4);
	printf("ASM Test 25 Result:%08X\t[%08X]\n", asm_u16ext((uint16_t)0x8001), (uint32_t)0x8001);
	printf("ASM Test 26 Result:%08X\t[%08X]\n", asm_rev(0x123456C8), __REV(0x123456C8));
	printf("ASM Test 27 Result:%08X\t[%08X]\n", asm_rev16(0x123456C8), __REV16(0x123456C8));
	printf("ASM Test 28 Result:%08X\t[%08X]\n", asm_revsh(0x123456C8), __REVSH(0x123456C8));

	//Part 8: Test SVC, MSR, MRS
	printf("Part 8\n");
	printf("ASM Test 29, Before SVC\n");
//	asm_svc_1(1000);
	printf("After SVC\n");

	printf("ASM Test 30 Result:%08X\n", asm_test_mrs());
	printf("ASM Test 31 Tick:%u\n", SysTick->VAL);
	asm_test_msr(0x00000001);
	uint32_t p1 = asm_test_mrs();
	asm_test_msr(0x00000000);
	uint32_t p2 = asm_test_mrs();
	printf("%08X\t%08X\n", p1, p2);

	XMC_SCU_StartTemperatureMeasurement();
}

static void error(void)
{
	while(1)
	{
		;
	}
}

void test_whets(void);
void test_whetd(void);

#define	TEST_LOOP_N	2000000

void __attribute__((section(".ram_code"))) test_div_psram(void){
	printf("%s %p\n", __func__, test_div_psram);

	for(uint32_t i=0; i<TEST_LOOP_N; ++i)
	{
		//Test unsigned integer division
		{
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

void test_div_flash(void)
{
	printf("%s %p\n", __func__, test_div_flash);

	for(uint32_t i=0; i<TEST_LOOP_N; ++i)
	{
		//Test unsigned integer division
		{
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

/*-----------------------------------------------------------*/

/**

 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the APP initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */

int main(void)
{
	DAVE_STATUS_t status;

	status = DAVE_Init();           /* Initialization of DAVE APPs  */

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
	{
		main_blinky();
	}
#else
	{
		main_full();
	}
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

