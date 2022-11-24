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
  #define UNREADY
  #warning "This library must be working under stm32f1xx series"
#endif

#ifndef UNREADY

#include "common.h"

#endif // UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _INTERFACE_SPI_H_
