/**
 * @file InterfaceUSART.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief
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
extern "C" {
#endif // __cplusplus

#if   defined( STM32F103x4 ) || defined( STM32F103x6 )
  #include "stm32f103x8.h"
#elif defined( STM32F103x8 ) || defined( STM32F103xB )
  #include "stm32f103xb.h"
#elif defined( STM32F103xC ) || defined( STM32F103xD ) || defined( STM32F103xE )
  #include "stm32f103xe.h"
#elif defined( STM32F103xF ) || defined( STM32F103xG )
  #include "stm32f103xg.h"
#else
  #define USART_UNREADY
  #warning "This library must be working under stm32f1xx series"
#endif

#ifndef USART_UNREADY

#include "common.h"

/** Interface Begin --------------------------------------------------------------------
 * @brief 
 * | Almost directly through the register operation. \n
 * | For each function, they provide an alternative, \n 
 * | with the same functionality, implemented with the LL library.
 * 
 * @warning Do not change these codes, it may cause errors
 */

/**
 * @brief 
 * 
 * @param USARTx 
 * @param byte 
 * @param thread 
 * @return task_t 
 */
static inline task_t _InterfaceUSART_TxByte(USART_TypeDef * USARTx, uint8_t byte)
{
  // TXE
  if( !_MASK(USARTx->SR, _BIT(7)) ) return Fail;

  USARTx->DR = byte;

  return Success;
}

/**
 * @brief 
 * 
 * @param USARTx 
 * @param byte 
 * @param thread 
 * @return task_t 
 */
static inline task_t _InterfaceUSART_RxByte(USART_TypeDef * USARTx, volatile uint8_t * byte)
{
  // RXNE
  if( !_MASK(USARTx->SR, _BIT(5)) ) return Fail;

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

static inline task_t __W_Series(USART_TypeDef * USARTx, const uint8_t array[], size_t len)
{
  for(size_t i = 0; i < len; ++i)
    while( _InterfaceUSART_TxByte(USARTx, array[i]) != Success ) { }

  return Success;
}

static inline task_t __R_Series(USART_TypeDef * USARTx, volatile uint8_t array[], size_t len)
{
  for(size_t i = 0; i < len; ++i) 
    while( _InterfaceUSART_RxByte(USARTx, &array[i]) != Success ) { }

  return Success;
}

/* --------------------------------------------------------------------- Demo Code End */

#endif // USART_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _INTERFACE_USART_H_
