/**
 * @file InterfaceSPI.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief
 * @warning
 * @version 0.1
 * @date 2022-11-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _INTERFACE_SPI_H_
#define _INTERFACE_SPI_H_

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
  #define SPI_UNREADY
  #warning "This library must be working under stm32f1xx series"
#endif

#ifndef SPI_UNREADY

#include "common.h"

/** Interface Begin --------------------------------------------------------------------
 * @brief 
 * | Almost directly through the register operation. \n
 * | For each function, they provide an alternative, \n 
 * | with the same functionality, implemented with the LL library.
 * 
 * @warning Do not change these codes, it may cause errors
 */

static inline task_t _InterfaceSPI_TxByte(SPI_TypeDef * SPIx, uint8_t byte)
{
  if( !_MASK(SPIx->SR, _BIT(1)) ) return Fail; // not TXE
	
	SPIx->DR = byte;

  return Success;
}

static inline task_t _InterfaceSPI_RxByte(SPI_TypeDef * SPIx, volatile uint8_t * const byte)
{
	if( !_MASK(SPIx->SR, _BIT(0)) ) return Fail; // not RXNE
	
	if( !byte ) {
		uint8_t temp = (uint8_t)SPIx->DR;
		(void)temp;
	} 
	else {
		*byte = (uint8_t)SPIx->DR;
	}
  
  return Success;
}

/* ---------------------------------------------------------------- Interface Code End */

/** Demo Code Begin --------------------------------------------------------------------
 * @brief 
 * | The following is the demo code for standard I2C communication protocol \n
 * | Users can rewrite the content of the function according to \n
 * | their own needs, or customize new functions
 */

static inline task_t _InterfaceSPI_ShiftByte(SPI_TypeDef * SPIx, uint8_t TxByte, volatile uint8_t * RxByte)
{
	while( _InterfaceSPI_TxByte(SPIx, TxByte) != Success ) { }
	
	while( _InterfaceSPI_RxByte(SPIx, RxByte) != Success ) { }
	
	return Success;
}

/* --------------------------------------------------------------------- Demo Code End */

#endif // UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _INTERFACE_SPI_H_
