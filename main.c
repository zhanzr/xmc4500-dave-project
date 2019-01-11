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

/*Data Packet 1 */
int8_t usecase1_Data1[] = "Lorem ipsum dolor sit amet, ";
/*Data Packet 2 */
int8_t usecase1_Data2[] = "consectetur adipiscing elit. Donec metus eros, a";
/*Data Packet 3 */
int8_t usecase1_Data3[] = "ccumsan ut vestibulum id, suscipit nec augue. Aenean et lectus ut libero lacinia faucibus ut vel lectus. Pellentesque iaculis libero ac lectus blandit accumsan. Pellentesque at nulla eget metus aliquam tincidunt. Aenean cursus purus vitae lacus cursus pharetra. In hac habitasse platea dictumst. Curabitur nulla odio, porttitor eu pharetra at, pulvinar gravida velit. Aenean eu dapibus purus. Proin blandit feugiat urna, at iaculis elit accumsan ut. Pellentesque molestie pharetra erat, eget semper urna vehicula nec. Nam tristique sollicitudin diam, faucibus aliquet velit pharetra a. Duis a placerat risus. Phasellus vel diam nibh, quis elementum nisl. Phasellus lectus quam, mollis eu consequat ut, bibendum eget arcu. Nullam at felis a elit auctor suscipit eu quis ipsum. Mauris luctus, diam sit amet iaculis malesuada, urna orci convallis tellus, vitae molestie diam justo sed metus. Etiam volutpat volutpat justo, vel facilisis mi eleifend ut. Aenean egestas, sem eu vulputate lacinia, odio ligula mollis risus, a semper eros risus sed arcu. Sed feugiat augue eget erat bibendum vitae consequat purus tempus. Morbi lobortis nunc eget ligula vehicula non pharetra dolor commodo. Pellentesque ligula nibh, eleifend blandit aliquam vel, euismod non tellus. Quisque dictum laoreet feugiat. Maecenas a varius sapien. Ut semper nulla id turpis cursus ornare. Nullam quis erat et augue imperdiet pharetra nec a sem. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Aenean feugiat, orci ultricies pellentesque viverra, nisl elit molestie augue, et scelerisque risus felis nec nulla. Maecenas congue arcu ac lectus bibendum at lacinia elit tristique. Cras fringilla vestibulum lectus. Praesent quis nisi turpis, sed tristique sem. Nam adipiscing posuere faucibus. In iaculis placerat semper. Curabitur in nunc quis enim vehicula aliquam quis at sapien. In hac habitasse platea orci aliquam...";

int8_t usecase1_Data4[] = "abcd";

/* FCE configuration for CRC32 operation using Kernal 0
 * Algorithm: IR Byte Wise Reflection disabled
 * 			: CRC 32-Bit Wise Reflection disabled
 *          : XOR with final CRC enabled
 * Initial seedvalue: 0U
 */
XMC_FCE_t FCE_config00 =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
		.seedvalue	 	= 0U
};

XMC_FCE_t FCE_config01 =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_SET,
		.seedvalue	 	= 0U
};
XMC_FCE_t FCE_config02 =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_SET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
		.seedvalue	 	= 0U
};
XMC_FCE_t FCE_config03 =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_SET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_SET,
		.seedvalue	 	= 0U
};
XMC_FCE_t FCE_config04 =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_SET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
		.seedvalue	 	= 0U
};
XMC_FCE_t FCE_config05 =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_SET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_SET,
		.seedvalue	 	= 0U
};
XMC_FCE_t FCE_config06 =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_SET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_SET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
		.seedvalue	 	= 0U
};
XMC_FCE_t FCE_config07 =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_SET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_SET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_SET,
		.seedvalue	 	= 0U
};
XMC_FCE_t FCE_config08 =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
		.seedvalue	 	= 0xFFFFFFFFU
};

XMC_FCE_t FCE_config09 =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_SET,
		.seedvalue	 	= 0xFFFFFFFFU
};
XMC_FCE_t FCE_config0A =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_SET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
		.seedvalue	 	= 0xFFFFFFFFU
};
XMC_FCE_t FCE_config0B =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_SET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_SET,
		.seedvalue	 	= 0xFFFFFFFFU
};
XMC_FCE_t FCE_config0C =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_SET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
		.seedvalue	 	= 0xFFFFFFFFU
};
XMC_FCE_t FCE_config0D =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_SET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_SET,
		.seedvalue	 	= 0xFFFFFFFFU
};
XMC_FCE_t FCE_config0E =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_SET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_SET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
		.seedvalue	 	= 0xFFFFFFFFU
};
XMC_FCE_t FCE_config0F =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_0,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_SET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_SET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_SET,
		.seedvalue	 	= 0xFFFFFFFFU
};


/* FCE configuration for CRC32 operation using Kernal 1
 * Algorithm: IR Byte Wise Reflection disabled
 * 			: CRC 32-Bit Wise Reflection disabled
 *          : XOR with final CRC disabled
 * Initial seedvalue: 0U
 */
XMC_FCE_t FCE_config1 =
{
		.kernel_ptr 	 = XMC_FCE_CRC32_1,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
		.seedvalue	 = 0U
};

/* FCE configuration for CRC16 operation using Kernal 2
 * Algorithm: IR Byte Wise Reflection enabled
 * 			: CRC 32-Bit Wise Reflection disabled
 *          : XOR with final CRC disabled
 * Initial seedvalue: 0U
 */
XMC_FCE_t FCE_config2 =
{
		.kernel_ptr 	 = XMC_FCE_CRC16,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_SET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
		.seedvalue	 = 0U
};

/* FCE configuration for CRC8 operation using Kernal 3
 * Algorithm: IR Byte Wise Reflection disabled
 * 			: CRC 32-Bit Wise Reflection disabled
 *          : XOR with final CRC disabled
 * Initial seedvalue: 0U
 */
XMC_FCE_t FCE_config3 =
{
		.kernel_ptr 	 = XMC_FCE_CRC8,    /**< FCE Kernel Pointer */
		.fce_cfg_update.config_refin = XMC_FCE_REFIN_RESET,
		.fce_cfg_update.config_refout = XMC_FCE_REFOUT_RESET,
		.fce_cfg_update.config_xsel = XMC_FCE_INVSEL_RESET,
		.seedvalue	 = 0U
};

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

	uint32_t Read_CRCResult32;
	uint16_t Read_CRCResult16;
	uint8_t  Read_CRCResult8;
	uint32_t temp_length;
	uint32_t temp_mismatch;
	uint32_t CRC_result;
	bool flagstatus;
	XMC_FCE_STATUS_t fce_status;

	/* Enable FCE module */
	XMC_FCE_Enable();

	/* Initialize the FCE Configuration */
	//	XMC_FCE_Init(&FCE_config0);
	XMC_FCE_Init(&FCE_config1);
	XMC_FCE_Init(&FCE_config2);
	XMC_FCE_Init(&FCE_config3);

	/* Initialize error counter*/
	temp_mismatch = 0;

	//	/* Step 1: Performs a CRC32 check using Kernel 0 on Usecase1_Data1
	//	 * Seed value is set to 0.
	//	 * => CRC = 0xbb8d49a6, RES = 0x4472b659
	//	 */
	//	XMC_FCE_InitializeSeedValue(&FCE_config0, 0);
	//	fce_status = XMC_FCE_CalculateCRC32(&FCE_config0, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	//	while(fce_status== XMC_FCE_STATUS_ERROR)
	//	{
	//		printf("error %d\n", __LINE__);
	//		error();
	//	}
	//	XMC_FCE_GetCRCResult(&FCE_config0, &CRC_result);
	//	printf("Result: %08X\n", CRC_result);

	XMC_FCE_t* p_fce_cfg;

	p_fce_cfg = &FCE_config00;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config01;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config02;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config03;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config04;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config05;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config06;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config07;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config08;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config09;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0A;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0B;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0C;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0D;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0E;
	XMC_FCE_Init(&FCE_config0E);
	XMC_FCE_CalculateCRC32(&FCE_config0E, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(&FCE_config0E, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0F;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	printf("\n");
	p_fce_cfg = &FCE_config00;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config01;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config02;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config03;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config04;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config05;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config06;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config07;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config08;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config09;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0A;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0B;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0C;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0D;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0E;
	XMC_FCE_Init(&FCE_config0E);
	XMC_FCE_CalculateCRC32(&FCE_config0E, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(&FCE_config0E, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	p_fce_cfg = &FCE_config0F;
	XMC_FCE_Init(p_fce_cfg);
	XMC_FCE_CalculateCRC32(p_fce_cfg, (uint32_t *) usecase1_Data4, strlen((const char *)(usecase1_Data4)), &Read_CRCResult32);
	XMC_FCE_GetCRCResult(p_fce_cfg, &CRC_result);
	printf("Result: %08X\n", CRC_result);
	printf("\n");
	/* Step 2: Performs a CRC32 check using Kernel 1 on Usecase1_Data1
	 * Seed value is set to 0. CRC check comparison is enabled
	 * CRC checksum is using result from earlier CRC check
	 * No CRC mismatch found.
	 * CRC = 0xbb8d49a6, RES = 0xbb8d49a6
	 */
	XMC_FCE_EnableOperation(&FCE_config1,XMC_FCE_CFG_CONFIG_CCE);

	/* Used in 32bit FCE, therefore temp_length is divided by 4*/
	temp_length = (strlen((char *)(usecase1_Data1)))>>2;
	XMC_FCE_UpdateCRCCheck(&FCE_config1, Read_CRCResult32);
	XMC_FCE_UpdateLength(&FCE_config1, temp_length);

	XMC_FCE_InitializeSeedValue(&FCE_config1, 0);
	fce_status = XMC_FCE_CalculateCRC32(&FCE_config1, (uint32_t *) usecase1_Data1, strlen((const char *)(usecase1_Data1)), &Read_CRCResult32);
	while(fce_status== XMC_FCE_STATUS_ERROR)
	{
		printf("error %d\n", __LINE__);
		error();
	}

	XMC_FCE_GetCRCResult(&FCE_config1, &CRC_result);
	printf("Result: %08X\n", CRC_result);
	if(XMC_FCE_GetEventStatus(&FCE_config1,XMC_FCE_STS_MISMATCH_CRC))
	{
		temp_mismatch += 1U;
	}

	/* Step 3: Performs a CRC32 check using Kernel 1 on Usecase1_Data2
	 * Seed value is set to 0. CRC check comparison is enabled
	 * CRC checksum is using result from earlier CRC check
	 * CRC mismatch found and Length Error found.
	 * CRC = 0x8f2d7440, RES = 0x8f2d7440
	 */
	XMC_FCE_InitializeSeedValue(&FCE_config1, 0);
	XMC_FCE_UpdateLength(&FCE_config1, temp_length);

	fce_status = XMC_FCE_CalculateCRC32(&FCE_config1, (uint32_t *) usecase1_Data2, strlen((const char *)(usecase1_Data2)), &Read_CRCResult32);
	while(fce_status== XMC_FCE_STATUS_ERROR)
	{
		printf("error %d\n", __LINE__);
		error();
	}

	XMC_FCE_GetCRCResult(&FCE_config1, &CRC_result);
	printf("Result: %08X\n", CRC_result);
	if (XMC_FCE_GetEventStatus(&FCE_config1,XMC_FCE_STS_MISMATCH_CRC))
	{
		temp_mismatch += 2U;
	}

	/* Step 4: Performs a CRC16 check using Kernel 2 on Usecase1_Data3
	 * Seed value is set to 0.
	 * CRC = 0x191e, RES = 0x191e
	 */
	XMC_FCE_InitializeSeedValue(&FCE_config2, 0);
	fce_status = XMC_FCE_CalculateCRC16(&FCE_config2, (uint16_t *) usecase1_Data3, strlen((const char *)(usecase1_Data3)), &Read_CRCResult16);
	while(fce_status== XMC_FCE_STATUS_ERROR)
	{
		printf("error %d\n", __LINE__);
		error();
	}

	XMC_FCE_GetCRCResult(&FCE_config2, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	/* Step 5: Performs a CRC8 check using Kernel 3 on Usecase1_Data4
	 * Seed value is set to 0. CRC = 0xbe, RES = 0xbe
	 */
	XMC_FCE_InitializeSeedValue(&FCE_config3, 0);
	fce_status = XMC_FCE_CalculateCRC8(&FCE_config3, (uint8_t *)usecase1_Data3, strlen((const char *)(usecase1_Data3)), &Read_CRCResult8);
	while(fce_status== XMC_FCE_STATUS_ERROR)
	{
		printf("error %d\n", __LINE__);
		error();
	}
	XMC_FCE_GetCRCResult(&FCE_config3, &CRC_result);
	printf("Result: %08X\n", CRC_result);

	/* Step 6: Trigger a mismatch flag
	 */
	flagstatus = XMC_FCE_GetEventStatus(&FCE_config3, XMC_FCE_STS_MISMATCH_CRC);
	while(flagstatus)
	{
		/* endless loop if mismatch flag is triggered */
	}
	XMC_FCE_TriggerMismatch(&FCE_config3, XMC_FCE_CTR_MISMATCH_CRC);
	flagstatus = XMC_FCE_GetEventStatus(&FCE_config3, XMC_FCE_STS_MISMATCH_CRC);
	while(flagstatus)
	{
		printf("mismatch flag is triggered\n");
		error();
	}

	/* Placeholder for user application code. The while loop below can be replaced with user application code. */
	while(1U)
	{
		//		test_whets();
		//		test_whetd();
	}
}
