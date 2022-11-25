/**
 * @file InterfaceUART.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief
 * @warning
 * @version 0.1
 * @date 2022-11-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _INTERFACE_UART_H_
#define _INTERFACE_UART_H_

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
  #define UART_UNREADY
  #warning "This library must be working under stm32f1xx series"
#endif

#ifndef UART_UNREADY

#include "common.h"

/**
 * @brief indicates that the caller is from the main thread or an interrupt
 * 
 */
typedef enum { UART_MAIN_THREAD, UART_INTERRUPT } InterfaceUART_ThreadType_Enum;

/** Interface Begin --------------------------------------------------------------------
 * @brief 
 * | Almost directly through the register operation. \n
 * | For each function, they provide an alternative, \n 
 * | with the same functionality, implemented with the LL library.
 * 
 * @warning Do not change these codes, it may cause errors
 */

static inline task_t _InterfaceUART_TxByte(USART_TypeDef * USARTx, uint8_t byte, InterfaceUART_ThreadType_Enum thread)
{
  switch( thread ) {
    case UART_MAIN_THREAD: {
      USARTx->DR = byte;
      while( !_MASK(USARTx->SR, _BIT(6)) ) { }
      return Success;
    }
    case UART_INTERRUPT: { // ? Not sure if it works
      if( !_MASK(USARTx->SR, _BIT(6)) ) return Fail;
      USARTx->DR = byte;
      return Success;
    }
    default: return Fail;
  }
}

static inline task_t _InterfaceUART_RxByte(USART_TypeDef * USARTx, volatile uint8_t * byte, InterfaceUART_ThreadType_Enum thread)
{
  switch( thread ) {
    case UART_MAIN_THREAD: {
      while( !_MASK(USARTx->SR, _BIT(5)) ) { }
      *byte = (uint8_t)USARTx->DR;
      return Success;
    }
    case UART_INTERRUPT: { // ? Not sure if it works
      if( !_MASK(USARTx->SR, _BIT(5)) ) return Fail;
      *byte = (uint8_t)USARTx->DR;
      return Success;
    }
    default: return Fail;
  }
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
  for(size_t i = 0; i < len; ++i) _InterfaceUART_TxByte(USARTx, array[i], UART_MAIN_THREAD);

  return Success;
}

static inline task_t __R_Series(USART_TypeDef * USARTx, volatile uint8_t array[], size_t len)
{
  for(size_t i = 0; i < len; ++i) _InterfaceUART_RxByte(USARTx, &array[i], UART_MAIN_THREAD);

  return Success;
}

/* --------------------------------------------------------------------- Demo Code End */

#endif // UART_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _INTERFACE_UART_H_
