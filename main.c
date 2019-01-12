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

void LED_Toggle_EverySec(void)
{
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

		 while(1U)
		 {

		 }
	 }

	 printf("%u Hz, %08X, CM:%d, FPU_USED:%d\n",
			 SystemCoreClock, SCB->CPUID,
			 __CORTEX_M, __FPU_USED);
	 printf("Boot Mode:%u\n", XMC_SCU_GetBootMode());

	 // Create Software timer
#define ONESEC	1000
	 uint32_t TimerId = (uint32_t)SYSTIMER_CreateTimer(1000*SYSTIMER_TICK_PERIOD_US,
			 SYSTIMER_MODE_PERIODIC,
			 (void*)LED_Toggle_EverySec,NULL);

	 SYSTIMER_Start();
	 SYSTIMER_StartTimer(TimerId);

	 //Test unsigned integer division
	 {
		 uint32_t au = 101;
		 uint32_t bu = 10;
		 uint32_t cu = au/bu;
		 uint32_t du = au%bu;

		 printf("%u %u -> %u %u\n",
				 au, bu, cu, du);
	 }

	 //Test signed integer division
	 {
		 int32_t ai = 101;
		 int32_t bi = 10;
		 int32_t ci = ai/bi;
		 int32_t di = ai%bi;

		 printf("%i %i -> %i %i\n",
				 ai, bi, ci, di);
	 }

	 //Test float division
	 {
		 float af = 101.0;
		 float bf = 10.0;
		 float cf = af/bf;
		 //		float df = af%bf;

		 printf("%f %f -> %f\n",
				 af, bf, cf);
	 }

	 /* Placeholder for user application code. The while loop below can be replaced with user application code. */
	 while(1U)
	 {
		 //		test_whets();
		 //		test_whetd();
	 }
}
