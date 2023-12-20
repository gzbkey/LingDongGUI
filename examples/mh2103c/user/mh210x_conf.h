/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MH210x_CONF_H
#define __MH210x_CONF_H

/* Includes ------------------------------------------------------------------*/
/* Uncomment/Comment the line below to enable/disable peripheral header file inclusion */
#include "mh210x_adc.h"
#include "mh210x_bkp.h"
#include "mh210x_can.h"
#include "mh210x_crc.h"
#include "mh210x_dac.h"
#include "mh210x_dbgmcu.h"
#include "mh210x_dma.h"
#include "mh210x_exti.h"
#include "mh210x_flash.h"
#include "mh210x_fsmc.h"
#include "mh210x_gpio.h"
#include "mh210x_i2c.h"
#include "mh210x_iwdg.h"
#include "mh210x_pwr.h"
#include "mh210x_rcc.h"
#include "mh210x_rtc.h"
#include "mh210x_sdio.h"
#include "mh210x_spi.h"
#include "mh210x_tim.h"
#include "mh210x_usart.h"
#include "mh210x_wwdg.h"
#include "misc.h" /* High level functions for NVIC and SysTick (add-on to CMSIS functions) */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __mh210x_CONF_H */

/******************* (C) COPYRIGHT  MHSEMICONDUCTOR *****END OF FILE****/
