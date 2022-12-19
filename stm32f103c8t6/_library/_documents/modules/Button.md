# Description
> ## - Involves the use of dynamic memory.
> ## - Only implement the most basic button interface.

---

# Dependent Header Files
> ## - Provides the base type and namespace of the device
```C
#include "Common.h" // ? check for namespace: STM32F103xx_UNREADY
```

---

# Data Structure
```C
typedef struct
{

  port_t pin;
  
} Button_DS;
```

---

# API
> ## - Constructor
```C
const port_t port = { .GPIOx = GPIOA, .order = 13 };

Button_DS * restrict button = Button_Constructor(&port);

if( !button ) // dynamic memory fail
{
  // ! Error Handling
}
```
>---

> ## - Destructor
```C
/*
  ? It's just a reserve function, \n
  | because heap pointer should be auto reset after restart power.
*/

Button_Destructor(button); 
```
>---

> ## - Check if button is free
```C
while( !Button_isFree(button) )
{
  // ? Waiting for the button to be released
}
```
---