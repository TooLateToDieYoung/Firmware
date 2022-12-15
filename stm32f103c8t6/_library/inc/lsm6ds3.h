/**
 * @file LSM6DS3.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief
 * @warning
 * @version 0.1
 * @date 2022-11-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _LSM6DS3_H_
#define _LSM6DS3_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "InterfaceSPI.h"

#ifndef STM32F103xx_UNREADY

  /** Def. Begin -------------------------------------------------------------------------
   * @brief only a few basic registers.
   * | for more infomation, plz refer to the datasheet.
   *
   */
  typedef enum
  {
    WHO_AM_I = 0x0F,
    CTRL_ACCE,
    CTRL_GYRO,
    STATUS_R = 0x1E,
    GYRO_X_L = 0x22,
    GYRO_X_H,
    GYRO_Y_L,
    GYRO_Y_H,
    GYRO_Z_L,
    GYRO_Z_H,
    ACCE_X_L = 0x28,
    ACCE_X_H,
    ACCE_Y_L,
    ACCE_Y_H,
    ACCE_Z_L,
    ACCE_Z_H
  } LSM6DS3_Register_Enum;

  /* -------------------------------------------------------------------------- Def. End */

  /** Data Structure Begin ---------------------------------------------------------------
   * @brief class data sturcture
   * @warning Plz operate the object through the interface
   *
   */

  typedef struct
  {

    SPI_TypeDef *SPIx; // * SPIx peripheral

    port_t CS; // * SPI CS wire for selecting the slave device             

  } LSM6DS3_DS;

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
   * @param SPIx: Defined in the stm32f103xx series header
   * @param CS: CS wire ask for SPI select slave device.
   * @return LSM6DS3_DS*: dynamic memory pointer
   */
  LSM6DS3_DS *LSM6DS3_Constructor(SPI_TypeDef *SPIx, const port_t *const CS);

  /**
   * @brief Destructor
   *
   * @param self: object pointer
   * @return task_t: Success / Fail
   */
  task_t LSM6DS3_Destructor(LSM6DS3_DS *const self);

  /**
   * @brief select device -> CS wire low
   *
   * @param self
   */
  void LSM6DS3_HoldDevice(LSM6DS3_DS *const self);

  /**
   * @brief release device -> CS wire high
   *
   * @param self
   */
  void LSM6DS3_FreeDevice(LSM6DS3_DS *const self);

  /**
   * @brief Transfer one byte
   *
   * @param self: object pointer
   * @param byte: byte to write
   * @param timeout: set try times
   * @return task_t: Success / Fail
   */
  task_t LSM6DS3_TxByte(LSM6DS3_DS *const self, const uint8_t byte, uint16_t timeout);

  /**
   * @brief Transfer one byte
   *
   * @param self: object pointer
   * @param byte: byte to save read data
   * @param timeout: set try times
   * @return task_t: Success / Fail
   */
  task_t LSM6DS3_RxByte(LSM6DS3_DS *const self, volatile uint8_t *const byte, uint16_t timeout);

  /**
   * @brief write one byte into the register
   *
   * @param self: object pointer
   * @param reg: target register, refer to LSM6DS3_Register_Enum or datasheet
   * @param byte: byte to write in register
   * @param timeout: try times
   * @return task_t: Success / Fail
   */
  task_t LSM6DS3_setRegister(LSM6DS3_DS *const self, uint8_t reg, const uint8_t byte, uint16_t timeout);

  /**
   * @brief read one byte from the register
   *
   * @param self: object pointer
   * @param reg: target register, refer to LSM6DS3_Register_Enum or datasheet
   * @param byte: save byte, read from register
   * @param timeout: try times
   * @return task_t: Success / Fail
   */
  task_t LSM6DS3_getRegister(LSM6DS3_DS *const self, uint8_t reg, volatile uint8_t *const byte, uint16_t timeout);

  /* ---------------------------------------------------------------- Interface Code End */

#endif // STM32F103xx_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _LSM6DS3_H_
