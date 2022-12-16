/**
 * @file VL53L1X.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-12-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _VL53L1X_H_
#define _VL53L1X_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "InterfaceI2C.h"

#ifndef STM32F103xx_UNREADY

  /** Data Structure Begin ---------------------------------------------------------------
   * @brief class data sturcture
   * @warning Plz operate the object through the interface
   *
   */
  typedef struct
  {

    I2C_TypeDef *I2Cx;

    uint8_t address;

  } VL53L1X_DS;

  /* ---------------------------------------------------------------- Data Structure End */

  /** Interface Begin --------------------------------------------------------------------
   * @brief
   * | Almost directly through the register operation. \n
   * | For each function, they provide an alternative, \n
   * | with the same functionality, implemented with the LL library.
   *
   * @warning Do not change these codes, it may cause errors
   */

  /**
   * @brief Constructor (dynamic memory)
   *
   * @param I2Cx: defined in the stm32f103xx series header
   * @param address: device address
   * @return VL53L1X_DS*: dynamic memory pointer
   */
  VL53L1X_DS *VL53L1X_Constructor(I2C_TypeDef *I2Cx, const uint8_t address);

  /**
   * @brief Destructor
   *
   * @param self: object pointer
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_Destructor(VL53L1X_DS *const self);

  /** 
   * @brief Configure module with default values 
   * 
   * @param self: object pointer
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_DefaultInit(VL53L1X_DS *const self);

  /**
   * @brief Continuously write multiple data to the specified device
   *
   * @param self: object pointer
   * @param index: index register
   * @param array: data to write
   * @param len: length of array
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_TxSeries(VL53L1X_DS *const self, uint16_t index, const uint8_t array[], size_t len);

  /**
   * @brief Continuously read multiple data from the specified device
   *
   * @param self: object pointer
   * @param index: index register
   * @param array: data to recieve
   * @param len: length of array
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_RxSeries(VL53L1X_DS *const self, uint16_t index, volatile uint8_t array[], size_t len);

  /**
   * @brief Check if the module hardware is ready
   * 
   * @param self: object pointer
   * @return bool_t: True / False
   */
  bool_t VL53L1X_isBootReady(VL53L1X_DS *const self);

  /**
   * @brief Enable module measurements
   * 
   * @param self: object pointer
   * @return task_t: Success / Fail 
   */
  task_t VL53L1X_StartRanging(VL53L1X_DS *const self);

  /**
   * @brief Check if data is measured
   * 
   * @param self: object pointer
   * @return bool_t: True / False
   */
  bool_t VL53L1X_isDataReady(VL53L1X_DS *const self);

  /**
   * @brief Get interrupt polarity to check data status
   * 
   * @param self: object pointer
   * @param polarity: store state 
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_GetInterruptPolarity(VL53L1X_DS *const self, volatile uint8_t *const polarity);
  
  /**
   * @brief Fetch distance
   * 
   * @param self: object pointer
   * @param distance: store data
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_GetDistance(VL53L1X_DS *const self, volatile uint16_t *const distance);
  
  /**
   * @brief Clear interrupt state
   * 
   * @param self: object pointer
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_ClearInterrupt(VL53L1X_DS *const self);
  
  /**
   * @brief Disable module measurements
   * 
   * @param self: object pointer
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_StopRanging(VL53L1X_DS *const self);

  /* ---------------------------------------------------------------- Interface Code End */

#endif // STM32F103xx_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _VL53L1X_H_
