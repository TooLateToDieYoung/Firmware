/**
 * @file hc05.h
 * @author Zhang, Zhen Yu (https://github.com/TooLateToDieYoung)
 * @brief
 * @warning
 * @version 0.1
 * @date 2022-11-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _HC05_H_
#define _HC05_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#if   defined( STM32F103x4 ) || defined( STM32F103x6 )
  #include "stm32f103x8.h"
#elif defined( STM32F103x8 ) || defined( STM32F103xB )
  #include "stm32f103xb.h"
#elif defined( STM32F103xC ) || defined( STM32F103xD ) || defined( STM32F103xE )
  #include "stm32f103xe.h"
#elif defined( STM32F103xF ) || defined( STM32F103xG )
  #include "stm32f103xg.h"
#else
  #define HC05_UNREADY
  #warning "This library must be working under stm32f1xx series"
#endif

#ifndef HC05_UNREADY

#include "common.h"
#include "InterfaceUART.h"

/** Def. Begin -------------------------------------------------------------------------
 * @brief 
 * 
 */
#define HC05_BUFFER_LENGTH 50

/* -------------------------------------------------------------------------- Def. End */

/** Data Structure Begin ---------------------------------------------------------------
 * @brief 
 * 
 */
typedef struct HC05_DS HC05_DS;

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
 * @param USARTx: defined in the stm32f103xx series header
 * @return HC05_DS*: dynamic memory pointer
 */
HC05_DS * HC05_Constructor(USART_TypeDef * USARTx);

/**
 * @brief Destructor
 * 
 * @param self: object pointer
 * @return task_t: Success / Fail
 */
task_t HC05_Destructor(HC05_DS * self);

/**
 * @brief Prepare Tx buffer to next transmission
 * 
 * @param self: object pointer
 * @param array: origin datas
 * @param len: array length
 * @return task_t: Success / Fail
 */
task_t HC05_setTxDatas(HC05_DS * const self, const uint8_t array[], size_t len);

/**
 * @brief To load Rx buffer datas
 * 
 * @param self: object pointer
 * @param array: origin datas
 * @param len: array length
 * @return task_t: Success / Fail
 */
task_t HC05_getRxDatas(HC05_DS * const self, volatile uint8_t array[], volatile size_t * len);

/**
 * @brief Transfer one byte
 * 
 * @param self: object pointer 
 * @param thread: indicates that the caller is from the main thread or an interrupt
 * @return task_t: Success / Fail 
 */
task_t HC05_TxByte(HC05_DS * const self, InterfaceUART_ThreadType_Enum thread);

/**
 * @brief Receive one byte
 * 
 * @param self: object pointer 
 * @param thread: indicates that the caller is from the main thread or an interrupt
 * @return task_t: Success / Fail 
 */
task_t HC05_RxByte(HC05_DS * const self, InterfaceUART_ThreadType_Enum thread);

/**
 * @brief All data in the tx buffer has been transmitted
 * 
 * @param self: object pointer 
 * @return bool_t: True / False
 */
bool_t HC05_isTxDone(HC05_DS * const self);

/**
 * @brief This set of data has been received
 * 
 * @param self: object pointer 
 * @return bool_t: True / False
 */
bool_t HC05_isRxDone(HC05_DS * const self);

/**
 * @brief Forcibly discard packet
 * 
 * @param self: object pointer 
 * @return bool_t: True / False
 */
task_t HC05_DiscardPacket(HC05_DS * const self);

/**
 * @brief Clear residual data
 * 
 * @param self: object pointer 
 * @return bool_t: True / False
 */
task_t HC05_ClearResidualData(HC05_DS * const self);

/* ---------------------------------------------------------------- Interface Code End */


#endif // HC05_UNREADY

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _HC05_H_
