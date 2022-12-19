# Description
> ## - People who use this library can easily realize their own I2C read / write functions via these interfaces.
> ## - All the functions wrapped in the "interface scope" can be called from the main thread or interrupts.

---

# Suggest
> ## - It works only with I2C master device & 7bit device address.
> ## - Only meet the basic I2C protocol.
> ## - Please refer to the demo code. (in InterfaceI2C.h file)

---

# Dependent Header Files
> ## - Provides the base type and namespace of the device
```C
#include "Common.h" // ? check for namespace: STM32F103xx_UNREADY
```

---

# API
> ## - Generate I2C start condition
```C
/*
  ? Should start I2C at main thread.
*/

while( _InterfaceI2C_Start(I2C1) != Success ) 
{
  // ? Waiting
}
```
> ---

> ## - Send device read / write address
```C
const uint8_t device = 0x52;
const bool_t  isRead = True;

// * In the main thread
while( _InterfaceI2C_Device(I2C1, device, isRead) != Success ) 
{
  // ? Waiting
}

// * In the interrupts
if( _InterfaceI2C_Device(I2C1, device, isRead) != Success )
{
  // ! Error Handling
}
```
> ---

> ## - Preload status
```C
const bool_t needAck = True;

// * In the main thread
while( _InterfaceI2C_PreloadStatus(I2C1, needAck) != Success ) 
{
  // ? Waiting
}

// * In the interrupts
if( _InterfaceI2C_PreloadStatus(I2C1, needAck) != Success )
{
  // ! Error Handling
}
```
> ---

> ## - Transfer a byte
```C
// * In the main thread
while( _InterfaceI2C_TxByte(I2C1, (uint8_t)txByte) != Success ) 
{
  // ? Waiting
}

// * In the interrupts
if( _InterfaceI2C_TxByte(I2C1, (uint8_t)txByte) != Success )
{
  // ! Error Handling
}
```
> ---

> ## - Receive a byte
```C
volatile uint8_t rxByte = 0;
volatile bool_t  needAck = True;

// * In the main thread
while( _InterfaceI2C_RxByte(I2C1, &rxByte, needAck) != Success ) 
{
  // ? Waiting
}

// * In the interrupts
if( _InterfaceI2C_RxByte(I2C1, &rxByte, needAck) != Success )
{
  // ! Error Handling
}
```
>---

> ## - Generate stop condition
```C
volatile bool_t isAfterTx = False;

// * In the main thread
while( _InterfaceI2C_Stop(I2C1, isAfterTx) != Success ) 
{
  // ? Waiting
}

// * In the interrupts
if( _InterfaceI2C_Stop(I2C1, isAfterTx) != Success )
{
  // ! Error Handling
}
```

---

# Demo Code
> ## - Main Thread: Refer to "InterfaceI2C.h" file.
>---

> ## - Interrupts
```C
#define DEVICE 0x52

extern const size_t     txLength;
extern volatile uint8_t txBytes[];

task_t TxOnly_IRQHandler(I2C_TypeDef *I2Cx)
{
  static volatile flag8_t step = 0;
  static volatile size_t  curr = 0;

  // ! Already started & enable I2Cx interrupt at main thread 
  switch( step )
  {
    case 0: { // * Call device with write address
      if( _InterfaceI2C_Device(I2Cx, DEVICE, False) != Success ) goto __Error;
      step = 1;
      return Success; 
    }

    case 1: { // * Clear address tx done flag
      if( _InterfaceI2C_PreloadStatus(I2Cx, False) != Success ) goto __Error;
      step = 2;
      return Success; 
    }

    case 2: { // * Continuous transfer bytes
      if( 
        _InterfaceI2C_TxByte(I2Cx, txBytes[curr]) != Success 
      ) goto __Error;
      curr = ( curr + 1 ) % txLength; // ? shift tx buffer
      if( curr == 0 ) step = 3; // ? check end
      return Success;
    }

    case 3: { // * Generate stop signal after tx done
      if( _InterfaceI2C_Stop(I2Cx, True) != Success ) goto __Error;
      step = 0;
      // ? please disable I2Cx interrupt here
      return Success;
    }

    default: goto __Error;
  }

__Error:
  // ! Error Handling
}

extern const size_t     rxLength;
extern volatile uint8_t rxBytes[];

task_t RxOnly_IRQHandler(I2C_TypeDef *I2Cx)
{
  static volatile flag8_t step = 0;
  static volatile size_t  curr = 0;

  const bool_t needAck = ( curr + 2 >= rxLength ) ? ( False ) : ( True );

  // ! Already started & enable I2Cx interrupt at main thread 
  switch( step )
  {
    case 0: { // * Call device with write address
      if( _InterfaceI2C_Device(I2Cx, DEVICE, True) != Success ) goto __Error;
      step = 1;
      return Success; 
    }

    case 1: { // * Preset ack or nack if it will be rx byte
      if( _InterfaceI2C_PreloadStatus(I2Cx, needAck) != Success ) goto __Error;
      step = 2;
      return Success; 
    }

    case 2: { // * Continuous recieve bytes (0 ~ penultimate)
      if( 
        _InterfaceI2C_RxByte(I2Cx, &rxBytes[curr], needAck) != Success 
      ) goto __Error;
      curr++; // ? shift rx buffer
      if( curr + 1 == rxLength ) step = 3; // ? check is penultimate
      return Success;
    }

    case 3: { // * Generate stop signal after rx done
      if( _InterfaceI2C_Stop(I2Cx, False) != Success ) goto __Error;
      step = 4;
      return Success;
    }

    case 4: { // * Receive last byte
      if( 
        _InterfaceI2C_RxByte(I2Cx, &rxBytes[curr], needAck) != Success 
      ) goto __Error;
      curr = step = 0;
      // ? please disable I2Cx interrupt here
      return Success;
    }

    default: goto __Error;
  }

__Error:
  // ! Error Handling
}
```

---