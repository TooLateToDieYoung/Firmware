# Description
> ## - Comply with the SPI 4-wire full-duplex protocol.
> ## - Involves the use of dynamic memory.
> ## - LSM6DS3_setRegister() and LSM6DS3_getRegister() are Only can be used from the main thread.

---

# Suggest
> ## - Can use Buffer_DS & LSM6DS3_DS to form a flexible module. (refer demo code)

---

# Dependent Header Files
> ## - Provides the base type and namespace of the device
```C
#include "Common.h" // ? check for namespace: STM32F103xx_UNREADY
```

---

# Tools
> ## - Several commonly used basic register addresses
```C
  typedef enum
  {
    WHO_AM_I = 0x0F,
    CTRL_ACCE,
    CTRL_GYRO,
    STATUS_R = 0x1E,
    GYRO_X_L = 0x22,
    GYRO_X_H,
    GYRO_Y_L,
    GYRO_Y_H,
    GYRO_Z_L,
    GYRO_Z_H,
    ACCE_X_L = 0x28,
    ACCE_X_H,
    ACCE_Y_L,
    ACCE_Y_H,
    ACCE_Z_L,
    ACCE_Z_H
  } LSM6DS3_Register_Enum;
```

---

# Data Structure
```C
typedef struct
{
  // SPIx peripheral
  SPI_TypeDef *SPIx; 

  // SPI CS wire for selecting the slave device
  port_t CS;

} LSM6DS3_DS;
```

---

# API
> ## - Constructor
```C
const port_t CS = { .GPIOx = GPIOB, .order = 15 };

LSM6DS3_DS * restrict lsm6ds3 = LSM6DS3_Constructor(SPI1, &CS);

if( !lsm6ds3 ) // dynamic memory fail
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

LSM6DS3_Destructor(lsm6ds3); 
```
>---

> ## - Configure module with default values
```C
if( LSM6DS3_DefaultInit(lsm6ds3) != Success )
{
  // ? Catch fail case
}
```
>---

> ## - Select device
```C
// ? Call it before any communication starts
LSM6DS3_HoldDevice(lsm6ds3);
```
>---

> ## - Release device
```C
// ? Call it after any communication is complete
LSM6DS3_FreeDevice(lsm6ds3);
```
>---

> ## - Write one byte into the register
```C
if( LSM6DS3_setRegister(lsm6ds3, CTRL_ACCE, 0x60, 1000) != Success )
{
  // ? Catch fail case
}
```
>---

> ## - Read one byte from the register
```C
volatile uint8_t result = 0;

if( LSM6DS3_setRegister(lsm6ds3, WHO_AM_I, &result, 1000) != Success )
{
  // ? Catch fail case
}
```
---

# Demo Code
```C
#include "Buffer.h" // Please refer to Buffer.md for more informations.
#include "LSM6DS3.h"

static struct {
  LSM6DS3_DS * restrict device;
  Buffer_DS  * restrict raw;
  volatile sint16_t acce[3], gyro[3];
} lsm6ds3;

task_t LSM6DS3_Init(void)
{
  const port_t CS = { .GPIOx = GPIOA, .order = 10 };

  lsm6ds3.device = LSM6DS3_Constructor(SPI1, &CS);
  lsm6ds3.raw    = Buffer_Constructor(12); // ACCE X, Y, Z & GYRO X, Y, Z

  if( !lsm6ds3.device || !lsm6ds3.raw )
  {
    LSM6DS3_Destructor(lsm6ds3.device);
    Buffer_Destructor(lsm6ds3.raw);
    return Fail;
  }

  // ! Turn on SPIx here.
  
  // ! Must delay until SPIx is ready.

  return LSM6DS3_DefaultInit(lsm6ds3.device);
}
```
---