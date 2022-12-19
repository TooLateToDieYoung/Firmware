/**
 * @file Button.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief
 * @warning
 * @version 0.1
 * @date 2022-12-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#include "Common.h"

#ifndef STM32F103xx_UNREADY

  /** Data Structure Begin ---------------------------------------------------------------
   * @brief class data sturcture
   * @warning Plz operate the object through the interface
   *
   */

  typedef struct
  {

    port_t pin;

  } Button_DS;

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
   * @param pin: button input pin
   * @return Button_DS*: dynamic memory pointer
   */
  Button_DS *Button_Constructor(const port_t *const pin);

  /**
   * @brief Destructor
   *
   * @param self: object pointer
   * @return task_t: Success / Fail
   */
  task_t Button_Destructor(Button_DS *const self);

  /**
   * @brief Check if button is free
   *
   * @param self: object pointer
   * @param byte: byte to write
   * @param timeout: set try times
   * @return task_t: Success / Fail
   */
  bool_t Button_isFree(Button_DS *const self);

  /* ---------------------------------------------------------------- Interface Code End */

#endif // STM32F103xx_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _BUTTON_H_
