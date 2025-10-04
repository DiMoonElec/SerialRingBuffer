#include "SerialRingBuffer/public-api.h"
#include "serial_ring_buffer_hal.h"

void SerialRingBuffer_Init(SerialRingBuffer_t *instance,
                           uint8_t *buff_rx, uint32_t buff_rx_size,
                           uint8_t *buff_tx, uint32_t buff_tx_size)
{
  instance->buff_rx = buff_rx;
  instance->buff_tx = buff_tx;
  
  RingBuffInit(&instance->fifo_rx, instance->buff_rx, buff_rx_size);
  RingBuffInit(&instance->fifo_tx, instance->buff_tx, buff_tx_size);
  
  instance->tx_active = 0;
  
  SerialRingBufferHAL_SerialEnable(instance);
}

int SerialRingBuffer_Putc(SerialRingBuffer_t *instance, uint8_t c)
{
  int ret = 0;
  SerialRingBufferHAL_EnterTxCritical(instance);
  
  int freeItems = RingBuffNumOfFreeItems(&instance->fifo_tx);
  
  if(freeItems > 0)
  {
    RingBuffPut(&instance->fifo_tx, c);
    instance->tx_active = 1;
    SerialRingBufferHAL_TxEnable(instance);
  }
  else
  {
    ret = 1;
  }
  
  SerialRingBufferHAL_ExitTxCritical(instance);
  
  return ret;
}

int SerialRingBuffer_Getc(SerialRingBuffer_t *instance)
{
  int ret = 0;
  
  SerialRingBufferHAL_EnterRxCritical(instance);
  
  ret = RingBuffGet(&instance->fifo_rx);
  
  SerialRingBufferHAL_ExitRxCritical(instance);
  
  return ret;
}

void SerialRingBuffer_CharRxHandler(SerialRingBuffer_t *instance, uint8_t c)
{
  RingBuffPut(&instance->fifo_rx, c);
}

int SerialRingBuffer_CharTxHandler(SerialRingBuffer_t *instance)
{
  if(instance->tx_active && (RingBuffNumOfItems(&instance->fifo_tx) > 0))
  {
    return RingBuffGet(&instance->fifo_tx);
  }
  else
  {
    instance->tx_active = 0;
    return -1;
  }
}

