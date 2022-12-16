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

task_t VL53L1X_Destructor(VL53L1X_DS *const self)
{
  free(self);

  return Success;
}

task_t VL53L1X_DefaultInit(VL53L1X_DS *const self)
{
  const uint8_t defaultTable[] = {
      0x00, /* 0x2d : set bit 2 and 5 to 1 for fast plus mode (1MHz I2C), else don't touch */
      0x00, /* 0x2e : bit 0 if I2C pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */
      0x00, /* 0x2f : bit 0 if GPIO pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD) */
      0x01, /* 0x30 : set bit 4 to 0 for active high interrupt and 1 for active low (bits 3:0 must be 0x1), use SetInterruptPolarity() */
      0x02, /* 0x31 : bit 1 = interrupt depending on the polarity, use CheckForDataReady() */
      0x00, /* 0x32 : not user-modifiable */
      0x02, /* 0x33 : not user-modifiable */
      0x08, /* 0x34 : not user-modifiable */
      0x00, /* 0x35 : not user-modifiable */
      0x08, /* 0x36 : not user-modifiable */
      0x10, /* 0x37 : not user-modifiable */
      0x01, /* 0x38 : not user-modifiable */
      0x01, /* 0x39 : not user-modifiable */
      0x00, /* 0x3a : not user-modifiable */
      0x00, /* 0x3b : not user-modifiable */
      0x00, /* 0x3c : not user-modifiable */
      0x00, /* 0x3d : not user-modifiable */
      0xff, /* 0x3e : not user-modifiable */
      0x00, /* 0x3f : not user-modifiable */
      0x0F, /* 0x40 : not user-modifiable */
      0x00, /* 0x41 : not user-modifiable */
      0x00, /* 0x42 : not user-modifiable */
      0x00, /* 0x43 : not user-modifiable */
      0x00, /* 0x44 : not user-modifiable */
      0x00, /* 0x45 : not user-modifiable */
      0x20, /* 0x46 : interrupt configuration 0->level low detection, 1-> level high, 2-> Out of window, 3->In window, 0x20-> New sample ready , TBC */
      0x0b, /* 0x47 : not user-modifiable */
      0x00, /* 0x48 : not user-modifiable */
      0x00, /* 0x49 : not user-modifiable */
      0x02, /* 0x4a : not user-modifiable */
      0x0a, /* 0x4b : not user-modifiable */
      0x21, /* 0x4c : not user-modifiable */
      0x00, /* 0x4d : not user-modifiable */
      0x00, /* 0x4e : not user-modifiable */
      0x05, /* 0x4f : not user-modifiable */
      0x00, /* 0x50 : not user-modifiable */
      0x00, /* 0x51 : not user-modifiable */
      0x00, /* 0x52 : not user-modifiable */
      0x00, /* 0x53 : not user-modifiable */
      0xc8, /* 0x54 : not user-modifiable */
      0x00, /* 0x55 : not user-modifiable */
      0x00, /* 0x56 : not user-modifiable */
      0x38, /* 0x57 : not user-modifiable */
      0xff, /* 0x58 : not user-modifiable */
      0x01, /* 0x59 : not user-modifiable */
      0x00, /* 0x5a : not user-modifiable */
      0x08, /* 0x5b : not user-modifiable */
      0x00, /* 0x5c : not user-modifiable */
      0x00, /* 0x5d : not user-modifiable */
      0x01, /* 0x5e : not user-modifiable */
      0xcc, /* 0x5f : not user-modifiable */
      0x0f, /* 0x60 : not user-modifiable */
      0x01, /* 0x61 : not user-modifiable */
      0xf1, /* 0x62 : not user-modifiable */
      0x0d, /* 0x63 : not user-modifiable */
      0x01, /* 0x64 : Sigma threshold MSB (mm in 14.2 format for MSB+LSB), use SetSigmaThreshold(), default value 90 mm  */
      0x68, /* 0x65 : Sigma threshold LSB */
      0x00, /* 0x66 : Min count Rate MSB (MCPS in 9.7 format for MSB+LSB), use SetSignalThreshold() */
      0x80, /* 0x67 : Min count Rate LSB */
      0x08, /* 0x68 : not user-modifiable */
      0xb8, /* 0x69 : not user-modifiable */
      0x00, /* 0x6a : not user-modifiable */
      0x00, /* 0x6b : not user-modifiable */
      0x00, /* 0x6c : Intermeasurement period MSB, 32 bits register, use SetIntermeasurementInMs() */
      0x00, /* 0x6d : Intermeasurement period */
      0x0f, /* 0x6e : Intermeasurement period */
      0x89, /* 0x6f : Intermeasurement period LSB */
      0x00, /* 0x70 : not user-modifiable */
      0x00, /* 0x71 : not user-modifiable */
      0x00, /* 0x72 : distance threshold high MSB (in mm, MSB+LSB), use SetD:tanceThreshold() */
      0x00, /* 0x73 : distance threshold high LSB */
      0x00, /* 0x74 : distance threshold low MSB ( in mm, MSB+LSB), use SetD:tanceThreshold() */
      0x00, /* 0x75 : distance threshold low LSB */
      0x00, /* 0x76 : not user-modifiable */
      0x01, /* 0x77 : not user-modifiable */
      0x0f, /* 0x78 : not user-modifiable */
      0x0d, /* 0x79 : not user-modifiable */
      0x0e, /* 0x7a : not user-modifiable */
      0x0e, /* 0x7b : not user-modifiable */
      0x00, /* 0x7c : not user-modifiable */
      0x00, /* 0x7d : not user-modifiable */
      0x02, /* 0x7e : not user-modifiable */
      0xc7, /* 0x7f : ROI center, use SetROI() */
      0xff, /* 0x80 : XY ROI (X=Width, Y=Height), use SetROI() */
      0x9B, /* 0x81 : not user-modifiable */
      0x00, /* 0x82 : not user-modifiable */
      0x00, /* 0x83 : not user-modifiable */
      0x00, /* 0x84 : not user-modifiable */
      0x01, /* 0x85 : not user-modifiable */
      0x00, /* 0x86 : clear interrupt, use ClearInterrupt() */
      0x00  /* 0x87 : start ranging, use StartRanging() or StopRanging(), If you want an automatic start after VL53L1X_init() call, put 0x40 in location 0x87 */
  };

  while (!VL53L1X_isBootReady(self))
  {
  }

  if (VL53L1X_TxSeries(self, 0x002D, defaultTable, 91) != Success)
    return Fail;

  if (VL53L1X_StartRanging(self) != Success)
    return Fail;

  while (!VL53L1X_isDataReady(self))
  {
  }

  if (VL53L1X_ClearInterrupt(self) != Success)
    return Fail;

  if (VL53L1X_StopRanging(self) != Success)
    return Fail;

  // ? VL53L1_VHV_CONFIG__TIMEOUT_MACROP_LOOP_BOUND: 0x0008
  const flag8_t bound = 0x09;
  VL53L1X_TxSeries(self, 0x0008, &bound, 1); /* two bounds VHV */

  const flag8_t start = 0x00;
  VL53L1X_TxSeries(self, 0x000B, &start, 1); /* start VHV from the previous temperature */

  return VL53L1X_StartRanging(self);
}

task_t VL53L1X_TxSeries(VL53L1X_DS *const self, uint16_t index, const uint8_t array[], size_t len)
{
  // I2C wire start: SDA low -> SCL low
  while (_InterfaceI2C_Start(self->I2Cx) != Success)
  {
  }

  // device address: direction write
  while (_InterfaceI2C_Device(self->I2Cx, self->address, False) != Success)
  {
  }

  // clear address tx done flag & preset ack or nack if it will be rx byte
  while (_InterfaceI2C_PreloadStatus(self->I2Cx, False) != Success)
  {
  }

  // index register, which length is 16 bits
  while (_InterfaceI2C_TxByte(self->I2Cx, (_MASK(index, 0xFF00) >> 8)) != Success)
  {
  }
  while (_InterfaceI2C_TxByte(self->I2Cx, (_MASK(index, 0x00FF) >> 0)) != Success)
  {
  }

  // tranfer datas
  for (uint32_t i = 0; i < len; ++i)
    while (_InterfaceI2C_TxByte(self->I2Cx, array[i]) != Success)
    {
    }

  // I2C wire stop: SCL high -> SDA high
  while (_InterfaceI2C_Stop(self->I2Cx, True) != Success)
  {
  }

  return Success;
}

task_t VL53L1X_RxSeries(VL53L1X_DS *const self, uint16_t index, volatile uint8_t array[], size_t len)
{
  if (len == 0)
    return Success;

  VL53L1X_TxSeries(self, index, 0, 0);

  bool_t needAck = (len > 1) ? (True) : (False);

  // I2C wire restart
  while (_InterfaceI2C_Start(self->I2Cx) != Success)
  {
  }

  // device address: direction read
  while (_InterfaceI2C_Device(self->I2Cx, self->address, True) != Success)
  {
  }

  // clear address tx done flag & preset ack or nack if it will be rx byte
  while (_InterfaceI2C_PreloadStatus(self->I2Cx, needAck) != Success)
  {
  }

  // receive datas: 0 to ( last - 1 )
  for (uint32_t i = 0; needAck; ++i)
  {
    if (i + 2 >= len)
      needAck = False;
    while (_InterfaceI2C_RxByte(self->I2Cx, &array[i], needAck) != Success)
    {
    }
  }

  // I2C wire stop: SCL high -> SDA high
  while (_InterfaceI2C_Stop(self->I2Cx, False) != Success)
  {
  }

  // receive last data
  while (_InterfaceI2C_RxByte(self->I2Cx, &array[len - 1], False) != Success)
  {
  }

  return Success;
}

bool_t VL53L1X_isBootReady(VL53L1X_DS *const self)
{
  volatile flag8_t status = 0x00;

  // ? VL53L1_FIRMWARE__SYSTEM_STATUS: 0x00E5
  if (VL53L1X_RxSeries(self, 0x00E5, &status, 1) != Success)
    return False;

  return status ? True : False;
}

task_t VL53L1X_StartRanging(VL53L1X_DS *const self)
{
  const uint8_t command = 0x40;

  // ? SYSTEM__MODE_START: 0x0087
  return VL53L1X_TxSeries(self, 0x0087, &command, 1);
}

bool_t VL53L1X_isDataReady(VL53L1X_DS *const self)
{
  volatile flag8_t polarity = 0x00, status = 0x00;

  if (VL53L1X_GetInterruptPolarity(self, &polarity) != Success)
    return False;

  // ? GPIO__TIO_HV_STATUS: 0x0031
  if (VL53L1X_RxSeries(self, 0x0031, &status, 1) != Success)
    return False;

  return _MASK(status, _BIT(0)) == polarity ? True : False;
}

task_t VL53L1X_GetInterruptPolarity(VL53L1X_DS *const self, volatile uint8_t *const polarity)
{
  volatile flag8_t temp = 0x00;

  // ? GPIO_HV_MUX__CTRL: 0x0030
  if (VL53L1X_RxSeries(self, 0x0030, &temp, 1) != Success)
    return Fail;

  *polarity = !(_MASK(temp, _BIT(4)) >> 4);

  return Success;
}

task_t VL53L1X_GetDistance(VL53L1X_DS *const self, volatile uint16_t *const distance)
{
  volatile uint8_t array[2] = {0};

  // ? PHASECAL_CONFIG__TIMEOUT_MACROP: 0x0096
  if (VL53L1X_RxSeries(self, 0x0096, array, 2) != Success)
    return Fail;

  *distance = (uint16_t)((uint16_t)(array[0] << 8) | array[1]);

  return Success;
}

task_t VL53L1X_ClearInterrupt(VL53L1X_DS *const self)
{
  const uint8_t command = 0x01;

  // ? SYSTEM__INTERRUPT_CLEAR: 0x0086
  return VL53L1X_TxSeries(self, 0x0086, &command, 1);
}

task_t VL53L1X_StopRanging(VL53L1X_DS *const self)
{
  const uint8_t command = 0x00;

  // ? SYSTEM__MODE_START: 0x0087
  return VL53L1X_TxSeries(self, 0x0087, &command, 1);
}

/* ---------------------------------------------------------------- Class Public Functions End */

#endif // STM32F103xx_UNREADY
