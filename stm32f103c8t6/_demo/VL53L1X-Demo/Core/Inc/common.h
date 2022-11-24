#ifndef _COMMON_H_
#define _COMMON_H_

#include "stm32f103xb.h"

/* **********
	Useful Tools Def.
*/
#define _BIT(n) ( 1 << n )
#define _MASK(reg, bits) ( ( reg ) & ( bits ) )

/* **********
  Types Def.
*/
typedef unsigned char  uint8_t ;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

typedef signed char    sint8_t ;
typedef signed short   sint16_t;
typedef signed int     sint32_t;

typedef uint8_t  flag8_t;
typedef uint16_t flag16_t;
typedef uint32_t flag32_t;

typedef uint32_t size_t;
typedef uint32_t pin_t;

typedef enum { False = 0, True = !False } bool_t;
typedef enum { Success, Fail = !Success } task_t;

typedef struct {
  GPIO_TypeDef * GPIOx;
  pin_t          pin;
} Port_t;

/* **********
  APIs
*/
static inline bool_t CopyPort(Port_t * const this, const Port_t * const refs)
{
  this->GPIOx = refs->GPIOx;
  this->pin   = refs->pin;
  return True;
}

#endif // _COMMON_H_
