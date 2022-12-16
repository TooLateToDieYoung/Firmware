#include "Buffer.h"
#include <stdlib.h>

/** Data Structure Begin ---------------------------------------------------------------
 * @brief class data sturcture
 * @warning Plz operate the object through the interface
 *
 */

// ! Do not expose it
struct Buffer_DS
{
  size_t head, tail, size;
  uint8_t *array;
};

/* ---------------------------------------------------------------- Data Structure End */

/** Class Public Functions Begin ---------------------------------------------------------------
 * @brief
 *
 */

Buffer_DS *Buffer_Constructor(size_t size)
{
  Buffer_DS *obj = (Buffer_DS *)calloc(1, sizeof(Buffer_DS));

  if (obj == NULL)
    return NULL;

  obj->array = (uint8_t *)calloc(size, sizeof(uint8_t));

  if (obj->array == NULL)
  {
    free(obj);
    return NULL;
  }

  obj->size = size;

  Buffer_Flush(obj);

  return obj;
}

task_t Buffer_Destructor(Buffer_DS *self)
{
  if (self == NULL)
    return Success;

  if (self->array != NULL)
    free(self->array);

  free(self);

  return Success;
}

task_t Buffer_Push(Buffer_DS *const self, const uint8_t byte)
{
  if (Buffer_Length(self) == self->size)
    return Fail;

  self->array[self->tail] = byte;

  self->tail++;

  return Success;
}

task_t Buffer_Take(Buffer_DS *const self, volatile uint8_t *byte)
{
  if (Buffer_Length(self) == 0)
    return Fail;

  *byte = self->array[self->head];

  self->head++;

  return Success;
}

task_t Buffer_Index(Buffer_DS *const self, const size_t index, volatile uint8_t *byte)
{
  if (Buffer_Length(self) < index)
    return Fail;

  *byte = self->array[index];

  return Success;
}

task_t Buffer_Flush(Buffer_DS *const self)
{
  self->head = self->tail = 0;

  return Success;
}

size_t Buffer_Length(Buffer_DS *const self)
{
  return (self->tail - self->head);
}

bool_t Buffer_isEndAs(Buffer_DS *const self, const uint8_t compare[], size_t len)
{
  if (Buffer_Length(self) < len)
    return False;

  for (size_t i = 1; i <= len; ++i)
    if (self->array[self->tail - i] != compare[len - i])
      return False;

  return True;
}

/* ---------------------------------------------------------------- Class Public Functions End */
