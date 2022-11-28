#include "hc05.h"
#include <stdlib.h>

#ifndef HC05_UNREADY

/** Forward Declarations for private data structure & functions
 * @brief Queue-like data structure for USART rx and tx buffers
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

static task_t _Buffer_Peek(buffer_t * const self, volatile uint8_t * byte)
{
  if( self->head >= self->tail ) return Fail; // has been touched end

  *byte = self->array[self->head];

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
 * @param timeout: set try times
 * @return task_t: Success / Fail 
 */
task_t HC05_TxByte(HC05_DS * const self, uint16_t timeout)
{
  uint8_t byte = 0;

  if( _Buffer_Peek(&self->tx, &byte) != Success ) return Fail;

  do {
    if( 
      _InterfaceUSART_TxByte(self->USARTx, byte) == Success 
    ) return _Buffer_Pop(&self->tx, &byte);
  } while( timeout-- );

  return Fail;
}

/**
 * @brief Transfer one byte
 * 
 * @param self: object pointer 
 * @param timeout: set try times
 * @return task_t: Success / Fail 
 */
task_t HC05_RxByte(HC05_DS * const self, uint16_t timeout)
{
  uint8_t byte = 0;

  do {
    if( 
      _InterfaceUSART_RxByte(self->USARTx, &byte) == Success 
    ) return _Buffer_Push(&self->rx, byte);
  } while( timeout-- );

  return Fail;
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
 * @param check: compare the last few bytes according to user needs
 * @param size: length to compare
 * @return bool_t: True / False
 */
bool_t HC05_isRxDone(HC05_DS * const self, const uint8_t check[], size_t size)
{
  if( _Buffer_Length(&self->rx) < size ) return False;

  for(size_t i = 1; i <= size;++i)
    if( self->rx.array[self->rx.tail - i] != check[size - i] ) return False;

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
 * @param timeout: set try times
 * @return bool_t: True / False
 */
task_t HC05_ClearResidualData(HC05_DS * const self, uint16_t timeout)
{
  uint8_t byte = 0;

  do {
    if( 
      _InterfaceUSART_RxByte(self->USARTx, &byte) == Success
    ) return Success;
  } while( timeout-- );

  return Success;
}

#endif // HC05_UNREADY
