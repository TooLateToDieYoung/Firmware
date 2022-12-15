/**
 * @file InterfaceSPI.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief
 * | People who use this library can easily realize \n
 * | their own I2C read / write functions via these interfaces.
 * | All the functions wrapped in the "interface" \n
 * | can be called from the main thread or interrupts.
 *
 * @warning
 * @version 0.1
 * @date 2022-11-29
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _INTERFACE_SPI_H_
#define _INTERFACE_SPI_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "Common.h"

#ifndef STM32F103xx_UNREADY

  /** Interface Begin --------------------------------------------------------------------
   * @brief
   * | Almost directly through the register operation. \n
   * | For each function, they provide an alternative, \n
   * | with the same functionality, implemented with the LL library.
   *
   * @warning Do not change these codes, it may cause errors
   */

  /**
   * @brief transfer a byte
   *
   * @param SPIx: defined in the stm32f103xx series header
   * @param byte: data to send
   * @return task_t: Success / Fail
   */
  static inline task_t _InterfaceSPI_TxByte(SPI_TypeDef *SPIx, const uint8_t byte)
  {
    // check if TXE
    if (!_MASK(SPIx->SR, _BIT(1)))
      return Fail;

    // send byte
    SPIx->DR = byte;

    return Success;
  }

  /**
   * @brief receive a byte
   *
   * @param SPIx: defined in the stm32f103xx series header
   * @param byte: buffer for recording data
   * @return task_t: Success / Fail
   */
  static inline task_t _InterfaceSPI_RxByte(SPI_TypeDef *SPIx, volatile uint8_t *const byte)
  {
    // check if RXNE
    if (!_MASK(SPIx->SR, _BIT(0)))
      return Fail;

    // ensure ( byte != NULL )
    if (!byte)
    {
      uint8_t temp = (uint8_t)SPIx->DR;
      (void)temp; // read SPIx->DR & ignore temp
    }
    else
    {
      *byte = (uint8_t)SPIx->DR; // record data
    }

    return Success;
  }

  /* ---------------------------------------------------------------- Interface Code End */

  /** Demo Code Begin --------------------------------------------------------------------
   * @brief
   * | The following is the demo code for standard I2C communication protocol \n
   * | Users can rewrite the content of the function according to \n
   * | their own needs, or customize new functions
   */

  /**
   * @brief Full Duplex Communication
   * | The data registers of the master and the slave \n
   * | will shift the same number of bits synchronously.
   *
   * @example
   * | if you want to tx a byte -> _InterfaceSPI_ShiftByte(SPIx, byte,     0);
   * | if you want to rx a byte -> _InterfaceSPI_ShiftByte(SPIx,    0, &byte);
   *
   * @param SPIx: defined in the stm32f103xx series header
   * @param TxByte: data to send
   * @param RxByte: buffer for recording data
   * @return task_t: Success / Fail
   */
  static inline task_t _InterfaceSPI_ShiftByte(SPI_TypeDef *SPIx, const uint8_t TxByte, volatile uint8_t *const RxByte)
  {
    while (_InterfaceSPI_TxByte(SPIx, TxByte) != Success)
    {
    }

    while (_InterfaceSPI_RxByte(SPIx, RxByte) != Success)
    {
    }

    return Success;
  }

  /* --------------------------------------------------------------------- Demo Code End */

#endif // STM32F103xx_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _INTERFACE_SPI_H_
