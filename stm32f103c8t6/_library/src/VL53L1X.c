#include "VL53L1X.h"
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

VL53L1X_DS *VL53L1X_Constructor(I2C_TypeDef *I2Cx, const uint8_t address)
{
  VL53L1X_DS *obj = (VL53L1X_DS *)calloc(1, sizeof(VL53L1X_DS));

  if (obj == NULL)
    return NULL;

  obj->I2Cx = I2Cx;
  obj->address = address;

  return obj;
}

task_t VL53L1X_Destructor(VL53L1X_DS *self)
{
  free(self);

  return Success;
}

task_t VL53L1X_Start(VL53L1X_DS *const self, uint16_t timeout)
{
  do
  {
    if (_InterfaceI2C_Start(self->I2Cx) == Success)
      return Success;
  } while (timeout--);

  return Fail;
}

task_t VL53L1X_Device(VL53L1X_DS *const self, bool_t isRead, uint16_t timeout)
{
  do
  {
    if (_InterfaceI2C_Device(self->I2Cx, self->address, isRead) == Success)
      return Success;
  } while (timeout--);

  return Fail;
}

task_t VL53L1X_PreloadStatus(VL53L1X_DS *const self, bool_t isAckNext, uint16_t timeout)
{
  do
  {
    if (_InterfaceI2C_PreloadStatus(self->I2Cx, isAckNext) == Success)
      return Success;
  } while (timeout--);

  return Fail;
}

task_t VL53L1X_TxByte(VL53L1X_DS *const self, const uint8_t byte, uint16_t timeout)
{
  do
  {
    if (_InterfaceI2C_TxByte(self->I2Cx, byte) == Success)
    {
      self->isAfterTx = True;
      return Success;
    }
  } while (timeout--);

  return Fail;
}

task_t VL53L1X_RxByte(VL53L1X_DS *const self, volatile uint8_t *const byte, bool_t isAckNext, uint16_t timeout)
{
  do
  {
    if (_InterfaceI2C_RxByte(self->I2Cx, byte, isAckNext) == Success)
      return Success;
  } while (timeout--);

  return Fail;
}

task_t VL53L1X_Stop(VL53L1X_DS *const self, bool_t isAfterTx, uint16_t timeout)
{
  do
  {
    if (_InterfaceI2C_Stop(self->I2Cx, self->isAfterTx) == Success)
    {
      self->isAfterTx = False;
      return Success;
    }
  } while (timeout--);

  return Fail;
}

/* ---------------------------------------------------------------- Class Public Functions End */

#endif // STM32F103xx_UNREADY
