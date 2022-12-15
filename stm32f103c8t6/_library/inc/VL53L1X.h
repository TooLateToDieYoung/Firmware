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
    bool_t isAfterTx;

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
  task_t VL53L1X_Destructor(VL53L1X_DS *self);

  /**
   * @brief vl53l1x try to generate I2C start condition
   *
   * @param self: object pointer
   * @param timeout:
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_Start(VL53L1X_DS *const self, uint16_t timeout);

  /**
   * @brief
   *
   * @param self
   * @param isRead
   * @param timeout
   * @return task_t
   */
  task_t VL53L1X_Device(VL53L1X_DS *const self, bool_t isRead, uint16_t timeout);

  /**
   * @brief
   *
   * @param self
   * @param isAckNext
   * @param timeout
   * @return task_t
   */
  task_t VL53L1X_PreloadStatus(VL53L1X_DS *const self, bool_t isAckNext, uint16_t timeout);

  /**
   * @brief Transfer one byte
   *
   * @param self: object pointer
   * @param byte: byte to write
   * @param timeout: set try times
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_TxByte(VL53L1X_DS *const self, const uint8_t byte, uint16_t timeout);

  /**
   * @brief Transfer one byte
   *
   * @param self: object pointer
   * @param byte: byte to save read data
   * @param isAckNext: decide whether to generate ack for next rx bytes
   * @param timeout: set try times
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_RxByte(VL53L1X_DS *const self, volatile uint8_t *const byte, bool_t isAckNext, uint16_t timeout);

  /**
   * @brief
   *
   * @param self: object pointer
   * @param timeout: set try times
   * @return task_t: Success / Fail
   */
  task_t VL53L1X_Stop(VL53L1X_DS *const self, bool_t isAfterTx, uint16_t timeout);

  /* ---------------------------------------------------------------- Interface Code End */

#endif // STM32F103xx_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _VL53L1X_H_
