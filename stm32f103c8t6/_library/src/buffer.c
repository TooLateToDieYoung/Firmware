#include "buffer.h"
#include <stdlib.h>

/** Data Structure Begin ---------------------------------------------------------------
 * @brief class data sturcture
 * @warning Plz operate the object through the interface
 * 
 */

// ! Do not expose it
struct Buffer_DS {
  size_t head, tail, size;
  uint8_t * array;
};

/* ---------------------------------------------------------------- Data Structure End */

/** Class Public Functions Begin ---------------------------------------------------------------
 * @brief 
 * 
 */

/**
 * @brief Constructor (dynamic memory)
 * 
 * @param size: max length for buffer
 * @return Buffer_DS*: dynamic memory pointer 
 */
Buffer_DS * Buffer_Constructor(size_t size)
{
  Buffer_DS * obj = (Buffer_DS*)calloc(1, sizeof(Buffer_DS));

  if( obj == NULL ) return NULL;

  obj->array = (uint8_t*)calloc(size, sizeof(uint8_t));

  if( obj->array == NULL ) { free( obj ); return NULL; }

  obj->size = size;
  
  Buffer_Flush(obj);

  return obj;
}

/**
 * @brief Destructor
 *
 * @param self: object pointer
 * @return task_t: Success / Fail
 */
task_t Buffer_Destructor(Buffer_DS * self)
{
  if( self == NULL ) return Success;

  if( self->array != NULL ) free(self->array);

  free(self);

  return Success;
}

/**
 * @brief push byte into buffer
 * 
 * @param self: object pointer
 * @param byte: byte to push in
 * @return task_t: Success / Fail
 */
task_t Buffer_Push(Buffer_DS * const self, const uint8_t byte)
{
  if( Buffer_Length(self) == self->size ) return Fail;

  self->array[self->tail] = byte;
  
  self->tail++;

  return Success;
}

/**
 * @brief pop current head in this buffer
 * 
 * @param self: object pointer
 * @param byte: data to save result
 * @return task_t: Success / Fail
 */
task_t Buffer_Take(Buffer_DS * const self, volatile uint8_t * byte)
{
  if( Buffer_Length(self) == 0 ) return Fail;

  *byte = self->array[self->head];

  self->head++;

  return Success;
}

/**
 * @brief peek the index data of buffer
 * 
 * @param self: object pointer
 * @param index: order
 * @param byte: data to save result
 * @return task_t: Success / Fail
 */
task_t Buffer_Index(Buffer_DS * const self, const size_t index, volatile uint8_t * byte)
{
  if( Buffer_Length(self) < index ) return Fail;

  *byte = self->array[index];

  return Success;
}

/**
 * @brief clear whole buffer datas
 * 
 * @param self: object pointer
 * @return task_t: Success / Fail
 */
task_t Buffer_Flush(Buffer_DS * const self)
{
  self->head = self->tail = 0;

  return Success;
}

/**
 * @brief get current buffer length
 * 
 * @param self: object pointer
 * @return size_t: current length
 */
size_t Buffer_Length(Buffer_DS * const self)
{
  return ( self->tail - self->head ) ;
}

/**
 * @brief check for items at the ends of buffer
 * 
 * @param self: object pointer
 * @param compare: check bytes
 * @param len: length of check bytes
 * @return bool_t: True / False 
 */
bool_t Buffer_isEndAs(Buffer_DS * const self, const uint8_t compare[], size_t len)
{
  if( Buffer_Length(self) < len ) return False;

  for(size_t i = 1; i <= len; ++i)
    if( self->array[self->tail - i] != compare[len - i] ) return False;
  
  return True;
}

/* ---------------------------------------------------------------- Class Public Functions End */
