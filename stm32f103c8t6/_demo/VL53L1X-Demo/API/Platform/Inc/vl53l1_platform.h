/**
 * @file  vl53l1_platform.h
 * @brief Those platform functions are platform dependent and have to be implemented by the user
 */
 
#ifndef _VL53L1_PLATFORM_H_
#define _VL53L1_PLATFORM_H_

#include "vl53l1_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* My Code Begin ------------------------------------------------- */

// Include necessary header files
#include "common.h"
#include "stm32f1xx_ll_i2c.h"   // for I2C API
#include "stm32f1xx_ll_utils.h" // for LL_mDelay();

// Definitions
typedef enum { DirW = 0, DirR = 1 } Direction_RW_Enum;
typedef enum { Nack, Ack, Invalid  } AckType_Enum;

// static inline func. for I2C interface
static inline int8_t __Start(void)
{
	// while( _MASK(I2C1->SR2, _BIT(1)) ) { } // while(LL_I2C_IsActiveFlag_BUSY(I2C1)) { } // while( I2Cx->SR2 & _BIT(1) ) { }

	I2C1->CR1 |= _BIT(8); // LL_I2C_GenerateStartCondition(I2C1); // I2Cx->CR1 |= _BIT(8);
	
  while( !_MASK(I2C1->SR1, _BIT(0)) ) { } // while(!LL_I2C_IsActiveFlag_SB(I2C1)) { }  // while( !( I2Cx->SR1 & _BIT(0) ) ) { }

	return 0;
}

static inline int8_t __Device(uint8_t device, Direction_RW_Enum rw, AckType_Enum type)
{
	I2C1->DR = (uint8_t)( device + rw ); // LL_I2C_TransmitData8(I2C1, ( device + DirW )); // I2Cx->DR = ( DEVICE_ADDRESS | DIRECTION_WRITE );
	
  while( !_MASK(I2C1->SR1, _BIT(1)) ) { } // while(!LL_I2C_IsActiveFlag_ADDR(I2C1)) { } // while( !( I2Cx->SR1 & _BIT(1) ) ) { }
	
  if( rw == DirR ) {
    if( type == Nack ) I2C1->CR1 &= ~_BIT(10); // LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK); // I2Cx->CR1 &= ~_BIT(10);
    else I2C1->CR1 |= _BIT(10);
  }

  uint8_t temp = ( I2C1->SR1 | I2C1->SR2 ); (void)temp; // LL_I2C_ClearFlag_ADDR(I2C1); // uint8_t temp = ( I2Cx->SR1 | I2Cx->SR2 ); (void)temp;

	return 0;
}

static inline int8_t __TxByte(uint8_t byte)
{
	while( !_MASK(I2C1->SR1, _BIT(7)) ) { } // while(!LL_I2C_IsActiveFlag_TXE(I2C1)) { } // while( !( I2Cx->SR1 & _BIT(7) ) ) { }
	
  I2C1->DR = byte; // LL_I2C_TransmitData8(I2C1, ( ( reg & 0xFF00 ) >> 8 )); // I2Cx->DR = _byteH(INDEX_REGISTER);

	while( !_MASK(I2C1->SR1, _BIT(2)) ) { } // while(!LL_I2C_IsActiveFlag_BTF(I2C1)) { } // while( !( I2Cx->SR1 & _BIT(2) ) ) { }
	
  return 0;
}

static inline int8_t __RxByte(uint8_t * byte, AckType_Enum ack)
{
	while( !_MASK(I2C1->SR1, _BIT(6)) ) { } // while(!LL_I2C_IsActiveFlag_RXNE(I2C1)) { } // while( !( I2Cx->SR1 & _BIT(6) ) ) { }
	
  *byte = (uint8_t)I2C1->DR; // *byte = (uint8_t)LL_I2C_ReceiveData8(I2C1); // uint8_t data = (uint8_t)I2Cx->DR;
	
  if( ack == Ack ) I2C1->CR1 |= _BIT(10);
  else I2C1->CR1 &= ~_BIT(10);

  return 0;
}

static inline int8_t __Stop(void)
{
  I2C1->CR1 |= _BIT(9); // LL_I2C_GenerateStopCondition(I2C1); // I2Cx->CR1 |= _BIT(9);
	
  return 0;
}

static inline int8_t __W_Series(uint8_t device, uint16_t index, uint8_t array[], uint32_t len, uint16_t timeout)
{
  if( len == 0 ) return 0;

  __Start();

  __Device(device, DirW, Invalid);

  __TxByte( ( ( index & 0xFF00 ) >> 8 ) );

  __TxByte( ( index & 0x00FF ) );

  for(uint32_t i = 0; i < len; ++i) __TxByte( array[i] );

  __Stop();

  (void)timeout;

	return 0;
}

static inline int8_t __R_Series(uint8_t device, uint16_t index, uint8_t array[], uint32_t len, uint16_t timeout)
{
  if( len == 0 ) return 0;

  AckType_Enum type = ( len > 1 ) ? ( Ack ) : ( Nack ) ;

  __Start();

  __Device(device, DirW, Invalid);

  __TxByte( ( ( index & 0xFF00 ) >> 8 ) );

  __TxByte( ( index & 0x00FF ) );

  __Start(); // Restart

  __Device(device, DirR, type);

  for(uint32_t i = 0; type == Ack; ++i) {
    if( i + 2 >= len ) type = Nack;
    __RxByte(&array[i], type);
  }

  __Stop();

  __RxByte(&array[len-1], Nack);

	return 0;
}

static inline int8_t __W_Single(uint8_t device, uint16_t index, uint8_t byte)
{
  return __W_Series(device, index, &byte, 1, 0);

  __Start();

  __Device(device, DirW, Invalid);

  __TxByte( ( ( index & 0xFF00 ) >> 8 ) );

  __TxByte( ( index & 0x00FF ) );

  __TxByte( byte );

  __Stop();

	return 0;
}

static inline int8_t __R_Single(uint8_t device, uint16_t index, uint8_t * byte)
{	
  return __R_Series(device, index, byte, 1, 0);

  __Start();

  __Device(device, DirW, Invalid);

  __TxByte( ( ( index & 0xFF00 ) >> 8 ) );

  __TxByte( ( index & 0x00FF ) );

  __Start(); // Restart

  __Device(device, DirR, Nack);

  __Stop();

  __RxByte(byte, Nack);
	
	return 0;
}

/* --------------------------------------------------- My Code End */

typedef struct {
	uint32_t dummy;
} VL53L1_Dev_t;

typedef VL53L1_Dev_t *VL53L1_DEV;

/** @brief VL53L1_WriteMulti() definition.\n
 * To be implemented by the developer
 */
int8_t VL53L1_WriteMulti(
		uint16_t 			dev,
		uint16_t      index,
		uint8_t      *pdata,
		uint32_t      count);
/** @brief VL53L1_ReadMulti() definition.\n
 * To be implemented by the developer
 */
int8_t VL53L1_ReadMulti(
		uint16_t 			dev,
		uint16_t      index,
		uint8_t      *pdata,
		uint32_t      count);
/** @brief VL53L1_WrByte() definition.\n
 * To be implemented by the developer
 */
int8_t VL53L1_WrByte(
		uint16_t dev,
		uint16_t      index,
		uint8_t       data);
/** @brief VL53L1_WrWord() definition.\n
 * To be implemented by the developer
 */
int8_t VL53L1_WrWord(
		uint16_t dev,
		uint16_t      index,
		uint16_t      data);
/** @brief VL53L1_WrDWord() definition.\n
 * To be implemented by the developer
 */
int8_t VL53L1_WrDWord(
		uint16_t dev,
		uint16_t      index,
		uint32_t      data);
/** @brief VL53L1_RdByte() definition.\n
 * To be implemented by the developer
 */
int8_t VL53L1_RdByte(
		uint16_t dev,
		uint16_t      index,
		uint8_t      *pdata);
/** @brief VL53L1_RdWord() definition.\n
 * To be implemented by the developer
 */
int8_t VL53L1_RdWord(
		uint16_t dev,
		uint16_t      index,
		uint16_t     *pdata);
/** @brief VL53L1_RdDWord() definition.\n
 * To be implemented by the developer
 */
int8_t VL53L1_RdDWord(
		uint16_t dev,
		uint16_t      index,
		uint32_t     *pdata);
/** @brief VL53L1_WaitMs() definition.\n
 * To be implemented by the developer
 */
int8_t VL53L1_WaitMs(
		uint16_t dev,
		int32_t       wait_ms);

#ifdef __cplusplus
}
#endif

#endif
