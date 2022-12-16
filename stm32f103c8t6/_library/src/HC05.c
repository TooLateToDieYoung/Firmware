#include "HC05.h"
#include <stdlib.h>

#ifndef STM32F103xx_UNREADY

/** Class Private Functions Begin ---------------------------------------------------------------
 * @brief
 *
 */

/* ---------------------------------------------------------------- Class Private Functions End */

/** Class Public Functions Begin ---------------------------------------------------------------
 * @brief
 *
 */

HC05_DS *HC05_Constructor(USART_TypeDef *USARTx)
{
  HC05_DS *obj = (HC05_DS *)calloc(1, sizeof(HC05_DS));

  if (obj == NULL)
    return NULL;

  obj->USARTx = USARTx;

  return obj;
}

task_t HC05_Destructor(HC05_DS *self)
{
  free(self);

  return Success;
}

task_t HC05_TxByte(HC05_DS *const self, const uint8_t byte, uint16_t timeout)
{
  do
  {
    if (_InterfaceUSART_TxByte(self->USARTx, byte) == Success)
      return Success;
  } while (timeout--);

  return Fail;
}

task_t HC05_RxByte(HC05_DS *const self, volatile uint8_t *byte, uint16_t timeout)
{
  do
  {
    if (_InterfaceUSART_RxByte(self->USARTx, byte) == Success)
      return Success;
  } while (timeout--);

  return Fail;
}

/* ---------------------------------------------------------------- Class Public Functions End */

#endif // STM32F103xx_UNREADY
