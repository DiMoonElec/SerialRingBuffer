#include "serial_ring_buffer_hal.h"

void SerialRingBufferHAL_Init(SerialRingBuffer_t *instance)
{
  usart_flag_clear(instance->hal.usart_x, USART_RDBF_FLAG);
  usart_interrupt_enable(instance->hal.usart_x, USART_RDBF_INT, TRUE);
  NVIC_EnableIRQ(instance->hal.IRQn);  
}

void SerialRingBufferHAL_SerialEnable(SerialRingBuffer_t *instance)
{
  usart_enable(instance->hal.usart_x, TRUE);
}

void SerialRingBufferHAL_EnterTxCritical(SerialRingBuffer_t *instance)
{
  NVIC_DisableIRQ(instance->hal.IRQn);
}

void SerialRingBufferHAL_ExitTxCritical(SerialRingBuffer_t *instance)
{
  NVIC_EnableIRQ(instance->hal.IRQn);
}

void SerialRingBufferHAL_EnterRxCritical(SerialRingBuffer_t *instance)
{
  NVIC_DisableIRQ(instance->hal.IRQn);
}

void SerialRingBufferHAL_ExitRxCritical(SerialRingBuffer_t *instance)
{
  NVIC_EnableIRQ(instance->hal.IRQn);
}

void SerialRingBufferHAL_TxEnable(SerialRingBuffer_t *instance)
{
  usart_interrupt_enable(instance->hal.usart_x, USART_TDBE_INT, TRUE);
}

void SerialRingBufferHAL_IRQHandler(SerialRingBuffer_t *instance)
{
  //Если что-то получили по uart
  if (usart_flag_get(instance->hal.usart_x, USART_RDBF_FLAG) == SET)
    SerialRingBuffer_CharRxHandler(instance, (uint8_t)usart_data_receive(instance->hal.usart_x));
  
  //Если прерывание пустого передатчика
  if(usart_flag_get(instance->hal.usart_x, USART_TDBE_FLAG) == SET)
  {
    int c = SerialRingBuffer_CharTxHandler(instance);
    
    if(c == -1)
      usart_interrupt_enable(instance->hal.usart_x, USART_TDBE_INT, FALSE);
    else
      usart_data_transmit(instance->hal.usart_x, (uint8_t)c);
  }
}
