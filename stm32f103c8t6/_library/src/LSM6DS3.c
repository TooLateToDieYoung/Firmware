#include "LSM6DS3.h"
#include <stdlib.h>

#ifndef STM32F103xx_UNREADY

/** Class Private Functions Begin ---------------------------------------------------------------
 * @brief
 *
 */

/**
 * @brief shift write / read byte in SPI data wires
 *
 * @param self: object pointer
 * @param TxByte: byte to write
 * @param RxByte: byte to save read data
 * @param timeout: try times
 * @return task_t: Success / Fail
 */
static task_t _LSM6DS3_ShiftByte(LSM6DS3_DS *const self, const uint8_t TxByte, volatile uint8_t *const RxByte, uint16_t timeout)
{
  while (_InterfaceSPI_TxByte(self->SPIx, TxByte) != Success)
  {
    if (!timeout)
      return Fail;
    timeout--;
  }

  while (_InterfaceSPI_RxByte(self->SPIx, RxByte) != Success)
  {
    if (!timeout)
      return Fail;
    timeout--;
  }

  return Success;
}

/* ---------------------------------------------------------------- Class Private Functions End */

/** Class Public Functions Begin ---------------------------------------------------------------
 * @brief
 *
 */

LSM6DS3_DS *LSM6DS3_Constructor(SPI_TypeDef *SPIx, const port_t *const CS)
{
  LSM6DS3_DS *obj = (LSM6DS3_DS *)calloc(1, sizeof(LSM6DS3_DS));

  if (obj == NULL)
    return NULL;

  obj->SPIx = SPIx;

  obj->CS.GPIOx = CS->GPIOx;
  obj->CS.order = CS->order;

  return obj;
}

task_t LSM6DS3_Destructor(LSM6DS3_DS *const self)
{
  free(self);

  return Success;
}

task_t LSM6DS3_DefaultInit(LSM6DS3_DS *const self)
{
  uint8_t value = 0x60; // 412Hz only enable Acce.
  do
  {
    value = 0x60;
    LSM6DS3_setRegister(self, CTRL_ACCE, value, 1000);
    LSM6DS3_getRegister(self, CTRL_ACCE, &value, 1000);
  } while (value != 0x60);

  return Success;
}

void LSM6DS3_HoldDevice(LSM6DS3_DS *const self)
{
  self->CS.GPIOx->BSRR = _BIT(self->CS.order) << 16;
}

void LSM6DS3_FreeDevice(LSM6DS3_DS *const self)
{
  self->CS.GPIOx->BSRR = _BIT(self->CS.order);
}

task_t LSM6DS3_setRegister(LSM6DS3_DS *const self, const uint8_t reg, const uint8_t byte, uint16_t timeout)
{
  task_t result = Success;

  LSM6DS3_HoldDevice(self);

  if (_LSM6DS3_ShiftByte(self, reg, 0, timeout) != Success)
    result = Fail;
  else if (_LSM6DS3_ShiftByte(self, byte, 0, timeout) != Success)
    result = Fail;

  LSM6DS3_FreeDevice(self);

  return result;
}

task_t LSM6DS3_getRegister(LSM6DS3_DS *const self, const uint8_t reg, volatile uint8_t *const byte, uint16_t timeout)
{
  task_t result = Success;

  const uint8_t rreg = (reg | 0x80);

  LSM6DS3_HoldDevice(self);

  if (_LSM6DS3_ShiftByte(self, rreg, 0, timeout) != Success)
    result = Fail;
  else if (_LSM6DS3_ShiftByte(self, 0, byte, timeout) != Success)
    result = Fail;

  LSM6DS3_FreeDevice(self);

  return result;
}
/* ---------------------------------------------------------------- Class Public Functions End */

#endif // STM32F103xx_UNREADY
