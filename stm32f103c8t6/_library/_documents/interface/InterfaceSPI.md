# Description
> ## - People who use this library can easily realize their own SPI read / write functions via these interfaces.
> ## - All the functions wrapped in the "interface scope" can be called from the main thread or interrupts.

---

# Suggest
> ## - If it works under the SPI 4-wire protocol, please refer to the demo code. (in InterfaceSPI.h file)

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
while( _InterfaceSPI_TxByte(SPI1, (uint8_t)txByte) != Success ) 
{
  // ? Waiting
}

// * In the interrupts
if( _InterfaceSPI_TxByte(SPI1, (uint8_t)txByte) != Success )
{
  // ! Error Handling
}
```
> ---

> ## - Receive a byte
```C
volatile uint8_t rxByte = 0;

// * In the main thread
while( _InterfaceSPI_RxByte(SPI1, &rxByte) != Success ) 
{
  // ? Waiting
}

// * In the interrupts
if( _InterfaceSPI_RxByte(SPI1, &rxByte) != Success )
{
  // ! Error Handling
}
```

---