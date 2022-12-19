# Description
> ## - Define common types and useful tools

---

# Tools
> ## - C-like namespace
```C
#define STM32F103xx_UNREADY // ! all files have to check this forward definition.
#warning "This library must be working under stm32f103xx series"
```
>---

> ## - Get offset bit
```C
#define _BIT(offset) (1U << (offset))
```
>---

> ## - Get bitmask of a flag
```C
#define _MASK(flag, bits) ((flag) & (bits))
``` 
>---

> ## - Boolean typedef
```C
// ? Defined with an enum to avoid conflicts.
typedef enum
{
  False = 0,
  True = !False
} bool_t;
```
>---

> ## - Task result typedef
```C
// ? For communication between modules or interrupt missions.
typedef enum
{
  Success = 0,
  Fail = !Success
} task_t;
```
>---

> ## - GPIO port & pin typedef
```C
// ? An easier way to bind ports and pins to pass arguments as functions
typedef struct
{
  GPIO_TypeDef *GPIOx;
  uint8_t order; // 0 ~ 15
} port_t;
```