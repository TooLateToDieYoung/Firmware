# Description
> ## - People who use this library can easily realize their own UART read / write functions via these interfaces.
> ## - All the functions wrapped in the "interface scope" can be called from the main thread or interrupts.

---

# Suggest
> ## - Only meet the basic UART protocol. (Extensible to RS232 or DMX512, etc.)
> ## - Please refer to the demo code. (in InterfaceUSART.h file)

---

# Dependent Header Files
> ## - Provides the base type and namespace of the device
```C
#include "Common.h" // ? check for namespace: STM32F103xx_UNREADY
```

---

# API
> ## - Transfer a byte
```C
// * In the main thread
while( _InterfaceUSART_TxByte(USART1, (uint8_t)txByte) != Success ) 
{
  // ? Waiting
}

// * In the interrupts
if( _InterfaceUSART_TxByte(USART1, (uint8_t)txByte) != Success )
{
  // ! Error Handling
}
```
> ---

> ## - Receive a byte
```C
volatile uint8_t rxByte = 0;

// * In the main thread
while( _InterfaceUSART_RxByte(USART1, &rxByte) != Success ) 
{
  // ? Waiting
}

// * In the interrupts
if( _InterfaceUSART_RxByte(USART1, &rxByte) != Success )
{
  // ! Error Handling
}
```

---