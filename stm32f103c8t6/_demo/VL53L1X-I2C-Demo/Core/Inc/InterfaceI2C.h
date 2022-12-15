/**
 * @file InterfaceI2C.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief People who use this library can easily realize their own I2C read / write functions via these interfaces
 * @version 0.1
 * @date 2022-11-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _INTERFACE_I2C_H_
#define _INTERFACE_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if   defined( STM32F103x4 ) || defined( STM32F103x6 )
  #include "stm32f103x8.h"
#elif defined( STM32F103x8 ) || defined( STM32F103xB )
  #include "stm32f103xb.h"
#elif defined( STM32F103xC ) || defined( STM32F103xD ) || defined( STM32F103xE )
  #include "stm32f103xe.h"
#elif defined( STM32F103xF ) || defined( STM32F103xG )
  #include "stm32f103xg.h"
#else
  #define I2C_UNREADY
  #warning "This library must be working under stm32f103xx series"
#endif

#ifndef I2C_UNREADY

#include "Common.h" // ask for some useful tools & typedef

// Definitions
typedef enum { DirW = 0, DirR = 1 } I2C_DirectionRW_Enum;
typedef enum { Nack, Ack, Invalid } I2C_AckType_Enum;

/** Interface Begin --------------------------------------------------------------------
 * @brief 
 * | Almost directly through the register operation. \n
 * | For each function, they provide an alternative, \n 
 * | with the same functionality, implemented with the LL library.
 * 
 * @warning Do not change these codes, it may cause errors
 */

/**
 * @brief generate I2C start condition
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @return task_t: Success / Fail
 */
static inline task_t _InterfaceI2C_Start(I2C_TypeDef * I2Cx)
{
	I2Cx->CR1 |= _BIT(8); 

  while( !_MASK(I2Cx->SR1, _BIT(0)) ) { } 

	return Success;

/* 
- The same function, using the LL library would look like:

  LL_I2C_GenerateStartCondition(I2Cx);

  while(!LL_I2C_IsActiveFlag_SB(I2Cx)) { }

  return Success;
*/
}

/**
 * @brief Send device read / write address
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @param device: I2C device address (7bit)
 * @param rw: DirR / DirR, direction read or write 
 * @param type: Nack, Ack, Invalid, transmission is followed by ack or nack, only valid when rw == DirR.
 * @return task_t: Success / Fail
 */
static inline task_t _InterfaceI2C_Device(I2C_TypeDef * I2Cx, uint8_t device, I2C_DirectionRW_Enum rw, I2C_AckType_Enum type)
{
	I2Cx->DR = device + (uint8_t)rw;
	
  while( !_MASK(I2Cx->SR1, _BIT(1)) ) { }
	
  if( rw == DirR ) {
    if( type == Nack ) I2Cx->CR1 &= ~_BIT(10);
    else I2Cx->CR1 |= _BIT(10);
  }

  uint8_t temp = ( I2Cx->SR1 | I2Cx->SR2 ); (void)temp;

	return Success;

/* 
- The same function, using the LL library would look like:

  LL_I2C_TransmitData8(I2Cx, device + (uint8_t)rw;);

  while(!LL_I2C_IsActiveFlag_ADDR(I2Cx)) { }

  if( rw == DirR ) {
    if( type == Nack ) LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);
    else LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
  }

  LL_I2C_ClearFlag_ADDR(I2Cx);

  return Success;
*/
}

/**
 * @brief transfer a byte
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @param byte: data to send
 * @return task_t: Success / Fail
 */
static inline task_t _InterfaceI2C_TxByte(I2C_TypeDef * I2Cx, uint8_t byte)
{
	while( !_MASK(I2Cx->SR1, _BIT(7)) ) { }
	
  I2Cx->DR = byte;

	while( !_MASK(I2Cx->SR1, _BIT(2)) ) { }
	
  return Success;

/* 
- The same function, using the LL library would look like:

  while(!LL_I2C_IsActiveFlag_TXE(I2Cx)) { }

  LL_I2C_TransmitData8(I2Cx, byte);

  while(!LL_I2C_IsActiveFlag_BTF(I2Cx)) { }

  return Success;
*/
}

/**
 * @brief receive a byte
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @param byte: buffer for recording data
 * @return task_t: Success / Fail
 */
static inline task_t _InterfaceI2C_RxByte(I2C_TypeDef * I2Cx, uint8_t * byte, I2C_AckType_Enum type)
{
	while( !_MASK(I2Cx->SR1, _BIT(6)) ) { }
	
  *byte = (uint8_t)I2Cx->DR;
	
  if( type == Ack ) I2Cx->CR1 |= _BIT(10);
  else I2Cx->CR1 &= ~_BIT(10);

  return Success;

/* 
- The same function, using the LL library would look like:

  while(!LL_I2C_IsActiveFlag_RXNE(I2Cx)) { }

  *byte = (uint8_t)LL_I2C_ReceiveData8(I2Cx);

  if( type == Ack ) LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
  else LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);

  return Success;
*/
}

/**
 * @brief generate stop condition
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @return task_t: Success / Fail
 */
static inline task_t _InterfaceI2C_Stop(I2C_TypeDef * I2Cx)
{
  I2Cx->CR1 |= _BIT(9);
	
  return Success;

/* 
- The same function, using the LL library would look like:

  LL_I2C_GenerateStopCondition(I2Cx);
	
  return Success;
*/
}
/* ---------------------------------------------------------------- Interface Code End */

/** Demo Code Begin --------------------------------------------------------------------
 * @brief 
 * | The following is the demo code for standard I2C communication protocol \n
 * | Users can rewrite the content of the function according to \n
 * | their own needs, or customize new functions
 */

/**
 * @brief Continuously write multiple data to the specified device
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @param device: I2C device address (7bit)
 * @param array: datas to send
 * @param len: array length
 * @return task_t: Success / Fail 
 *//*
static inline task_t __W_Series(I2C_TypeDef * I2Cx, uint8_t device, uint8_t array[], uint32_t len)
{
  if( len == 0 ) return Success;

  __Start(I2Cx);

  __Device(I2Cx, device, DirW, Invalid);

  for(uint32_t i = 0; i < len; ++i) __TxByte(I2Cx, array[i]);

  __Stop(I2Cx);

	return Success;
}*/

/**
 * @brief Continuously read multiple data from the specified device
 * @warning user must ensure len > data lengh
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @param device: I2C device address (7bit)
 * @param array: buffer to receive datas
 * @param len: array length
 * @return task_t: Success / Fail 
 *//*
static inline task_t __R_Series(I2C_TypeDef * I2Cx, uint8_t device, uint8_t array[], uint32_t len)
{
  if( len == 0 ) return Success;

  I2C_AckType_Enum type = ( len > 1 ) ? ( Ack ) : ( Nack ) ;

  __Start(I2Cx); // Restart

  __Device(I2Cx, device, DirR, type);

  for(uint32_t i = 0; type == Ack; ++i) {
    if( i + 2 >= len ) type = Nack;
    __RxByte(I2Cx, &array[i], type);
  }

  __Stop(I2Cx);

  __RxByte(I2Cx, &array[len-1], Nack);

	return Success;
}
*/
/* --------------------------------------------------------------------- Demo Code End */


#endif // UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _INTERFACE_I2C_H_
