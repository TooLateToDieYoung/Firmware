#include "SevenSegment.h"
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

SevenSegment_DS *SevenSegment_Constructor(const port_t *const common, const port_t io[])
{
  SevenSegment_DS *obj = (SevenSegment_DS *)calloc(1, sizeof(SevenSegment_DS));

  if (obj == NULL)
    return NULL;

  obj->common.GPIOx = common->GPIOx;
  obj->common.order = common->order;

  for (size_t i = 0; i < 8; ++i)
  {
    obj->io[i].GPIOx = io[i].GPIOx;
    obj->io[i].order = io[i].order;
  }

  return obj;
}

task_t SevenSegment_Destructor(SevenSegment_DS *const self)
{
  free(self);
  return Success;
}

task_t SevenSegment_Set(SevenSegment_DS *const self, flag8_t segment)
{
  for (size_t i = 0; i < 8; ++i)
    self->io[i].GPIOx->BSRR = _MASK(segment, _BIT(i)) ? _BIT(self->io[i].order) << 16 : _BIT(self->io[i].order);

  self->common.GPIOx->BSRR = _BIT(self->common.order) << 16;

  return Success;
}

task_t SevenSegment_Reset(SevenSegment_DS *const self)
{
  self->common.GPIOx->BSRR = _BIT(self->common.order);
  return Success;
}

/* ---------------------------------------------------------------- Class Public Functions End */

#endif // STM32F103xx_UNREADY
