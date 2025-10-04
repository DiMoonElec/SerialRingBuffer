#ifndef __SERIAL_RING_BUFFER_H__
#define __SERIAL_RING_BUFFER_H__

#include <stdint.h>
#include "RingFIFO/public-api.h"
#include "serial_ring_buffer_hal_type.h"

typedef struct 
{
  uint8_t *buff_rx;
  uint8_t *buff_tx;
  
  RingBuff_t fifo_rx;
  RingBuff_t fifo_tx;
  
  uint8_t tx_active;
  
  SerialRingBufferHAL_t hal;
} SerialRingBuffer_t;


void SerialRingBuffer_Init(SerialRingBuffer_t *instance,
                           uint8_t *buff_rx, uint32_t buff_rx_size,
                           uint8_t *buff_tx, uint32_t buff_tx_size);
int SerialRingBuffer_Putc(SerialRingBuffer_t *instance, uint8_t c);
int SerialRingBuffer_Getc(SerialRingBuffer_t *instance);


void SerialRingBuffer_CharRxHandler(SerialRingBuffer_t *instance, uint8_t c);
int SerialRingBuffer_CharTxHandler(SerialRingBuffer_t *instance);

#endif
