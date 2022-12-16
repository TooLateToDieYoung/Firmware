#ifndef _COMMON_H_
#define _COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#if defined(STM32F103x4) || defined(STM32F103x6)
#include "stm32f103x8.h"
#elif defined(STM32F103x8) || defined(STM32F103xB)
#include "stm32f103xb.h"
#elif defined(STM32F103xC) || defined(STM32F103xD) || defined(STM32F103xE)
#include "stm32f103xe.h"
#elif defined(STM32F103xF) || defined(STM32F103xG)
#include "stm32f103xg.h"
#else
#define STM32F103xx_UNREADY
#warning "This library must be working under stm32f103xx series"
#endif

#ifndef STM32F103xx_UNREADY

/**
 * @brief
 *
 */
#define _BIT(offset) (1 << offset)
#define _MASK(flag, bits) ((flag) & (bits))

  /**
   * @brief
   *
   */
  typedef unsigned char uint8_t;
  typedef unsigned short uint16_t;
  typedef unsigned int uint32_t;

  typedef signed char sint8_t;
  typedef signed short sint16_t;
  typedef signed int sint32_t;

  typedef unsigned char flag8_t;
  typedef unsigned short flag16_t;
  typedef unsigned int flag32_t;

  typedef unsigned int size_t;

  typedef enum
  {
    False = 0,
    True = !False
  } bool_t;
  typedef enum
  {
    Success = 0,
    Fail = !Success
  } task_t;

  typedef struct
  {
    GPIO_TypeDef *GPIOx;
    uint8_t order; // 0 ~ 15
  } port_t;

#endif // STM32F103xx_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _COMMON_H_
