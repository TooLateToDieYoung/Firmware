#include "hc05.h"
#include <stdlib.h>

/**
 * @brief Queue-like data structure for uart rx and tx buffers
 * 
 */
typedef struct {
  size_t  head, tail;
  uint8_t array[HC05_BUFFER_LENGTH];
} buffer_t;

static size_t _Buffer_Length(buffer_t * const self)
{
  return ( self->tail - self->head );
}

static task_t _Buffer_Clear(buffer_t * const self)
{
  self->head = self->tail = 0;

  return Success;
}

static task_t _Buffer_Push(buffer_t * const self, const uint8_t byte)
{
  if( self->tail >= HC05_BUFFER_LENGTH ) return Fail; // out of buffer size

  self->array[self->tail] = byte;

  self->tail++;
  
  return Success;
}

static task_t _Buffer_Pop(buffer_t * const self, volatile uint8_t * byte)
{
  if( self->head >= self->tail ) return Fail; // has been touched end

  *byte = self->array[self->head];

  self->head++;
  
  return Success;
}

/* ------------------------------------------------------------------------- */

/**
 * @brief 
 * 
 */
struct HC05_DS {
  USART_TypeDef * USARTx;
  buffer_t rx, tx;
};

/**
 * @brief Constructor (dynamic memory)
 * 
 * @param USARTx: defined in the stm32f103xx series header
 * @param rxSize: rx buffer length
 * @param txSize: tx buffer length
 * @return HC05_DS*: dynamic memory pointer
 */
HC05_DS * HC05_Constructor(USART_TypeDef * USARTx)
{
  HC05_DS * obj = (HC05_DS*)calloc(1, sizeof(HC05_DS));

  if( obj == NULL ) return NULL;

  obj->USARTx = USARTx;

  _Buffer_Clear(&obj->rx);
	_Buffer_Clear(&obj->tx);

  return obj;
}

/**
 * @brief Destructor
 * 
 * @param self: object pointer
 * @return task_t: Success / Fail
 */
task_t HC05_Destructor(HC05_DS * self)
{
  free(self);

  return Success;
}

/**
 * @brief Prepare Tx buffer to next transmission
 * 
 * @param self: object pointer
 * @param array: origin datas
 * @param len: array length
 * @return task_t: Success / Fail
 */
task_t HC05_setTxDatas(HC05_DS * const self, const uint8_t array[], size_t len)
{
  if( len >= HC05_BUFFER_LENGTH ) return Fail;

  _Buffer_Clear(&self->tx);

  for(size_t i = 0; i < len; ++i) _Buffer_Push(&self->tx, array[i]);

  return Success;
}

/**
 * @brief To load Rx buffer datas
 * 
 * @param self: object pointer
 * @param array: origin datas
 * @param len: array length
 * @return task_t: Success / Fail
 */
task_t HC05_getRxDatas(HC05_DS * const self, volatile uint8_t array[], volatile size_t * len)
{
  *len = _Buffer_Length(&self->rx);
  
  for(size_t i = 0; i < *len; ++i) _Buffer_Pop(&self->rx, &array[i]);

  _Buffer_Clear(&self->rx);

  return Success;
}

/**
 * @brief Transfer one byte
 * 
 * @param self: object pointer 
 * @param thread: indicates that the caller is from the main thread or an interrupt
 * @return task_t: Success / Fail 
 */
task_t HC05_TxByte(HC05_DS * const self, InterfaceUART_ThreadType_Enum thread)
{
  uint8_t byte = 0;

  if( _Buffer_Pop(&self->tx, &byte) != Success ) return Fail;

  return _InterfaceUART_TxByte(self->USARTx, byte, thread);
}

/**
 * @brief Receive one byte
 * 
 * @param self: object pointer 
 * @param thread: indicates that the caller is from the main thread or an interrupt
 * @return task_t: Success / Fail 
 */
task_t HC05_RxByte(HC05_DS * const self, InterfaceUART_ThreadType_Enum thread)
{
  uint8_t byte = 0;

  if( _InterfaceUART_RxByte(self->USARTx, &byte, thread) != Success ) return Fail;

  return _Buffer_Push(&self->rx, byte);
}

/**
 * @brief All data in the tx buffer has been transmitted
 * 
 * @param self: object pointer 
 * @return bool_t: True / False
 */
bool_t HC05_isTxDone(HC05_DS * const self)
{
  return (bool_t)( _Buffer_Length(&self->tx) == 0 );
}

/**
 * @brief This set of data has been received
 * 
 * @param self: object pointer 
 * @return bool_t: True / False
 */
bool_t HC05_isRxDone(HC05_DS * const self)
{
  if( _Buffer_Length(&self->rx) < 4 ) return False;

  if( self->rx.array[self->rx.tail - 1] != '\n' ) return False;
  if( self->rx.array[self->rx.tail - 2] != '\r' ) return False;
  if( self->rx.array[self->rx.tail - 3] !=  'K' ) return False;
  if( self->rx.array[self->rx.tail - 4] !=  'O' ) return False;

  return True;
}
/**
 * @brief Forcibly discard packet
 * 
 * @param self: object pointer 
 * @return bool_t: True / False
 */
task_t HC05_DiscardPacket(HC05_DS * const self)
{
	return _Buffer_Clear(&self->rx);
}

/**
 * @brief Clear residual data
 * 
 * @param self: object pointer 
 * @return bool_t: True / False
 */
task_t HC05_ClearResidualData(HC05_DS * const self)
{
  uint8_t byte = 0;
  
  _InterfaceUART_RxByte(self->USARTx, &byte, UART_INTERRUPT);

  return Success;
}
