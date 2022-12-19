# Description
> ## - Only common anode type devices are allowed.
> ## - Involves the use of dynamic memory.
> ## - Can be used from the main thread or from an interrupt.

---

# Suggest
> ## - If there are more than 1 seven segment display device, you can switch the common anode pin through Timer, so that you can share the same IO pins. (refer demo code)

---

# Dependent Header Files
> ## - Provides the base type and namespace of the device
```C
#include "Common.h" // ? check for namespace: STM32F103xx_UNREADY
```

---

# Tools
> ## - Segment mapping
```C
typedef enum
{
  SegA = 0x01,
  SegB = 0x02,
  SegC = 0x04,
  SegD = 0x08,
  SegE = 0x10,
  SegF = 0x20,
  SegG = 0x40,
  SegP = 0x80
} SevenSegmentIndex_Enum;
```
>---

> ## - General number format
```C
  typedef enum
  {
    Num0 = (SegA | SegB | SegC | SegD | SegE | SegF),
    Num1 = (SegB | SegC),
    Num2 = (SegA | SegB | SegD | SegE | SegG),
    Num3 = (SegA | SegB | SegC | SegD | SegG),
    Num4 = (SegB | SegC | SegF | SegG),
    Num5 = (SegA | SegC | SegD | SegF | SegG),
    Num6 = (SegA | SegC | SegD | SegE | SegF | SegG),
    Num7 = (SegA | SegB | SegC),
    Num8 = (SegA | SegB | SegC | SegD | SegE | SegF | SegG),
    Num9 = (SegA | SegB | SegC | SegD | SegF | SegG),
  } SevenSegmentTable_Enum;
``` 

---

# Data Structure
```C
typedef struct
{
  // ? common anode pin to open / close device (open drain output)
  port_t common;

  // ? io pins to control LEDs (open drain output)
  port_t io[8];  
  
} SevenSegment_DS;
```

---

# API
> ## - Constructor
```C
const port_t common = { .GPIOx = GPIOB, .order = 15 };
const port_t ioTable[8] = {
  [0] = { .GPIOx = GPIOB, .order =  2 }, // SegA
  [1] = { .GPIOx = GPIOB, .order =  0 }, // SegB
  [2] = { .GPIOx = GPIOB, .order =  1 }, // SegC
  [3] = { .GPIOx = GPIOB, .order =  3 }, // SegD
  [4] = { .GPIOx = GPIOA, .order =  9 }, // SegE
  [5] = { .GPIOx = GPIOA, .order = 11 }, // SegF
  [6] = { .GPIOx = GPIOA, .order =  8 }, // SegG
  [7] = { .GPIOx = GPIOA, .order = 12 }  // SegP
};

SevenSegment_DS * restrict display = SevenSegment_Constructor(&common, ioTable);

if( !display ) // dynamic memory fail
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

SevenSegment_Destructor(display); 
```
>---

> ## - Set the output segment type
```C
SevenSegment_Set(display, Num9);
```
>---

> ## - Reset output (close it)
```C
SevenSegment_Reset(display);
```
---

# Demo Code
```C
#include "SevenSegment.h"

#define DEVICE_AMOUNT 4

typedef struct {
  SevenSegment_DS * restrict device;
  volatile flag8_t           segments;
} SevenSegmentModule_t;

SevenSegmentModule_t display[DEVICE_AMOUNT];

task_t SevenSegment_Init(void)
{
  const port_t commonTable[DEVICE_AMOUNT]; // = { ... };
  const port_t ioTable[8]; // = { ... };

  for(size_t i = 0; i < DEVICE_AMOUNT; ++i)
  {
    display[i].device = SevenSegment_Constructor(&commonTable[i], ioTable);

    if( !display[i].device )
    {
      for(size_t j = 0; j < i; ++j) 
        SevenSegment_Destructor(display[j].device);

      return Fail;
    }
  }

  return Success;
}

// In Timer IRQ Handler
void TIMx_IRQHandler(void)
{
  static uint8_t index = 0;
  
  if ( _MASK(TIM4->SR, _BIT(0)) ) // Check ISR flag
  {
    TIM4->SR &= ~_BIT(0); // Clear ISR flag

    SevenSegment_Reset(display[index].device); // Close last one

    index = ( index + 1 ) % DEVICE_AMOUNT; // Shift focus

    SevenSegment_Set(display[index].device, display[index].segments); // Open next one
  }
}
```