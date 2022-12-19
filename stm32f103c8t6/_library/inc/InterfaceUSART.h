/**
 * @file InterfaceUSART.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief
 * | People who use this library can easily realize \n
 * | their own UART read / write functions via these interfaces.
 * | All the functions wrapped in the "interface" \n
 * | can be called from the main thread or interrupts.
 *
 * @warning
 * @version 0.1
 * @date 2022-11-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _INTERFACE_USART_H_
#define _INTERFACE_USART_H_

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
   * @param USARTx: defined in the stm32f103xx series header
   * @param byte: data to send
   * @return task_t: Success / Fail
   */
  static inline task_t _InterfaceUSART_TxByte(USART_TypeDef *USARTx, const uint8_t byte)
  {
    // check if TXE
    if (!_MASK(USARTx->SR, _BIT(7)))
      return Fail;

    // send byte
    USARTx->DR = byte;

    return Success;
  }

  /**
   * @brief receive a byte
   *
   * @param USARTx: defined in the stm32f103xx series header
   * @param byte: buffer for recording data
   * @return task_t: Success / Fail
   */
  static inline task_t _InterfaceUSART_RxByte(USART_TypeDef *USARTx, volatile uint8_t *const byte)
  {
    // check if RXNE
    if (!_MASK(USARTx->SR, _BIT(5)))
      return Fail;

    // receive byte
    *byte = (uint8_t)USARTx->DR;

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
   * @brief Continuously send multiple data
   *
   * @param USARTx: defined in the stm32f103xx series header
   * @param array: datas to send
   * @param len: array length
   * @return task_t: Success / Fail
   */
  static inline task_t _InterfaceUSART_TxSeries(USART_TypeDef *USARTx, const uint8_t array[], size_t len)
  {
    for (size_t i = 0; i < len; ++i)
      while (_InterfaceUSART_TxByte(USARTx, array[i]) != Success)
      {
      }

    return Success;
  }

  /**
   * @brief Continuously receive multiple data
   *
   * @param USARTx: defined in the stm32f103xx series header
   * @param array: buffer to receive datas
   * @param len: array length
   * @return task_t: Success / Fail
   */
  static inline task_t _InterfaceUSART_RxSeries(USART_TypeDef *USARTx, volatile uint8_t array[], size_t len)
  {
    for (size_t i = 0; i < len; ++i)
      while (_InterfaceUSART_RxByte(USARTx, &array[i]) != Success)
      {
      }

    return Success;
  }

  /* --------------------------------------------------------------------- Demo Code End */

#endif // STM32F103xx_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _INTERFACE_USART_H_
