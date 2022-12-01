/**
 * @file InterfaceI2C.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief 
 * | People who use this library can easily realize \n 
 * | their own I2C read / write functions via these interfaces.
 * | All the functions wrapped in the "interface" \n 
 * | can be called from the main thread or interrupts.
 * 
 * @warning It works only with I2C master device & 7bit device address
 * @version 1.1
 * @date 2022-12-01
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

#include "common.h" // ask for some useful tools & typedef

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
  // check if I2Cx is busy
  if( _MASK(I2Cx->SR2, _BIT(1)) ) return Fail;

  // generate start signal
	I2Cx->CR1 |= _BIT(8);

	return Success;
}

/**
 * @brief send device read / write address
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @param device: I2C device address (7bit)
 * @param isRead: is direction read or not ( !read == write ) 
 * @return task_t: Success / Fail
 */
static inline task_t _InterfaceI2C_Device(I2C_TypeDef * I2Cx, uint8_t device, bool_t isRead)
{
  // check if start condition is generated
  if( !_MASK(I2Cx->SR1, _BIT(0)) ) return Fail;

  // send device read / write address
	I2Cx->DR = ( isRead ) ? ( device | 0x01 ) : ( device ) ;

	return Success;
}

/**
 * @brief clear address tx done flag & preset ack or nack if it will be rx byte
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @param isAckNext: decide whether to generate ack for next rx bytes
 * @return task_t: Success / Fail
 */
static inline task_t _InterfaceI2C_PreloadStatus(I2C_TypeDef * I2Cx, bool_t isAckNext)
{
  // check if it is the end of address transmission
  if( !_MASK(I2Cx->SR1, _BIT(1)) ) return Fail;

  // decide whether to generate ack for next rx bytes
  if( isAckNext ) I2Cx->CR1 |= _BIT(10);
  else I2Cx->CR1 &= ~_BIT(10);

  // read SR1 & SR2 to clear I2Cx->SR1 _BIT(1), refer to datasheet
  uint8_t temp = ( I2Cx->SR1 | I2Cx->SR2 ); (void)temp; // ? ignore temp

  return Success;
}

/**
 * @brief transfer a byte
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @param byte: data to send
 * @return task_t: Success / Fail
 */
static inline task_t _InterfaceI2C_TxByte(I2C_TypeDef * I2Cx, const uint8_t byte)
{
  // check if TXE
  if( !_MASK(I2Cx->SR1, _BIT(7)) ) return Fail;
	
  // send byte
  I2Cx->DR = byte;

  return Success;
}

/**
 * @brief receive a byte
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @param byte: buffer for recording data
 * @param isAckNext: decide whether to generate ack for next rx bytes
 * @return task_t: Success / Fail
 */
static inline task_t _InterfaceI2C_RxByte(I2C_TypeDef * I2Cx, volatile uint8_t * const byte, bool_t isAckNext)
{
  // check if RXNE
  if( !_MASK(I2Cx->SR1, _BIT(6)) ) return Fail;
	
  // recive byte
  *byte = (uint8_t)I2Cx->DR;
	
  // decide whether to generate ack for next rx bytes
  if( isAckNext ) I2Cx->CR1 |= _BIT(10);
  else I2Cx->CR1 &= ~_BIT(10);

  return Success;
}

/**
 * @brief generate stop condition
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @param needCheckTxDone: check if this function is called after TxByte()
 * @return task_t: Success / Fai
 */
static inline task_t _InterfaceI2C_Stop(I2C_TypeDef * I2Cx, bool_t isAfterTx)
{ 
  // check if BTF( Byte transfer finished ) follows TxByte()
  if( isAfterTx && !_MASK(I2C1->SR1, _BIT(2)) ) return Fail; 

  // generate stop signal
  I2Cx->CR1 |= _BIT(9);
	
  return Success;
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
 */
static inline task_t _InterfaceI2C_TxSeries(I2C_TypeDef * I2Cx, uint8_t device, const uint8_t array[], size_t len)
{
	if( len == 0 ) return Success;

	// I2C wire start: SDA low -> SCL low
  while( _InterfaceI2C_Start(I2Cx) != Success ) { }

	// device address: direction write
  while( _InterfaceI2C_Device(I2Cx, device, False) != Success ) { }

  // clear address tx done flag & preset ack or nack if it will be rx byte
  while( _InterfaceI2C_PreloadStatus(I2Cx, False) != Success ) { }
	
	// tranfer datas
  for(uint32_t i = 0; i < len; ++i) 
    while( _InterfaceI2C_TxByte(I2Cx, array[i]) != Success ) { }

	// I2C wire stop: SCL high -> SDA high
  while( _InterfaceI2C_Stop(I2Cx, True) != Success ) { }

	return Success;
}

/**
 * @brief Continuously read multiple data from the specified device
 * @warning user must ensure len > data lengh
 * 
 * @param I2Cx: defined in the stm32f103xx series header
 * @param device: I2C device address (7bit)
 * @param array: buffer to receive datas
 * @param len: array length
 * @return task_t: Success / Fail 
 */
static inline task_t _InterfaceI2C_RxSeries(I2C_TypeDef * I2Cx, uint8_t device, volatile uint8_t array[], size_t len)
{
	if( len == 0 ) return Success;

  bool_t needAck = ( len > 1 ) ? ( True ) : ( False ) ;
	
	// I2C wire restart
  while( _InterfaceI2C_Start(I2Cx) != Success ) { }
	
	// device address: direction read
  while( _InterfaceI2C_Device(I2Cx, device, True) != Success ) { }

  // clear address tx done flag & preset ack or nack if it will be rx byte
  while( _InterfaceI2C_PreloadStatus(I2Cx, needAck) != Success ) { }

	// receive datas: 0 to ( last - 1 )
  for(uint32_t i = 0; needAck; ++i) {
    if( i + 2 >= len ) needAck = False;
    while( _InterfaceI2C_RxByte(I2Cx, &array[i], needAck) != Success ) { }
  }
	
	// I2C wire stop: SCL high -> SDA high
  while( _InterfaceI2C_Stop(I2Cx, False) != Success ) { }

	// receive last data
  while( _InterfaceI2C_RxByte(I2Cx, &array[len-1], False) != Success ) { }

	return Success;
}

/* --------------------------------------------------------------------- Demo Code End */


#endif // UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _INTERFACE_I2C_H_
