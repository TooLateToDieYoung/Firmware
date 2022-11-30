#include "hc05.h"
#include <stdlib.h>

#ifndef USART_UNREADY

/** Class Private Functions Begin ---------------------------------------------------------------
 * @brief 
 * 
 */

/* ---------------------------------------------------------------- Class Private Functions End */

/** Class Public Functions Begin ---------------------------------------------------------------
 * @brief 
 * 
 */

/**
 * @brief Constructor (dynamic memory)
 * 
 * @param USARTx: defined in the stm32f103xx series header
 * @return HC05_DS *: dynamic memory pointer
 */
HC05_DS * HC05_Constructor(USART_TypeDef * USARTx)
{
  HC05_DS * obj = (HC05_DS*)calloc(1, sizeof(HC05_DS));

  if( obj == NULL ) return NULL;

  obj->USARTx = USARTx;

  return obj;
}

/**
 * @brief Destructor
 * 
 * @param self: object pointer
 * @return task_t: Success / Fail
 */
task_t HC05_Destructor(HC05_DS * self)
{
  free(self);

  return Success;
}

/**
 * @brief Transfer one byte
 * 
 * @param self: object pointer 
 * @param byte: byte to write
 * @param timeout: set try times
 * @return task_t: Success / Fail 
 */
task_t HC05_TxByte(HC05_DS * const self, const uint8_t byte, uint16_t timeout)
{
  do {
    if( _InterfaceUSART_TxByte(self->USARTx, byte) == Success ) return Success;
  } while( timeout-- );

  return Fail;
}

/**
 * @brief Transfer one byte
 * 
 * @param self: object pointer 
 * @param byte: byte to save read data
 * @param timeout: set try times
 * @return task_t: Success / Fail 
 */
task_t HC05_RxByte(HC05_DS * const self, volatile uint8_t * byte, uint16_t timeout)
{
  do {
    if( _InterfaceUSART_RxByte(self->USARTx, byte) == Success ) return Success;
  } while( timeout-- );

  return Fail;
}

/* ---------------------------------------------------------------- Class Public Functions End */

#endif // USART_UNREADY
