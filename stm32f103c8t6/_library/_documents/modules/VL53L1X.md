# Description
> ## - Involves the use of dynamic memory.
> ## - Only can be used from the main thread.
> ## - Refactored from the official API library. (UM2501)

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
  // I2Cx peripheral
  I2C_TypeDef *I2Cx;

  // device address
  uint8_t address;

} VL53L1X_DS;
```

---

# API
> ## - Constructor
```C
VL53L1X_DS * restrict vl53l1x = VL53L1X_Constructor(I2C1, 0x52);

if( !vl53l1x ) // dynamic memory fail
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

VL53L1X_Destructor(vl53l1x); 
```
>---

> ## - Configure module with default values
```C
if( VL53L1X_DefaultInit(vl53l1x) != Success )
{
  // ? Catch fail case
}
```
>---

> ## - Continuously write multiple data to the specified device
```C
const uint16_t   FirstRegister; // = ...;
const size_t     length; // = ...;
volatile uint8_t array[length]; // = { ... };

if( VL53L1X_TxSeries(vl53l1x, FirstRegister, array, length) != Success )
{
  // ? Catch fail case
}
```
>---

> ## - Continuously read multiple data from the specified device
```C
const uint16_t   FirstRegister; // = ...;
const size_t     length; // = ...;
volatile uint8_t array[length]; // = { ... };

if( VL53L1X_RxSeries(vl53l1x, FirstRegister, array, length) != Success )
{
  // ? Catch fail case
}
```
>---

> ## - Check if the module hardware is ready
```C
while( !VL53L1X_isBootReady(vl53l1x) )
{
  // ? Waiting for the device to boot up
}
```
>---

> ## - Enable module measurements
```C
if( VL53L1X_StartRanging(vl53l1x) != Success )
{
  // ? Catch fail case
}
```
>---

> ## - Check if data is measured
```C
while( !VL53L1X_isDataReady(vl53l1x) )
{
  // ? Waiting for the measurement data to complete
}
```
>---

> ## - Get interrupt polarity to check data status
```C
volatile uint8_t polarity = 0;

if( VL53L1X_GetInterruptPolarity(vl53l1x, &polarity) != Success )
{
  // ? Catch fail case
}
else 
{
  // ? Deal with "polarity"
}
```
>---

> ## - Fetch distance
```C
volatile uint16_t distance = 0;

if( VL53L1X_GetDistance(vl53l1x, &distance) != Success )
{
  // ? Catch fail case
}
else 
{
  // ? Deal with "distance"
}
```
>---

> ## - Clear interrupt state
```C
volatile uint16_t distance = 0;

if( VL53L1X_ClearInterrupt(vl53l1x) != Success )
{
  // ? Catch fail case
}
```
>---

> ## - Disable module measurements
```C
if( VL53L1X_StopRanging(vl53l1x) != Success )
{
  // ? Catch fail case
}
```

---

# Demo Code
```C
#include "VL53L1X.h"

static struct {
  VL53L1X_DS * restrict device;
  volatile uint16_t     distance;
} vl53l1x;

task_t VL53L1X_Init(void)
{
  vl53l1x.device = VL53L1X_Constructor(I2C1, 0x52);

  if (!vl53l1x.device) 
  {
    VL53L1X_Destructor(vl53l1x.device);
    return Fail;
  }

  // ! Turn on I2Cx here.

  // ! Must delay until I2Cx is ready.

  return VL53L1X_DefaultInit(vl53l1x.device);
}
```
---