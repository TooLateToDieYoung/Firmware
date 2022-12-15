#include "Button.h"
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

Button_DS *Button_Constructor(const port_t *const pin)
{
  Button_DS *obj = (Button_DS *)calloc(1, sizeof(Button_DS));

  if (obj == NULL)
    return NULL;

  obj->pin.GPIOx = pin->GPIOx;
  obj->pin.order = pin->order;

  return obj;
}

task_t Button_Destructor(Button_DS *const self)
{
  free(self);

  return Success;
}

bool_t Button_isFree(Button_DS *const self)
{
  return _MASK(self->pin.GPIOx->IDR, _BIT(self->pin.order)) ? True : False;
}

/* ---------------------------------------------------------------- Class Public Functions End */

#endif // STM32F103xx_UNREADY
