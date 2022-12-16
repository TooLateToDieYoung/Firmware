#ifndef _BUFFER_H_
#define _BUFFER_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "Common.h"

  /** Data Structure Begin ---------------------------------------------------------------
   * @brief class data sturcture
   * @warning Plz operate the object through the interface
   *
   */

  typedef struct Buffer_DS Buffer_DS;

  /* ---------------------------------------------------------------- Data Structure End */

  /** Interface Begin --------------------------------------------------------------------
   * @brief
   * | Almost directly through the register operation. \n
   * | For each function, they provide an alternative, \n
   * | with the same functionality, implemented with the LL library.
   *
   * @warning Do not change these codes, it may cause errors
   */

  /**
   * @brief Constructor (dynamic memory)
   *
   * @param size: max length for buffer
   * @return Buffer_DS*: dynamic memory pointer
   */
  Buffer_DS *Buffer_Constructor(size_t size);

  /**
   * @brief Destructor
   *
   * @param self: object pointer
   * @return task_t: Success / Fail
   */
  task_t Buffer_Destructor(Buffer_DS *self);

  /**
   * @brief push byte into buffer
   *
   * @param self: object pointer
   * @param byte: byte to push in
   * @return task_t: Success / Fail
   */
  task_t Buffer_Push(Buffer_DS *const self, const uint8_t byte);

  /**
   * @brief pop current head in this buffer
   *
   * @param self: object pointer
   * @param byte: data to save result
   * @return task_t: Success / Fail
   */
  task_t Buffer_Take(Buffer_DS *const self, volatile uint8_t *byte);

  /**
   * @brief peek the index data of buffer
   *
   * @param self: object pointer
   * @param index: order
   * @param byte: data to save result
   * @return task_t: Success / Fail
   */
  task_t Buffer_Index(Buffer_DS *const self, const size_t index, volatile uint8_t *byte);

  /**
   * @brief clear whole buffer datas
   *
   * @param self: object pointer
   * @return task_t: Success / Fail
   */
  task_t Buffer_Flush(Buffer_DS *const self);

  /**
   * @brief get current buffer length
   *
   * @param self: object pointer
   * @return size_t: current length
   */
  size_t Buffer_Length(Buffer_DS *const self);

  /**
   * @brief check for items at the ends of buffer
   *
   * @param self: object pointer
   * @param compare: check bytes
   * @param len: length of check bytes
   * @return bool_t: True / False
   */
  bool_t Buffer_isEndAs(Buffer_DS *const self, const uint8_t compare[], size_t len);

  /* ---------------------------------------------------------------- Interface Code End */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _BUFFER_H_
