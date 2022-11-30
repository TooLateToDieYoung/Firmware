#include "lsm6ds3.h"
#include <stdlib.h>

#ifndef SPI_UNREADY

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
static task_t _LSM6DS3_ShiftByte(LSM6DS3_DS * const self, uint8_t TxByte, volatile uint8_t * const RxByte, uint16_t timeout)
{
  while( _InterfaceSPI_TxByte(self->SPIx, TxByte) != Success ) {
    if( !timeout ) return Fail;
    timeout--;
  }
	
	while( _InterfaceSPI_RxByte(self->SPIx, RxByte) != Success ) {
    if( !timeout ) return Fail;
    timeout--;
  }

	return Success;
}

/* ---------------------------------------------------------------- Class Private Functions End */

/** Class Public Functions Begin ---------------------------------------------------------------
 * @brief 
 * 
 */

/**
 * @brief Constructor (dynamic memory)
 * 
 * @param SPIx: Defined in the stm32f103xx series header
 * @param GPIOx: 
 * | Defined in the stm32f103xx series header.
 * | CS wire ask for SPI select slave device.
 * @param pinOrder: 0 ~ 15, pin order of CS wire
 * @return LSM6DS3_DS*: dynamic memory pointer
 */
LSM6DS3_DS * LSM6DS3_Constructor(SPI_TypeDef * SPIx, GPIO_TypeDef * GPIOx, uint8_t pinOrder)
{
  LSM6DS3_DS * obj = (LSM6DS3_DS*)calloc(1, sizeof(LSM6DS3_DS));

  if( obj == NULL ) return NULL;

  obj->SPIx = SPIx;

  obj->CS.GPIOx = GPIOx;
  obj->CS.pin   = _BIT(pinOrder);

  return obj;
}

/**
 * @brief Destructor
 *
 * @param self: object pointer
 * @return task_t: Success / Fail
 */
task_t LSM6DS3_Destructor(LSM6DS3_DS * self)
{
  free(self);

  return Success;
}

/**
 * @brief write one byte into the register
 * 
 * @param self: object pointer
 * @param reg: target register, refer to LSM6DS3_Register_Enum or datasheet
 * @param byte: byte to write in register
 * @param timeout: try times
 * @return task_t: Success / Fail
 */
task_t LSM6DS3_setRegister(LSM6DS3_DS * const self, uint8_t reg, const uint8_t byte, uint16_t timeout)
{
  task_t result = Success;

  self->CS.GPIOx->BSRR = self->CS.pin << 16;

  if     ( _LSM6DS3_ShiftByte(self,  reg, 0, timeout) != Success ) result = Fail;
  else if( _LSM6DS3_ShiftByte(self, byte, 0, timeout) != Success ) result = Fail;

  self->CS.GPIOx->BSRR = self->CS.pin ;

  return result;
}

/**
 * @brief read one byte from the register
 * 
 * @param self: object pointer
 * @param reg: target register, refer to LSM6DS3_Register_Enum or datasheet
 * @param byte: save byte, read from register
 * @param timeout: try times
 * @return task_t: Success / Fail
 */
task_t LSM6DS3_getRegister(LSM6DS3_DS * const self, uint8_t reg, volatile uint8_t * const byte, uint16_t timeout)
{
  task_t result = Success;

  reg |= 0x80;

  self->CS.GPIOx->BSRR = self->CS.pin << 16;
  
  if     ( _LSM6DS3_ShiftByte(self, reg,  0, timeout) != Success ) result = Fail;
  else if( _LSM6DS3_ShiftByte(self, 0, byte, timeout) != Success ) result = Fail;

  self->CS.GPIOx->BSRR = self->CS.pin ;

  return result;
}

/* ---------------------------------------------------------------- Class Public Functions End */

#endif // SPI_UNREADY
