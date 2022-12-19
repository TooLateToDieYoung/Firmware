# Description
> ## - Involves the use of dynamic memory.
> ## - API is queue-like operation (FIFO).
> ## - Implemented with a dynamic array, easier to search.

---

# Suggest
> ## - Do not attempt to expose data structure from Buffer.c file.
> ## - The size of each data is 1 byte in unsigned char format.
> ## - It is recommended to use it to store the raw data from the communication protocol, and process the raw data after receiving it.
> ## - Can also be used to prepare or temporarily store data about to be transferred.

---

# Dependent Header Files
> ## - Provides the base type and namespace of the device
```C
#include "Common.h" // ? check for namespace: STM32F103xx_UNREADY
```

---

# Data Structure
> ## Hidden implementation, can only be operated through API.
```C
// ? in Buffer.h
typedef struct Buffer_DS Buffer_DS;

// ? in Buffer.c
struct Buffer_DS
{
  size_t head, tail, size;
  uint8_t *array;
};
```

---

# API
> ## - Constructor
```C
const size_t length = 10; 

Buffer_DS * restrict buffer = Buffer_Constructor(length);

if( !buffer ) // dynamic memory fail
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

Buffer_Destructor(buffer); 
```
>---

> ## - Push 1 byte in
>> ### it automatically checks if the buffer size is exceeded. (queue-like)
```C
/* 
  ? it automatically checks if the buffer size is exceeded. (queue-like) 
*/

if( Buffer_Push(buffer, data) != Success ) // If buffer is full
{
  // ! Error Handling
}
```
>---

> ## - Pop up 1 byte
```C
/*
  ? it automatically checks if the buffer is already empty. (queue-like)
*/

volatile uint8_t result = 0;

if( Buffer_Take(buffer, &result) != Success ) // If buffer is empty
{
  // ! Error Handling
}
```
>---

> ## - Peek the index byte
```C
/* 
  ? it automatically checks if the buffer size is exceeded. (queue-like) 
*/

volatile uint8_t peek = 0;

if( Buffer_Index(buffer, &peek) != Success ) // If index is out of buffer length
{
  // ! Error Handling
}
```
>---

> ## - Clear whole buffer data
```C
Buffer_Flush(buffer);
```
>---

> ## - Get current buffer length
```C
const size_t currLength = Buffer_Length(buffer);
```
>---

> ## - Compare end data
```C
const size_t  checkLength = 4;
const uint8_t checkTable[checkLength] = { 0xAA, 0x55, 0xAA, 0x55 };

if( Buffer_isEndAs(buffer, checkTable, checkLength) )
{
  // pass
}
else
{
  // !pass
}
```
>---