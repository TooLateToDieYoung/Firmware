# Description
> ## - Define common types and useful tools

---

# Tools
> ## - C-like namespace
>> ### all files have to check this forward definition.
```C
#define STM32F103xx_UNREADY
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
>> ### defined with an enum to avoid conflicts
```C
typedef enum
{
  False = 0,
  True = !False
} bool_t;
```
>---

> ## - Task result typedef
>> ## for communication between modules or interrupt missions.
```C
typedef enum
{
  Success = 0,
  Fail = !Success
} task_t;
```
>---

> ## - GPIO port & pin typedef
>> ## an easier way to bind ports and pins to pass arguments as functions
```C
typedef struct
{
  GPIO_TypeDef *GPIOx;
  uint8_t order; // 0 ~ 15
} port_t;
```