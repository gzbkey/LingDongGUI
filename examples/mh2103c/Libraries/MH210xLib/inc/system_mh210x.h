/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup mh210x_system
  * @{
  */  
  
/**
  * @brief Define to prevent recursive inclusion
  */
#ifndef __SYSTEM_MH210X_H
#define __SYSTEM_MH210X_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup mh210x_System_Includes
  * @{
  */

/**
  * @}
  */


/** @addtogroup mh210x_System_Exported_types
  * @{
  */

extern uint32_t SystemCoreClock;          /*!< System Clock Frequency (Core Clock) */

/**
  * @}
  */

/** @addtogroup mh210x_System_Exported_Constants
  * @{
  */

/**
  * @}
  */

/** @addtogroup mh210x_System_Exported_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup mh210x_System_Exported_Functions
  * @{
  */
  
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_mh210X_H */

/**
  * @}
  */
  
/**
  * @}
  */  
/******************* (C) COPYRIGHT  MHSEMICONDUCTOR *****END OF FILE****/
