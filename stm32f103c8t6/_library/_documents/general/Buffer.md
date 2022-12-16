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
>> ### user has to check if return value is NULL
```C
/**
 * @brief Constructor (dynamic memory)
 *
 * @param size: max length for buffer
 * @return Buffer_DS*: dynamic memory pointer
 */
Buffer_DS *Buffer_Constructor(size_t size);
```
>---

> ## - Destructor
```C
/**
 * @brief Destructor
 *
 * @param self: object pointer
 * @return task_t: Success / Fail
 */
task_t Buffer_Destructor(Buffer_DS *self);
```
>---

> ## - Push 1 byte in
>> ### it automatically checks if the buffer size is exceeded. (queue-like)
```C
/**
 * @brief push byte into buffer
 *
 * @param self: object pointer
 * @param byte: byte to push in
 * @return task_t: Success / Fail
 */
task_t Buffer_Push(Buffer_DS *const self, const uint8_t byte);
```
>---

> ## - Pop up 1 byte
>> ### it automatically checks if the buffer is already empty. (queue-like)
```C
/**
 * @brief pop current head in this buffer
 *
 * @param self: object pointer
 * @param byte: data to save result
 * @return task_t: Success / Fail
 */
task_t Buffer_Take(Buffer_DS *const self, volatile uint8_t *byte);
```
>---

> ## - Peek the index byte
>> ### it automatically checks if the buffer size is exceeded. (array-like)
```C
/**
 * @brief peek the index data of buffer
 *
 * @param self: object pointer
 * @param index: order
 * @param byte: data to save result
 * @return task_t: Success / Fail
 */
task_t Buffer_Index(Buffer_DS *const self, const size_t index, volatile uint8_t *byte);
```
>---

> ## - Clear whole buffer datas
```C
/**
 * @brief clear whole buffer datas
 *
 * @param self: object pointer
 * @return task_t: Success / Fail
 */
task_t Buffer_Flush(Buffer_DS *const self);
```
>---

> ## - Get current buffer length
```C
/**
 * @brief get current buffer length
 *
 * @param self: object pointer
 * @return size_t: current length
 */
size_t Buffer_Length(Buffer_DS *const self);
```
>---

> ## - Compare end data
```C
/**
 * @brief check for items at the ends of buffer
 *
 * @param self: object pointer
 * @param compare: check bytes
 * @param len: length of check bytes
 * @return bool_t: True / False
 */
bool_t Buffer_isEndAs(Buffer_DS *const self, const uint8_t compare[], size_t len);
```
>---