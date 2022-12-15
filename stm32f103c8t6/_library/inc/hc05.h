/**
 * @file HC05.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief
 * @warning
 * @version 0.1
 * @date 2022-11-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _HC05_H_
#define _HC05_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "InterfaceUSART.h"

#ifndef STM32F103xx_UNREADY

/** Data Structure Begin ---------------------------------------------------------------
 * @brief class data sturcture
 * @warning Plz operate the object through the interface
 * 
 */

typedef struct { 

  USART_TypeDef * USARTx; 
  
} HC05_DS;

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
 * @param USARTx: defined in the stm32f103xx series header
 * @return HC05_DS*: dynamic memory pointer
 */
HC05_DS * HC05_Constructor(USART_TypeDef * USARTx);

/**
 * @brief Destructor
 *
 * @param self: object pointer
 * @return task_t: Success / Fail
 */
task_t HC05_Destructor(HC05_DS * self);

/**
 * @brief Transfer one byte
 * 
 * @param self: object pointer 
 * @param byte: byte to write
 * @param timeout: set try times
 * @return task_t: Success / Fail 
 */
task_t HC05_TxByte(HC05_DS * const self, const uint8_t byte, uint16_t timeout);

/**
 * @brief Transfer one byte
 * 
 * @param self: object pointer 
 * @param byte: byte to save read data
 * @param timeout: set try times
 * @return task_t: Success / Fail 
 */
task_t HC05_RxByte(HC05_DS * const self, volatile uint8_t * byte, uint16_t timeout);

/* ---------------------------------------------------------------- Interface Code End */

#endif // STM32F103xx_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _HC05_H_
