/**
 * @file SevenSegment.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief
 * @version 0.1
 * @date 2022-12-13
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _SEVEN_SEGMENT_H_
#define _SEVEN_SEGMENT_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "Common.h"

#ifndef STM32F103xx_UNREADY

  /** Def. Begin -------------------------------------------------------------------------
   * @brief only a few basic registers.
   * | for more infomation, plz refer to the datasheet.
   *
   */

  typedef enum
  {
    SegA = 0x01,
    SegB = 0x02,
    SegC = 0x04,
    SegD = 0x08,
    SegE = 0x10,
    SegF = 0x20,
    SegG = 0x40,
    SegP = 0x80
  } SevenSegmentIndex_Enum;

  typedef enum
  {
    Num0 = (SegA | SegB | SegC | SegD | SegE | SegF),
    Num1 = (SegB | SegC),
    Num2 = (SegA | SegB | SegD | SegE | SegG),
    Num3 = (SegA | SegB | SegC | SegD | SegG),
    Num4 = (SegB | SegC | SegF | SegG),
    Num5 = (SegA | SegC | SegD | SegF | SegG),
    Num6 = (SegA | SegC | SegD | SegE | SegF | SegG),
    Num7 = (SegA | SegB | SegC),
    Num8 = (SegA | SegB | SegC | SegD | SegE | SegF | SegG),
    Num9 = (SegA | SegB | SegC | SegD | SegF | SegG),
  } SevenSegmentTable_Enum;

  /* -------------------------------------------------------------------------- Def. End */

  /** Data Structure Begin ---------------------------------------------------------------
   * @brief class data sturcture
   * @warning Plz operate the object through the interface
   *
   */

  typedef struct
  {
    port_t common;
    port_t io[8];
  } SevenSegment_DS;

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
   * @param common: common anode control pin
   * @param io: segment open drain control pins, which [7:0] stand to SegP to SegA 
   * @return SevenSegment_DS*: dynamic memory pointer
   */
  SevenSegment_DS *SevenSegment_Constructor(const port_t *const common, const port_t io[]);

  /**
   * @brief Destructor
   *
   * @param self: object pointer
   * @return task_t: Success / Fail
   */
  task_t SevenSegment_Destructor(SevenSegment_DS *const self);

  /**
   * @brief Set the output segment type
   * 
   * @param self: object pointer 
   * @param segment: display type 
   * @return task_t: Success / Fail
   */
  task_t SevenSegment_Set(SevenSegment_DS *const self, flag8_t segment);

  /**
   * @brief Reset output(close it)
   * 
   * @param self: object pointer  
   * @return task_t: Success / Fail
   */
  task_t SevenSegment_Reset(SevenSegment_DS *const self);

  /* ---------------------------------------------------------------- Interface Code End */

#endif // STM32F103xx_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _SEVEN_SEGMENT_H_
