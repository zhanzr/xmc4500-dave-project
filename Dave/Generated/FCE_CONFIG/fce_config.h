/**
 * @file fce_config.h
 * @date 2016-07-08
 *
 * NOTE:
 * This file is generated by DAVE. Any manual modification done to this file will be lost when the code is regenerated.
 *
 * @cond
 ***********************************************************************************************************************
 * FCE_CONFIG v4.0.6 - Configuration of FCE peripheral for generating the CRC.
 *
 * Copyright (c) 2015-2016, Infineon Technologies AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the
 * following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this list of conditions and the  following
 *   disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *   following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *   Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote
 *   products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT  OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes
 * with Infineon Technologies AG (dave@infineon.com).
 ***********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2016-01-18:
 *     - Initial version for DAVEv4.<BR> *
 *
 * 2016-07-08:
 *     - Fixed incorrect case for an included header.<br>
 *
 * @endcond
 *
 */

#ifndef FCE_CONFIG_H_
#define FCE_CONFIG_H_

/***********************************************************************************************************************
* HEADER FILES
**********************************************************************************************************************/
#include <xmc_fce.h>
#include <DAVE_Common.h>
#include "fce_config_conf.h"

/**********************************************************************************************************************
* MACROS
**********************************************************************************************************************/
#if (!((XMC_LIB_MAJOR_VERSION == 2U) && \
   (XMC_LIB_MINOR_VERSION >= 0U) && \
   (XMC_LIB_PATCH_VERSION >= 0U)))
#error "FCE_CONFIG requires XMC Peripheral Library v2.0.0 or higher"
#endif

/**********************************************************************************************************************
* ENUMERATION
**********************************************************************************************************************/
/**
 * @ingroup FCE_CONFIG_enumerations
 * @{
 */
typedef enum FCE_CONFIG_STATUS
{
	FCE_CONFIG_STATUS_SUCCESS = 0, /**< Returns on success */
	FCE_CONFIG_STATUS_FAILURE   /**< Returns on failure */
} FCE_CONFIG_STATUS_t;

/**
* @}
*/




/**********************************************************************************************************************
* DATA STRUCTURES
**********************************************************************************************************************/
/**
* @ingroup FCE_CONFIG_datastructures
* @{
*/

/**
* @brief Configuration structure for FCE_CONFIG APP
*/
typedef struct FCE_CONFIG
{
	XMC_FCE_Kernel_t *fce_Kernel_ptr;/**< FCE kernel pointer*/
	XMC_FCE_CONFIG_t fce_config;/**< FCE input and output configuration*/
	uint32_t event_mask;/**< event mask used to enable the FCE error events.*/

} FCE_CONFIG_t;

/**
* @}
*/

#ifdef __cplusplus
extern "C" {
#endif
/***********************************************************************************************************************
* API Prototypes
**********************************************************************************************************************/
/**
* @ingroup FCE_CONFIG_apidoc
* @{
*/
/**
* @brief Get FCE_CONFIG APP version
* @return DAVE_APP_VERSION_t APP version information (major, minor and patch number)
*
* \par<b>Description: </b><br>
* The function can be used to check application software compatibility with a
* specific version of the APP.
*
* \par<b>Example Usage:</b><br>
*
* @code
* #include <DAVE.h>
*
* int main(void) {
*   DAVE_STATUS_t init_status;
*   DAVE_APP_VERSION_t version;
*
*   // Initialize FCE_CONFIG APP:
*   // FCE_CONFIG_Init() is called from within DAVE_Init().
*   init_status = DAVE_Init();
*
*   if (DAVE_STATUS_SUCCESS == init_status)
*   {
*     version = FCE_CONFIG_GetAppVersion();
*     if (version.major != 4U) {
*       // Probably, not the right version.
*     }
*   }
*
*     // More code here
*     while(1)
*     {
*     }
*   return (1);
* }
* @endcode<BR>
*/
DAVE_APP_VERSION_t FCE_CONFIG_GetAppVersion(void);

/**
* @brief Initializes a FCE_CONFIG APP instance
* @param handle address of FCE_CONFIG APP handler
* @return
*            FCE_CONFIG_STATUS_SUCCESS                : if initialization is successful\n
*            FCE_CONFIG_STATUS_FAILURE                : on failure*
*
* \par<b>Description:</b><br>
* FCE_CONFIG_Init API is called during initialization of DAVE APPS. This API Initializes FCE_CONFIG module.
* By using the FCE_CONFIG UI, user can input CRC input data reflection,output data reflection and CRC output
* inversion. Also user can enable the required error events using the UI.
*
* \par<b>Example Usage:</b><br>
*
* @code
* #include <DAVE.h>
* uint8_t data[40] = "infineon technologies india pvt limited";
*
* int main(void)
* {
*   DAVE_STATUS_t status;
*   uint8_t initial_crc = 0;
*   uint32_t crc8 = 0;
*
*   status = DAVE_Init();  //  FCE_CONFIG_Init API is called during initialization of DAVE APPS
*   if(DAVE_STATUS_SUCCESS == status)
*   {
*    // user code
*
*    XMC_FCE_CalculateCRC8((XMC_FCE_t*)&FCE_CONFIG_0,data,39,&initial_crc);
*    XMC_FCE_GetCRCResult((XMC_FCE_t*)&FCE_CONFIG_0,&crc8);
*
*     while(1)
*     {
*
*     }
*   }
*   return (1);
* }
*
* @endcode<BR>
*/
FCE_CONFIG_STATUS_t FCE_CONFIG_Init(const FCE_CONFIG_t *handle);

/**
* @}
*/
#ifdef __cplusplus
}
#endif

/* Inclusion of extern file */
#include"fce_config_extern.h"

#endif /* End of FCE_CONFIG_H */